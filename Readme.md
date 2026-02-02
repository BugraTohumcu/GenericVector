# Veclib

`veclib::vector` is a custom C++20 dynamic array implementation, inspired by `std::vector`.  
It emphasizes **modern C++ features**, **perfect forwarding**, and **callable support**.

---

## Features

- **Dynamic resizing**: Doubles capacity automatically when more space is needed.
- **Memory management**: Uses `std::allocator` with placement new and explicit destructor calls.
- **Supports any type**: Including callables (`std::function`, lambdas, function pointers).
- **Initializer list support**: `vector<int> v = {1, 2, 3};`
- **Variadic constructor for callables**: `vector<std::function<void()>> v(foo, bar, lambda);`
- **Iterators**: `begin()` / `end()` compatible with range-based for loops.
- **Safe element access**: `at()` throws `std::out_of_range` for invalid indices.
- **Perfect forwarding**: Avoids unnecessary copies when appending or constructing elements.

---

## Modern C++ Techniques

### 1. Perfect Forwarding

```cpp
template<std::convertible_to<T> U>
void append(U&& value) {
    new (data + _size) T(std::forward<U>(value));
    ++_size;
}
```

### 2. Variadic Templates for Callables

```cpp
template<std::convertible_to<T> U>
void append(U&& value) {
    new (data + _size) T(std::forward<U>(value));
    ++_size;
}
```

Example usage:

```cpp
veclib::vector<std::function<void()>> funcs(foo, bar, [](){ std::cout << "lambda\n"; });
```

### 3. In-place Memory Management

- Uses `std::allocator::allocate` for raw memory allocation.
- Constructs objects with placement new.
- Explicitly calls destructors when deallocating or resizing.

### 4. Type Safety

- `append()` and callable constructor use `std::convertible_to<T>` constraints.
- Prevents appending incompatible types, while allowing implicit conversions.
- You can switch to `std::constructible_from` for stricter checks if desired.

---

## Usage Example

```cpp
#include "vector.h"
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
```