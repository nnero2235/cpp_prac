#ifndef POLLER_NNERO_HPP
#define POLLER_NNERO_HPP

#include<poll.h>
#include<map>
#include"tcp_socket.hpp"

namespace nnero{
    namespace net{

        enum IO_STATE{
            READ,
            WRITE,
            ERROR
        };

        //for multiplexing IO
        class PollImpl;
        
        class PollImpl{
        public:
            PollImpl();
            PollImpl(const PollImpl&) = delete;
            PollImpl(PollImpl&&) =delete;
            PollImpl& operator=(const PollImpl&) = delete;
            PollImpl& operator=(PollImpl&&) = delete;
            ~PollImpl();
            static const int POLL_MAX = 1024;
            static const int POLL_TIMEOUT = 500;
            void poll();
            const std::map<int,IO_STATE>& getResultMap()const;
            void registerSocket(const TCPSocket& socket);
            void unregisterSocket(const TCPSocket& socket);
        private:
            struct pollfd m_poll_fd[POLL_MAX];
            std::map<int,IO_STATE> m_revents_map;
        };
    }
}

#endif
