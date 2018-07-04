// -*-c++-*-
#ifndef NNERO_H
#define NNERO_H
#include<string>
#include<iostream>

class Screen{
public:
    using s_int = unsigned int;
    Screen() = default;
    Screen(s_int x,s_int y,const std::string& contents):
        width(x),
        height(y),
        contents(contents)
    {std::cout<<"three args init"<<std::endl;}
    
    Screen(s_int x,s_int y):Screen(x,y,""){
        std::cout<<"two args init"<<std::endl;
    }
    Screen& setX(s_int x);
    Screen& setY(s_int y);
    Screen& setContents(const std::string& contents);
    void display()const;
private:
    s_int width = 0;
    s_int height = 0;
    std::string contents;
};

#endif
