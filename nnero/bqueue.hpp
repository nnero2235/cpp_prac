#ifndef NNERO_BLOCKING_QUEUE_HPP
#define NNERO_BLOCKING_QUEUE_HPP

#include<queue>
#include<thread>
#include<mutex>
#include<memory>
#include<exception>
#include<stdexcept>
#include<condition_variable>

namespace nnero{
    namespace queue{

        /**
           blocking queue for thread safe
         */
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
            /*return null ptr when queue is empty*/
            std::shared_ptr<T> get(){
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.empty()){
                    std::shared_ptr<T> n_ptr;
                    return std::move(n_ptr);
                }
                std::shared_ptr<T> ptr = m_inner_queue.front();
                m_inner_queue.pop();
                m_cond.notify_all();
                return std::move(ptr);
            }
            
            /*throw runtime_exception when queue is empty*/
            std::shared_ptr<T> poll(){
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.empty()){
                    throw std::runtime_error("BlockingQueue is empty. can't poll a element!");
                }
                std::shared_ptr<T> ptr = m_inner_queue.front();
                m_inner_queue.pop();
                m_cond.notify_all();
                return std::move(ptr);
            }
            /*blocking when queue is empty*/
            std::shared_ptr<T> take(){ 
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cond.wait(lock,[this](){
                        return !m_inner_queue.empty();
                });
                std::shared_ptr<T> ptr = m_inner_queue.front();
                m_inner_queue.pop();
                m_cond.notify_all();
                return std::move(ptr);
            }
            /*return false when queue is full*/
            bool add(std::shared_ptr<T> t){
                if(!t){
                    return false;
                }
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.size() == m_max){
                    return false;
                }
                m_inner_queue.push(std::move(t));
                m_cond.notify_all();
                return true;
            }
            
            /*throw runtime_error when queue is full*/
            void put(std::shared_ptr<T> t){
                if(!t){
                    return;
                }
                std::lock_guard<std::mutex> lock(m_mutex);
                if(m_inner_queue.size() == m_max){
                    throw std::runtime_error("Blockingqueue is full. can't put a element");
                }
                m_inner_queue.push(std::move(t));
                m_cond.notify_all();
            }
            /*blocking when queue is full that decided m_max's value*/
            void offer(std::shared_ptr<T> t){ 
                if(!t){
                    return;
                }
                std::unique_lock<std::mutex> lock(m_mutex);
                m_cond.wait(lock,[this](){
                        return m_inner_queue.size() < m_max;
                });
                m_inner_queue.push(std::move(t));
                m_cond.notify_all();
            }
        private:
            mutable std::mutex m_mutex;
            std::condition_variable m_cond;
            size_t m_max{10000};
            std::queue<std::shared_ptr<T>> m_inner_queue;
        };
    }
}

#endif
