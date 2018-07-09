#ifndef TCP_CONNECTION_NNERO_HPP
#define TCP_CONNECTION_NNERO_HPP

#include"tcp_socket.hpp"
#include<string>

namespace nnero{
    namespace net{

        using namespace nnero::net;
        
        //tcp connection
        //offer read write and close
        class TCPConnection;

        class TCPConnection{
        public:
            TCPConnection(TCPSocket& socket);
            TCPConnection(const std::string& ip,const unsigned int& port);
            //can't be copy and move
            TCPConnection(const TCPConnection&) = delete;
            TCPConnection(TCPConnection&&) = delete;
            TCPConnection& operator=(const TCPConnection&) = delete;
            TCPConnection& operator=(TCPConnection&&) = delete;
            ~TCPConnection();

            //for client socket to connect
            void connect();
            int readBytes(char* buf,const size_t& buf_size);
            int writeBytes(const char* bytes_ptr,const size_t& bytes_size);
            //it's different with close
            //it's half-close for read or write
            void shutdownRead();
            void shutdownWrite();
            void close();

            const char* getIp()const;
            unsigned int getPort()const;
        private:
            char m_wbuf[512];
            TCPSocket m_socket;
        };
    }
}

#endif
