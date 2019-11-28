#include "request.h"

class cmds ;
void cmds :: build() {
    cmdList.insert(make_pair("set", 3)) ;
    cmdList.insert(make_pair("get", 2)) ;
    //验证命令
    cmdList.insert({"save", 1}) ;
    //-1代表不定参数
    cmdList.insert({"hset", -1}) ;
    cmdList.insert({"hget", 3}) ;
    cmdList.insert({"bgsave", 1}) ;
    cmdList.insert({"lpush", -1}) ;
}

cmds :: cmds() {
}

cmds :: ~cmds() {}

//匹配键值对
//创建命令表
int request :: processCmd(vector<string>&res, Command& com) {
    //创建命令集合
    static cmds cd ;
    cd.build() ;
    int len = res.size() ;

    if(!strcasecmp(res[0].c_str(), "set")) {
        //获取该命令的参数个数
        int ret = cd.cmdList[res[0]] ;
        if(len != ret) {
            cout << "error usage" << endl ;        
            return -1 ;
        }
        cmdProcess :: setSet(res, com) ;
    } 

    else if(!strcasecmp(res[0].c_str(), "lpush")) {
        cmdProcess :: getListObject(res, com) ;
    }

    //get命令
    else if(!strcasecmp(res[0].c_str(), "get")) {
        int ret = cd.cmdList[res[0]] ;
        if(ret != len) {
            cout << "error command!" << endl ;
            return -1 ;
        }   
        cmdProcess :: setGet(res, com) ;
    }

    else if(!strcasecmp(res[0].c_str(), "save")) {
        int ret = cd.cmdList[res[0]] ;
        if(ret != len) {
            cout << "error command!" << endl ;
            return -1 ;
        }
        cmdProcess :: setSave(res, com) ;
    }

    else if(!strcasecmp(res[0].c_str(), "hset")) {
        //不定参数,哈希表的总参数个数是偶数
        if(cd.cmdList[res[0]] != -1 || (len%2))  {
            cout << "error command!" << endl ;
            return -1 ;
        } 
        cmdProcess :: setHset(res, com) ;
    }

    else if(!strcasecmp(res[0].c_str(), "hget")) {
        int ret = cd.cmdList[res[0]] ;
        if(ret != len) {
            cout << "command error!" << endl ;
            return -1 ;
        }
        cmdProcess :: setHget(res, com) ;
    }
    else if(!strcasecmp(res[0].c_str(), "bgsave")) {
        int ret = cd.cmdList[res[0]] ; 
        if(ret != len) {
            cout << "command error!" << endl ;
            return -1 ;
        }
        cmdProcess :: setSave(res, com) ;
    }
    else {
        cout << "command not found" << endl ;
        return -1 ;
    }
    return 1 ;
}


int request :: sendReq(int fd, vector<string>&res, int num) {

    Command cmd ;
    cmds cd ;
    cd.build() ;
    auto ret = cd.cmdList.find(res[0]) ;
    cmd.set_num(num) ;
    //没找到命令
    if(ret == cd.cmdList.end()) {
        cout << "command not found!"<< endl ;  
        return -1;
    }
    else {
        //从第一个数据
        int r = processCmd(res, cmd) ;
        if(r < 0) {
            return -1;
        }
    }
    int s = sendAfterSerial(fd, cmd) ;
    //cout << "fasongwancheng!" << endl ;
    return s ;
}

//判断是否与服务器断开连接
int request :: isConnect(int conFd) {
    
    struct tcp_info info ;
    int len = sizeof(info) ;
    int ret = getsockopt(conFd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&len) ;
    if(ret < 0) {
        cout << __FILE__ << "     " <<__LINE__ <<"     "<< strerror(errno)<< endl ; 
        return -1 ;
    }
    //连接正常返回
    if(info.tcpi_state == TCP_ESTABLISHED) {
        return 1 ;
    }
    else {
        return 0 ;
    }
}

int request :: sendAfterSerial(int fd, Command& cmd) { 
    string a ;
    //序列化的结果
    cmd.SerializeToString(&a) ;
    int len = a.size() ;
    //检验是否与服务器器建立了连接
    int ret = isConnect(fd) ;
    if(ret == 0) {
        return 5 ;
    }
    char buff[REQ_SIZE] ;
    bzero(buff, sizeof(buff)) ;
    strcpy(buff, a.c_str()) ;
    if(send(fd, buff, sizeof(buff), 0) < 0) {
        cout << "errno connect" << endl ;
        return -1;
    }
    //cout << "发送完成！hahah" << endl ;
    cmd.clear_cmd() ;
    return 1 ;
}

