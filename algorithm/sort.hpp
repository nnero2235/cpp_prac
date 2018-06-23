#ifndef SORT_NNERO_HPP
#define SORT_NNERO_HPP

#include<vector>
#include<iostream>
#include<memory>

namespace nnero{
    namespace algorithm{

        static int g_counter=0;

        void resetCounter(){
            g_counter = 0;
        }

        int getGCounter(){
            return g_counter;
        }
        
        /*
          select sort for array
          time: O(n2) 
         */
        std::unique_ptr<std::vector<int>> selectSort(const std::vector<int>& array){
            if(array.empty() || array.size() == 1){
                std::cout<<"select sort count:1"<<std::endl;
                std::unique_ptr<std::vector<int>> ptr(new std::vector<int>(array));
                return ptr;
            }
            int counter = 0;
            int temp = 0;
            std::unique_ptr<std::vector<int>> result(new std::vector<int>(array));
            for(size_t i=0;i<result->size();++i){
                for(size_t j=i+1;j<result->size();++j){
                    if((*result)[i] < (*result)[j]){
                        temp = (*result)[i];
                        (*result)[i] = (*result)[j];
                        (*result)[j] = temp;
                    }
                    ++counter;
                }
            }
            std::cout<<"select sort count:"<<counter<<std::endl;
            return result;
        }

        /*
          bubble sort for array
          time: O(n2)
         */
        std::unique_ptr<std::vector<int>> bubbleSort(const std::vector<int>& array){
            if(array.empty() || array.size() == 1){
                std::cout<<"bubble sort count:1"<<std::endl;
                std::unique_ptr<std::vector<int>> ptr(new std::vector<int>(array));
                return ptr;
            }
            int counter = 0;
            int temp = 0;
            std::unique_ptr<std::vector<int>> result(new std::vector<int>(array));
            for(size_t i=0;i<result->size();++i){
                for(size_t j=0;j+1<result->size()-i;++j){
                    if((*result)[j] < (*result)[j+1]){
                        temp = (*result)[j+1];
                        (*result)[j+1] = (*result)[j];
                        (*result)[j] = temp;
                    }
                    ++counter;
                }
            }
            std::cout<<"bubble sort count:"<<counter<<std::endl;
            return result;
        }

        /*
          quick sort for array
          time: O(nlogn)
         */
        std::unique_ptr<std::vector<int>> quickSort(const std::vector<int>& array){
            if(array.empty() || array.size() == 1){
                std::unique_ptr<std::vector<int>> ptr(new std::vector<int>(array));
                return ptr;
            }
            size_t piovt = array.size()/2;
            std::unique_ptr<std::vector<int>> high(new std::vector<int>);
            std::unique_ptr<std::vector<int>> low(new std::vector<int>);
            for(size_t i = 0;i<piovt;++i){
                if(array[i] < array[piovt]){
                    low->push_back(array[i]);
                } else {
                    high->push_back(array[i]);
                }
                ++g_counter;
            }
            for(size_t i=piovt+1;i<array.size();++i){
                if(array[i] < array[piovt]){
                    low->push_back(array[i]);
                } else {
                    high->push_back(array[i]);
                }
                ++g_counter;
            }
            std::unique_ptr<std::vector<int>> r_h = quickSort(*high);
            std::unique_ptr<std::vector<int>> r_l = quickSort(*low);
            r_h->insert(r_h->end(), array[piovt]);
            r_h->insert(r_h->end(), r_l->begin(),r_l->end());
            return r_h;
        }

        /*
          merge sort for array
          time: O(nlogn)
         */
        std::unique_ptr<std::vector<int>> mergeSort(const std::vector<int>& array){
            if(array.empty() || array.size() == 1){
                std::unique_ptr<std::vector<int>> ptr(new std::vector<int>(array));
                return ptr;
            }
            size_t piovt = array.size()/2;
            std::vector<int> left;
            std::vector<int> right;
            for(size_t i=0;i<piovt;++i){
                left.push_back(array[i]);
                ++g_counter;
            }
            for(size_t i=piovt;i<array.size();++i){
                ++g_counter;
                right.push_back(array[i]);
            }
            std::unique_ptr<std::vector<int>> left_ptr = mergeSort(left);
            std::unique_ptr<std::vector<int>> right_ptr = mergeSort(right);
            std::unique_ptr<std::vector<int>> result_ptr(new std::vector<int>);
            size_t j_index = 0;
            bool is_break = false;
            for(size_t i=0;i<left_ptr->size();++i){
                for(size_t j=j_index;j<right_ptr->size();++j){
                    if((*right_ptr)[j] > (*left_ptr)[i]){
                        result_ptr->push_back((*right_ptr)[j]);
                        g_counter++;
                    } else {
                        result_ptr->push_back((*left_ptr)[i]);
                        j_index =j;
                        is_break = true;
                        g_counter++;
                        break;
                    }
                }
                if(is_break){
                    is_break = false;
                } else {
                    j_index = right_ptr->size();
                    result_ptr->push_back((*left_ptr)[i]);
                    ++g_counter;
                }
            }
            return result_ptr;
        }
    }
}

#endif
