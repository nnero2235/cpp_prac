#include <iostream>
#include <string>
#include "cal.hpp"
#include <memory>

using namespace Cal;

bool CalResult::isSuccess()const{
    return code == CODE_SUCCESS;
}

const std::string& CalResult::getMsg()const{
    return msg;
}

double CalResult::getResult()const{
    return result;
}

std::unique_ptr<Elem> Elem::parseElem(const std::string& symbol){
    if ("+" == symbol){
        std::unique_ptr<Elem> e(new Elem(0,EType::ADD));
        return e;
    } else if("-" == symbol){
        std::unique_ptr<Elem> e(new Elem(0,EType::SUB));
        return e;
    } else if("/" == symbol){
        std::unique_ptr<Elem> e(new Elem(0,EType::DIV));
        return e;
    } else if("*" == symbol){
        std::unique_ptr<Elem> e(new Elem(0,EType::MULTI));
        return e;
    } else if("(" == symbol){
        std::unique_ptr<Elem> e(new Elem(0,EType::LEFT_PARENTH));
        return e;
    } else if(")" == symbol){
        std::unique_ptr<Elem> e(new Elem(0,EType::RIGHT_PARENTH));
        return e;
    } 
}

int main(){
    return 0;
}
