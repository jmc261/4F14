#include <iostream>
#include <thread>
#include <list>

int main() {
    std::list<int> l;
    l.push_back(5);
    l.push_back(4);
    l.push_back(3);

    std::cout<<l[2]<<std::endl;
};
