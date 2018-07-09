#ifndef TCP_SOCKET_NNERO_HPP
#define TCP_SOCKET_NNERO_HPP

#include<string>
#include<sys/socket.h>
#include<netinet/in.h>

namespace nnero{
    namespace net{

        //for ip and port
        class InetAdress;

        //pack socket api
        class TCPSocket;

        class InetAdress{
            friend class TCPSocket;
        public:
            InetAdress(const unsigned int& port);
            InetAdress(const std::string& ip,const unsigned int& port);
            InetAdress(const struct sockaddr_in& raw_addr);
            InetAdress(const InetAdress&);
            InetAdress& operator=(const InetAdress&);
            ~InetAdress() = default;
            const char* getIpCString()const;
            std::string getIp()const;
            unsigned int getPort()const;
        private:
            std::string m_ip{"127.0.0.1"};
            unsigned int m_port;
        };

        class TCPSocket{
        public:
            //for server socket
            TCPSocket(const unsigned int& port);
            //for client socket
            TCPSocket(const std::string& ip,const unsigned int& port);
            //init by fd and addr
            //it means tcpsocket already get
            //it should not use bind or accept or connect
            TCPSocket(const int& fd,const struct sockaddr_in& raw_addr);
            //not allow copy but allow move
            TCPSocket(const TCPSocket&) = delete;
            TCPSocket& operator=(const TCPSocket&) = delete;
            TCPSocket(TCPSocket&&);
            TCPSocket& operator=(TCPSocket&&);
            ~TCPSocket() = default;

            // exit when bind error or listen error
            // for server socket
            void bindAndListen();
            //return a client socket when accept success
            TCPSocket accept();
            //for client socket
            //connect to server
            //throw std::runtime_error when connect fail
            void connect();
            //close listen socket or connected socket
            //if socket is not listened or connected,do nothing.
            void close();

            void shutdownRead();
            void shutdownWrite();

            int getSocketFD()const;
            const InetAdress& getAdress()const;
            bool isConnected()const;
            bool isListened()const;
        private:
            int m_fd{-1}; //socket fd. -1 means socket not init
            InetAdress m_addr;
            bool m_is_connected{false}; 
            bool m_is_listened{false};
        };
    }
}

#endif
