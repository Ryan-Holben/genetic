#include <core/core.h>
#include <lib/example.h>

#include <iostream>

int main(int argc, char** argv) {
    std::cout << "Hello, world!\n";

    core::Dataset d;

    const int val = 5;
    std::cout << "Successor of " << val << " is " << ExampleFunc(val) << ".\n";
}