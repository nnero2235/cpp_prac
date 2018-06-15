#include<gtest/gtest.h>
#include"../nnero/util.hpp"
#include"../nnero/logging.hpp"
#include<string>
#include<iostream>



TEST(UTIL_TEST,time_test){
    using namespace nnero::util;
    std::string today = getToday();
    std::string secTime = getNowSecTime();
    std::string millTime = getNowMilliTime();
    std::string microTime = getNowMicroTime();
    std::cout<<"today:"<<today<<std::endl;
    std::cout<<"sec:"<<secTime<<std::endl;
    std::cout<<"milli:"<<millTime<<std::endl;
    std::cout<<"mirco:"<<microTime<<std::endl;
    EXPECT_EQ(1,1);
}

TEST(LOG_TEST,log_console){
    using namespace nnero::logging;
    logInit();
    EXPECT_EQ(1,1);
}
