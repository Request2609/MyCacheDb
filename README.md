<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <p align=center></strong>cacheDb2.0  键值数据库的设计实现总结(SDS)</strong></p>
  <link rel="stylesheet" href="https://stackedit.io/style.css" />
</head>

<body class="stackedit">
  <div class="stackedit__html"><p>写这个数据库也已经有段时间了，在我心中此项目的第一个版本算是完成了，所以做一下小结，更多的功能以后再继续迭代即可！</p>
<h3><a id="1__2"></a>1. 引言</h3>
<h4><a id="11___3"></a>1.1  编写目的</h4>
<p>学习搭建rpc框架，自己阅读了部分redis实现源码，想实现一个加深印象，将学习的网络方面的知识学以致用。等等…</p>
<h4><a id="12__6"></a>1.2 项目概述</h4>
<p>客户端和服务器协议：Google Protocol  buffer<br>
RPC部分实现的功能：序列化和反序列化，发送远程命令<br>
项目服务器使用Reactor模式设计的框架，为每一个连接设置了相应的读回调，写回调，每个连接套接字设置了EPOLLIN+非阻塞模式。实现了string和hash两种操作，并实现了save和bgsave两种持久化操作，bgsave操作通过创建新进程实现。项目中添加了最大堆定时器，为每个客户端设置了保活时间。当客户端产生一次活动时，会自动为该客户端延长时间，对于一些一直不活动的客户端，服务器会定时检查连接队列，并剔除闲置客户端。定时器通过统一事件源触发。<br>
客户端实现比较简单，使用readline进行命令行输入，并设置了检测命令的合法模块，检测客户端是否与服务器建立了连接，要是没建立连接，会尝试和服务器建立５次连接。直到建立连接成功。</p>
<h4><a id="13__12"></a>1.3 术语定义</h4>

<table>
<thead>
<tr>
<th>术语名称</th>
<th>术语含义</th>
</tr>
</thead>
<tbody>
<tr>
<td>GPB</td>
<td>Google Protocol Buffer</td>
</tr>
<tr>
<td>Server</td>
<td>服务器</td>
</tr>
<tr>
<td>Client</td>
<td>客户端</td>
</tr>
</tbody>
</table><h4><a id="14__18"></a>1.4 引用文档</h4>

