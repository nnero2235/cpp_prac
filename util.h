// -*-c++-*-
#ifndef UTIL_H
#define UTIL_H
#include<iostream>
#include<vector>
#include<string>

inline void printVector(const std::vector<std::string>& v){
    for(const std::string& arg : v){
        std::cout<<arg<<std::endl;
    }
}

#endif
