#include"tcp_socket.hpp"
#include<arpa/inet.h>
#include<unistd.h> //for close
#include<sys/socket.h> 
#include<cstring> //for memset
#include"../nnero/logging.hpp"
#include<stdexcept>

#define sock_cast(addr_in) (struct sockaddr*)&addr_in

static constexpr unsigned int BACKLOG = 512;
static constexpr unsigned int SOCK_ADDR_SIZE = sizeof(struct sockaddr);

using namespace nnero::net;
using namespace nnero::logging;

InetAdress::InetAdress(const unsigned int& port):
    m_port{port}
{}

InetAdress::InetAdress(const std::string& ip,const unsigned int& port):
    m_ip{ip},
    m_port{port}
{}

InetAdress::InetAdress(const struct sockaddr_in& raw_addr):
    m_ip{::inet_ntoa(raw_addr.sin_addr)},
    m_port{::ntohs(raw_addr.sin_port)}
{}

InetAdress::InetAdress(const InetAdress& addr){
    m_ip = addr.m_ip;
    m_port = addr.m_port;
}

InetAdress& InetAdress::operator=(const InetAdress& addr){
    if(this != &addr){
        m_ip = addr.m_ip;
        m_port = addr.m_port;
    }
    return *this;
}

const char* InetAdress::getIpCString()const{
    return m_ip.c_str();
}

std::string InetAdress::getIp()const{
    return m_ip;
}

unsigned int InetAdress::getPort()const{
    return m_port;
}

TCPSocket::TCPSocket(const unsigned int& port):
    m_addr{port}
{}

TCPSocket::TCPSocket(const std::string& ip,const unsigned int& port):
    m_addr{ip,port}
{}

TCPSocket::TCPSocket(const int& fd,const struct sockaddr_in& raw_addr):
    m_fd{fd},
    m_addr{raw_addr},
    m_is_connected{true}
{}

TCPSocket::TCPSocket(TCPSocket&& sock):
    m_addr{0}
{
    *this = std::move(sock);
}

TCPSocket& TCPSocket::operator=(TCPSocket&& sock){
    if(this != &sock){
        this->m_addr = sock.m_addr;
        this->m_fd = sock.m_fd;
    }
    return *this;
}
            
void TCPSocket::bindAndListen(){
    if(m_is_connected){
        throw std::runtime_error("socket has already bean connected.So it can't listen");
    }

    if(m_is_listened){
        throw std::runtime_error("socket already has been listend.");
    }

    if(m_fd != -1){
        throw std::runtime_error("fd has already init. can't listen");
    }

    if(m_addr.getPort() < 0){
        throw std::runtime_error("port < 0. can't listen");
    }
    
    //get socket from kernel
    m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(m_fd == -1){
        LOG(FATAL)<<"get socket from kernel fail!";
        abort();
    }

    struct sockaddr_in server_addr;
    //init server ip and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = ::htons(m_addr.m_port);
    server_addr.sin_addr.s_addr = ::htonl(INADDR_ANY);

    //set 0 to other attr.
    ::memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));

    //bind ip and port
    if(::bind(m_fd,sock_cast(server_addr),SOCK_ADDR_SIZE) == -1){
        LOG(FATAL)<<"bind fail!";
        abort();
    }

    //listen on port
    if(::listen(m_fd, BACKLOG) == -1){
        LOG(FATAL)<<"listen fail!";
        abort();
    }
    m_is_listened = true;
}

TCPSocket TCPSocket::accept(){
    if(m_is_connected){
        throw std::runtime_error("socket has already connected.can't accept");
    }
    if(!m_is_listened){
        throw std::runtime_error("socket is not listened.it should bindAndlisten first");
    }
    struct sockaddr_in client_addr;
    unsigned int sin_size = SOCK_ADDR_SIZE;
    int c_fd = ::accept(m_fd, sock_cast(client_addr), &sin_size);
    if(c_fd == -1){
        throw std::runtime_error("accept error!");
    }
    TCPSocket client_sock(c_fd,client_addr);
    return client_sock;
}


void TCPSocket::connect(){
    if(m_is_listened or m_is_connected){
        throw std::runtime_error("socket has already listened or connected.can't connect.");
    }
    //get socket from kernel
    m_fd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(m_fd == -1){
        LOG(FATAL)<<"socket fail!";
        abort();
    }

    struct sockaddr_in server_addr;
    //init server ip and port
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = ::htons(m_addr.m_port);
    server_addr.sin_addr.s_addr = ::inet_addr(m_addr.getIpCString());

    //set 0 to other attr.
    ::memset(&server_addr.sin_zero, 0, sizeof(server_addr.sin_zero));
    
    if(::connect(m_fd, sock_cast(server_addr), SOCK_ADDR_SIZE) == -1){
        LOG(FATAL)<<"connect fail";
    }
    m_is_connected = true;
}

void TCPSocket::close(){
    if(m_is_listened or m_is_connected or m_fd != -1){
        ::close(m_fd);
        m_fd = -1;
        m_is_listened = false;
        m_is_connected = false;
    }
}

int TCPSocket::getSocketFD()const{
    return m_fd;
}
InetAdress TCPSocket::getAdress()const{
    return m_addr;
}
bool TCPSocket::isConnected()const{
    return m_is_connected;
}
bool TCPSocket::isListened()const{
    return m_is_listened;
}






