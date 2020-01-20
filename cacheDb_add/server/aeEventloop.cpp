#include "aeEventloop.h"

int aeEventloop :: efd ;
int aeEventloop ::  wakeBlpopFd ;
//初始化事件
//创建epoll句柄等工作
aeEventloop :: aeEventloop() {
    maxFd = -1 ;
    setSize = -1 ;
    stop = false ;
    //创建时间管理
    apiData = NULL ;
    aeEventloop::wakeBlpopFd = signalSet :: createWakeBlpop() ;
}

int aeEventloop:: init() {
    //从工厂中直接获取
    tman = timeManagerFactory::getManager(1) ;
    wakeblpop = timeManagerFactory::getManager(2) ;
    //刚开始创建16个数据库
    //创建一个epoll对象
    aep = make_shared<aeEpoll>() ;
    aep->epCreate(SIZE) ;
}

//添加监听套接字，及注册相应的读回调函数
int aeEventloop :: addServerEvent(string addr, string port) {
    //将相应套接字加入到epoll中
    shared_ptr<aeEvent> ae  = make_shared<aeEvent>() ; 
    //先获取套接字对象
    aeSocket *aeSock = ae->getSocket() ;  
    //创建套接字对象
    int servFd = aeSock->anetCreateSocket() ;
    //将监听套接字保存单独容器中
    listenFd.push_back(servFd) ;
    //绑定端口地址并进行监听
    aeSock->tcpServer(port.c_str(), addr.c_str(), 20) ;
    //设置非阻塞
    aeSock->setNoBlocking(servFd) ;
    //将该描述符加入到epoll中
    int ret = aep->add(servFd, READ) ;
    if(ret < 0) {
        return -1 ;
    }
    eventData[servFd] = ae ;
    return 1 ;
}   

void aeEventloop :: initDataInfo() {
    MyTimer ::data = &eventData ;
    MyTimer :: aep = aep ;
}

//开始监听事件
int aeEventloop :: start() {
    //初始化数据信息
    initDataInfo() ;
    signalSet :: efd = signalSet  :: createEventFd() ;
    efd = signalSet :: efd ;
    aep->add(efd, READ) ;
    //创建eventFd，内部已经设置了非阻塞
    //设置信号
    signalSet :: addSig(SIGALRM) ;
    //设置时钟信号
    signalSet :: setAlarm(signalSet :: timeSlot) ;
    while(!stop) {
        int ret = aep->wait(fireList) ;
        if(ret < 0 && errno != EINTR) {
            return -1 ;
        }
        vector<epoll_event>ls  = fireList ;
        int len = fireList.size() ;
        for(int i=0; i<len; i++) {
            eventfd_t count ;
            int fd = ls[i].data.fd ;
            //是唤醒等待阻塞链表的fd
            if(efd == fd) {
                //同时唤醒
                int ret = read(efd, &count, sizeof(count)) ;
                if(ret < 0) {
                    cout << __LINE__ << "       " << __FILE__ << endl ;
                    return -1 ;
                }
                //检查时间堆
                tman->detect_timers(1) ;
                //检测是否有阻塞的客户端
                wakeblpop->detect_timers(2) ;
                //继续添加超时时间
                signalSet :: setAlarm(signalSet :: timeSlot) ;
                continue ;
            }
            //设置epoll_event
            eventData[fd]->setEvent(&ls[i]) ;
            //处理完成以后
            aeProcessEvent(fd) ;
        }
        //清除活跃事件表
        fireList.clear() ;     
    }
    return 1 ;
}

int aeEventloop :: notifyToSave(int fd) {
    uint64_t ret = 1 ;
    int res = write(fd, &ret, sizeof(ret)) ;
    if(res < 0) {
        cout << __FILE__ << "    " << __LINE__ << endl ;
        return -1 ;
    }
    return 1 ;
}

