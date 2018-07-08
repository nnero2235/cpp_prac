#ifndef EVENT_LOOP_NNERO_HPP
#define EVENT_LOOP_NNERO_HPP

#include<thread>

namespace nnero{
    namespace net{

        //event loop for multilexing IO
        //it only loop in create thread. usually IO thread
        //one thread can only has one eventloop.
        class EventLoop;

        class EventLoop{
        public:
            EventLoop();
            //can't copy and move
            EventLoop(const EventLoop&) = delete;
            EventLoop& operator=(const EventLoop&) = delete;
            EventLoop(EventLoop&&) = delete;
            EventLoop&& operator=(EventLoop&&) = delete;
            ~EventLoop();

            //loop event
            //check whether is in create thread
            void loop();
            
        private:
            bool m_looping;
            std::thread::id m_thread_id;
        };
    }
}

#endif
