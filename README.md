## UChat

### 当前问题

**bug1.** 当先登录一个不存在的名字（假设mano3）并注册后，再上线另外一个（无论存在于数据库还是不存在），这时候在第一个不点击第二个的情况下，第二个给第一个发信息，只在第二个显示，第一个收不到

**bug2.** 创建和邀请入群 groupList更新bug

**bug3.** 聊天打印缓存bug复现和修复

### 升级与准备

**1.** 升级1：我们的群聊历史记录搜索要不要添加按名字搜索（感觉这个Ricky每次都有问pre的组）

**2.** 升级2：历史记录要不要改为时间段

**3.** PPT重构, pre练习
