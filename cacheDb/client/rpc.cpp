#include "rpc.h"

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

string rpc :: getResponse() {
    string res  ;
    char buf[BFSIZE] ;
    if(read(conFd, buf, sizeof(buf)) < 0) {
        cout << __FILE__ << "           " << __LINE__<< "  " << strerror(errno)<< endl ;
        return "" ;
    }
    Response re ;
    //反序列化
    re.ParseFromArray(buf, sizeof(buf)) ;
    res = re.reply() ;
    return res ;
}
void rpc :: setCallMethod(parse par) {
    parseMethod = move(par) ;
}

//连接服务器
int rpc :: Connect() {
    client.anetCreateSock() ;
    conFd = client.clientConnect(ipPort.first, ipPort.second) ;
    if(conFd < 0) {
        return -1 ;
    }
    return conFd ;
}

int rpc :: sendRequest(vector<string>&argStl, int num) { 
    int count = 0 ;
    this->num = num ;
    cout<< "hhhhhhhhhhhhh" << endl ;
    int ret = request(conFd, argStl, num) ; 
    //断开连接
    if(ret == 5) {
        while(count < 5) {
            conFd = Connect() ;
            if(ret < 0) {
                count ++ ;
                return 5 ;
            }
            else {
                break ;
            }
        }
    }
    else {
        return 1 ;
    }
    ret = request(conFd, argStl, num) ;
    if(ret < 0) {
        return -1 ;
    }
}
