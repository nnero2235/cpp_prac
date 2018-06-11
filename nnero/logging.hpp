#ifndef NNERO_LOGGING_HPP
#define NNERO_LOGGING_HPP
#include<sstream>
#include<string>
#include"util.hpp"

namespace nnero{

    using namespace nnero;
    
    namespace logging{
        
        enum LOG_LEVEL{ 
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        class Log;

        class Log{
        public:
            Log(LOG_LEVEL l,int line,const char* func,const char* file):
                m_level{l},
                m_file{file},
                m_func{func},
                m_line(line)
            {}
            Log(const Log&) = delete;
            Log& operator=(const Log&) = delete;
            /*this is the point it will flush to file*/
            ~Log();
            /*return the real stream*/
            std::ostringstream& stream();
        private:
            LOG_LEVEL m_level;
            std::string m_file;
            std::string m_func;
            int m_line;
            std::ostringstream m_stream;
        };

        static LOG_LEVEL s_level{INFO};

        static void setLevel(LOG_LEVEL l){
            s_level = l;
        }

#define LOG(level) if(level >= s_level)       \
            Log(level,__LINE__,__PRETTY_FUNCTION__,__FILE__).stream()
    }
}

#endif
