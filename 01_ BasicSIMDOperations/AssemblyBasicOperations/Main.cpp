#include <iostream>
#include <chrono>

// Declare the external assembly functions
extern "C" void simd_addition();   // SIMD addition function
extern "C" void loop_addition();   // Loop-based addition function
extern "C" int* get_result_array();  // Function to get the result array
extern "C" unsigned int simdTime;   // SIMD timing value
extern "C" unsigned int loopTime;   // Loop timing value

int main()
{
    // Measure simple loop based addition
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Performing simple loop based addition...\n";
    loop_addition();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Simple loop based addition took: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";


    // Retrieve the result array
    int* resultArray = get_result_array();

    // Print the result array from SIMD
    std::cout << "Result from SIMD Addition:\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << resultArray[i] << " ";
    }
    std::cout << "\n";

    // Output the time taken by SIMD
    std::cout << "SIMD Addition Time: " << simdTime << " cycles\n\n";

    start = std::chrono::high_resolution_clock::now();
    // Call the loop-based addition function
    std::cout << "Performing Loop-based Addition...\n";
    simd_addition();
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Loop addition matrix multiplication took: "
        << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds\n";

    // Retrieve the result array (same result array reused for both operations)
    resultArray = get_result_array();

    // Print the result array from Loop-based addition
    std::cout << "Result from Loop-based Addition:\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << resultArray[i] << " ";
    }
    std::cout << "\n";

    // Output the time taken by loop-based addition
    std::cout << "Loop-based Addition Time: " << loopTime << " cycles\n\n";

    return 0;
}
