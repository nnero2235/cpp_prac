#include"../nnero/logging.hpp"
#include"event_loop.hpp"
#include<thread>
#include<stdexcept>

thread_local nnero::net::EventLoop* loop_ptr{nullptr};

using namespace nnero::logging;
using namespace nnero::net;

EventLoop::EventLoop(TCPServer& server):
    m_tcp_server{server},
    m_looping{false},
    m_thread_id{std::this_thread::get_id()}
{
    if(loop_ptr){
        throw std::runtime_error("this thread has already a eventLoop.");
    } else {
        loop_ptr = this;
    }
}

EventLoop::~EventLoop(){
    loop_ptr = nullptr;
}

void EventLoop::loop(){
    if(m_looping){
        LOG(WARN)<<"looping now";
        return;
    }
    if(m_thread_id != std::this_thread::get_id()){
        throw std::runtime_error("event loop is not create in this thread.abort!");
    }
    m_looping = true;

    while(!m_quit){
        m_tcp_server.handleEvent();
    }

    m_looping = false;
    LOG(INFO)<<"exit loop";
}

void EventLoop::quit(){
    if(!m_quit){
        m_quit = true;
    }
}
