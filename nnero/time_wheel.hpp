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
            template<typename FUNC>
            TimeTask(FUNC f,const int& rotation):
                m_rotation{rotation},
                m_func{std::function<void()>(f)}
            {}
            bool checkCanRun(){
                if(m_rotation == 0){
                    return true;
                } else {
                    --m_rotation;
                    return false;
                }
            }
            std::function<void()> get(){
                return m_func;
            }
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
            TimeWheel(){
                for(int i=0;i<10;++i){
                    m_time_box[i] = std::make_shared<std::list<TimeTask>>();
                }
            }
            ~TimeWheel(){
                close();
            }
            //no allow to copy and move
            TimeWheel(const TimeWheel&) = delete;
            TimeWheel& operator=(const TimeWheel&) = delete;
            TimeWheel(TimeWheel&&) = delete;
            TimeWheel&& operator=(TimeWheel&&)=delete;

            //delay_time is milli seconds
            template<typename FUNC>
            void addTask(FUNC f,const long& delay_time){
                if(delay_time <= 10){
                    auto task_list = m_time_box[m_curr_index+1];
                    task_list->push_back(TimeTask(f,0));
                } else {
                    int execute_unit = delay_time/MIN_UNIT + m_curr_index;
                    int slot = execute_unit % m_time_box.size();
                    int rotation = delay_time/ MIN_UNIT / m_time_box.size();
                    LOG(INFO)<<"slot :"<<slot<<" rotation:"<<rotation;
                    TimeTask task(f,rotation);
                    auto task_list = m_time_box[slot];
                    task_list->push_back(task);
                }
            }
            void close(){
                if(!m_close){
                    m_close = true;
                    m_thread.join();
                    m_execute_pool.shutdown();
                }
            }
            void startTick(){
                m_thread = std::thread(&TimeWheel::tick,this);
            }
        private:
            void tick(){
                while(!m_close){
                    int current = m_curr_index;
                    ++m_curr_index;
                    if(m_curr_index >= static_cast<int>(m_time_box.size())){
                        m_curr_index = 0;
                    }
                    auto list = m_time_box[current];
                    if(list->size() > 0){
                        std::list<TimeTask> remain_list;
                        while(list->size() > 0){
                            TimeTask t = list->front();
                            if(t.checkCanRun()){
                                m_execute_pool.execute(t.get());
                            } else {
                                remain_list.push_back(t);
                            }
                            list->pop_front();
                        }
                        list->insert(list->end(), remain_list.begin(),remain_list.end());
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(MIN_UNIT)));
                }
            }
            std::array<std::shared_ptr<std::list<TimeTask>>,10> m_time_box;
            std::atomic<int> m_curr_index{0};
            std::thread m_thread;
            ThreadPool m_execute_pool{2};
            volatile bool m_close{false};
        };
    }
}

#endif
