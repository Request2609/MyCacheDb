#include <iostream>
#include <gtest/gtest.h>
#include <sys/time.h>
#include <MyCacheDB/clientLoop.h>
using namespace std ;
/*
class stringTest : public testing::Test{

protected:

    virtual void SetUp()
    {
        //传入IP 和地址
    }
    virtual void TearDown()
    {
    }
    clientLoop clp ;
} ;

TEST_F(stringTest, send) {
    //EXPECT_EQ(1, clp.sendRequest("set", "name", "wc", END)) ;
    EXPECT_EQ(1, clp.sendRequest("set", "name", "wc", END)) ;
    EXPECT_EQ(1, clp.sendRequest("hset", "student", "name","ck", "age", "18", END)) ;
}

*/

int main(int argc, char** argv) {
    //     ::testing::InitGoogleTest(&argc, argv);
    //     return RUN_ALL_TESTS();
    struct timeval tv, tend;
    clientLoop clp ;
    clp.init("127.0.0.1", "8888") ;
    gettimeofday(&tv,NULL);
    int a = 10000 ;
    while(a--) {
        clp.sendRequest("get", "name", END) ;
        clp.getResult() ;
    }

    gettimeofday(&tend,NULL);
    cout << "读取测试" << endl ;
    cout << "==================================================" << endl ;
    cout <<"开始时间戳:"<< tv.tv_sec<<"." << tv.tv_usec<<"s"<< endl; 
    cout <<"结束时间戳:"<< tend.tv_sec<<"." << tend.tv_usec<<"s"<< endl; 
    int s = (tend.tv_sec-tv.tv_sec)*1000000 ;
    int us = tend.tv_usec - tv.tv_usec ;
    double h = 10000; 
    int end = s + us ;
    cout <<h*1000000/end<< endl ;
    cout << "speed:" << h*1000000/end <<"cmd/s"<< endl ;
    struct timeval tv1, tend1;
    gettimeofday(&tv1,NULL);
    a = 10000 ;
    while(a--) {
        clp.sendRequest("hset", "student", "name","ck", "age", "18", END) ;
        clp.getResult() ;
    }
    gettimeofday(&tend1,NULL);
    cout << "修改测试" << endl ;
    cout << "==================================================" << endl ;
    cout <<"开始时间戳:"<< tv1.tv_sec<<"." << tv1.tv_usec<<"s"<< endl; 
    cout <<"结束时间戳:"<< tend1.tv_sec<<"." << tend1.tv_usec<<"s"<< endl; 
    s = (tend1.tv_sec-tv1.tv_sec)*1000000 ;
    us = tend1.tv_usec - tv1.tv_usec ;
    h = 10000; 
    end = s + us ;
    cout <<"speed:" << h*1000000/end<<"cmd/s"<< endl ;
    return 0;
}

