#include"nthread.hpp"

using namespace nnero::logging;
using namespace nnero::thread;

thread_local InterruptFlag interrupt_flag;

void nnero::thread::interruptPoint(){
    if(interrupt_flag.get()){
        throw InterruptException(interrupt_flag.getName());
    }
}

bool InterruptFlag::get(){
    return m_flag;
}
void InterruptFlag::set(){
    m_flag = true;
}
void InterruptFlag::setName(const std::string& name){
    m_name = name;
}
std::string InterruptFlag::getName(){
    return m_name;
}

InterruptException::InterruptException(const std::string& name){
    m_msg = name + " is interrupted";
}
const char* InterruptException::what()const throw(){
    return m_msg.c_str();
}

Nthread::Nthread(std::function<void()> func,const std::string& name):
    m_name{name}
{
    std::promise<InterruptFlag*> p;
    m_thread = std::thread([func,&p,this](){
            try{
                p.set_value(&interrupt_flag);
                func();
            } catch (const std::exception& e){
                LOG(ERROR)<<"crash in thread:"<<m_name<<" :"<<e.what();
                abort();
            }
        });
    m_interrupt_flag = p.get_future().get();
    m_interrupt_flag->setName(name);
}

//should be move
Nthread::Nthread(Nthread&& other){
    *this = std::move(other);
}
Nthread& Nthread::operator=(Nthread&& other){
    if(this != &other){
        this->m_interrupt_flag = other.m_interrupt_flag;
        this->m_thread = std::move(other.m_thread);

        other.m_interrupt_flag = nullptr;
    }
    return *this;
}

bool Nthread::isInterrupted(){
    return m_interrupt_flag->get();
}
void Nthread::detach(){
    m_thread.detach();
}
void Nthread::interrupt(){
    if(!m_interrupt_flag->get()){
        m_interrupt_flag->set();
    }
}
bool Nthread::joinable()const{
    return m_thread.joinable();
}
void Nthread::join(){
    if(m_thread.joinable()){
        m_thread.join();
    }
}
std::string Nthread::getName(){
    return m_name;
}


