# networking_chatroom

#### **实现思路**

Server： 绑定socket对象->设置监听数->等待连接->有客户端连接就新建一个线程，这个线程中，一旦就收到这个客户发送的消息，就广播的向其他客户端发送同样的消息。

Client：向客户端连接->新建线程用来接收服务器端发送的消息，同时主进程用来发送消息



#### **当前实现功能**

Server：

​		可接收一个客户端的信息，并把信息传递回去，然后没啥用了

Client：

​		可以向服务器端发送一条信息，然后中断



#### **剩余任务待处理**

Server：

1. 处理当多个client进行连接的请求，并接收信息广播给所有人 （多线程，封装accept，receive）
2. 用数据库来存储client发送的信息
3. search method

Client：

1. 单用户可多次发送，可多次接收
2. search历史记录 method

Additional：

1. GUI
2. 管理员禁言等功能
3. 用户可以传文件，图片等支持多种格式





