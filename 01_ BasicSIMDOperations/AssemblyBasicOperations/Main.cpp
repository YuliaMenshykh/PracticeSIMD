#include <iostream>
#include <chrono>

extern "C" void simdAddition();   
extern "C" void loopAddition();   
extern "C" int* getResultArray();   

int main()
{
    // Measure simple loop based addition
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 100; i++)
    {
        loopAddition();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Simple loop based addition took: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";

    // Print the result array from simple loop based addition
    int* resultArray = getResultArray();
    std::cout << "Result from loop based addition : ";
    for (int i = 0; i < 8; ++i) {
        std::cout << resultArray[i] << " ";
    }
    std::cout <<"\n"<< std::endl;



    // Measure SIMD addition
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 100; i++)
    {
        simdAddition();
    }
    end = std::chrono::high_resolution_clock::now();
    std::cout << "SIMD addition took: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";

    // Print the result array from SIMD addition (same result array reused for both operations)
    resultArray = getResultArray();
    std::cout << "Result from SIMD addition: ";
    for (int i = 0; i < 8; ++i) {
        std::cout << resultArray[i] << " ";
    }
    std::cout << "\n";

    return 0;
}
