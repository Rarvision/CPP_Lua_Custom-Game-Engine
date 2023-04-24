#pragma once
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif

/// @brief Camera component
struct Camera
{
    /// @brief viewport of the camera
    SDL_FRect   viewport;
    /// @brief camera x position
    int         cameraX;
    /// @brief camera y position
    int         cameraY;
    /// @brief camera viewport width
    int         viewportWidth;
    /// @brief camera viewport height
    int         viewportHeight; 
    /// @brief map width size
    int         worldWidth;
    /// @brief map height size
    int         worldHeight;
    /// @brief camera max move width
    int         cameraMaxX;
    /// @brief edge is for object follow, when the object moves beyond the edge, camera moves
    float       edge;
};
