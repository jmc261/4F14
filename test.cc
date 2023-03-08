#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>

void wait_and_print() {

    std::cout<<"Here"<<std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    std::cout<<"Now here"<<std::endl;

    }

int main() {

    std::thread t(wait_and_print);
    t.join();

    return 0;
}