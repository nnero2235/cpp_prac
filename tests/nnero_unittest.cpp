#include<gtest/gtest.h>
#include"../nnero/util.hpp"
#include"../nnero/logging.hpp"
#include"../nnero/bqueue.hpp"
#include"../nnero/thread_pool.hpp"
#include"../nnero/nthread.hpp"
#include<string>
#include<iostream>
#include<fstream>
#include<thread>
#include<vector>
#include<algorithm>
#include<exception>
#include<stdexcept>
#include<chrono>
#include<memory>
#include<functional>
#include<atomic>



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

TEST(BQUEUE_TEST,single_thread){
    using namespace nnero::queue;
    BlockingQueue<std::shared_ptr<int>> bqueue(5);
    ASSERT_TRUE(bqueue.empty());
    bqueue.put(std::make_shared<int>(5));
    bqueue.put(std::make_shared<int>(1));
    bqueue.put(std::make_shared<int>(3));
    bqueue.put(std::make_shared<int>(10));
    bqueue.put(std::make_shared<int>(9));
    try{
        bqueue.put(std::make_shared<int>(11));
    }catch(std::runtime_error e){
        ASSERT_STREQ(e.what(),"Blockingqueue is full. can't put a element");
    }
    ASSERT_EQ(bqueue.size(),5);
    ASSERT_FALSE(bqueue.empty());
    std::shared_ptr<int> p = bqueue.take();
    ASSERT_EQ(*p,5);
    ASSERT_EQ(bqueue.size(),4);
    bqueue.poll();
    bqueue.poll();
    bqueue.poll();
    bqueue.poll();
    ASSERT_TRUE(bqueue.empty());
    try{
        bqueue.poll();
    }catch(std::runtime_error e){
        ASSERT_STREQ(e.what(),"BlockingQueue is empty. can't poll a element!");
    }
    bqueue.add(std::make_shared<int>(9));
    bqueue.add(std::make_shared<int>(19));
    bqueue.add(std::make_shared<int>(119));
    bqueue.add(std::make_shared<int>(911));
    bqueue.add(std::make_shared<int>(9111));
    bool b = bqueue.add(std::make_shared<int>(100));
    ASSERT_FALSE(b);
}

TEST(BQUEUE_TEST,multi_thread){
    using namespace nnero::queue;
    BlockingQueue<std::shared_ptr<int>> bqueue(3);
    bool exit = false;
    auto productor = [&bqueue](){
        std::chrono::seconds sec(2);
        std::this_thread::sleep_for(sec);//wait 2s
        bqueue.offer(std::make_shared<int>(10));
        bqueue.offer(std::make_shared<int>(11));
        bqueue.offer(std::make_shared<int>(12));
        bqueue.offer(std::make_shared<int>(13));
        bqueue.offer(std::make_shared<int>(14));
        bqueue.offer(std::make_shared<int>(15));
        bqueue.offer(std::make_shared<int>(16));
        bqueue.offer(std::make_shared<int>(17));
        bqueue.offer(std::make_shared<int>(18));
        bqueue.offer(std::make_shared<int>(19));
    };
    auto customer1 = [&bqueue,&exit](){
        while(!exit){
            std::shared_ptr<int> ptr = bqueue.take();
            ASSERT_TRUE(*ptr < 22);
        }
    };
    auto customer2 = [&bqueue,&exit](){
        while(!exit){
            std::shared_ptr<int> ptr = bqueue.take();
            ASSERT_TRUE(*ptr < 22);
        }
    };
    auto commander = [&bqueue,&exit](){
        std::this_thread::sleep_for(std::chrono::seconds(5));
        exit = true;
        bqueue.offer(std::make_shared<int>(20));
        bqueue.offer(std::make_shared<int>(21));
    };
    std::thread t1(productor);
    std::thread t2(customer1);
    std::thread t3(customer2);
    std::thread t4(commander);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}

TEST(THREAD_POOL,simple){
    std::atomic<int> value(0);
    auto func = [&value](){
        std::cout<<"value is:"<<value<<std::endl;
        ++value;
    };
    {
        using namespace nnero::thread;
        ThreadPool pool;
        pool.execute(func);
        pool.execute(func);
        pool.execute(func);
        pool.execute(func);
    }
    ASSERT_EQ(value,4);
}

TEST(NTHREAD,simple){
    using namespace nnero::thread;
    
    std::atomic<int> value{0};
    auto func = [&value](){
        bool shutdown{false};
        while(!shutdown){
            try{
                interruptPoint();
                std::cout<<value<<std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                ++value;
            } catch(InterruptException e){
                std::cout<<e.what()<<std::endl;
                shutdown=true;
            }
        }
    };

    std::vector<Nthread> threads;

    threads.push_back(Nthread(func));
    threads.push_back(Nthread(func));
    threads.push_back(Nthread(func));

    std::for_each(threads.begin(), threads.end(), [](Nthread& t){
            t.interrupt();
            t.join();
        });
}

