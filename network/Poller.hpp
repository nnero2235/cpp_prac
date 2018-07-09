#ifndef POLLER_NNERO_HPP
#define POLLER_NNERO_HPP

#include<poll.h>

namespace nnero{
    namespace net{

        class PollImpl{
        public:
            static const int POLL_MAX = 1024;
            static const int POLL_TIMEOUT = 500;
            void handleEvent();
            void addEvent(int fd);
            void removeEvent(int fd);
        private:
            struct pollfd m_poll_fd[POLL_MAX];
            int m_current_index{0};
        };
    }
}

#endif
