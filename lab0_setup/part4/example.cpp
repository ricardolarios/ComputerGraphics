#include <iostream>
#include <string>

// Entry point function, return type of int as opposed to void etc.
int main()
{
    // ======= for loops =======
    // Allocates an array on the stack, cannot be called outside 
    // main (or whatever current function).
    int myArray[10];

    // Loop that pre-increments var i by 1 on each iteration
    for (int i = 0; i < 10; ++i)
    {
        // For modifying values, use a normal for loop 
        myArray[i] = i;
        // cout prints to standard output, usually terminal
        std::cout << i << " ";
    }

    // Here we use create a string type, which is part of the 
    // standard library and why it is prefixed with `std::`
    std::string s = "hello";

    // Different type of for loop
    for (char c : s)
    {
        // Range based for loops, which is useful if you are not
        // modifying values, or otherwise need to go over the range
        // of an object.
        // Again, std::cout
        std::cout << c;
    }
}

// To run (on my machine), run `g++ -std=c++14 example.cpp -o example`
