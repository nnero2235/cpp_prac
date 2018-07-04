#include<iostream>
#include<thread>
#include<vector>
#include<algorithm>
#include<functional>
#include<mutex>

void process(int& i,int j,std::mutex& m){
    std::lock_guard<std::mutex> lg(m);
    std::cout<<i<<'\n';
    i = i+j;
}

class Obj{
public:
    Obj() = default;
    void operator()(int& i){
        ++m_j;
        ++i;
        std::cout<<"i+j:"<<m_j+i<<'\n';
    }
private:
    int m_j{0};
};


int main(){
    std::vector<std::thread> threads;
    int integer = 0;
    std::mutex m;
    for(int i=0;i<5;i++){
        std::thread t(process,std::ref(integer),i,std::ref(m));
        threads.push_back(std::move(t));
    }
    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
    std::cout<<integer<<'\n';
    return 0;
}
