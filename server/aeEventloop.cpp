#include "aeEventloop.h"
int aeEventloop :: efd ;
int aeEventloop ::  wakeBlpopFd ;
int aeEventloop:: canSave ;
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
	//初始化检查的规则
	//从工厂中直接获取
	auto save = saveTimerHandle::getSaveTimerObject() ;
	tman = make_shared<TimerManager>() ;
	aeSocket::createSocketPair() ;
	saveFd = aeSocket::getReadFd() ;
	canSave = 1 ;
	//创建一个epoll对象
	aep = make_shared<aeEpoll>() ;
	aep->epCreate(SIZE) ;
	save->setEpoll(aep) ;
}
int getIpPort(string& ip, string&port) {
	ifstream in("../conf/IP_PORT.conf", ios::in|ios::out) ;
	if(in.fail()) {
		cout << __FILE__ << "     " << __LINE__ << endl ;
		return -1 ;
	}
	string info;
	while(!in.eof()) {
		in>> info >> ip ;
		in>> info >> port ;
	}
	in.close() ;
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
	auto sth = saveTimerHandle::getSaveTimerObject() ;
	//启动备份文件事件
	sth->startDetect() ;
	//初始化数据信息
	initDataInfo() ;
	//将备份通知句柄加到epoll
	while(!stop) {
		int ret = aep->wait(fireList, timeSlot) ;
		if(ret < 0 && errno != EINTR) {
			cout << strerror(errno) << endl ;
			return -1 ;
		}
		//定时器是事件
		if(ret == 0) {
			//探测以便事件堆
			tman->detect_timers(1) ;
			continue ;
		}
		vector<epoll_event>ls  = fireList ;
		int len = fireList.size() ;
		for (int i=0; i<len; i++) {
			int fd = ls[i].data.fd ;
			//检测fd为持久化事件的句柄
			int ret = sth->isSaveHandle(fd) ;
			if(ret != -1) {
				continue ;
			}
			if(fd == saveFd) {
				read(saveFd, &ret, sizeof(ret)) ;
				canSave = 1 ;
				continue ;
			}
			if(eventData.find(fd) == eventData.end()) {
				continue ;
			}
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
			for (int i=0; i<len; i++) {
				if(fd == listenFd[i]) {
					ret = 1 ;
					break ;
				}
			}
			return ret ;
		}
		;
		//该fd要是能在监听套接字中找到，新事件
		if(find() == 1) {
			int ret = acceptNewConnect(fd) ;
			if(ret < 0) {
				return 0 ;
			}
		}
		//其他可读事件 else {
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
	//可写事件 else if(ev->events&WRITE) {
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
	if(eventData.empty())
	        cout << "找不到了！"<< endl ; 
	else {
		cout << eventData.size() << endl ;
	}
	auto res = eventData.find(kickFd) ;
	if(res == eventData.end()) {
		return -1 ;
	}
	cout << "删除了套接字为:" <<kickFd << endl ;
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
