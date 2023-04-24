#pragma once

#include "Singleton.hpp"

/// @brief class for tool functions
class Utils : public Singleton<Utils>{
public:
    /// @brief clamp a number between low and high
    /// @param v your number
    /// @param lo mininum boundary
    /// @param hi maxinum boundary
    /// @return a number between low and high
    inline float clamp(float v, float lo, float hi)
    {
        return v < lo ? lo : hi < v ? hi
                                    : v;
    }
};