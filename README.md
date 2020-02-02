### MyCacheDB介绍
MyCacheDB一个基于C/S架构的小型缓存数据库，支持和redis相同的string，hash，list，set，sortset对象的基本操作，
提供用户编程和终端输入request-response两种模式，
使用方法简单方便。

### 运行环境

- 系统：ubuntu19.10
- 处理器：Intel® Core™ i5-7300HQ CPU @ 2.50GHz × 4
- 内存：7.7 GiB
- 依赖：[Google Protocol Buffers](https://github.com/protocolbuffers/protobuf)，支持proto命令
- 编译器：gcc编译器，vim+vimplus编写实现

### 特性

- 双端协议都是使用[Google Protocol Buffers](https://developers.google.cn/protocol-buffers/),程序中都设计了序列化和反序列化模块
- 所有对象使用只能指针进行管理，少内存泄漏

**服务端**
- 分模块实现，主要包含IO处理模块、命令及其回调函数模块、数据局对象模块、日志记录模块和持久化及数据库恢复模块
- IO处理模块中，采用事件驱动IO设计思想，Epoll IO多路复用机制，每个被监听事件的类型都为Epoll LT+EPOLLIN+非阻塞，有事件到时，通过回调函数处理
- 使用最小堆定时器对超时连接进行检测，使用eventfd对定时器事件进行通知
- 设计模式，日志对象在系统中是单例，数据库对象使用工厂模式生成
- 日志系统参考LevelDB日志记录形式
- 有序集合对象[自己实现红黑树数据结构](https://blog.csdn.net/qq_41681241/article/details/103552988)进行管理
- 持久化模块，设计分库管理机制，1号数据库存储在.db_1文件中依次类推，所有数据库名称保存在另一个文件中，开机时通过mmap对文件进行读取

**客户端**
- 用户要是使用客户端编程接口，发送命令和接收命令是异步的，接收命令使用Epoll+线程池，套接字为非阻塞+EPOLLIN+EPOLLONESHOT LT触发模式
- 使用队列机制管理发送的命令和接收的命令
- 用户使用终端接口直接运行./MyCacheDB就能使用，使用方法和redis中的命令操作形式相同

### 目录说明


|名称|作用|
|:---:|:--:|
|client|客户端源代码实现|
|server|服务器源代码|
|conf|配置文件|
|image|一些截图信息|
|logInfo|日志记录文件|
|start.sh|编译程序的shell命令脚本|
### 使用与安装

（1）修改配置文件，进入conf文件，进入配置文件填写IP和端口信息，格式为冒号+空格+IP/端口

（2）修改start.sh文件的权限

```
chmod a+x start.sh   //当前目录中必须有start.sh文件
```
（3）进入server目录，运行服务器

```
./Server
```

（4）进入client目录，执行

```
sudo cp -R MyCacheDB /usr/include
sudo mv DBClient /usr/sbin
```
（5）编写自定义的程序