//处理事件
int aeEventloop :: aeProcessEvent(int fd) {
    epoll_event* ev = eventData[fd]->getEvent() ;
    if(ev->events&READ) {
        //如果找a到fd就退出
        auto find = [&]()->int {
            int ret = 0 ;
            int len = listenFd.size() ;
            for(int i=0; i<len; i++) {
                if(fd == listenFd[i]) {
                    ret = 1 ;
                    break ;
                }
            }
            return ret ;
        };
        //该fd要是能在监听套接字中找到，新事件
        if(find() == 1) {
            int ret = acceptNewConnect(fd) ;
            if(ret < 0) {
                return 0 ;
            }
        }
        //其他可读事件
        else {
            //先检测是否为定时事件
            int ret = eventData[fd]->processRead() ; 
            //tman->detect_timers() ;
            //收到处理失败
            //读到０表示退出
            if(ret == 0) {
                //持久化该客户端的数据
                aep->del(fd) ;
                //从eventData删除fd对应的对象
                map<int, shared_ptr<aeEvent>>::iterator ls= eventData.find(fd) ;
                ///删除相应的数据               
                eventData.erase(ls) ;
            }
            //修改客户端超时时间
            tman->modify_timers(fd) ;
        }
    }
    //可写事件
    else if(ev->events&WRITE) {
        int ret = eventData[fd]->processWrite()  ; 
        if(ret < 0) {
            return -1 ;
        }
    }
    return 1 ;
}

int aeEventloop :: acceptNewConnect(int fd) {
    //接收新连接
    int newFd = eventData[fd]->getSocket()->acceptClient() ;
    if(newFd < 0) {
        cout << __FILE__ <<  "       " << __LINE__ << " " << strerror(errno)<< endl ;
        return -1 ;
    }
    //创建.相应的aeEvent事件
    //设置度写回调函
    shared_ptr<aeEvent>tmp = make_shared<aeEvent>() ;
    //设置数据库号码，刚开始是０号数据库
    tmp->setNum(0) ;
    //设置事件非阻塞
    aeSocket :: setNoBlocking(newFd) ;
    //加入到事件列表
    eventData[newFd] = tmp ;
    tmp->setReadCallBack(readCall) ;
    tmp->setWriteCallBack(writeCall) ;
    tmp->setConnFd(newFd)  ;
    tmp->setServFd(fd) ;
    tmp->setAep(aep) ;
    //将事件加入到epoll中
    aep->add(newFd, READ) ;
    //为每个描述符设置超时时间
    MyTimer timer(tman) ;
    //设置超时时间
    timer.setFd(newFd) ;
    timer.setTimeSlot(signalSet :: timeSlot) ;
    //事件没到就循环在累加上一定的事件继续等待
    timer.start(&aeEventloop :: kickClient, signalSet :: timeSlot, MyTimer :: TimerType ::CIRCLE) ;
    return 1 ;
}

//给客户端回送响应

//断连接的函数
int aeEventloop :: kickClient(map<int, shared_ptr<aeEvent>>&eventData, 
                              int kickFd, 
                              shared_ptr<aeEpoll>&aep) {
    cout << "是否为空：" << eventData.empty() << endl ;
    if(eventData.empty())
        cout << "找不到了！"<< endl ;
    else {
        cout << eventData.size() << endl ;
    }
    auto res = eventData.find(kickFd) ; 
    if(res == eventData.end()) {
        cout << __FILE__ << "          " << __LINE__ << endl ;
        return -1 ;
    }
    eventData.erase(res) ;
    //epoll中移除该描述符
    aep->del(kickFd) ;
    close(kickFd) ;
    return 1 ;
}


//添加时间事件
int aeEventloop :: addTimerEvent() {
    shared_ptr<aeEvent> aev = make_shared<aeEvent>() ;
    int efd = eventfd(0, 0) ;
    timeFd = efd ;
    aev->setNoBlock(efd) ;
    aev->setReadCallBack(timerCall) ;
    aev->setConnFd(efd) ;
    aep->add(efd, EPOLLIN) ;
    eventData[efd] = aev ;
    return efd ;
}

//在服务器启动的时候执行,设置回调函数
void aeEventloop :: setCallBack(callBack readCb, callBack writeCb) {
    //设置callback函数
    readCall = readCb ;
    writeCall = writeCb ;
}

aeEventloop :: ~aeEventloop() {
        
}
