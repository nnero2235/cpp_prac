#include"tcp_server.hpp"
#include"../nnero/logging.hpp"

using namespace nnero::logging;

namespace nnero{
    namespace net{
        TCPServer::TCPServer(const unsigned int& port,
                             const std::function<void(TCPConnection*)>& read_callback,
                             const std::function<void(TCPConnection*)>& write_callback,
                             const std::function<void(TCPConnection*)>& connected_callback):
            m_server_socket{port},
            m_read_callback{read_callback},
            m_write_callback{write_callback},
            m_connected_callback{connected_callback}
        {}

        TCPServer::~TCPServer(){
            if(!m_shutdown){
                m_shutdown = true;
                m_connection_pool.shutdown();
            }
        }

        void TCPServer::start(){
            if(!m_shutdown and !m_server_socket.isListened()){
                m_server_socket.bindAndListen();
            }
            LOG(INFO)<<"server started at:"<<m_server_socket.getAdress().getPort();
            m_poller.registerSocket(m_server_socket);
        }

        void TCPServer::handleEvent(){
            m_poller.poll();
            const std::map<int,IO_STATE>& result_map = m_poller.getResultMap();
            if(result_map.empty()){
                return;
            }
            auto begin = result_map.begin();
            while(begin != result_map.end()){
                if(begin->second == IO_STATE::READ){
                    if(begin->first == m_server_socket.getSocketFD()){
                        //server_sock so do accept
                        TCPSocket client = m_server_socket.accept();
                        if(m_current_clients == MAX_CLIENTS){
                            LOG(INFO)<<"clients is full. So close it";
                            client.close();
                        } else {
                            ++m_current_clients;
                            int fd = client.getSocketFD();
                            std::shared_ptr<TCPConnection> conn = newConnection(client);
                            m_connection_map[fd] = conn;
                            if(m_connected_callback){
                                m_connected_callback(conn.get());
                            }
                            m_poller.registerSocket(client);
                            LOG(INFO)<<"connection from "<<conn->getIp()<<":"<<conn->getPort();
                        }
                    } else {
                        if(m_read_callback){
                            m_read_callback(m_connection_map[begin->first].get());
                        }
                    }
                } else if(begin->second == IO_STATE::WRITE){
                    if(m_write_callback){
                        m_write_callback(m_connection_map[begin->first].get());
                    }
                } else {
                    std::lock_guard<std::mutex> lock(m_main_lock);
                    --m_current_clients;
                    m_connection_map.erase(begin->first);
                }
                ++begin;
            }
        }

        std::shared_ptr<TCPConnection> TCPServer::newConnection(TCPSocket& socket){
            std::shared_ptr<TCPConnection> conn(new TCPConnection(socket));
            return conn;
        }
    }
}
