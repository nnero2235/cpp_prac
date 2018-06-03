#include"common.hpp"
#include<iostream>
#include<thread>
#include<algorithm>
#include<sys/socket.h>
#include<netinet/in.h>
#include<cstring>
#include<unistd.h>
#include<arpa/inet.h>
#include<string>

void printClientInput(int sock_fd){
    write(sock_fd,"server success",15);
}

class Server{
public:
    Server(unsigned int port):
        port(port)
    {}
    void start()const{
        int listen_fd;
        int connect_fd;
        struct sockaddr_in server_addr;
        struct sockaddr_in client_addr;
        listen_fd = socket(AF_INET,SOCK_STREAM,0);
        memset(&server_addr,0,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        server_addr.sin_port = htons(port);
        bind(listen_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
        listen(listen_fd,0);
        std::cout<<"server start... at port:"<<port<<'\n';
        for(;;){
            connect_fd = accept(listen_fd,(struct sockaddr*)&client_addr,nullptr);
            std::cout<<"connected client:ip->"<<inet_ntoa(client_addr.sin_addr)<<" port->"<<client_addr.sin_port<<'\n';
            std::thread t(printClientInput,connect_fd);
            t.detach();
            close(connect_fd);
        }
    }
private:
    unsigned int port{8888};
};

int main(){
    Server s(8888);
    s.start();
    return 0;
}
