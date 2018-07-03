#ifndef NTHREAD_NNERO_HPP
#define NTHREAD_NNERO_HPP

#include<thread>
#include<exception>
#include<stdexcept>
#include<future>

namespace nnero{
    namespace thread{

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
        private:
            bool m_flag{false};
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
            const char* what()const throw(){
                return "Thread is interrputed";
            }
        };

        class Nthread{
        public:
            template<typename FUNC>
            Nthread(FUNC func){
                std::promise<InterruptFlag*> p;
                m_thread = std::thread([func,&p](){
                        p.set_value(&interrupt_flag);
                        func();
                    });
                m_interrupt_flag = p.get_future().get();
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
                m_thread.join();
            }
        private:
            std::function<void()> m_func;
            InterruptFlag* m_interrupt_flag;
            std::thread m_thread;
            
        };

        static void interruptPoint(){
            if(interrupt_flag.get()){
                throw InterruptException();
            }
        }
    }
}

#endif
