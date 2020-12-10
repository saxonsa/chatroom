# networking_chatroom

#### **实现思路**

Server： 绑定socket对象->设置监听数->等待连接->有客户端连接就新建一个线程，这个线程中，一旦就收到这个客户发送的消息，就广播的向其他客户端发送同样的消息。

Client：向客户端连接->新建线程用来接收服务器端发送的消息，同时主进程用来发送消息



#### **当前实现功能**

Server：

​		可支持指定数目(MAX_ALLOWED)个客户端发信息，收到信息后并广播给所有人

Client：

​		可以多个client向服务器端多次发送信息 + GUI



#### **剩余任务待处理**

***Main Task:***

1. 一个用户可以向另一个指定用户自由传信息
2. 用户可以自由form group进行communication
3. search功能对应GUI

Server：

1. rich set of search function

Client：

1. 检测输入中的'和"，如果有需要转义--> /' & /"
2. search历史记录 method

Additional：

2. 管理员禁言等功能
3. 用户可以传文件，图片等支持多种格式





