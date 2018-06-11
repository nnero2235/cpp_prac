#include"util.hpp"
#include"logging.hpp"
#include<iostream>
#include<thread>

using namespace nnero::logging;

/*this is the point*/
Log::~Log(){
    std::cout<<m_stream.str()<<std::endl;
}

/*
construct a log message prefix like:
LEVEL yyyy-MM-dd HH:mm:ss.z <thread_id>fileName->funcName:lineNumber : 
*/
std::ostringstream& Log::stream(){
    switch(m_level){
    case DEBUG:
        m_stream << "[DEBUG ";
        break;
    case INFO:
        m_stream << "[INFO  ";
        break;
    case WARN:
        m_stream << "[WARN  ";
        break;
    case ERROR:
        m_stream << "[ERROR ";
        break;
    case FATAL:
        m_stream << "[FATAL ";
        break;
    }
    m_stream << util::getNowMilliTime()<<" ";
    m_stream <<"<"<< std::this_thread::get_id() << ">";
    m_stream << m_file << "->" << m_func<< ":"<<m_line<<"]: ";
    return m_stream;
}

