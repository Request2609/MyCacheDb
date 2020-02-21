#include "clientLoop.h"

int clientLoop:: mode = 0;
//使用gpb命名空间
std::vector<std::string> clientLoop :: split(const std::string &s, const std::string &seperator){
    std::vector<std::string> result;
    typedef std::string::size_type string_size;
    string_size i = 0;
    while(i != s.size()){
        //找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while(i != s.size() && flag == 0){ 
            flag = 1;
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[i] == seperator[x]){
                    ++i;
                    flag = 0;
                    break;
                }
        }
        //找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){ 
            for(string_size x = 0; x < seperator.size(); ++x)
                if(s[j] == seperator[x]){
                    flag = 1;
                    break;
                }
            if(flag == 0)                                                                                                                                                                                                                                                       
                ++j;
        }
        if(i != j){ 
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }   
    return result;
}

int clientLoop :: getIpPort() {
    std::ifstream in("../conf/IP_PORT.conf", std::ios::in|std::ios::out) ;   
    if(in.fail()) {
        std::string s =std::to_string(__LINE__) +"  "+ +strerror(errno) + __FILE__;
        aofRecord::log(s) ;
        return -1 ;
    }
    std::string info;
    while(!in.eof()) {
        in>> info >> ip ;
        in>> info >> port ;
    }
    in.close() ;
}

int clientLoop::init(std::string ip, std::string port) {
    client = std::make_shared<clientSock>() ;
    cmd = std::make_shared<cmds>() ;
    this->ip = ip ;
    this->port = port ;
    rc = std::make_shared<rpc>() ;
    rc->init() ;
    rc->setAddress(ip, port) ;
    rc->setCallMethod(request::sendReq) ;
    return 1 ;
}

int clientLoop::init() {
    int ret = getIpPort() ;
    if(ret < 0 || ip.empty()||port.empty()) {
        return -1;
    }
    client = std::make_shared<clientSock>() ;
    cmd = std::make_shared<cmds>() ;
    rc = std::make_shared<rpc>() ;
    rc->init() ;
    rc->setAddress(ip, port) ;
    rc->setCallMethod(request::sendReq) ;
    return 1 ;
}

int clientLoop :: sendRequest(std::string md, ...) {
    va_list va ;
    int num = cmd->isCmdExist(md) ;
    if(num < 0) {
        std::cout << "命令不存在" << std::endl ;
        return -1;
    }
    va_start(va, md) ;
    char* val = va_arg(va, char*) ;
    std::vector<std::string>ls ;
    ls.push_back(md) ;
    while(strcasecmp(val, END)) {
        ls.push_back(val) ;
        val = va_arg(va, char*) ;
    }
    if(ls.empty())  {
        return  -1;
    } 

    va_end(va) ;
    if(servFd <= 0) {
        int ret = rc->Connect(servFd) ;
        if(ret < 0) { 
            std::cout << "连不上服务器" << std::endl ;
            return  -1;
        }
    }
    int res = rc->sendRequest(ls, num) ;
    if(res == -5 || res == -1) {
        return -1;
    }
    if(res != 1) {
        servFd = res ;
    }
    return 1 ;
}

int clientLoop::getResult() {

    auto que = syncQueue::getQueue() ;
    int num = 0 ;
    rc->getResponse() ;
    while((num=que->reqEmpty())) {
        if(que->resEmpty()) {
            que->getResponse() ;
            break ;
        }
    }
    return 1 ;
}

void clientLoop :: start() {

    clientLoop::mode=1 ;
    rc->setAddress(ip, port) ;
    //连接服务器
    int ret = rc->Connect(servFd) ;
    if(ret < 0) {
        std::cout << "服务器找不到！" << std::endl ;
        return ;
    }
    num = 0 ;
    //处理命令
    while(!stop) {
        std::string cmd, res; 
        char* p = readline("myRedis >> ") ;
        //加入历史列表
        add_history(p) ;
        cmd = p ;
        if(cmd.size() == 0) {
            continue ;
        }
        if(!strcasecmp(cmd.c_str(), "clear") || !strcasecmp(cmd.c_str(), "cls")) {
            system("clear") ;
            continue ;
        }
        //解析命令
        cmdStl = split(cmd, " ") ;
        //退出
        if(cmd == "quit" || cmd == "q") {
            rc->disConnect() ; 
            std::cout << "bye bye!" << std::endl ; 
            break ;
        }
        //解析并序列化发送命令
        int ret = rc->sendRequest(cmdStl, num) ;
        if(ret == -5 || ret == -1) {
            continue ;
        }
        //返回结果并打印
        rc->getResponse() ;
        //序列化
    }
}

int clientLoop :: setEndSig() {

}

//向服务器发送请求
int clientLoop :: sendRequest(std::string& res) {
    int ret = 0;
    int len = res.size() ;
    if(len < SIZE) {
        //设置最后一个元素为１，表示已经发送完成
        res+='1' ;
        char buff[SIZE] ;
        strcpy(buff, res.c_str()) ;
        //使用servFd发送消息
        ret = writen(servFd, buff, sizeof(buff)) ;  
        if(ret < 0) {
            std::string s =std::to_string(__LINE__) +"  "+ +strerror(errno) + __FILE__;
            aofRecord::log(s) ;
            return  -1 ;
        }
    }
    return ret ;
}

int clientLoop :: processMsg(Messages::Command& cmd, std::string& res) {
    int ret = 0 ;
    //第一个是命令
    //第二个是key
    //第三个是value
    int len = cmdStl.size() ;
    if(len < 3) {
        return -1 ;
    }
    //第一个是命令
    cmd.set_cmd(cmdStl[0]) ;
    //第二个是key
    Messages::Key* keys = cmd.add_keys() ;
    
    //设置值
    Messages::Value* val = cmd.add_vals() ;
    
    //设置值,值可能重复
    for(int i=2; i<len; i++) {
        std::string* a = val->add_val() ;
        *a = cmdStl[i] ;
    }
    //将设置好的结果序列化成string
    cmd.SerializeToString(&res) ;
    return ret ;
}
