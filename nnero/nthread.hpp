#ifndef NTHREAD_NNERO_HPP
#define NTHREAD_NNERO_HPP

#include<thread>
#include<exception>
#include<stdexcept>
#include<future>
#include"logging.hpp"

namespace nnero{
    namespace thread{
        using namespace nnero::logging;
        /*
          this is for thread that can be interrupt
          when thread was waiting.
          Of cause, we must use condition_variable to implement it.
         */
        class InterruptFlag{
        public:
            bool get(){
                return m_flag;
            }
            void set(){
                m_flag = true;
            }
            void setName(const std::string& name){
                m_name = name;
            }
            std::string& getName(){
                return m_name;
            }
        private:
            bool m_flag{false};
            std::string m_name{"thread"};
        };
        
        thread_local InterruptFlag interrupt_flag;

        /*
          a wrapper for std::thread
          it can be interrupted
          Nthread object is not allowed to copy.
          using std::move  to move it control.
         */
        class Nthread;

        /*
          when a thread is interrupted
          throw this.
         */
        class InterruptException:public std::exception{
        public:
            InterruptException(const std::string& name)
            {
                m_msg = name + " is interrupted";
            }
            const char* what()const throw(){
                return m_msg.c_str();
            }
        private:
            std::string m_msg;
        };

        class Nthread{
        public:
            Nthread(std::function<void()> func,const std::string& name):
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
            //can't be copy
            Nthread(const Nthread& t) = delete;
            Nthread& operator=(const Nthread& t) = delete;

            //should be move
            Nthread(Nthread&& other){
                *this = std::move(other);
            }
            Nthread& operator=(Nthread&& other){
                if(this != &other){
                    this->m_interrupt_flag = other.m_interrupt_flag;
                    this->m_thread = std::move(other.m_thread);

                    other.m_interrupt_flag = nullptr;
                }
                return *this;
            }

            bool isInterrupted(){
                return m_interrupt_flag->get();
            }
            void detach(){
                m_thread.detach();
            }
            void interrupt(){
                if(!m_interrupt_flag->get()){
                    m_interrupt_flag->set();
                }
            }
            bool joinable()const{
                return m_thread.joinable();
            }
            void join(){
                if(m_thread.joinable()){
                    m_thread.join();
                }
            }
            std::string& getName(){
                return m_name;
            }
        private:
            InterruptFlag* m_interrupt_flag;
            std::string m_name{"nthread"};
            std::thread m_thread;
        };

        static void interruptPoint(){
            if(interrupt_flag.get()){
                throw InterruptException(interrupt_flag.getName());
            }
        }
    }
}

#endif
