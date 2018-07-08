#include"../network/tcp_socket.hpp"
#include<gtest/gtest.h>
#include"../nnero/logging.hpp"
#include"../network/event_loop.hpp"
#include<thread>
#include<stdexcept>

TEST(SOCKET,simple){
    using namespace nnero::net;
    using namespace nnero::logging;
    
    auto server = [](){
        TCPSocket server_sock(8080);
        if(!server_sock.isListened()){
            server_sock.bindAndListen();
            TCPSocket c_sock = server_sock.accept();
            if(c_sock.isConnected()){
                InetAdress addr = c_sock.getAdress();
                LOG(INFO)<<"connected from "<<addr.getIp()<<":"<<addr.getPort();
                char buf[12] = "hello nnero";
                ::write(c_sock.getSocketFD(), buf, 12);
                c_sock.close();
                server_sock.close();
                LOG(INFO)<<"server closed";
            } else {
                LOG(ERROR)<<"error is not connected";
            }
        } else {
            LOG(ERROR)<<"already listen";
        }
    };

    auto client = [](){
        std::this_thread::sleep_for(std::chrono::seconds(3));
        TCPSocket client_sock(8080);
        client_sock.connect();
        if(client_sock.isConnected()){
            InetAdress addr = client_sock.getAdress();
            LOG(INFO)<<"connected to "<<addr.getIp()<<":"<<addr.getPort();
            char buf[32] = {0};
            ::read(client_sock.getSocketFD(),buf,32);
            ASSERT_STREQ(buf,"hello nnero");
            LOG(INFO)<<"msg from server:"<<buf;
        } else {
            LOG(ERROR)<<"client error not connected";
        }
    };

    std::thread server_t(server);
    std::thread client_t(client);

    server_t.join();
    client_t.join();
}

TEST(EVENTLOOP,normal){
    using namespace nnero::net;

    EventLoop event_loop;
    event_loop.loop();

    auto e_l = [](){
        EventLoop el;
        el.loop();
    };
    
    std::thread t(e_l);

    t.join();
}

TEST(EVENTLOOP,error1){
    using namespace nnero::net;
    using namespace nnero::logging;

    EventLoop event_loop;

    try{
        std::thread t(&EventLoop::loop,&event_loop);
        t.join();
    } catch(const std::runtime_error& e){
        ASSERT_STREQ(e.what(),"event loop is not create in this thread.abort!");
        LOG(ERROR)<<e.what();
    }

}

TEST(EVENTLOOP,error2){
    using namespace nnero::net;
    using namespace nnero::logging;
    try{
        EventLoop el1;
        EventLoop el2;
    }catch(const std::runtime_error& e){
        ASSERT_STREQ(e.what(),"this thread has already a eventLoop.");
        LOG(ERROR)<<e.what();
    }
}
