#include <iostream>
#include <string>
#include "cal.hpp"
#include <memory>
#include <sstream>
#include <exception>
#include <stdexcept>

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
    } else if('/' == c){
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
    } else if(c >= 48 and c <= 57){
        std::unique_ptr<Elem> e(new Elem(c,EType::NUMBER));
        return e;
    } else {
        std::unique_ptr<Elem> e(new Elem(c,EType::UNKNOW));
        return e;
    }
}

std::unique_ptr<CalResult> Calculator::eval(const std::string& expr){
    std::istringstream istrm(expr);
    char c = ' ';
    while(istrm >> c){
        std::unique_ptr<Elem> e = Elem::parseElem(c);
        switch(e->type){
        case Elem::UNKNOW:{
            std::ostringstream ostrm;
            ostrm << "unknow symbol '";
            ostrm << e->c;
            ostrm << "' can't parse. please check out your expr!";
            std::unique_ptr<CalResult> r(new CalResult(CalResult::CODE_FAIL,0, ostrm.str()));
            clearStack();
            return r;
        }
        case Elem::NUMBER:{
            bufNumber(e->c);
            continue;
        }
        case Elem::RIGHT_PARENTH:{
            evalNumber();
            elem_stack.push(*e);
            evalStack();
            continue;
        }
        default:{
            evalNumber();
            switch(e->type){
            case Elem::EType::SUB:
            case Elem::EType::ADD:{
                evalStack();
                break;
            }
            default:
                break;
            }
            elem_stack.push(*e);
            break;
        }}
    }
    evalNumber();
    evalStack();
    if(elem_stack.size() > 1){
        std::ostringstream ostrm;
        ostrm << "error expression '";
        ostrm << expr;
        ostrm << "'!";
        std::unique_ptr<CalResult> r(new CalResult(CalResult::CODE_FAIL,0, ostrm.str()));
        clearStack();
        return r;         
    }
    std::unique_ptr<CalResult> r(new CalResult(CalResult::CODE_SUCCESS,elem_stack.top().value,"ok"));
    elem_stack.pop();
    return r;
}

void Calculator::bufNumber(const char& c){
    numberBuf[index] = c;
    ++index;
}

void Calculator::evalNumber(){
    if(index > 0){
        numberBuf[index] = '\0';
        Elem e('\0',Elem::EType::VALUE);
        e.value = std::stoi(numberBuf);
        elem_stack.push(e);
        index = 0;
    }
}

void Calculator::clearStack(){
    while(!elem_stack.empty()){
        elem_stack.pop();
    }
}

void Calculator::evalStack(){
    if(elem_stack.size() < 3){
        return;
    }
    bool parenthing = false;
    while(elem_stack.size() > 1){
        Elem& opt_right = elem_stack.top();
        elem_stack.pop();
        if(opt_right.type == Elem::EType::RIGHT_PARENTH){
            parenthing = true;
            continue;
        }
        if(opt_right.type != Elem::EType::VALUE){
            throw std::runtime_error("error expr : "+std::to_string(opt_right.type));
        }
        Elem& opt_eval = elem_stack.top();
        if(opt_eval.type == Elem::EType::LEFT_PARENTH){
            if(parenthing){
                elem_stack.pop();
                parenthing = false;
            }
            elem_stack.push(opt_right);
            break;
        }
        elem_stack.pop();
        Elem& opt_left = elem_stack.top();
        elem_stack.pop();
        Elem v('\0',Elem::EType::VALUE);
        v.value = evalOpt(opt_left,opt_eval,opt_right);
        elem_stack.push(v);
    }
    if(parenthing){
        throw std::runtime_error("missing '(' in expr");
    }
}

double Calculator::evalOpt(const Elem& left,const Elem& opt,const Elem& right){
    switch(opt.type){
    case Elem::EType::ADD:{
        double value = left.value + right.value;
        return value;
    }
    case Elem::EType::SUB:{
        double value = left.value -  right.value;
        return value;
    }
    case Elem::EType::MULTI:{
        double value = left.value * right.value;
        return value;
    }
    case Elem::EType::DIV:{
        double value = left.value / right.value;
        return value;
    }
    default:
        throw std::runtime_error("unknow opt!");
    }
}

int main(){
    std::cout<<"welcome to cal!"<<'\n';
    std::string line;
    Calculator cal;
    while(std::cin >> line){
        if(line == "exit"){
            std::cout<<"exit cal!"<<std::endl;
            break;
        }
        try{
            std::unique_ptr<CalResult> r = cal.eval(line);
            if(r->isSuccess()){
                std::cout<<r->getResult()<<std::endl;
            } else {
                std::cout<<r->getMsg()<<std::endl;
            }    
        } catch (std::runtime_error &e){
            std::cout<<e.what()<<std::endl;
        }
    }
    return 0;
}
