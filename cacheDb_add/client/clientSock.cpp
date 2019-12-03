#include "clientSock.h"

int clientSock :: anetCreateSock() {

    sockFd = socket(AF_INET, SOCK_STREAM, 0) ;
    if(sockFd < 0) {
        cout << __FILE__ << "      " << __LINE__ << endl ;
        return  -1;
    }
    return sockFd ;
}       

int clientSock :: clientConnect(string ip, string port) {
    int ret = 0 ;
    addr.sin_family = AF_INET ;
    addr.sin_port = htons(atoi(port.c_str())) ;
    addr.sin_addr.s_addr = inet_addr(ip.c_str()) ;
    ret = connect(sockFd, (struct sockaddr*)&addr, sizeof(addr)) ;
    if(ret < 0) {
        cout << __FILE__  << "      " << __LINE__ << "   " << endl ;
    }
    return  sockFd;
}
