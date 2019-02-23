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

#define MTIME(a,b,c) measure_execution_time(a,b,c)
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
    
    if(c < a)
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
        assert(binary_search_linear(data, 80000) == -1);
    });
    
    MTIME("recursive-binary-search", 100, [&data](){
        std::for_each(data.begin(), data.end(), [&data](int &i){
            assert(binary_search_recursive(data, i) == i);
        });
        assert(binary_search_recursive(data, 80000) == -1);
    });
    
    #undef DATA_SIZE
}

int main()
{
    binary_search_test();
    return 0;
}
