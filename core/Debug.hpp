#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <string>

/// @brief class for output messages in console
class Debug
{
public:
    template<typename T>
    // && means rvalue reference, which is used for move semantics
    static void PrintMessage(T&& t){
        std::cout << t << std::endl;
    }
};

#endif