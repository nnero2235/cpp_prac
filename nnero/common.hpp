#ifndef NNERO_COMMON_HPP
#define NNERO_COMMON_HPP
#include<iostream>
#include<chrono>
#include<iomanip>
#include<string>
#include<cstring>

namespace nnero{
    namespace logger{
        enum Level{
            INFO,
            WARN,
            ERROR
        };
        class LoggerStream{
        public:
            template<typename T>
            const LoggerStream& operator<<(T t)const{
                std::cout<<t;
                return *this;
            }
        };
        class Log{
        public:
            Log() = default;
            Log(const Log&) = delete;
            Log& operator=(const Log&) = delete;
            ~Log() = default;
            const LoggerStream& operator()(const Level log_level)const{
                auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto t_s = std::put_time(std::localtime(&t),"%Y-%m-%d %X");
                switch(log_level){
                case Level::INFO:
                    lstream <<"[INFO] "<<t_s<<" : ";
                    break;
                case Level::WARN:
                    lstream <<"[WARN] "<<t_s<<" : ";
                    break;
                case Level::ERROR:
                    lstream <<"[ERROR] "<<t_s<<" : ";
                    break;
                }
                return lstream;
            }
        private:
            LoggerStream lstream;
        };
        const Log LOG;
    }
}

#endif
