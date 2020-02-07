### MyCacheDB介绍
MyCacheDB一个基于C/S架构的小型缓存数据库，支持string，hash，list，set，sortset对象的基本操作，
提供用户编程接口，以及终端readline接收输入，正则表达式进行验证的request-response模式，使用方法简单方便。

### 运行环境

- 系统：ubuntu19.10
- 处理器：Intel® Core™ i5-7300HQ CPU @ 2.50GHz × 4
- 内存：7.7 GiB
- 依赖：[Google Protocol Buffers](https://github.com/protocolbuffers/protobuf)，支持proto命令
- 编译器：Gcc编译器，Vim+Vimplus编写实现

### 特性

- 双端协议都是使用[Google Protocol Buffers](https://developers.google.cn/protocol-buffers/),程序中都设计了序列化和反序列化模块
- 所有对象使用只能指针进行管理，少内存泄漏

**服务端**
- 分模块实现，主要包含IO处理模块、命令及其回调函数模块、数据局对象模块、日志记录模块和持久化及数据库恢复模块
- IO处理模块中，采用事件驱动IO设计思想，Epoll IO多路复用机制，每个被监听事件的类型都为Epoll LT+EPOLLIN+非阻塞，有事件到时，通过回调函数处理
- 使用最小堆定时器对超时连接进行检测，使用eventfd对定时器事件进行通知
- 定时持久化使用timerfd，加到epoll中检测，有可读事件，检测修改次数，并开辟进程进行持久化
- 设计模式，项目中使用到了工厂模式和单实例懒汉模式
- 日志模块参考Redis Append-On-File的思想，使用Easyloging++进行日志文件滚动记录
- 有序集合对象通过[自己实现红黑树数据结构](https://blog.csdn.net/qq_41681241/article/details/103552988)进行管理
- 持久化模块，写保存数据库快照，防止之前的数据丢失，分为阻塞持久化和异步持久化。异步持久化，fork子进程进行，给客户端直接返回OK，异步持久化完成后，向服务端通过socketpair发送消息
- 设计分库管理机制，1号数据库存储在.db\_1文件中依次类推，所有数据库名称保存在另一个文件中，开机时通过mmap对.db\_\*文件进行读取
- 当缓存不够用的时候，会先随机选一些键，然后模拟LRU设计二叉搜索树淘汰


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
|test|客户端测试文件|
|example|例子程序|

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
将client下的lib目录中的静态库文件移动到你自定义程序的目录下，需要在编译时链接
（5）编写自定义的程序如下

`main.cpp`


```c
#include <iostream>
#include <MyCacheDB/clientLoop.h>
using namespace std ;
int main(int argc, char** argv) {
    clientLoop clp ;
    //传入IP 和地址
    clp.init("127.0.0.1", "8888") ;
   //在配置文件中找
    //clp.init() ;
//////////////////////////////程序中使用
    clp.sendRequest("set", "name", "wc", END) ;
    clp.sendRequest("get", "name", END) ;
    clp.sendRequest("hset", "student", "name","ck", "age", "18", END) ;
    clp.sendRequest("hget", "student", "name", END) ;
    clp.sendRequest("hget", "student", "age", END) ;
    //........   

    //////////////////////////获取结果
    cout << "获取的返回结果：" << endl ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;

///////////////////////////////////////////////终端使用
//    clp.start() ;
    return 0;
}
```

编译
```
g++ main.cpp libclient.a -lpthread -g -std=c++11 -w -lprotobuf -lreadline `pkg-config --cflags --libs protobuf` -o test_exe
```
运行：

```
./test_exe
```

运行截图：

![haha](image/jiekou.png)

运行client目录下DBClient，终端用户C/S交互截图：

![term](image/zhongduan.png)


### 测试

- 编写的测试程序：

`test.cpp`

```c

#include <iostream>
#include <sys/time.h>
#include <MyCacheDB/clientLoop.h>
using namespace std ;
int main(int argc, char** argv) {
    struct timeval tv, tend;
    clientLoop clp ;
    clp.init("127.0.0.1", "8888") ;
    gettimeofday(&tv,NULL);
    int a = 10000 ;
    while(a--) {
        clp.sendRequest("get", "name", END) ;
        clp.getResult() ;
    }

    gettimeofday(&tend,NULL);
    cout << "读取测试" << endl ;
    cout << "==================================================" << endl ;
    cout <<"开始时间戳:"<< tv.tv_sec<<"." << tv.tv_usec<<"s"<< endl;
    cout <<"结束时间戳:"<< tend.tv_sec<<"." << tend.tv_usec<<"s"<< endl;
    int s = (tend.tv_sec-tv.tv_sec)*1000000 ;
    int us = tend.tv_usec - tv.tv_usec ;
    double h = 10000;
    int end = s + us ;
    cout <<h*1000000/end<< endl ;
    cout << "speed:" << h*1000000/end <<"cmd/s"<< endl ;
    struct timeval tv1, tend1;
    gettimeofday(&tv1,NULL);
    a = 10000 ;
    while(a--) {
        clp.sendRequest("hset", "student", "name","ck", "age", "18", END) ;
        clp.getResult() ;
    }
    gettimeofday(&tend1,NULL);
    cout << "修改测试" << endl ;
    cout << "==================================================" << endl ;
    cout <<"开始时间戳:"<< tv1.tv_sec<<"." << tv1.tv_usec<<"s"<< endl;
    cout <<"结束时间戳:"<< tend1.tv_sec<<"." << tend1.tv_usec<<"s"<< endl;
    s = (tend1.tv_sec-tv1.tv_sec)*1000000 ;
    us = tend1.tv_usec - tv1.tv_usec ;
    h = 10000;
    end = s + us ;
    cout <<"speed:" << h*1000000/end<<"cmd/s"<< endl ;
    return 0;
}

```

- 事件精确微妙，编译

```
g++ test.cpp libclient.a -lpthread -g -std=c++11 -w -lprotobuf -lreadline `pkg-config --cflags --libs protobuf` -o test_exe
```

运行结果：

- 第一次客户端发送命令，服务器以也是刚启动，先不进行初始化。在接收到命令后，会进行一系列初始化，显示结果，第一次执行get命令和第二次执行结果相比较慢

![haha](image/test1.png)

- 第二次运行客户端发送命令，服务器不需要初始化了，相比第一次执行较快

![lala](image/test2.png)


