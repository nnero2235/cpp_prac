#ifndef NNERO_COMMON_HPP
#define NNERO_COMMON_HPP
#include<iostream>
#include<chrono>
#include<iomanip>
#include<string>

namespace nnero{
    void INFO(const std::string& msg){
        auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::cout<<"["<<std::put_time(std::localtime(&t),"%Y-%m-%d %X")<<"]: "<<msg<<'\n';
    }
}

#endif
