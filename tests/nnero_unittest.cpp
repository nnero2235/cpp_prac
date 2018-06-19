#include<gtest/gtest.h>
#include"../nnero/util.hpp"
#include"../nnero/logging.hpp"
#include<string>
#include<iostream>
#include<fstream>
#include<thread>
#include<vector>
#include<algorithm>


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
    logInit("/home/nnero/project/cplusplus/cpp_prac/tests/log_config.properties");
    LOG(INFO)<<"hello console";
    EXPECT_EQ(1,1);
}

TEST(LOG_TEST,log_file){
    using namespace nnero::logging;
    logInit("/home/nnero/project/cplusplus/cpp_prac/tests/log_config_file.properties");
    LOG(INFO)<<"hello file info";
    LOG(WARN)<<"hello file WARN";
    LOG(ERROR)<<"hello file error";
    EXPECT_EQ(1,1);
}

void logForThread(int i){
    using namespace nnero::logging;
    LOG(WARN)<<"hello "<<i;
}

TEST(LOG_TEST,multi_thread){
    using namespace nnero::logging;
    logInit("/home/nnero/project/cplusplus/cpp_prac/tests/log_config_file.properties");
    std::vector<std::thread> list;
    for(int i=0;i<5;i++){
        list.push_back(std::thread(logForThread,i));
    }
    std::for_each(list.begin(), list.end(), [](std::thread& t){t.join();});
}

TEST(LOG_TEST,performance){
    using namespace nnero::logging;
    logInit("/home/nnero/project/cplusplus/cpp_prac/tests/log_config_file2.properties");
    LOG(WARN)<<"hello1";
    LOG(WARN)<<"hello1";
    LOG(WARN)<<"hello1";
    LOG(WARN)<<"hello1";
    LOG(WARN)<<"hello1";
}
