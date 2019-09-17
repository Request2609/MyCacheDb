#include <iostream>
#include <unistd.h>
#include <signal.h>

using namespace std ;

void sigAlarm(int signo) {
    printf("时间到！");
    //重启计时器，设置为1秒
    alarm(1) ;
}

int main() {
        
    if(signal(SIGALRM, sigAlarm) == SIG_ERR) {
        cout << "信号处理失败！处理失败！" << endl ;
        return 1 ;
    }
    cout << "开始！"<< endl ;
    alarm(1) ;
    while(1) {
        //等待信号！ 
        pause() ;
    }
    return 0;
}
