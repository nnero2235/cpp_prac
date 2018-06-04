#include<iostream>
#include<thread>
#include<algorithm>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>
#include<string>
#include<vector>

const std::string HOST{"localhost"};
constexpr int PORT = 8080;

int connect_sock(const std::string& host,int port){
    struct ::sockaddr_in server_addr;
    int sock_fd = ::socket(AF_INET,SOCK_STREAM,0);
    ::memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = ::htons(port);
    ::inet_pton(AF_INET,host.c_str(),&server_addr.sin_addr);
    if(::connect(sock_fd,(struct ::sockaddr*)&server_addr,sizeof(server_addr)) == -1){
        std::cout<<"error connect..."<<'\n';
        exit(0);
    }
    return sock_fd;
}

int main(){
    int conn_fd = connect_sock(HOST,PORT);
    std::string msg;
    char buf[256];
    int len = 0;
    for(;;){
        while(true){
            len = ::read(conn_fd,buf,256);
            std::cout<<buf;
            if(len < 256){
                break;
            }
        }
        std::cout<<std::endl;
        
        std::cin >> msg;
        if(msg == "bye"){
            break;
        }
        ::write(conn_fd, msg.c_str(), msg.length());
    }
    std::cout<<"bye bye!\n";
    ::close(conn_fd);
    return 0;
}
