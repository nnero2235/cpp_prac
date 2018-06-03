#include<iostream>
#include<thread>
#include<list>
#include<functional>
#include<algorithm>
#include<typeinfo>
#include<memory>

template<typename T>
void printAddr(const T& t){
    std::cout<<typeid(t).name()<<"-->mem addr:"<<&t<<'\n';
}

int count = 0;

template<typename T>
std::list<T> q_sort(std::list<T> input){
    if(input.empty()){
        return input;
    }
    std::list<T> result;
    result.splice(result.end(), input,input.begin());
    const T& divide = *result.begin();
    auto divide_iter = std::partition(input.begin(), input.end(), [&](const T& t){return divide < t;});
    std::list<T> low_part;
    low_part.splice(low_part.end(), input,input.begin(),divide_iter);
    std::list<T> low = q_sort(std::move(low_part));
    std::list<T> high = q_sort(std::move(input));
    result.splice(result.begin(), low);
    result.splice(result.end(), high);
    return result;
}

int main(){
    std::list<int> l;
    l.push_back(11);
    l.push_back(20);
    l.push_back(15);
    l.push_back(2);
    l.push_back(8);
    l.push_back(5);
    l.push_back(45);
    l.push_back(99);
    l.push_back(22);
    l.push_back(1);
    std::list<int> nl{q_sort(l)};
    std::for_each(l.begin(), l.end(), [&](const int& t){std::cout<<t<<'\n';});
    std::for_each(nl.begin(), nl.end(), [&](const int& t){std::cout<<t<<'\n';});
    return 0;
}
