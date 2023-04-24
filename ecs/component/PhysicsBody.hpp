#pragma once

#include <box2d/box2d.h>

/// @brief PhysicsBody Component
struct PhysicsBody{
    b2Body* mBody;
    // bool hasCollided;
};