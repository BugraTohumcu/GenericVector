#include "custom_vector.h"
#include <iostream>
#include <functional>
#include <string>

void foo() { std::cout << "foo\n"; }

int main() {
    // Basic types
    veclib::vector<int> numbers = {1, 2, 3};
    numbers.append(4);
    numbers.append(5);

    std::cout << "Numbers: " << numbers << std::endl;

    // Pre-allocating memory for performance
    veclib::vector<int> preallocated;
    preallocated.reserve(10);
    for (int i = 0; i < 10; ++i)
        preallocated.append(i);

    // Storing callables
    veclib::vector<std::function<void()>> funcs(foo);
    funcs.append([](){ std::cout << "lambda\n"; });

    for (auto &f : funcs)
        f();

    // Strings with initializer list and perfect forwarding
    veclib::vector<std::string> words = {"hello"};
    words.append(std::string("world"));
    std::cout << words << std::endl;

    // Range-based for loop
    for (auto &num : numbers)
        std::cout << num << " ";
    std::cout << std::endl;

    return 0;
}