#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono>
#include <tuple>

void never_stops () {
    while (true) {
        std::cout<<1<<std::endl;
    };
}

void stops () {
    while (true) {
        for (int i = 0; i<400; i++){
            std::cout<<i<<std::endl;
        };
        std::this_thread::~thread;
    };
}

int main() {

    std::thread t1(never_stops);
    std::thread t2(stops);

    t2.join();
    t1.join();

    return 0;
}