#include "request.h"

class cmds ;
void cmds :: build() {
    cmdList.insert(make_pair("set", 3)) ;
    cmdList.insert(make_pair("get", 2)) ;
    //验证命令
    cmdList.insert({"bgsave", 1}) ;
}

cmds :: cmds() {
}

cmds :: ~cmds() {}

void request :: setSet(vector<string>& res, Command& com) {
    cout << "set 命令" << endl ;
    com.set_cmd(res[0]) ;
    Key* key = com.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
    Value* val = com.add_vals() ;
    string* v = val->add_val() ;
    //设置数据库编号
    com.set_num(0) ;
    *v = res[2] ;
}

void request :: setGet(vector<string>& res, Command& cmd) {
    cmd.set_cmd("get") ;
    Key* key = cmd.add_keys() ;
    string* k = key->add_key() ;
    *k = res[1] ;
}

void request :: setBgSave(vector<string>&res,Command& com) {
    com.set_cmd(res[0]) ;
}
//匹配键值对
//创建命令表
int request :: processCmd(vector<string>&res, Command&com) {
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
        setSet(res, com) ;
    } 
    //get命令
    else if(!strcasecmp(res[0].c_str(), "get")) {
        int ret = cd.cmdList[res[0]] ;
        if(ret != len) {
            cout << "error command!" << endl ;
            return -1 ;
        }   
        setGet(res, com) ;
    }

    else if(!strcasecmp(res[0].c_str(), "bgsave")) {
        int ret = cd.cmdList[res[0]] ;
        if(ret != len) {
            cout << "error command!" << endl ;
            return -1 ;
        }
        setBgSave(res, com) ;
    }
    else {
        cout << "command not found" << endl ;
        return -1 ;
    }
    return 1 ;
}


int request :: sendReq(int fd, vector<string>&res, int num) {
    cout << "发送消息！" << endl ;
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
        cout <<"处理命令完成！＠" << endl ;
        //序列化,并发送给服务器
    }
    int s = sendAfterSerial(fd, cmd) ;
    return s ;
}

//判断是否与服务器断开连接
int request :: isConnect(int conFd) {
    
    struct tcp_info info ;
    int len = sizeof(info) ;
    int ret = getsockopt(conFd, IPPROTO_TCP, TCP_INFO, &info, (socklen_t*)&len) ;
    cout << "连接" << endl ;
    if(ret < 0) {
        cout << __FILE__ << "     " <<__LINE__ <<"     "<< strerror(errno)<< endl ; 
        return -1 ;
    }
    cout << "连接成功！" << endl ;
    //连接正常返回
    if(info.tcpi_state == TCP_ESTABLISHED) {
        return 1 ;
    }
    else {
        return 0 ;
    }
}
///////////////////
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
    cout << "刚发送完成！" << endl; 
    cmd.clear_cmd() ;
}

