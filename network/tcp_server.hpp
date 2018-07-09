#ifndef TCP_SERVER_NNERO_HPP
#define TCP_SERVER_NNERO_HPP

#include"tcp_connection.hpp"
#include"tcp_socket.hpp"
#include"../nnero/thread_pool.hpp"
#include<vector>
#include<memory>


namespace nnero{
    namespace net{

        using namespace nnero::net;
        using namespace nnero::thread;

        //tcp server manage tcp connections
        //response:
        //    create server socket. listen
        //    manage tcp connections
        class TCPServer;

        class TCPServer{
        public:
            static const int MAX_CLIENTS = 3;
            TCPServer(const unsigned int& port,const std::function<void(TCPConnection*)>& callback);
            //can't be copy and move
            TCPServer(const TCPServer&) = delete;
            TCPServer(TCPServer&&) = delete;
            TCPServer& operator=(const TCPServer&) = delete;
            TCPServer& operator=(TCPServer&&) = delete;
            ~TCPServer();

            //bind and listen
            //new connection in child thread
            //blocking in accept
            void start();
        private:
            void handleMsg(const std::shared_ptr<TCPConnection>& conn);
            std::shared_ptr<TCPConnection> newConnection(TCPSocket& socket);
            TCPSocket m_server_socket;
            ThreadPool m_connection_pool{MAX_CLIENTS};
            std::atomic<int> m_current_clients{0};
            bool m_shutdown{false};
            //            std::vector<std::shared_ptr<TCPConnection>> m_connections;
            mutable std::mutex m_main_lock;
            std::function<void(TCPConnection*)> m_callback;
        };
    }
}

#endif
