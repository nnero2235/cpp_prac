#ifndef TCP_CLIENT_NNERO_HPP
#define TCP_CLIENT_NNERO_HPP

#include"tcp_connection.hpp"

namespace nnero{
    namespace net{

        //tcp client for tcp socket
        class TCPClient;

        class TCPClient{
        public:
            TCPClient(const std::string& ip,const unsigned int& port);
            TCPClient(const TCPClient&) = delete;
            TCPClient(TCPClient&&) = delete;
            TCPClient& operator=(const TCPClient&) = delete;
            TCPClient& operator=(TCPClient&&) = delete;
            ~TCPClient();

            void connect();
        private:
            TCPConnection m_conn;
        };
    }
}

#endif
