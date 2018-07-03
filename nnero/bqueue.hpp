#ifndef NNERO_BLOCKING_QUEUE_HPP
#define NNERO_BLOCKING_QUEUE_HPP

#include<queue>
#include<thread>
#include<mutex>
#include<exception>
#include<stdexcept>
#include<condition_variable>
#include"nthread.hpp"

namespace nnero{
    namespace queue{

        //blocking queue for thread safe
        template<typename T>
        class BlockingQueue{
        public:
            BlockingQueue() = default;
            BlockingQueue(const size_t max):
                m_max{max}
            {}
            //no allow to copy
            BlockingQueue(const BlockingQueue&) = delete;
            BlockingQueue& operator=(const BlockingQueue&) =delete;
            ~BlockingQueue() = default;
            bool empty()const{
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_inner_queue.empty();
            }
            size_t size()const{
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_inner_queue.size();
            }
            
            //throw runtime_exception when queue is empty
            T poll(){
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.empty()){
                    throw std::runtime_error("BlockingQueue is empty. can't poll a element!");
                }
                T& ptr = m_inner_queue.front();
                m_inner_queue.pop();
                m_full_cond.notify_one();
                return std::move(ptr);
            }
            //blocking when queue is empty
            T take(){ 
                std::unique_lock<std::mutex> lock(m_mutex);
                m_empty_cond.wait(lock,[this](){
                        nnero::thread::interruptPoint();
                        return !m_inner_queue.empty();
                });
                T& ptr = m_inner_queue.front();
                m_inner_queue.pop();
                m_full_cond.notify_one();
                return std::move(ptr);
            }
            //return false when queue is full
            bool add(const T& t){
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.size() == m_max){
                    return false;
                }
                m_inner_queue.push(std::move(t));
                m_empty_cond.notify_one();
                return true;
            }
            
            //throw runtime_error when queue is full
            void put(const T& t){
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.size() == m_max){
                    throw std::runtime_error("Blockingqueue is full. can't put a element");
                }
                m_inner_queue.push(std::move(t));
                m_empty_cond.notify_one();
            }
            //blocking when queue is full that decided m_max's value
            void offer(const T& t){ 
                std::unique_lock<std::mutex> lock(m_mutex);
                m_full_cond.wait(lock,[this](){
                        nnero::thread::interruptPoint();
                        return m_inner_queue.size() < m_max;
                });
                m_inner_queue.push(std::move(t));
                m_empty_cond.notify_one();
            }

            //wake up all threads to check condition. that should be only used for interrupt.
            void wakeUpAll(){
                m_empty_cond.notify_all();
                m_full_cond.notify_all();
            }
        private:
            mutable std::mutex m_mutex;
            std::condition_variable m_full_cond;
            std::condition_variable m_empty_cond;
            size_t m_max{10000};
            std::queue<T> m_inner_queue;
        };
    }
}

#endif
