#ifndef NNERO_LOGGING_HPP
#define NNERO_LOGGING_HPP
#include<sstream>
#include<fstream>
#include<string>
#include<memory>
#include"util.hpp"
#include<iostream>
#include<map>

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

        enum LOG_MODE{
            CONSOLE,
            FILE,
            BOTH
        };

        class Log;
        class LogConfig;

        class LogConfig{
            friend Log;
        public:
            LogConfig(const std::string& log_path,const std::string& log_name,
                      const LOG_MODE log_mode,const LOG_LEVEL log_level):
                m_log_path{log_path},
                m_log_name{log_name},
                m_log_mode{log_mode},
                m_log_level{log_level}
            {}
            LogConfig(const LogConfig&) = delete;
            LogConfig& operator=(const LogConfig&) = delete;
        private:
            std::string m_log_path;
            std::string m_log_name;
            LOG_MODE m_log_mode;
            LOG_LEVEL m_log_level;
        };

        class Log{
        public:
            Log(LOG_LEVEL l,int line,const char* func,const char* file,std::shared_ptr<LogConfig> p):
                m_level{l},
                m_file{file},
                m_func{func},
                m_line(line),
                m_log_confg_ptr{p}
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
            std::shared_ptr<LogConfig> m_log_confg_ptr;
            std::ostringstream m_stream;
            std::ofstream m_fstream;
        };

        extern std::shared_ptr<LogConfig> s_config_ptr;
        extern LOG_LEVEL s_level;

        /*
          init with code. explict set config properties
         */
        extern void logInit(const std::string& log_path,const std::string& log_name,const LOG_MODE log_mode,const LOG_LEVEL log_level);

        /*
          from file in current dir with file named 'log_config.properties'
          the default:
              log.path=/home/nnero/xxx/
              log.file=nnero
              log.level=info
              log.mode=all
        */
        extern void logInit();
        
        /*for outside use*/
#define LOG(level) if(level >= s_level)                        \
            Log(level,__LINE__,__PRETTY_FUNCTION__,__FILE__,s_config_ptr).stream()
    }
}

#endif
