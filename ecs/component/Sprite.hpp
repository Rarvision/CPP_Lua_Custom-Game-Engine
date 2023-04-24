#pragma once
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif

#include <TinyMath.hpp>

/// @brief Sprite Component
struct Sprite{
	/// @brief a sprite's SDL_Texture ptr
	SDL_Texture*    m_Texture       =  nullptr;	
	/// @brief a sprite's SDL_Surface ptr
    SDL_Surface*    m_SpriteSheet   =  nullptr;
	/// @brief a sprite's SDL_Renderer ptr
    SDL_Renderer*   m_Render        =  nullptr;
    /// @brief a sprite's source image position, where to start cut out the image
    SDL_Rect        m_Src;
	/// @brief where do you want render the image in screen
	SDL_Rect        m_Dest;			
    /// @brief frames of the sprite, if it's a static image, just use 0
    int             m_Frame         = 0;
    /// @brief a sprite's current running frame
    int             m_CurrentFrame  = 0;
	/// @brief sprite size x
	int				m_SizeX			= 0;
	/// @brief sprite size y
	int				m_SizeY			= 0;
	/// @brief a sprite's x size in screen
	int 			m_RenderSizeX	= 0;
	/// @brief a sprite's y size in screen
	int 			m_RenderSizeY	= 0;
};