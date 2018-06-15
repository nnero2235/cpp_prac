#ifndef NNERO_UTIL_HPP
#define NNERO_UTIL_HPP

#include<string>

namespace nnero{
    namespace util{
        extern std::string getNowMicroTime();
        extern std::string getNowMilliTime();
        extern std::string getNowSecTime();
        extern std::string getToday();
    }
}

#endif
