#include"tcp_server.hpp"
#include"../nnero/logging.hpp"
#include<vector>

int main(){
    using namespace nnero::net;
    using namespace nnero::logging;
    std::vector<TCPConnection*> active_conns;
    auto callback = [&active_conns](TCPConnection* conn){
        std::string msg;
        msg.append("conn ");
        msg.append(std::to_string(conn->getPort()));
        msg.append(" join to chatroom!\n");
        auto begin = active_conns.begin();
        while(begin != active_conns.end()){
            (*begin)->writeBytes(msg.c_str(), msg.length());
            ++begin;
        }
        active_conns.push_back(conn);
        char buf[32];
        while(true){
            msg.clear();
            msg.append(std::to_string(conn->getPort()));
            msg.append(" say:");
            while(true){
                int bytes = conn->readBytes(buf, 32);
                if(bytes == 0 || bytes == -1){
                    LOG(INFO)<<"client has closed.";
                    auto begin = active_conns.begin();
                    while(begin != active_conns.end()){
                        if((*begin)->getPort() == conn->getPort()){
                            break;
                        }
                        ++begin;
                    }
                    active_conns.erase(begin);
                    return;
                }
                if(buf[bytes-1] == '\n'){
                    msg.append(buf,bytes);
                    break;
                }
                msg.append(buf,bytes);
            }
            auto begin = active_conns.begin();
            while(begin != active_conns.end()){
                if((*begin)->getPort() != conn->getPort()){
                    (*begin)->writeBytes(msg.c_str(), msg.length());
                }
                ++begin;
            }
        }
    };
    
    TCPServer server(8080,callback);
    server.start();
    return 0;
}
