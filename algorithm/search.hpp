#ifndef SEARCH_NNERO_HPP
#define SEARCH_NNERO_HPP

#include<iostream>
#include<memory>
#include<vector>

namespace nnero{
    namespace algorithm{
        /*
          simple search for array
          time: O(n)
         */
        std::unique_ptr<int> simpleSearch(const std::vector<int>& array,const int& target){
            if(array.empty()){
                std::unique_ptr<int> p;
                return p;
            }
            size_t counter = 0;
            auto begin = array.begin();
            std::unique_ptr<int> r;
            while(begin != array.end()){
                if(*begin == target){
                    r.reset(new int(counter));
                    ++counter;
                    break;
                }
                ++begin;
                ++counter;
            }
            std::cout<<"simple search count:"<<counter<<std::endl;
            return r;
        }

        /*
          binary search for array
          time: O(logn)
         */
        std::unique_ptr<int> binarySearch(const std::vector<int>& array,const int& target){
            if(array.empty()){
                std::unique_ptr<int> p;
                return p;
            }
            size_t counter = 0;
            std::unique_ptr<int> r;
            size_t high = array.size()-1;
            size_t low = 0;
            size_t mid = 0;
            while(low <= high){
                mid = (high+low)/2;
                if(target == array[mid]){
                    r.reset(new int(mid));
                    ++counter;
                    break;
                } else if(target > array[mid]){
                    high = mid - 1;
                } else if(target < array[mid]){
                    low = mid + 1;
                }
                ++counter;
            }
            std::cout<<"binary search count:"<<counter<<std::endl;
            return r;
        }
    }
}

#endif
