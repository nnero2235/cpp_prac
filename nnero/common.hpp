#ifndef NNERO_COMMON_HPP
#define NNERO_COMMON_HPP
#include<iostream> /*for std::cout*/
#include<chrono> /*for format time and now time*/
#include<iomanip> 
#include<string> 
#include<cstring> 
#include<sstream> /*for stringstream to make string*/
#include<fstream> /*for output to log file*/

namespace nnero{
    /*
      util for global and common calls
     */
    namespace util{
        static std::string getLocalDateTimeFormat(){
            auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto t_s = std::put_time(std::localtime(&t),"%Y-%m-%d %X");
            std::ostringstream ostr;
            ostr<<t_s;
            return ostr.str();
        }

        static std::string getLocalDateFormat(){
            auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            auto t_s = std::put_time(std::localtime(&t),"%Y-%m-%d");
            std::ostringstream ostr;
            ostr<<t_s;
            return ostr.str();
        }
    }
    /*
      logger for console and file to format msg output.
     */
    namespace logger{
        using namespace util;
        
        enum LOG_LEVEL{
            DEBUG,
            INFO,
            WARN,
            ERROR
        };

        class Log{
        public:
            Log() = default;
            Log(const Log&) = delete;
            Log& operator=(const Log&) = delete;
            ~Log() = default;
            //init and ready to print
            const Log& operator()(const LOG_LEVEL level){
                switch(level){
                case LOG_LEVEL::DEBUG:
                    ostr << "[DEBUG] " <<getLocalDateTimeFormat()<<" ["<<__FILE__<<":"<<__LINE__<<":"<<__FUNCTION__<<"]"<<" : ";
                    break;
                case LOG_LEVEL::INFO:
                    ostr << "[INFO] " <<getLocalDateTimeFormat()<<" : ";
                    break;
                case LOG_LEVEL::WARN:
                    ostr << "[WARN] " <<getLocalDateTimeFormat()<<" : ";
                    break;
                case LOG_LEVEL::ERROR:
                    ostr << "[ERROR] " <<getLocalDateTimeFormat()<<" : ";
                    break;
                }
                return *this;
            }
            template<typename T>
            const Log& operator<<(T t){
                ostr<<t;
            }
        private:
            std::ostringstream ostr;
        };
        
        const Log LOG;
    }
}

#endif
