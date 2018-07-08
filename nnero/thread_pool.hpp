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

        /* 
           threadPool live circle should be all the time. 
           manually invoke shutdown when you want to exit.
         */
        class ThreadPool;


        class ThreadPool{
        public:
            ThreadPool();
            ThreadPool(const unsigned int& thread_num);
            ThreadPool(const unsigned int& thread_num,const size_t& queue_size);
            ThreadPool(const ThreadPool&) = delete;
            ThreadPool& operator=(const ThreadPool&) = delete;
            ~ThreadPool();

            //shutdown will abandon all the tasks in waiting queue.
            //shutdown should use in case of tasks is all done and threadpool is free
            void shutdown();

            void execute(const std::function<void()>& func);
        private:
            void runWorker();
            std::function<void()> getTask();
            void runWithExit();
            
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
