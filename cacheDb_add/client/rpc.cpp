#include "rpc.h"

//处理结果，并返回相应的结果
Response rpc :: getParseString(string* buff) {
    //在消息处理处，反序列化
    auto res = parseMethod(buff) ;
    cout << res.reply()<< endl ;
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
    string res  ;
    char buf[BFSIZE] ;
    int n = 0 ;
    if((n = read(conFd, buf, sizeof(buf))) < 0) {
        cout << __FILE__ << "           " << __LINE__<< "  " << strerror(errno)<< endl ;
        return -1 ;
    }
    Response re ;
    //反序列化
    re.ParseFromArray(buf, sizeof(buf)) ;
    res = re.reply() ;
    cout << re.reply() << endl ;
    return 1 ;
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
    servFd =  conFd ;
    return conFd ;
}

int rpc :: init() {
    conFd = -1 ;
    pool = make_shared<threadpool>(8) ;
    que = syncQueue::getQueue() ;
}

int rpc :: sendRequest(vector<string>&argStl, int num) { 
    int count = 0 ;
    int flag = 0 ;
    this->num = num ;
    int ret = request(conFd, argStl, num) ; 
    if(ret < 0) {
        return -5 ;
    }
    //断开连接，重新连接五次
    if(ret == 5) {
        while(count < 5) {
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

