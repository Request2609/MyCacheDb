#include <iostream>
#include <gtest/gtest.h>
#include "clientLoop.h"
using namespace std ;
/*
class stringTest : public testing::Test{

protected:

    virtual void SetUp()
    {
        //传入IP 和地址
        clp.init("127.0.0.1", "8888") ;
    }
    virtual void TearDown()
    {
    }
    clientLoop clp ;
} ;

TEST_F(stringTest, HandleNoneZeroInput) {
    EXPECT_EQ(1, clp.sendRequest("set", "name", "wc", END)) ;
    EXPECT_EQ(1,  clp.getResult()) ;
}
*/

int main(int argc, char** argv) {
    /*   ::testing::InitGoogleTest(&argc, argv);
         return RUN_ALL_TESTS();
         */
    ///////////////////////////////必须初始化
    //初始化令表
    clientLoop clp ;
    //传入IP 和地址
   /// clp.init("127.0.0.1", "8888") ;
   //在配置文件中找
    clp.init() ;
//////////////////////////////程序中使用
    clp.sendRequest("set", "name", "wc", END) ;
    clp.sendRequest("get", "name", END) ;
    clp.sendRequest("hset", "student", "name","ck", "age", "18", END) ;
    clp.sendRequest("hget", "student", "name", END) ;
    clp.sendRequest("hget", "student", "age", END) ;
    //........   

    //////////////////////////获取结果
    cout << "获取的返回结果：" << endl ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;
    clp.getResult() ;


///////////////////////////////////////////////终端使用
//    clp.start() ;
    return 0;
}

