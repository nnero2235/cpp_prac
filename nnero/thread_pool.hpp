#ifndef THREAD_POOL_NNERO_HPP
#define THREAD_POOL_NNERO_HPP

#include<thread>
#include<functional>
#include<iostream>
#include<vector>
#include<atomic>
#include"bqueue.hpp"
#include"nthread.hpp"
#include"logging.hpp"

namespace nnero{
    namespace thread{
        using namespace nnero::queue;
        using namespace nnero::logging;

        class ThreadPool;

        /* 
           threadPool live circle should be all the time. 
           manually invoke shutdown when you want to exit.
         */
        class ThreadPool{
        public:
            ThreadPool():
                m_core_size{static_cast<int>(std::thread::hardware_concurrency())}
            {}
            ThreadPool(const unsigned int& thread_num):
                m_core_size{static_cast<int>(thread_num)}
            {}
            ThreadPool(const unsigned int& thread_num,const size_t& queue_size):
                m_core_size{static_cast<int>(thread_num)},
                m_task_queue{queue_size}
            {}
            ThreadPool(const ThreadPool&) = delete;
            ThreadPool& operator=(const ThreadPool&) = delete;
            ~ThreadPool(){
                if(!m_shutdown || m_worker_cnt > 0){
                    shutdown();
                }
            }

            //shutdown will abandon all the tasks in waiting queue.
            //shutdown should use in case of tasks is all done and threadpool is free
            void shutdown(){
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

            template<typename FUNC>
            void execute(FUNC func){
                if(m_shutdown){
                    LOG(WARN)<<"thread pool is already shutdown!";
                    return;
                }
                bool b = m_task_queue.add(std::function<void()>(func));
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
        private:
            void runWorker(){
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
            std::function<void()> getTask(){
                return m_task_queue.take();
            }
            void runWithExit(){
                try{
                    while(!m_task_queue.empty()){
                        auto task = m_task_queue.poll();
                        task();
                    }
                } catch (const std::runtime_error& e){
                    LOG(INFO)<<"empty queue";
                }
            }
            volatile bool m_shutdown{false};//for memory visiable in all threads
            std::atomic<int> m_worker_cnt{0};
            const int m_core_size;
            std::vector<Nthread> m_threads;
            BlockingQueue<std::function<void()>> m_task_queue{100};
            mutable std::mutex m_main_lock; //for shutdown
            std::condition_variable m_shutdown_cond; //for shutdown
        };
    }
}

#endif
