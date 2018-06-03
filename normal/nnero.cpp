#include "nnero.h"

Screen& Screen::setX(Screen::s_int x){
    this->width = x;
    return *this;
}

Screen& Screen::setY(Screen::s_int y){
    this->height = y;
    return *this;
}

Screen& Screen::setContents(const std::string& contents){
    this->contents = contents;
    this->contents += ",HI";
    return *this;
}

void Screen::display()const{
    std::cout<<contents<<" display on "<<width<<","<<height<<std::endl;
}
