#include<iostream>
#include<algorithm>
#include<functional>
#include<list>

void printList(const std::list<int>& l){
    std::for_each(l.begin(), l.end(), [&](const int& i){ std::cout<<i<<'\n';});
}

template <typename T>
std::list<T> q_sort(std::list<T> input){
    if(input.empty()){
        return input;
    }
    std::list<T> result;
    result.splice(result.end(), input,input.begin());
    const T& divide_point = *input.begin();
    auto divide_point_iter = std::partition(input.begin(), input.end(),
                                            [&](const T& t){return t < divide_point;});
    std::list<T> low_part;
    low_part.splice(low_part.end(), input,input.begin(),divide_point_iter);

    std::list<T> low = q_sort(std::move(low_part));
    std::list<T> high = q_sort(std::move(input));

    result.splice(result.end(), high);
    result.splice(result.begin(), low);
    return result;
}

int main(){
    std::list<int> l;
    l.push_back(8);
    l.push_back(11);
    l.push_back(1);
    l.push_back(7);
    l.push_back(9);
    l.push_back(2);
    l = q_sort(l);
    printList(l);
    return 0;
}
