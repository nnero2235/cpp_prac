#include<iostream>
#include<cstring>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<string>
#include<fcntl.h>
#include<nnero/common.hpp>
#include<vector>

using namespace nnero;

constexpr unsigned int PORT = 8080;
constexpr unsigned int EPOLL_MAX_EVENTS = 100;
constexpr unsigned int BUF_MAX = 1024;
std::vector<int> conn_fds;

/*set non block fd*/
void setNonBlock(int fd){
    int flags = ::fcntl(fd,F_GETFL,0);
    flags |= O_NONBLOCK;
    ::fcntl(fd,F_SETFL, flags);
}

/* bind port to listen*/
int bind_socket(int port){
    int listen_fd;
    struct ::sockaddr_in server_addr;
    listen_fd = ::socket(AF_INET, ::SOCK_STREAM, 0);
    ::memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    server_addr.sin_port = ::htons(port);
    int b = ::bind(listen_fd, (struct ::sockaddr*)&server_addr, sizeof(server_addr));
    if(b == -1){
        std::cout<<"bind port error\n";
        ::exit(0);
    }
    return listen_fd;
}

/* register a fd to epoll. e.g: new connection open*/
void add_epoll_event(int epoll_fd,int fd,int state){
    struct ::epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    if(::epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1){
        std::cout<<"add_event error:epoll_ctl\n";
        ::exit(0);
    }
}

/* delete a fd to epoll. e.g: closed sockect*/
void delete_epoll_event(int epoll_fd,int fd,int state){
    struct ::epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    if(::epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &ev) == -1){
        std::cout<<"del_event error:epoll_ctl\n";
        ::exit(0);
    }
}

/* modify a fd to epoll*/
void modify_epoll_event(int epoll_fd,int fd,int state){
    struct ::epoll_event ev;
    ev.events = state;
    ev.data.fd = fd;
    if(::epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &ev) == -1){
        std::cout<<"modify_event error:epoll_ctl\n";
        ::exit(0);
    }
}

void accept_socket(int epoll_fd,int listen_fd){
    struct ::sockaddr_in client_addr;
    ::socklen_t client_len = sizeof(struct ::sockaddr_in);
    int client_fd = ::accept(listen_fd, (struct ::sockaddr*)&client_addr, &client_len);
    if(client_fd == -1){
        std::cout<<"accept error\n";
    } else {
        char* ip = ::inet_ntoa(client_addr.sin_addr);
        std::cout<<"connect client:ip->"<<ip<<" port->"<<client_addr.sin_port<<'\n';
        setNonBlock(client_fd);
        std::string wel_msg{"welcome "};
        wel_msg += ip;
        wel_msg += "\n";
        conn_fds.push_back(client_fd);
        add_epoll_event(epoll_fd,client_fd,::EPOLLIN);
        for(int fd : conn_fds){
            ::write(fd,wel_msg.c_str(),wel_msg.length());
        }
    }
}

void read_socket(int epoll_fd,int fd,char* buf){
    while(true){
        auto len = ::read(fd,buf,BUF_MAX);
        if(len == -1){
            std::cout<<"client close.\n";
            delete_epoll_event(epoll_fd,fd,::EPOLLIN);
            break;
        } else if(len == 0){
            std::cout<<"client shutdown.\n";
            delete_epoll_event(epoll_fd,fd,::EPOLLIN);
            break;
        } else {
            for(int fd : conn_fds){
                ::write(fd,buf,len);
            }
            if(len <= BUF_MAX){
                continue;
            } else {
                for(int fd : conn_fds){
                    ::write(fd,"\n",1);
                }
                break;
            }
        }    
    }
}

void write_socket(int epoll_fd,int fd,char* buf){
    ::strncpy(buf, "server echo!", 20);
    auto len = ::write(fd, buf, BUF_MAX);
    if(len == -1){
        std::cout<<"client shutdown.\n";
        delete_epoll_event(epoll_fd,fd,::EPOLLOUT);
    } else {
        modify_epoll_event(epoll_fd,fd,::EPOLLIN);
    }
}

void handle_events(int epoll_fd,struct ::epoll_event *events,int nfds,int listen_fd,char* buf){
    int fd;
    for(int i=0;i<nfds;++i){
        fd = events[i].data.fd;
        if((fd == listen_fd) && (events[i].events & ::EPOLLIN)){
            accept_socket(epoll_fd,listen_fd);
        } else if(events[i].events & ::EPOLLIN){
            read_socket(epoll_fd,events[i].data.fd,buf);
        } else if(events[i].events & ::EPOLLOUT){
            std::memset(buf, 0, BUF_MAX);
            write_socket(epoll_fd,events[i].data.fd,buf);
        }
    }
}


/* init epoll and loop fds to handle event*/
void epoll_run(int listen_fd){
    int epoll_fd = ::epoll_create1(0);
    if(epoll_fd == -1){
        INFO("error epoll create");
        ::exit(0);
    }
    add_epoll_event(epoll_fd,listen_fd,::EPOLLIN);
    //loop for epoll_wait and looping events to do things
    int nfds = -1;
    struct ::epoll_event events[EPOLL_MAX_EVENTS];
    char buf[BUF_MAX];
    for(;;){
        nfds = ::epoll_wait(epoll_fd, events, EPOLL_MAX_EVENTS, -1);
        if(nfds == -1){
            std::cout<<"epoll_wait error"<<'\n';
            ::exit(0);
        }
        handle_events(epoll_fd,events,nfds,listen_fd,buf);
    }
    ::close(epoll_fd);
}


int main(){
    INFO("server start!");
    //bind a port to listen
    int listen_fd = bind_socket(PORT);
    ::listen(listen_fd,0);
    INFO("bind and listen ON:"+std::to_string(PORT));
    epoll_run(listen_fd);
    return 0;
}
