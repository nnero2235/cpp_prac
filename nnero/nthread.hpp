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
          a wrapper for std::thread
          it can be interrupted
          Nthread object is not allowed to copy.
          using std::move  to move it control.
         */
        class Nthread;
        
        /*
          this is for thread that can be interrupt
          when thread was waiting.
          Of cause, we must use condition_variable to implement it.
         */
        class InterruptFlag;

        /*
          when a thread is interrupted
          throw this.
         */
        class InterruptException;

        class InterruptFlag{
        public:
            bool get();
            void set();
            void setName(const std::string& name);
            std::string getName();
        private:
            bool m_flag{false};
            std::string m_name{"thread"};
        };
        
        class InterruptException:public std::exception{
        public:
            InterruptException(const std::string& name);
            const char* what()const throw();
        private:
            std::string m_msg;
        };

        class Nthread{
        public:
            Nthread(std::function<void()> func,const std::string& name);
            //can't be copy
            Nthread(const Nthread& t) = delete;
            Nthread& operator=(const Nthread& t) = delete;
            //should be move
            Nthread(Nthread&& other);
            Nthread& operator=(Nthread&& other);

            bool isInterrupted();
            void detach();
            void interrupt();
            bool joinable()const;
            void join();
            std::string getName();
        private:
            InterruptFlag* m_interrupt_flag;
            std::string m_name{"nthread"};
            std::thread m_thread;
        };

        extern void interruptPoint();
        
    }
}

#endif
