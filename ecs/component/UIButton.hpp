#pragma once
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif

/// @brief UIButton Component
struct UIButton
{
    /// @brief the position of the button
    SDL_Rect 	rect;
};
