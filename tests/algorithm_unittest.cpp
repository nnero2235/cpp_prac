#include<iostream>
#include<gtest/gtest.h>
#include"../algorithm/sort.hpp"
#include"../algorithm/search.hpp"
#include"../algorithm/tree.hpp"
#include<vector>
#include<algorithm>
#include<memory>
#include<random>

void printArr(const std::vector<int>& arr){
    std::for_each(arr.begin(), arr.end(), [](const int& v){
            std::cout<<v<<'\n';
        });
}

void assertSorted(const std::vector<int>& arr){
    for(size_t i=0;i+1<arr.size() ;i++){
        ASSERT_TRUE(arr[i] >= arr[i+1]);
    }
}

TEST(ALGORITHM,s_sort){
    using namespace nnero::algorithm;
    /*gen a lot of numbers to sort*/
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> u(-9999,9999);
    std::vector<int> array;
    for(int i=0;i<10000;++i){
        array.push_back(random_engine());
    }

    std::unique_ptr<std::vector<int>> ptr1 = selectSort(array);
    assertSorted(*ptr1);
}

TEST(ALGORITHM,b_sort){
    using namespace nnero::algorithm;
    /*gen a lot of numbers to sort*/
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> u(-9999,9999);
    std::vector<int> array;
    for(int i=0;i<10000;++i){
        array.push_back(random_engine());
    }
    std::unique_ptr<std::vector<int>> ptr2 = bubbleSort(array);
    assertSorted(*ptr2);
}

TEST(ALGORITHM,q_sort){
    using namespace nnero::algorithm;
    /*gen a lot of numbers to sort*/
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> u(-9999,9999);
    std::vector<int> array;
    for(int i=0;i<10000;++i){
        array.push_back(random_engine());
    }
    resetCounter();
    std::unique_ptr<std::vector<int>> ptr3 = quickSort(array);
    assertSorted(*ptr3);
    std::cout<<"quick sort count:"<<getGCounter()<<std::endl;
}

TEST(ALGORITHM,m_sort){
    using namespace nnero::algorithm;
    /*gen a lot of numbers to sort*/
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> u(-9999,9999);
    std::vector<int> array;
    for(int i=0;i<10000;++i){
        array.push_back(random_engine());
    }
    resetCounter();
    std::unique_ptr<std::vector<int>> ptr3 = mergeSort(array);
    assertSorted(*ptr3);
    std::cout<<"quick sort count:"<<getGCounter()<<std::endl;
}

TEST(ALGORITHM,search){
    using namespace nnero::algorithm;
    std::default_random_engine random_engine;
    std::uniform_int_distribution<int> u(-9999,9999);
    std::vector<int> array;
    for(int i=0;i<10000;++i){
        array.push_back(random_engine());
    }
    resetCounter();
    std::unique_ptr<std::vector<int>> ptr3 = quickSort(array);
    assertSorted(*ptr3);
    std::cout<<"quick sort count:"<<getGCounter()<<std::endl;
    std::unique_ptr<int> p = simpleSearch(*ptr3,1876);
    if(p){
        std::cout<<*p<<std::endl;
    }
    std::unique_ptr<int> p1 = binarySearch(*ptr3,1876);
    if(p1){
        std::cout<<*p1<<std::endl;        
    }
}

TEST(TREE,binary_tree){
    using namespace nnero::tree;
    BinaryTree<int> b_tree;
    ASSERT_TRUE(b_tree.size() == 0);
    b_tree.add(std::make_shared<int>(19));
    b_tree.add(std::make_shared<int>(18));
    b_tree.add(std::make_shared<int>(-1));
    b_tree.add(std::make_shared<int>(-2));
    b_tree.add(std::make_shared<int>(-3));
    b_tree.add(std::make_shared<int>(8));
    b_tree.add(std::make_shared<int>(79));
    b_tree.add(std::make_shared<int>(66));
    b_tree.add(std::make_shared<int>(100));
    b_tree.add(std::make_shared<int>(5));
    b_tree.add(std::make_shared<int>(9));
    ASSERT_TRUE(b_tree.size() == 11);
    std::shared_ptr<int> result = b_tree.search(100);
    ASSERT_EQ(*result,100);
    std::shared_ptr<int> r = b_tree.search(55);
    ASSERT_FALSE(r);
}
