#include <iostream>
#include <gtest/gtest.h>
#include "clientLoop.h"
using namespace std ;
/*
class stringTest : public testing::Test{

protected:

    virtual void SetUp()
    {
        rc = make_shared<rpc>() ;
        rc->setCallMethod(request::sendReq) ;
    }

    virtual void TearDown()
    {

    }
    shared_ptr<rpc> rc ;
} ;

TEST_F(stringtest, HandleNoneZeroInput) {
    vector<string>ss{"set", "name", "haha"} ;
    EXPECT_EQ(1, rc->sendRequest(ss, num)) ;
    EXPECT_EQ("OK", rc->getResponse()) ;
}
*/

int main(int argc, char** argv) {
    //初始化令表
    clientLoop clp ;
    //传入IP 和地址
    clp.init("127.0.0.1", "8888") ;
    clp.sendRequest("set", "name", "wc", END) ;
    return 0;
}

