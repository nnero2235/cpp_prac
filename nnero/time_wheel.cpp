#include"time_wheel.hpp"

using namespace nnero::logging;
using namespace nnero::time;


TimeTask::TimeTask(const std::function<void()>& f,const int& rotation):
        m_rotation{rotation},
        m_func{f}
    {}
bool TimeTask::checkCanRun(){
    if(m_rotation == 0){
        return true;
    } else {
        --m_rotation;
        return false;
    }
}
std::function<void()> TimeTask::get(){
    return m_func;
}


TimeWheel::TimeWheel(){
    for(int i=0;i<10;++i){
        m_time_box[i] = std::make_shared<std::list<TimeTask>>();
    }
}
TimeWheel::~TimeWheel(){
    close();
}

//delay_time is milli seconds
void TimeWheel::addTask(const std::function<void()>& f,const long& delay_time){
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
void TimeWheel::close(){
    if(!m_close){
        m_close = true;
        m_thread.join();
        m_execute_pool.shutdown();
    }
}
void TimeWheel::startTick(){
    m_thread = std::thread(&TimeWheel::tick,this);
}

void TimeWheel::tick(){
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
