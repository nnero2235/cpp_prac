#include"util.hpp"
#include"logging.hpp"
#include<string>
#include<iostream>
#include<thread>
#include<vector>
#include<algorithm>

using namespace nnero;

static void time_test(){
    using namespace std::chrono;
    std::string s = util::getNowMicroTime();
    std::cout<<"mirco:"<<s<<'\n';
    s = util::getNowMilliTime();
    std::cout<<"milli:"<<s<<'\n';
    s = util::getNowSecTime();
    std::cout<<"second:"<<s<<'\n';
}

static void log_each(){
    using namespace nnero::logging;
    LOG(DEBUG) <<"hello DEBUG";
    LOG(INFO) << "hello INFO" << 10 <<" nice";
    LOG(WARN) << "hello WARN " << 10.1 <<" nice";
    LOG(ERROR) << "hello ERROR " << true << " fuck";
    LOG(FATAL) << "hello FATAL " << 'a' << " fuck";    
}

static void log_test(){
    using namespace nnero::logging;
    for(int i=0;i<5;i++){
        switch(i){
        case 0:
            setLevel(DEBUG);
            std::cout<<"using DEBUG.............."<<'\n';
            log_each();
            break;
        case 1:
            setLevel(INFO);
            std::cout<<"using INFO.............."<<'\n';
            log_each();
            break;
        case 2:
            setLevel(WARN);
            std::cout<<"using WARN.............."<<'\n';
            log_each();
            break;
        case 3:
            setLevel(ERROR);
            std::cout<<"using ERROR.............."<<'\n';
            log_each();
            break;
        case 4:
            setLevel(FATAL);
            std::cout<<"using FATAL.............."<<'\n';
            log_each();
            break;
        }
    }
}

static void log_threads_test(){
    std::cout<<"threads log test......"<<std::endl;
    using namespace nnero::logging;
    std::vector<std::thread> threads;
    setLevel(FATAL);
    for(int i=0;i<5;i++){
        threads.push_back(std::thread(log_each));
    }
    std::for_each(threads.begin(), threads.end(), [](std::thread &t){t.join();});
}

int main(){
    time_test();
    log_test();
    log_threads_test();
    return 0;
}
