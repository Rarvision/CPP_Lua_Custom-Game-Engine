#pragma once

#include "System.hpp"
#include "Event.hpp"
#include "Camera.hpp"

class GameplaySystem : public ISystem{
public:
    void Init() override;

	void Update() override;

    void Clear() override;
    
};