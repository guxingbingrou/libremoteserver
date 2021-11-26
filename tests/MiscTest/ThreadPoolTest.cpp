#include "TaskSchduler/ThreadPool.hpp"
#include <memory>
#include <iostream>
#include <unistd.h>
int main(){
    auto pool = std::make_unique<ThreadPool>(3);
    
    std::cout << "add first task\n";
    auto num = pool->CommitTask([]{
        int i=0;
        for(; i<10; ++i){
            std::cout << "this is the first task: " << i << std::endl;
            sleep(1);
        }
        return i;
    }).get();
    std::cout << "return value is :  " << num << std::endl;

    std::cout << "add second task\n";
    pool->CommitTask([]{
        for(int i=0; i<10; ++i){
            std::cout << "this is the second task: " << i << std::endl;
            sleep(1);
        }  
    }); 

    std::cout << "add third task\n";
    pool->CommitTask([]{
        for(int i=0; i<10; ++i){
            std::cout << "this is the third task: " << i << std::endl;
            sleep(1);
        }
    });   

    return 0;
}