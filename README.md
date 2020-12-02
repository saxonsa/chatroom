# networking_chatroom

#### **实现思路**

Server： 绑定socket对象->设置监听数->等待连接->有客户端连接就新建一个线程，这个线程中，一旦就收到这个客户发送的消息，就广播的向其他客户端发送同样的消息。

Client：向客户端连接->新建线程用来接收服务器端发送的消息，同时主进程用来发送消息



#### **当前实现功能**

Server：

​		可支持指定数目(MAX_ALLOWED)个客户端发信息，收到信息后并广播给所有人

Client：

​		可以多个client向服务器端多次发送信息，当输入end的时候断开连接



#### **剩余任务待处理**

Server：

1. 获取client名字，发送给不同的client的时候同时发送名字
2. 用数据库来存储client发送的信息
3. search method

Client：

1. 连接的时候发送聊天姓名
2. search历史记录 method

Additional：

1. GUI
2. 管理员禁言等功能
3. 用户可以传文件，图片等支持多种格式





