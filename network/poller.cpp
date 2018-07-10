#include"poller.hpp"

namespace nnero{
    namespace net{

        PollImpl::PollImpl(){
            for(int i=0;i<POLL_MAX;++i){
                m_poll_fd[i].fd = -1;
                m_poll_fd[i].events = 0;
            }
        }

        PollImpl::~PollImpl(){
            for(int i=0;i<POLL_MAX;++i){
                m_poll_fd[i].fd = -1;
                m_poll_fd[i].events = 0;
            }
        }

        void PollImpl::poll(){
            int nready = ::poll(m_poll_fd, POLL_MAX, POLL_TIMEOUT);
            m_revents_map.clear();
            if(nready > 0){
                for(int i=0;i<POLL_MAX;++i){
                    if(m_poll_fd[i].revents & POLLIN){
                        m_revents_map[m_poll_fd[i].fd] = IO_STATE::READ;
                        --nready;
                    } else if(m_poll_fd[i].revents & POLLOUT){
                        m_revents_map[m_poll_fd[i].fd] = IO_STATE::WRITE;
                        --nready;
                    } else if(m_poll_fd[i].revents & POLLERR){
                        m_revents_map[m_poll_fd[i].fd] = IO_STATE::ERROR;
                        m_poll_fd[i].fd = -1;
                        m_poll_fd[i].events = 0;
                        --nready;
                    }
                    
                    if(nready == 0){
                        return;
                    }
                }
            }
        }

        const std::map<int,IO_STATE>& PollImpl::getResultMap()const{
            return m_revents_map;
        }

        void PollImpl::registerSocket( const TCPSocket &socket ){
            for(int i=0;i<POLL_MAX;++i){
                if(m_poll_fd[i].fd == -1){
                    m_poll_fd[i].fd = socket.getSocketFD();
                    m_poll_fd[i].events = POLLIN | POLLOUT | POLLERR;
                    break;
                }
            }
        }

        void PollImpl::unregisterSocket( const TCPSocket &socket ){
            int fd = socket.getSocketFD();
            for(int i=0;i<POLL_MAX;++i){
                if(m_poll_fd[i].fd == fd){
                    m_poll_fd[i].fd = -1;
                    m_poll_fd[i].events = 0;
                    break;
                }
            }
        }
    }
}
