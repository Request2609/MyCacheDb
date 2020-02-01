#include "rpc.h"

shared_ptr<aeEpoll> rpc::aep ;
//处理结果，并返回相应的结果
Response rpc :: getParseString(string* buff) {
    //在消息处理处，反序列化
    auto res = parseMethod(buff) ;
    return res ;
}

rpc :: rpc() {
    cmd = make_shared<Command>() ;
}

rpc :: ~rpc() {

}

void rpc :: setCallMethod(call cb) { 
    request = move(cb); 
}   

int rpc :: getResponse() {

    int flag = clientLoop::mode ;
    if(flag == 0) {
        vector<epoll_event>ls ;
        if(rpc::aep->wait(ls)) {
            int num = ls.size() ;
            for(int i=0; i<num; i++) {
                if(ls[i].events&EPOLLIN) {
                    //使用线程池处
                    int fd = ls[i].data.fd ;
                    pool->commit(&rpc::readInfo, fd) ;
                }
            }   
        }
    }
    else {
        string res  ;
        char buf[BFSIZE] ;
        int n = 0 ;
        n = read(conFd, buf, sizeof(buf)) ;
        if(n < 0) {
            cout << __FILE__ <<"    " << __LINE__ << strerror(errno)<< endl ;
            return 0;
        }
        Response re ;
        //反序列化
        re.ParseFromArray(buf, sizeof(buf)) ;
        res = re.reply() ;
        cout << res << endl ;
    }
    return 1 ;
}

void rpc :: readInfo(int fd) {
    string res  ;
    char buf[BFSIZE] ;
    int n = 0 ;
    n = read(fd, buf, sizeof(buf)) ;
    if(n < 0) {
        cout << __FILE__ <<"    " << __LINE__ << strerror(errno)<< endl ;
        return ;
    }
    Response re ;
    //反序列化
    re.ParseFromArray(buf, sizeof(buf)) ;
    res = re.reply() ;
    auto que = syncQueue:: getQueue() ;
    que->addToResponse(atol(re.seq().c_str()), re.reply()) ; 
    rpc::aep->modify(fd, EPOLLIN|EPOLLONESHOT) ;
    return  ;
}

void rpc :: setCallMethod(parse par) {
    parseMethod = move(par) ;
}

//连接服务器
int rpc :: Connect(int& servFd) {
    client.anetCreateSock() ;
    conFd = client.clientConnect(ipPort.first, ipPort.second) ;
    if(conFd < 0) {
        servFd = -1;
        return -1 ;
    }

    if(clientLoop::mode == 0) {
        client.setnoBlocking() ;
        servFd =  conFd ;
        rpc::aep->add(conFd, EPOLLIN|EPOLLONESHOT) ;
    }
    return conFd ;
}

int rpc :: init() {
    conFd = -1 ;
    pool = make_shared<threadPool>(8) ;
    que = syncQueue::getQueue() ;
    rpc::aep = make_shared<aeEpoll>() ;
    rpc::aep->epCreate(1024) ;
}

int rpc :: sendRequest(vector<string>&argStl, int num) { 
    int count = 0 ;
    int flag = 0 ;
    this->num = num ;
    int ret = request(conFd, argStl, num) ; 
    if(ret < 0) {
        cout << "没有发送成功!" << endl ;
        return -5 ;
    }
    //断开连接，重新连接五次
    if(ret == 5) {
        while(count < 5) {
            ret  = Connect(conFd) ;
            if(ret < 0) {
                count ++ ;
                return 5 ;
            }
            else {
                flag = 1 ;
                break ;
            }
        }
    }
    else {
        return conFd ;
    }
    ret = request(conFd, argStl, num) ;
    if(flag == 1) {
        return conFd ;
    }
    return 1 ;
}

