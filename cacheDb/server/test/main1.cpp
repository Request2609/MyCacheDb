#include <iostream>
#include <chrono>
#include <thread>
#include "heap1.h"

using namespace std;
 
void  test() {
	cout << "test"<<endl;
}
 
void temp_func() {
	cout << "--------------" << endl;
}
 
int  main() {
    
	TimerManager manager;
	MyTimer timer1(manager);
    //循环任务
	timer1.start(&test,1000, MyTimer::TimerType::CIRCLE);
	MyTimer timer2(manager);
    //一次性任务
	timer2.start(&temp_func, 1000, MyTimer::TimerType::ONCE);
	MyTimer timer3(manager);
	timer3.start(&temp_func, 1000, MyTimer::TimerType::ONCE);
	while (1) {
		this_thread::sleep_for(chrono::microseconds(1000));
		manager.detect_timers();
	}
	getchar();
	getchar();
	return 0;
}
