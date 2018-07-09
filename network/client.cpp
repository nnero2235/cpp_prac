#include"tcp_connection.hpp"
#include"../nnero/logging.hpp"
#include<thread>
#include<iostream>

int main(){
    using namespace nnero::net;
    using namespace nnero::logging;
    TCPConnection conn("127.0.0.1",8080);
    conn.connect();
    LOG(INFO)<<"connected to "<<conn.getIp()<<":"<<conn.getPort();

    bool shutdown{false};
    auto output = [&conn,&shutdown](){
        char buf[32];
        std::string msg;
        while(!shutdown){
            msg.clear();
            while(true){
                int bytes = conn.readBytes(buf, 32);
                if(bytes == 0 || bytes == -1){
                    LOG(INFO)<<"server closed";
                    shutdown = true;
                    return;
                }
                if(buf[bytes-1] == '\n'){
                    msg.append(buf,bytes-1);
                    break;
                }
                msg.append(buf,bytes);
            }
            LOG(INFO)<<msg;
        }
    };
    
    std::thread output_thread(output);

    std::string msg;
    while(!shutdown){
        std::getline(std::cin, msg);
        if("exit" == msg){
            break;
        }
        msg.append("\n");
        conn.writeBytes(msg.c_str(), msg.length());
    }
    conn.close();
    output_thread.join();
    LOG(INFO)<<"exit!";
}
