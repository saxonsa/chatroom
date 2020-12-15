#include "client.h"

#include <QDebug>


SOCKET connect_sock = INVALID_SOCKET;

WSADATA wsaData;

struct sockaddr_in server_addr;  // server end address

char serverIP[32] = {0};
char server_port[10] = {0};
char* server_name = (char*)"localhost";
unsigned short port = DEFAULT_PORT;
unsigned int addr;

char szBuff[];
int msg_len;
struct hostent* hp;

char name[255] = {0};

usrData usr;

int client_connect(char* serverIP, char* server_port, char* name) {
  printf("%s %s %s", serverIP, server_port, name);
  port = atoi(server_port);

  if (WSAStartup(0x202, &wsaData) == SOCKET_ERROR) {
    // stderr: standard error are printed to the screen.
    fprintf(stderr, "WSAStartup failed with error %d\n", WSAGetLastError());
    // WSACleanup function terminates use of the Windows Sockets DLL.
    WSACleanup();
    return -1;
  }

  if (isalpha(server_name[0]))
    hp = gethostbyname(server_name);
  else {
    addr = inet_addr(server_name);
    hp = gethostbyaddr((char*)&addr, 4, AF_INET);
  }

  if (hp == NULL) {
    fprintf(stderr, "Cannot resolve address: %d\n", WSAGetLastError());
    WSACleanup();
    return -1;
  }

  // copy the resolved information into the sockaddr_in structure
  memset(&server_addr, 0, sizeof(server_addr));
  memcpy(&(server_addr.sin_addr), hp->h_addr, hp->h_length);
  server_addr.sin_family = hp->h_addrtype;
  server_addr.sin_port = htons(port);

  connect_sock = socket(AF_INET, SOCK_STREAM, 0);  // TCp socket

  if (connect_sock == INVALID_SOCKET) {
    fprintf(stderr, "socket() failed with error %d\n", WSAGetLastError());
    WSACleanup();
    return -1;
  }

  printf("Client connecting to: %s\n", hp->h_name);

  if (connect(connect_sock, (struct sockaddr*)&server_addr,
              sizeof(server_addr)) == SOCKET_ERROR) {
    fprintf(stderr, "connect() failed with error %d\n", WSAGetLastError());
    WSACleanup();
    return -1;
  }

  // print connected server information
  printf("Successful connect to server IP: %s; Port: %d\n",
         inet_ntoa(server_addr.sin_addr), htons(server_addr.sin_port));  

  return 0;
}
