#include "cmdProcess.h"

//将结果集返回
int cmdProcess :: findCmd(const shared_ptr<Command>tmp) {
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
    buffer* bf = tmp->getBuf() ;
    //获取到对端序列化的结果
    string* buff = bf->getBuf() ;
    //获取对端序列化到结果
    //反序列化,弱引用
    shared_ptr<Command>wcmd = rc->getParseString(buff) ;
    //获取到相应的智能指针后，进行解析
    int ret = findCmd(wcmd) ;
    Response res ;
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
        cmdSet_->redisCommandProc(num, wcmd) ;
        shared_ptr<Response>r = cmdSet_->getResponse() ;
        res.set_reply(r->reply()) ;
        //销毁响应的智能指针
        r = nullptr ;
    } 
    shared_ptr<Response>re(new Response(res)) ;
    rc->response(re, tmp->getConnFd()) ;
    bf->clear() ;
    //获取到响应的结果
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

