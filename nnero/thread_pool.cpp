#include"thread_pool.hpp"

using namespace nnero::logging;
using namespace nnero::thread;

ThreadPool::ThreadPool():
    m_core_size{static_cast<int>(std::thread::hardware_concurrency())}
{}
ThreadPool::ThreadPool(const unsigned int& thread_num):
    m_core_size{static_cast<int>(thread_num)}
{}
ThreadPool::ThreadPool(const unsigned int& thread_num,const size_t& queue_size):
    m_core_size{static_cast<int>(thread_num)},
    m_task_queue{queue_size}
{}
ThreadPool::~ThreadPool(){
    if(!m_shutdown || m_worker_cnt > 0){
        shutdown();
    }
}

void ThreadPool::shutdown(){
    std::unique_lock<std::mutex> lock(m_main_lock);
    if(!m_shutdown){
        m_shutdown = true;
        auto begin = m_threads.begin();
        while(begin != m_threads.end()){
            begin->interrupt();
            ++begin;
        }
        m_task_queue.wakeUpAll();
        m_shutdown_cond.wait(lock,[this](){
                bool b = m_worker_cnt == 0;
                if(!b){
                    m_task_queue.wakeUpAll();
                }
                return b;
            });
        begin = m_threads.begin();
        while(begin != m_threads.end()){
            if(begin->joinable()){
                begin->join();
            }
            ++begin;
        }
        LOG(INFO)<<"threadPool is shutdown!";
    }
}

void ThreadPool::execute(const std::function<void()>& func){
    if(m_shutdown){
        LOG(WARN)<<"thread pool is already shutdown!";
        return;
    }
    bool b = m_task_queue.add(func);
    if(!b){
        //just simple ignore
        LOG(INFO)<<"task queue is full.so omitted!";
    }
    //worker low than core_size so create new thread 
    if(m_worker_cnt < m_core_size){
        ++m_worker_cnt;
        auto f = std::bind(&ThreadPool::runWorker,this);
        m_threads.push_back(Nthread(f,"thread-pool-"+std::to_string(m_worker_cnt)));
    } 
}

void ThreadPool::runWorker(){
    try{
        while(!m_shutdown){
            auto task = getTask();
            task();
        }
    }catch (const InterruptException& e){
        LOG(WARN)<<e.what()<<" so exit";
    }
    //do finish worker with queue.
    runWithExit();

    std::lock_guard<std::mutex> lock(m_main_lock);
    --m_worker_cnt;
    m_shutdown_cond.notify_one();
}
std::function<void()> ThreadPool::getTask(){
    return m_task_queue.take();
}
void ThreadPool::runWithExit(){
    try{
        while(!m_task_queue.empty()){
            auto task = m_task_queue.poll();
            task();
        }
    } catch (const std::runtime_error& e){
        LOG(INFO)<<"empty queue";
    }
}
