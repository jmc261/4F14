#include <iostream>
#include <chrono>
#include <vector>

int main () {

    // Compare how quick std::iter_swap and vector indexing are

    // Create vector with values between 0 and 100
    std::vector<int> v;
    for (int i=0; i<100; i++) {
        v.push_back(i);
    };

    // Generate two random values to swap
    int el_1 = rand() % 100;
    int el_2 = rand() % 100;

    // Time the swap function and output result
    auto start = std::chrono::high_resolution_clock::now();
    std::iter_swap(v.begin() + el_1, v.begin() + el_2);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end-start);
    std::cout<<"Swap took: "<<duration.count()<<" Nanoseconds"<<std::endl;

    // Time the access action and output result
    auto start_2 = std::chrono::high_resolution_clock::now();
    int c = v[el_1];
    auto end_2 = std::chrono::high_resolution_clock::now();

    auto duration_2 = std::chrono::duration_cast<std::chrono::nanoseconds>(end_2-start_2);
    std::cout<<"Access took: "<<duration_2.count()<<" Nanoseconds"<<std::endl;



    return 0;
}