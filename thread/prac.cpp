#include "prac.hpp"
#include <string>
#include <iostream>
#include <memory>
#include <typeinfo>

using namespace prac;

int main(){
    auto p1 = std::make_shared<Cat>("nnero",10);
    Cat c2("black",5);
    bool r = compare(*p1,c2);
    std::cout<<"result: "<<std::boolalpha<<r<<std::endl;

    Animal<Cat> a(p1);
    auto& c = a.get();
    std::cout<<c.getName()<<std::endl;
    
    std::cout<<"c1:"<<&*p1<<std::endl;
    std::cout<<"c:"<<&c<<std::endl;
    std::cout<<"use count:"<<p1.use_count()<<std::endl;

    Person p(p1);
    p.fuck();
    auto& c3 = p.get();
    std::cout<<"c3:"<<&c3<<std::endl;
    std::cout<<c3.getName()<<std::endl;
    return 0;
}
