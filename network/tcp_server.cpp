#include"tcp_server.hpp"
#include"../nnero/logging.hpp"

using namespace nnero::net;
using namespace nnero::logging;

TCPServer::TCPServer(const unsigned int& port,const std::function<void(TCPConnection*)>& callback):
    m_server_socket{port},
    m_callback{callback}
{}

TCPServer::~TCPServer(){
    if(!m_shutdown){
        m_shutdown = true;
        m_connection_pool.shutdown();
    }
}

void TCPServer::start(){
    if(!m_callback){
        throw std::runtime_error("callback is empty");
    }
    if(!m_shutdown and !m_server_socket.isListened()){
        m_server_socket.bindAndListen();
    }
    LOG(INFO)<<"server started at:"<<m_server_socket.getAdress().getPort();
    while(!m_shutdown){
        TCPSocket client = m_server_socket.accept();
        if(m_current_clients == MAX_CLIENTS){
            LOG(INFO)<<"clients is full. So close it";
            client.close();
        } else {
            ++m_current_clients;
            std::shared_ptr<TCPConnection> conn = newConnection(client);
            //            m_connections.push_back(conn);
            auto f = std::bind(&TCPServer::handleMsg,this,conn);
            m_connection_pool.execute(f);
            LOG(INFO)<<"connection from "<<conn->getIp()<<":"<<conn->getPort();
        }
    }
}

void TCPServer::handleMsg(const std::shared_ptr<TCPConnection>& conn){
    m_callback(conn.get());
    std::lock_guard<std::mutex> lock(m_main_lock);
    --m_current_clients;
}

std::shared_ptr<TCPConnection> TCPServer::newConnection(TCPSocket& socket){
    std::shared_ptr<TCPConnection> conn(new TCPConnection(socket));
    return conn;
}
