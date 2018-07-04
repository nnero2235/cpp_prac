#ifndef PRAC_HPP
#define PRAC_HPP

#include<string>
#include<iostream>
#include<memory>

namespace prac{
    template <typename T1,typename T2>
    bool compare(const T1& t1,const T2& t2){
        return t1 < t2;
    }

    class Cat{
    public:
        Cat(const std::string& name,int age):
            name(name),
            age(age)
        {}
        bool operator<(const Cat& c)const{
            return this->age < c.age;
        }
        std::string getName(){
            return name;
        }
    private:
        std::string name{""};
        int age{0};
    };

    template <typename T>
    class Animal{
    public:
        Animal(std::shared_ptr<T> p):
            t_ptr(p)
        {}
        virtual void fuck(){
            std::cout<<"animal fuck"<<std::endl;
        }
        T& get(){
            return *t_ptr;
        }
    private:
        std::shared_ptr<T> t_ptr;
    };

    class Person:public Animal<Cat>{
    public:
        using Animal<Cat>::Animal;
        void fuck()override{
            std::cout<<"person fuck"<<std::endl;
        }
    };
}

#endif
