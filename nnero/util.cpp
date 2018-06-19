#include"util.hpp"
#include<string>
#include<cstring>
#include<chrono>
#include<iomanip>
#include<ctime>
#include<sstream>

namespace nnero{
    namespace util{
        //get yyyy-MM-dd HH:mm:ss.z like date string
        std::string getNowMicroTime(){
            using namespace std::chrono;
            auto tp = time_point_cast<microseconds>(system_clock::now());
            auto timestamp = tp.time_since_epoch().count();
            auto micro = timestamp%1000000;
            auto tm = system_clock::to_time_t(tp);
            auto t_s = std::put_time(std::localtime(&tm),"%Y-%m-%d %T");
            std::ostringstream ostr;
            ostr<<t_s << "." << micro;
            return ostr.str();
        }

        std::string getNowMilliTime(){
            using namespace std::chrono;
            auto tp = time_point_cast<milliseconds>(system_clock::now());
            auto timestamp = tp.time_since_epoch().count();
            auto micro = timestamp % 1000;
            auto tm = system_clock::to_time_t(tp);
            auto t_s = std::put_time(std::localtime(&tm),"%Y-%m-%d %T");
            std::ostringstream ostr;
            ostr<<t_s << "." << micro;
            return ostr.str();
        }

        std::string getNowSecTime(){
            using namespace std::chrono;
            auto tm = system_clock::to_time_t(system_clock::now());
            auto t_s = std::put_time(std::localtime(&tm),"%Y-%m-%d %T");
            std::ostringstream ostr;
            ostr<<t_s;
            return ostr.str();
        }

        std::string getToday(){
            using namespace std::chrono;
            auto tm = system_clock::to_time_t(system_clock::now());
            auto t_s = std::put_time(std::localtime(&tm),"%Y-%m-%d");
            std::ostringstream ostr;
            ostr<<t_s;
            return ostr.str();
        }
    }
}
