#include"tcp_connection.hpp"
#include"../nnero/logging.hpp"
#include<sys/socket.h>
#include<unistd.h>

using namespace nnero::net;
using namespace nnero::logging;

TCPConnection::TCPConnection(TCPSocket& socket):
    m_wbuf{0},
    m_socket{std::move(socket)}
{}


TCPConnection::TCPConnection(const std::string& ip,const unsigned int& port):
    m_wbuf{0},
    m_socket{ip,port}
{}

TCPConnection::~TCPConnection(){
    close();
}

void TCPConnection::connect(){
    m_socket.connect();
}

int TCPConnection::readBytes( char *buf, const size_t &buf_size ){
    if(m_socket.isConnected()){
        int bytes = ::read(m_socket.getSocketFD(),buf,buf_size);
        return bytes;
    } else {
        LOG(WARN)<<"socket is not connected";
    }
    return 0;
}

int TCPConnection::writeBytes( const char *bytes_ptr, const size_t &bytes_size ){
    if(m_socket.isConnected()){
        int bytes = ::write(m_socket.getSocketFD(), bytes_ptr, bytes_size);
        return bytes;
    }
    return 0;
}

void TCPConnection::shutdownRead(){
    m_socket.shutdownRead();
}

void TCPConnection::shutdownWrite(){
    m_socket.shutdownWrite();
}

void TCPConnection::close(){
    m_socket.close();
}

const char* TCPConnection::getIp()const{
    return m_socket.getAdress().getIpCString();
}

unsigned int TCPConnection::getPort()const{
    return m_socket.getAdress().getPort();
}

