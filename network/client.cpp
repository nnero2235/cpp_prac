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
#include<vector>

class Client{
public:
    Client(const std::string& host,unsigned int port):
        host(host),
        port(port)
    {}
    void connectToServer()const{
        int sock_fd;
        struct sockaddr_in server_addr;
        sock_fd = socket(AF_INET,SOCK_STREAM,0);
        memset(&server_addr,0,sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET,host.c_str(),&server_addr.sin_addr);

        int error = connect(sock_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
        if(error != 0){
            std::cout<<"error connect..."<<'\n';
            exit(0);
        }
        std::cout<<"client connect successful!"<<'\n';
        char buf[1024];
        ::strncpy(buf, "how are you!", 1024);
        ::write(sock_fd,buf,1024);
        ::close(sock_fd);
    }
private:
    std::string host{"127.0.0.1"};
    unsigned int port{8888};
};

int main(){
    Client c("127.0.0.1",8080);
    c.connectToServer();
    return 0;
}
