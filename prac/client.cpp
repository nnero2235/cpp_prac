#include<sys/socket.h>
#include<arpa/inet.h>
#include<cstring>
#include<string>
#include"../nnero/logging.hpp"
#include<unistd.h>
#include<iostream>
#include<sstream>

#define sock_cast(addr_in) (struct sockaddr*)&addr_in

int main(){
    using namespace nnero::logging;

    static const std::string IP="127.0.0.1";
    static constexpr unsigned int PORT = 8080;
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
    server_addr.sin_addr.s_addr = ::inet_addr(IP.c_str());

    //set 0 to other attr.
    ::memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
    
    if(::connect(sock_fd, sock_cast(server_addr), SOCK_ADDR_SIZE) == -1){
        LOG(FATAL)<<"connect fail";
    } else {
        LOG(INFO)<<"connected-> "<<IP<<":"<<PORT;
        char buf[128];
        std::string msg;
        while(true){
            std::getline(std::cin,msg);
            if(msg == "exit"){
                break;
            }
            ::memcpy(buf, msg.c_str(), msg.length()+1);
            ::write(sock_fd,buf,msg.length()+1);
            ::memset(buf, 0, 128);
            msg.clear();
            int recv_len = read(sock_fd,buf,128);
            if(recv_len == -1){
                LOG(INFO)<<"connection close.";
                ::close(sock_fd);
                abort();
            }
            msg.append(buf,recv_len);
            LOG(INFO)<<msg;
        }
    }
    ::close(sock_fd);
    return 0;
}
