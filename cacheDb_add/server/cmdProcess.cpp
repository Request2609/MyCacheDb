#include "cmdProcess.h"

//将结果集返回
int cmdProcess :: findCmd(shared_ptr<Command>tmp) {
    int ret = 0 ;
    pair<string, vector<string>>t;
    string cc = tmp->cmd() ;
    //查找消息
    ret = cmdSet_->findCmd(cc) ;
    return ret ;
}

//向数据库中导入数据
int cmdProcess :: initRedis() {
    //导入数据库中的数据
    cmdSet_->initRedis() ;
}
//消息回调
void cmdProcess :: timeCb() {
    //保存当前数据中的所有数据
}

void cmdProcess :: initCmdCb() {
    cmdSet_->initCmdCb() ;
}

//处理消息
int cmdProcess :: processMsg(shared_ptr<aeEvent>&tmp) {
    int flag = 0 ;
    buffer* bf = tmp->getBuf() ;
    //获取到对端序列化的结果
    string* buff = bf->getBuf() ;
    //获取对端序列化到结果
    //反序列化,弱引用
    shared_ptr<Command>wcmd = rc->getParseString(buff) ;
    ListObject lob = wcmd->lob() ;
    string key = lob.key() ;
   // cout << "传过来的键值:" << key << endl ;
    Value* val =  lob.add_vals() ;
    //获取到相应的智能指针后，进行解析
    int ret = findCmd(wcmd) ;
    shared_ptr<Response> res = nullptr;
    //解析命令不合法
    if(ret == NOT_FOUND) {
        //给客户端发送处理结果
        //序列化，并向客户端发送消息，暂时不做处理
        res = backInfo :: notFind() ;
    } 
    else {
        //处理命令
        string cc = wcmd->cmd() ;
        int num = wcmd->num() ;
        //获取当前所在数据库
        //没找到
        //只需要关注blpop是否成功，不成功将当前处理的事件加到
        //队列中，并设置定时器
        int a = cmdSet_->redisCommandProc(num, wcmd) ;
        res = cmdSet_->getResponse() ;
        //cout <<"回复的结果：--->" << res->reply() << endl ;
        //链表阻塞形式获取对象的回复，a=0队列为空
        if(a == 0) {
            flag = 1 ;
            listWaitQueue :: add(tmp) ;
            //创建一个定时器，时间到就返回客户端
            int t = wcmd->time() ;
            //设置定时器 
            setClock(tmp, t) ;
            return 1;
        }
    } 

    if(flag != 1) {
        cout << "智能指针设置的值:" << res->reply() << endl ;
        rc->response(res, tmp->getConnFd()) ;
        bf->clear() ;
    }
    //获取到响应的结果
    return 1 ;
}

//设置定时事件
void cmdProcess :: setClock(shared_ptr<aeEvent>aet, unsigned int t) {
    //将事件加到队列中
    listWaitQueue :: add(aet) ;
    //将该对象加到定时器中
    shared_ptr<TimerManager> tman = 
        timeManagerFactory::getManager(2) ;
    MyTimer  timer(tman) ;
    int index = timer.getIndex() ;
    //设置在堆中的index，在删除的时候方便删除
    aet->setIndex(index) ;
    timer.setTimeSlot(signalSet::timeSlot) ;
    timer.setFd(aet->getConnFd()) ;
    timer.setTimeSlot(t) ;
    timer.start(&cmdProcess :: responseFunc, t, MyTimer::TimerType::ONCE) ;
}   

int cmdProcess :: responseFunc(int fd) {
    string aa = "null" ;
    Response res ;
    res.set_reply("null") ;
    res.SerializeToString(&aa) ;
    char buf[4096] ;
    bzero(buf, sizeof(buf)) ;
    strcpy(buf, aa.c_str()) ;
    //向客户端发送响应
    send(fd, buf, sizeof(buf), 0) ;
    listWaitQueue :: del(fd) ;
    return 1 ;
}

int cmdProcess :: sendMsg(shared_ptr<aeEvent>tmp) {
    int fd = tmp->getConnFd() ;
    Command cmd ;
    char buf[SIZE] ;
    cmd.set_status(0) ;
    int ret = write(fd, buf, sizeof(buf)) ;
    if(ret < 0) {
        cout << __FILE__ << "     " << __LINE__ << endl ;
        return -1 ;
    }
    return 1 ;
}

