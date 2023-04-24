#pragma once
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif

enum UIType
{
	MENU_BUTTON,
	MENU_IMAGE,
	EDITOR_IMAGE,
	EDITOR_BUTTON,
	GAME_IMAGE,
	GAME_BUTTON
};

/// @brief UIComponent Component
struct UIComponent
{
	/// @brief defines UI type here: MENU_BUTTON, GAME_BUTTON, ...
	UIType 		m_Type;
};
