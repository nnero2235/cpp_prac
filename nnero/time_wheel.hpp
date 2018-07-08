#ifndef TIME_WHEEL_NNERO_HPP
#define TIME_WHEEL_NNERO_HPP

#include"logging.hpp"
#include<array>
#include<list>
#include<functional>
#include"thread_pool.hpp"
#include<mutex>
#include<atomic>
#include<exception>
#include<stdexcept>
#include<memory>

namespace nnero{
    namespace time{

        using namespace nnero::logging;
        using namespace nnero::thread;

        //time wheel is a timer
        //time wheel usually use in timer task
        class TimeWheel;

        //the timer task wrapper
        //contains info to execute
        //TimeTask is execute in other threads
        //Not in timer thread.
        class TimeTask;

        class TimeTask{
        public:
            TimeTask(const std::function<void()>& f,const int& rotation);
            bool checkCanRun();
            std::function<void()> get();
        private:
            //means how many time wheeling to rotation
            //when 0 to trigger the timer task
            int m_rotation{0};
            //the execute's task func.
            std::function<void()> m_func;
        };
        
        class TimeWheel{
            //MIN Time unit 1ms
            //it means 1ms to check the task 
            static constexpr unsigned short MIN_UNIT=10;
        public:
            TimeWheel();
            ~TimeWheel();
            //no allow to copy and move
            TimeWheel(const TimeWheel&) = delete;
            TimeWheel& operator=(const TimeWheel&) = delete;
            TimeWheel(TimeWheel&&) = delete;
            TimeWheel&& operator=(TimeWheel&&)=delete;

            //delay_time is milli seconds
            void addTask(const std::function<void()>& f,const long& delay_time);
            void close();
            void startTick();
        private:
            void tick();
            std::array<std::shared_ptr<std::list<TimeTask>>,10> m_time_box;
            std::atomic<int> m_curr_index{0};
            std::thread m_thread;
            ThreadPool m_execute_pool{1};
            volatile bool m_close{false};
        };
    }
}

#endif
