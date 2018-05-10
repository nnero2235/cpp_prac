#include <iostream>
#include <string>
#include "cal.hpp"
#include <memory>
#include <sstream>

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

std::unique_ptr<Elem> Elem::parseElem(const char& c){
    if ('+' == c){
        std::unique_ptr<Elem> e(new Elem(c,EType::ADD));
        return e;
    } else if('-' == c){
        std::unique_ptr<Elem> e(new Elem(c,EType::SUB));
        return e;
    } else if('\\' == c){
        std::unique_ptr<Elem> e(new Elem(c,EType::DIV));
        return e;
    } else if('*' == c){
        std::unique_ptr<Elem> e(new Elem(c,EType::MULTI));
        return e;
    } else if('(' == c){
        std::unique_ptr<Elem> e(new Elem(c,EType::LEFT_PARENTH));
        return e;
    } else if(')' == c){
        std::unique_ptr<Elem> e(new Elem(c,EType::RIGHT_PARENTH));
        return e;
    } else if(c >= 30 and c <= 39){
        std::unique_ptr<Elem> e(new Elem(c,EType::NUMBER));
        return e;
    } else {
        std::unique_ptr<Elem> e(new Elem(c,EType::UNKNOW));
        return e;
    }
}

std::unique_ptr<CalResult> Calculator::parse(const std::string& expr){
    std::istringstream istrm(expr);
    char c = ' ';
    char buf[64] = {'\0'};
    size_t index = 0;
    while(istrm >> c){
        std::unique_ptr<Elem> e = Elem::parseElem(c);
        switch(e->type){
        case Elem::UNKNOW:{
            std::ostringstream ostrm("unknow symbol '");
            ostrm << e->c;
            ostrm << "' can't parse. please check out your expr!";
            std::unique_ptr<CalResult> r(new CalResult(CalResult::CODE_FAIL,0, ostrm.str()));
            return r;
        }
        case Elem::NUMBER:{
            buf[index] = e->c;
            ++index;
            continue;
        }
        default:{
            if(index > 0){
                buf[index] = '\0';
                Elem e('\0',Elem::EType::VALUE);
                e.value = std::stoi(buf);
                elem_stack.push(e);
                index = 0;
            }
            elem_stack.push(*e);
            break;
        }}
    }
    std::unique_ptr<CalResult> r(new CalResult(CalResult::CODE_SUCCESS));
    return r;
}

int main(){
    return 0;
}