<table>
<thead>
<tr>
<th>名称</th>
<th>作者</th>
</tr>
</thead>
<tbody>
<tr>
<td>Redis设计与实现</td>
<td>黄建宏</td>
</tr>
<tr>
<td>Redis深度探险</td>
<td>钱文品</td>
</tr>
<tr>
<td>Google Protocol Buffer</td>
<td><a href="https://developers.google.com/protocol-buffers">Google Protocol Buffer</a></td>
</tr>
<tr>
<td>高性能服务器</td>
<td>游双</td>
</tr>
</tbody>
</table><h3><a id="__26"></a>２. 设计决策</h3>
<h4><a id="21__27"></a>2.1 设计目标</h4>
<h5><a id="211__28"></a>2.1.1 运行环境</h5>
<p>LInux Ubuntu18.10</p>
<h5><a id="212__30"></a>2.1.2 开发环境及工具</h5>
<p>vim+vimplus+gcc 编译器</p>
<h5><a id="213__32"></a>2.1.3 技术限制</h5>
<p>书读的有点少</p>
<h4><a id="22__34"></a>2.2 设计原则</h4>
<p>严格遵守需求规格说明书设计，使用一些面向对象语言中的一些设计模式例如工厂模式，减少代码的复用性。</p>
<h3><a id="3__38"></a>3. 逻辑架构设计</h3>
<h4><a id="31__39"></a>3.1 设计决策</h4>
<p>Server设计领域模型如下：</p>
<p><img src="https://img-blog.csdnimg.cn/2019100518300256.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"><br>
client设计：</p>
<p><img src="https://img-blog.csdnimg.cn/20191005180215199.png" alt="在这里插入图片描述"></p>
<h4><a id="32__47"></a>3.2 软件单元</h4>
<ul>
<li>server端<br>
直接看.cpp文件吧，类中的成员比较多，不好陈述，所以选择每个.cpp文件中的相关方法，将整个项目的功能实现加以描述。</li>
</ul>
<p><strong>aeEventloop.cpp</strong><br>
<img src="https://img-blog.csdnimg.cn/2019100518091652.png" alt="在这里插入图片描述"><br>
第一个就不说了，见名知意！<br>
addServerEvent主要功能就是初始化时创建监听套接字，并设置好相应的回调函数。<br>
addTimerEvent该方法创建一个eventfd并设置好相应的回调函数。<br>
aeEventloop构造方法，创建了时间堆，创建了epoll检测引擎<br>
下面是析构函数<br>
aeProcessEvent该方法处理不同的事件，当aeEventloop将事件收集到vecotor中，该方法就会将其中的各种方法进行分类处理。<br>
initDataInfo初始化时间堆中的相关数据<br>
kickClient处理不活跃的客户端<br>
notifyToSave 通知保存<br>
setCallBack 设置回调函数<br>
start 开始启动服务例程</p>
<p><strong>aeEpoll.cpp</strong></p>
<p><img src="https://img-blog.csdnimg.cn/20191005182201708.png" alt="在这里插入图片描述"><br>
很简单的操作，可以看懂！</p>
<p><strong>rpc.cpp</strong></p>
<p><img src="https://img-blog.csdnimg.cn/20191005182316250.png" alt="在这里插入图片描述"><br>
反序列化，发送响应请求。</p>
<p><strong>aeEvent.cpp</strong></p>
<p><img src="https://img-blog.csdnimg.cn/20191005182456602.png" alt="在这里插入图片描述"><br>
读回调，和写回调函数。</p>
<p><strong>timerHeap.cpp</strong></p>
<p><img src="https://img-blog.csdnimg.cn/20191005182621874.png" alt="在这里插入图片描述"><br>
MyTimer 时间堆上的节点，TimerManager是时间堆管理器。<br>
detect_timers检测到期时间，若有超时客户端，就执行回调函数处理。<br>
其它函数太鸡肋，不解释了。</p>
<p><strong>cmdProcess.cpp</strong></p>
<p><img src="https://img-blog.csdnimg.cn/20191005183040694.png" alt="在这里插入图片描述"><br>
findCmd在进行发序列化之后，判查找命令是否存在。<br>
initCmdCb初始化命令回调函数，每个命令对应一个回调函数。<br>
initRedis 初始化数据库（就是从文件中将已经持久化的数据读出来）<br>
processMsg处理不同的命令请求<br>
sendMsg发送消息</p>
<p><strong>cmdSet.cpp</strong><br>
<img src="https://img-blog.csdnimg.cn/20191005183500713.png" alt="在这里插入图片描述"></p>
<p>addObjectToDb添加对象到数据库。<br>
append增加数据库<br>
countRedis获取当前数据库的数量<br>
expend扩大容纳数据库容器的容量<br>
findCmd 查找命令<br>
getDB获取指定数据库对象<br>
initCmdCb初始化命令集合<br>
initRedis初始化数据库<br>
redisCommandProc 数据库各种命令处理函数，根据命令的不同回调在命令集中找相应的回调函数<br>
cb 数据库各种命令对象对应的回调函数</p>
<p><strong>redisDb.cpp和redisDb.h</strong><br>
因为是键值数据库，所以对于大部分对象而言操作比较相似，所以利用C++多态机制实现了工厂模式，父类为抽象类dbObject，设置了许多虚方法，对于strings和hashSet都是继承了dbObject，redisDb通过hashmap管理dbObject，如图。键值为请求的key对象，key对象，成员是要访问对象的键值，类型，以及数据库编号三元组唯一确定对象在数据库中的值。</p>
<p><img src="https://img-blog.csdnimg.cn/20191005185221263.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"><br>
<img src="https://img-blog.csdnimg.cn/20191005185333574.png" alt="在这里插入图片描述"></p>
<ul>
<li>redisDb.cpp<br>
<img src="https://img-blog.csdnimg.cn/20191005184214466.png" alt="在这里插入图片描述"><br>
getValue<br>
getValues hash获取值，将以vector对象的形式返回。</li>
</ul>
<p>该数据库管理对象的模型如下：</p>
<p><img src="https://img-blog.csdnimg.cn/20191005190226585.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"></p>
<ul>
<li>redisDb.h</li>
</ul>
<p><img src="https://img-blog.csdnimg.cn/20191005184715905.png" alt="在这里插入图片描述"></p>
<p>目前实现了两种类型的操作。<br>
<img src="https://img-blog.csdnimg.cn/20191005184438558.png" alt="在这里插入图片描述"><br>
抽象基类。<br>
<img src="https://img-blog.csdnimg.cn/20191005184510624.png" alt="在这里插入图片描述"></p>
<p><img src="https://img-blog.csdnimg.cn/20191005184546996.png" alt="在这里插入图片描述"><br>
hash操作，setValue实现多参数设置。<br>
<img src="https://img-blog.csdnimg.cn/20191005184647804.png" alt="在这里插入图片描述"><br>
strings对象，内部数据。<br>
<img src="https://img-blog.csdnimg.cn/20191005184614570.png" alt="在这里插入图片描述"><br>
equalFunc和hashFunc设置hash函数。<br>
factory工厂根据不同提示，返回不同对象。</p>
<p><img src="https://img-blog.csdnimg.cn/20191005190601375.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"><br>
<strong>rdb.cpp</strong><br>
持久化功能<br>
下面是两大对象在文件中的存储格式：</p>
<ul>
<li>hash<br>
<img src="https://img-blog.csdnimg.cn/20191005190903221.png" alt="在这里插入图片描述"></li>
<li>string</li>
</ul>
<p><img src="https://img-blog.csdnimg.cn/20191005191126967.png" alt="在这里插入图片描述"></p>
<p>ctp存的是类型编号，数字编号之类，在初始化数据库的时候，都会通过正则表达式辨别。然后其他解析之类的函数就不作解释了！可以看源代码。</p>
<p><img src="https://img-blog.csdnimg.cn/20191005191333172.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"><br>
<strong>recoverDb.cpp</strong></p>
<p>恢复数据库，因为我将每个数据库单独存在一个文件中，0号数据存在.db_0文件，后面以此类推，其中或有一个redis_fileName文件，存放的是数据库文件名，在恢复的时候，先读取.redis_fileName，将所有数据库文件名存在vector中，然后遍历vector再通过mmap映射将文件内容映射到string中，然后解析string就行。</p>
<p><img src="https://img-blog.csdnimg.cn/20191005191724556.png" alt="在这里插入图片描述"><br>
<img src="https://img-blog.csdnimg.cn/20191005191504661.png" alt="在这里插入图片描述"></p>
<p>客户端设计比较简单，读者可自行研究！</p>
<h4><a id="33__171"></a>3.3 处理流程</h4>
<p><img src="https://img-blog.csdnimg.cn/20191005213737701.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"></p>
<h3><a id="4__176"></a>4 运行演示</h3>
<p><img src="https://img-blog.csdnimg.cn/20191005214320966.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQxNjgxMjQx,size_16,color_FFFFFF,t_70" alt="在这里插入图片描述"><br>
<a href="https://github.com/Request2609/cacheDb/tree/master/cacheDb2.0">源代码</a> 感谢star或者fork！本代码仅供阅读参考学习，在个人平台上运行的话可能得安装配置google protocol buffer！并且要考虑系统兼容性！</p>
</div>
</body>

</html>
