#include<sys/socket.h> //for socket operator
#include<arpa/inet.h> //for network bytes to int 
#include<string>
#include<cstring> //for memset
#include"../nnero/logging.hpp"
#include<unistd.h>
#include"../nnero/thread_pool.hpp"
#include<functional>
#include<poll.h>

#define sock_cast(addr_in) (struct sockaddr*)&addr_in

using namespace nnero::logging;

void processConn(int c_fd){
    char buf[128];
    std::string msg;
    while(true){
        ::memset(buf, 0, 128);
        int recv_len{0};
        msg.clear();
        recv_len = ::read(c_fd, buf, 128);
        msg.append(buf,recv_len);
        LOG(INFO)<<msg;
        msg.append(" haha");
        ::write(c_fd,msg.c_str(),msg.length()+1);
    }
}

void processConnOnce(int c_fd,
                     int *connected_fd_ptr,
                     struct pollfd* fd_arr_ptr,int index,
                     int *connected_fd_num){
    char buf[128];
    std::string msg;
    ::memset(buf, 0, 128);
    int recv_len = ::read(c_fd, buf, 128);
    if(recv_len == -1 || recv_len == 0){ //means client close connection
        (*(fd_arr_ptr+index)).fd = -1;
        *(connected_fd_ptr+index) = -1;
        --(*connected_fd_num);
        LOG(INFO)<<"client close fd:"<<c_fd;
        return;
    }
    msg.append("someone->say: ");
    msg.append(buf,recv_len);
    int finished{0};
    for(int i=0;i<1024;++i){
        if(*(connected_fd_ptr+i) != -1){
            if(c_fd == *(connected_fd_ptr+i)){
                continue;
            } else {
                ::write(*(connected_fd_ptr+i),msg.c_str(),msg.length()+1);
            }
            ++finished;
        }
        if(finished == *connected_fd_num){
            break;
        }
    }
    
}

int main(){
    
    static constexpr int POLL_MAX = 1024;
    static constexpr unsigned long POLL_TIMEOUT = 500;
    static constexpr unsigned int PORT = 8080;
    static constexpr unsigned int BACKLOG = 5;
    static constexpr unsigned int SOCK_ADDR_SIZE = sizeof(struct sockaddr);

    //get socket from kernel
    int sock_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd == -1){
        LOG(FATAL)<<"socket fail!";
        abort();
    }

    //init server ip and port
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = ::htons(PORT);
    server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

    //set 0 to other attr.
    ::memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    //bind ip and port
    if(::bind(sock_fd,sock_cast(server_addr),SOCK_ADDR_SIZE) == -1){
        LOG(FATAL)<<"bind fail!";
        abort();
    }

    //listen on port
    if(::listen(sock_fd, BACKLOG) == -1){
        LOG(FATAL)<<"listen fail!";
        abort();
    }

    //loop for accept client socket and write something
    int c_fd;
    unsigned int sin_size;
    struct sockaddr_in client_addr;

    //to save connected fds
    int connected_fds[POLL_MAX];
    int connected_num{0};
    //init poll fd arr with -1
    struct pollfd fd_arr[POLL_MAX];
    for(int i=0;i<POLL_MAX;++i){
        fd_arr[i].fd = -1;
        connected_fds[i] = -1;
    }

    //interest listen socket when a socket want to connect.
    fd_arr[0].fd = sock_fd;
    fd_arr[0].events = POLLIN|POLLERR;

    int n_ready = 0;
    while(true){
        n_ready = ::poll(fd_arr,POLL_MAX,POLL_TIMEOUT);

        if(n_ready > 0){//n_ready's fd was ready 
            for(int i=0;i<POLL_MAX;++i){
                if(n_ready == 0){
                    break;
                }
                if(fd_arr[i].revents & POLLIN){
                    if(fd_arr[i].fd == sock_fd){ //connect event happens so add new_fd to fd_arr
                        sin_size = SOCK_ADDR_SIZE;
                        c_fd = ::accept(sock_fd, sock_cast(client_addr), &sin_size);
                        if(c_fd == -1){
                            LOG(ERROR)<<"accept error!";
                        } else {
                            LOG(INFO)<<"connected from "<<::inet_ntoa(client_addr.sin_addr)<<":"<<::ntohs(client_addr.sin_port);
                            for(int i=0;i<POLL_MAX;++i){
                                if(fd_arr[i].fd == -1){
                                    fd_arr[i].fd = c_fd;
                                    fd_arr[i].events = POLLIN | POLLERR;
                                    connected_fds[i] = c_fd;
                                    ++connected_num;
                                    break;
                                }
                            }
                        } 
                    } else { //other connected fd happens so read client data and write response.
                        processConnOnce(fd_arr[i].fd,connected_fds,fd_arr,i,&connected_num);
                    }
                    --n_ready;
                } else if(fd_arr[i].revents & POLLERR){
                    LOG(ERROR)<<"fd:"<<fd_arr[i].fd<<"POLLERR was happened";
                    connected_fds[i] = -1;
                    fd_arr[i].fd = -1;
                    ::close(fd_arr[i].fd);
                    --connected_num;
                    --n_ready;
                }
            }
        } else if(n_ready == 0){//no fd was ready
            continue;
        } else { //error in poll
            LOG(ERROR)<<"poll error!";
        }
    }
    
}

