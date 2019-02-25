//
//  main.cpp
//  Algorithm
//
//  Created by Al Mamun on 20/2/19.
//  Copyright © 2019 Al Mamun. All rights reserved.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <random>
/**********MACRO OVERLOAD*************/
#define CAT( A, B ) A ## B
#define SELECT( NAME, NUM ) CAT( NAME ## _, NUM )

#define GET_COUNT( _1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ... ) COUNT
#define VA_SIZE( ... ) GET_COUNT( __VA_ARGS__, 6, 5, 4, 3, 2, 1 )

#define VA_SELECT( NAME, ... ) SELECT( NAME, VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)
/**********MACRO OVERLOAD*************/

#define RANGE( ... ) VA_SELECT( RANGE, __VA_ARGS__ )
#define RANGE_3(a,b,i) for(int aa = a; aa < b; aa += i)
#define RANGE_2(a,b) RANGE_3(a,b,1)
#define RANGE_1(count) RANGE_3(0,count,1)

#define MTIME(message,nrun,function) measure_execution_time(message,nrun,function)
void measure_execution_time(std::string message, const int nrun, std::function<void()> function) {
    
    long long sum = 0;
    RANGE(nrun){
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        function();
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        
        sum += std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
    }
    
    std::cout << message << " took : " << sum / nrun << " microsecond(s) each of "
        << nrun << " run." << std::endl;
}

int binary_search_linear(std::vector<int> &data, int target) {
    
    int a = 0, b = 0, c = (int) data.size();

    while(a <= c) {
        b = (a + c) / 2;
//        int iteration = 0;
//        printf("a:%d b:%d c:%d target:%d iteration:%d\n", a,b,c,target,iteration++);
        if(data[b] == target)
            return b;
        if(data[b] < target) {
            a = b + 1;
        } else {
            c = b - 1;
        }
    }
    
    return -1;
}

int binary_search_recursive(std::vector<int> &data, int a, int c, int target) {
    
    if(a > c)
        return -1;
    
    int b = (a + c) / 2;
    
    if(data[b] == target)
        return b;
    if(data[b] < target) {
        a = b + 1;
    } else {
        c = b - 1;
    }
    
    return binary_search_recursive(data, a, c, target);
}

int binary_search_recursive(std::vector<int> &data, int target) {
    return binary_search_recursive(data, 0, (int)data.size(), target);
}

void binary_search_test() {
    #define DATA_SIZE 50000
    std::vector<int> data;
    data.resize(DATA_SIZE);
    std::generate(data.begin(), data.end(), []() {
        static int counter = -1;
        return ++counter;
    });
    
    MTIME("linear-binary-search", 100, [&data](){
        std::for_each(data.begin(), data.end(), [&data](int &i){
            assert(binary_search_linear(data, i) == i);
        });
        assert(binary_search_linear(data, DATA_SIZE + 1000) == -1);
    });
    
    MTIME("recursive-binary-search", 100, [&data](){
        std::for_each(data.begin(), data.end(), [&data](int &i){
            assert(binary_search_recursive(data, i) == i);
        });
        assert(binary_search_recursive(data, DATA_SIZE + 1000) == -1);
    });
    
    MTIME("std-binary-search", 100, [&data](){
        std::for_each(data.begin(), data.end(), [&data](int &i){
            auto itr = std::lower_bound(data.begin(), data.end(), i);
            if(itr != data.end()) {
                assert(*itr == i);
            } else {
                assert(false);
            }
        });
        
        auto itr = std::lower_bound(data.begin(), data.end(), DATA_SIZE + 1000);
        assert(itr == data.end());
    });
    
    #undef DATA_SIZE
}

void selection_sort(std::vector<int> &data) {
    auto a = data.begin();
    while(a != data.end()) {
        auto mn = std::min_element(a, data.end());
        std::iter_swap(a, mn);
        a++;
    }
}

void sort_test(std::function<void(std::vector<int> &data)> sort_function, std::string algorithm_name) {
#define DATA_SIZE 50000
    
    std::vector<int> data;
    data.resize(DATA_SIZE);
    //populate
    std::generate(data.begin(), data.end(), [](){
        static int counter = -1;
        return ++counter;
    });
    
    std::vector<int> data_copy;
    std::copy(data.begin(), data.end(), std::back_inserter(data_copy));
    
    //shuffle
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(data.begin(), data.end(), g);
    
    MTIME(algorithm_name, 5, [&](){
        sort_function(data);
    });
    
    auto itr = std::mismatch(data.begin(), data.end(),
                  data_copy.begin(), data_copy.end()).first;
    assert(itr == data.end());
    
#undef DATA_SIZE
}

int main()
{
//    binary_search_test();
    sort_test(selection_sort, "selection-sort");
    return 0;
}
