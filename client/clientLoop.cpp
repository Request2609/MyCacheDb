#include "clientLoop.h"

int clientLoop:: mode = 0;
//使用gpb命名空间
vector<string> clientLoop :: split(const string &s, const string &seperator){
    vector<string> result;
    typedef string::size_type string_size;
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

int clientLoop::init(string ip, string port) {
    client = make_shared<clientSock>() ;
    cmd = make_shared<cmds>() ;
    this->ip = ip ;
    this->port = port ;
    rc = make_shared<rpc>() ;
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
    client = make_shared<clientSock>() ;
    cmd = make_shared<cmds>() ;
    rc = make_shared<rpc>() ;
    rc->init() ;
    rc->setAddress(ip, port) ;
    rc->setCallMethod(request::sendReq) ;
    return 1 ;
}

int clientLoop :: sendRequest(string md, ...) {
    va_list va ;
    int num = cmd->isCmdExist(md) ;
    if(num < 0) {
        cout << "命令不存在" << endl ;
        return -1;
    }
    va_start(va, md) ;
    char* val = va_arg(va, char*) ;
    vector<string>ls ;
    ls.push_back(md) ;
    while(strcasecmp(val, END)) {
        ls.push_back(val) ;
        val = va_arg(va, char*) ;
    }
    if(ls.empty())  {
        cout << "kongde" << endl ;
        return  -1;
    } 

    va_end(va) ;
    if(servFd <= 0) {
        int ret = rc->Connect(servFd) ;
        if(ret < 0) { 
            cout << "连不上服务器" << endl ;
            return  -1;
        }
    }
    int res = rc->sendRequest(ls, num) ;
    if(res == -5 || res == -1) {
        cout << "没有发送成功" << endl ;
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
        cout << "服务器找不到！" << endl ;
        return ;
    }
    num = 0 ;
    //处理命令
    while(!stop) {
        string cmd, res; 
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
            cout << "bye bye!" << endl ; 
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
int clientLoop :: sendRequest(string& res) {
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
            cout << __FILE__ << "     " << __LINE__ << "          " << strerror(errno)<< endl ;
            return  -1 ;
        }
    }
    return ret ;
}

int clientLoop :: processMsg(Command& cmd, string& res) {
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
    Key* keys = cmd.add_keys() ;
    
    //设置值
    Value* val = cmd.add_vals() ;
    
    //设置值,值可能重复
    for(int i=2; i<len; i++) {
        string* a = val->add_val() ;
        *a = cmdStl[i] ;
    }
    //将设置好的结果序列化成string
    cmd.SerializeToString(&res) ;
    return ret ;
}
