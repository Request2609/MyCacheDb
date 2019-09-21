#include "clientLoop.h"
#include <signal.h>
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

void clientLoop :: start(string ip, string port) {

    rc->setAddress(ip, port) ;
    //连接服务器
    int ret = rc->Connect() ;
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
        cout <<"发送完成！" << endl ;
        //返回结果并打印
        string a = rc->getResponse() ;
        cout << a << endl ;
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
    /*
    //要是消息长度大于65535
    else {
        //设置最后一个元素为０，表示没发完
        char buff[65535] ;  
        int pos = 0 ;
        while(len <= 0) {
            if(len < 65535) {

            }
            strcpy(buff, res.substr(pos, pos+65535).c_str()) ;
            ret = writen(connFd, buff, sizeof(buff)) ;
            if(ret < 0) {
                cout << __FILE__ <<"       "<< __LINE__ << endl ;
                return -1 ;
            }
            pos+=65535 ;
            len-=ret ;
        } 
    }*/
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
