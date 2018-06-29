#ifndef THREAD_POOL_NNERO_HPP
#define THREAD_POOL_NNERO_HPP

#include<thread>
#include<functional>
#include<iostream>
#include<vector>
#include<atomic>
#include"bqueue.hpp"
#include"nthread.hpp"

namespace nnero{
    namespace thread{
        using namespace nnero::queue;

        class ThreadPool;

        enum RUN_STATE{
            RUNNING,//active thread doing task or waiting for task
            SHUTDOWN, //shutdown thread doing queue's left tasks
            STOP //stop thread. queue is empty and thread done all.
        };

        //wrapper for func
        class Task{
        public:
            Task(const std::function<void()>& f):
                m_f{f},
                m_state{RUNNING}
            {}
            void run(){
                m_f();
            }
        private:
            std::function<void()> m_f;
            RUN_STATE m_state;
        };

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

            /*
              shutdown will abandon all the tasks in waiting queue.
              shutdown should use in case of tasks is all done and threadpool is free
             */
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
                            return this->m_task_queue.empty() && m_worker_cnt == 0;
                        });
                    std::cout<<"threadPool is shutdown!"<<std::endl;
                }
            }
            
            template<typename Func>
            void execute(const Func& func){
                Task task(func);
                //worker low than core_size so create new thread 
                if(m_worker_cnt < m_core_size){
                    ++m_worker_cnt;
                    auto f = std::bind(&ThreadPool::runWorker,this,task);
                    Nthread t(f);
                    t.detach();
                    m_threads.push_back(std::move(t));
                } else {
                    bool b = m_task_queue.add(task);
                    if(!b){
                        //just simple ignore
                        std::cout<<"tasks is full threads and queue.omitted!"<<std::endl;
                    }
                }
            }
        private:
            void runWorker(const Task& f){
                auto task = std::move(f);
                task.run();
                try{
                    while(!m_shutdown){
                        task = getTask();
                        task.run();
                    }
                }catch (InterruptException e){
                    std::cout<<"thread was interrupted.so exit."<<std::endl;
                }
                --m_worker_cnt;
                m_shutdown_cond.notify_one();
            }
            Task getTask(){
                Task t = m_task_queue.take();
                return t;
            }
            volatile bool m_shutdown{false};//for memory visiable in all threads
            std::atomic<int> m_worker_cnt{0};
            const int m_core_size;
            std::vector<Nthread> m_threads;
            BlockingQueue<Task> m_task_queue{100};
            mutable std::mutex m_main_lock; //for shutdown
            std::condition_variable m_shutdown_cond; //for shutdown
        };
    }
}

#endif
