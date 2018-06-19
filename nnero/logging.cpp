#include"util.hpp"
#include"logging.hpp"
#include<iostream>
#include<thread>
#include<fstream>
#include<sstream>
#include<memory>
#include<unistd.h>

using namespace nnero::logging;

namespace nnero{
    namespace logging{
        std::shared_ptr<LogConfig> s_config_ptr;
        LOG_LEVEL s_level{LOG_LEVEL::INFO};

        void logInit(const std::string& log_path,const std::string& log_name,
                     const LOG_MODE log_mode,const LOG_LEVEL log_level,const LOG_RATIO log_ratio){
            s_config_ptr.reset(new LogConfig(log_path,log_name,log_mode,log_level,log_ratio));
            s_level = log_level;
        }

        void logInit(std::string file_path){
            std::ifstream ifs(file_path);
            if(ifs.is_open()){
                std::string line;
                std::map<std::string,std::string> config_map;
                while(!ifs.eof()){
                    ifs >> line;
                    auto pos = line.find("=");
                    if(pos != std::string::npos){
                        std::string name = line.substr(0, pos);
                        std::string value = line.substr(pos+1, line.length());
                        config_map[name]=value;
                    } else {
                        std::cerr<<"config is broken. line is:"<<line<<"\n";
                        abort();
                    }
                }
                std::string log_path = config_map["log.path"];
                std::string log_file = config_map["log.file"];
                std::string log_level_str = config_map["log.level"];
                std::string log_mode_str = config_map["log.mode"];
                std::string log_time = config_map["log.time"];
                LOG_RATIO log_ratio{LOG_RATIO::MILLI};
                LOG_LEVEL log_level{LOG_LEVEL::INFO};
                LOG_MODE log_mode{LOG_MODE::BOTH};
                if(!log_mode_str.empty()){
                    if(log_mode_str == "console"){
                        log_mode = LOG_MODE::CONSOLE;
                    } else if(log_mode_str == "file"){
                        log_mode = LOG_MODE::FILE;
                    }
                }
                if(!log_level_str.empty()){
                    if(log_level_str == "debug"){
                        log_level = LOG_LEVEL::DEBUG;
                    } else if(log_level_str == "info"){
                        log_level = LOG_LEVEL::INFO;
                    } else if(log_level_str == "warn"){
                        log_level = LOG_LEVEL::WARN;
                    } else if(log_level_str == "error"){
                        log_level = LOG_LEVEL::ERROR;
                    } else if(log_level_str == "fatal"){
                        log_level = LOG_LEVEL::FATAL;
                    }
                }
                if(!log_time.empty()){
                    if(log_time == "second"){
                        log_ratio = LOG_RATIO::SECOND;
                    } else if(log_time == "milli"){
                        log_ratio = LOG_RATIO::MILLI;
                    } else if(log_time == "micro"){
                        log_ratio = LOG_RATIO::MIRCO;
                    } else {
                        std::cout<<"unknow config: log.time="<<log_time<<" so use default"<<std::endl;
                        log_ratio = LOG_RATIO::MILLI;
                    }
                }
                if((log_mode == LOG_MODE::BOTH or log_mode == LOG_MODE::FILE) and log_path.empty()){
                    std::cerr<<"log.path is not config. abort!\n";
                    abort();
                }
                if((log_mode == LOG_MODE::BOTH or log_mode == LOG_MODE::FILE) and log_file.empty()){
                    std::cerr<<"log.file is not config. abort!\n";
                    abort();
                }
                s_config_ptr.reset(new LogConfig(log_path,log_file,log_mode,log_level,log_ratio));
                s_level = log_level;
            } else {
                std::cerr<<"config file: "<<file_path<<" is not found!\n";
                abort();
            }
        }

    }
}

/*this is the point*/
Log::~Log(){
    std::string str = m_stream.str();
    LogConfig& config = *m_log_confg_ptr;
    if(config.m_log_mode == LOG_MODE::CONSOLE or config.m_log_mode == LOG_MODE::BOTH){
        std::cout<<str<<std::endl;
    }
    if(config.m_log_mode == LOG_MODE::FILE or config.m_log_mode == LOG_MODE::BOTH){
        std::ostringstream os;
        os <<config.m_log_path<< config.m_log_name << util::getToday() << ".log";
        std::cout<<os.str()<<std::endl;
        m_fstream.open(os.str(), std::ios::app);
        m_fstream <<str<<std::endl;
        m_fstream.close();
    }
}

/*
construct a log message prefix like:
LEVEL yyyy-MM-dd HH:mm:ss.z <thread_id>fileName->funcName:lineNumber : 
*/
std::ostringstream& Log::stream(){
    switch(m_level){
    case LOG_LEVEL::DEBUG:
        m_stream << "[DEBUG ";
        break;
    case LOG_LEVEL::INFO:
        m_stream << "[INFO  ";
        break;
    case LOG_LEVEL::WARN:
        m_stream << "[WARN  ";
        break;
    case LOG_LEVEL::ERROR:
        m_stream << "[ERROR ";
        break;
    case LOG_LEVEL::FATAL:
        m_stream << "[FATAL ";
        break;
    }
    if(m_log_confg_ptr->m_log_ratio == LOG_RATIO::MIRCO){
        m_stream << util::getNowMicroTime()<<" ";
    } else if(m_log_confg_ptr->m_log_ratio == LOG_RATIO::SECOND){
        m_stream << util::getNowSecTime()<<" ";
    } else {
        m_stream << util::getNowMilliTime()<<" ";
    }
    m_stream <<"<"<< std::this_thread::get_id() << ">";
    m_stream << m_file << "->" << m_func<< ":"<<m_line<<"]: ";
    return m_stream;
}


