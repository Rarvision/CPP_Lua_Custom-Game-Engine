#pragma once
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif


#include "Common.hpp"
#include "Debug.hpp"
#include "Event.hpp"

#include "ResourceManager.hpp"

#include <set>

/// @brief The base class of all sub-systems
class ISystem{
public:
    /// @brief the set store all entities that the system is caring about
    set<Entity> m_Entities;

	virtual void Init() = 0;
	
    virtual void Update() = 0;

	virtual void Clear(){
		m_Entities.clear();
	};
};