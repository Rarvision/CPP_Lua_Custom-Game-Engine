#pragma once
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	// Windows and Mac use a different path
	// If you have compilation errors, change this as needed.
	#include <SDL.h>
#endif

/// @brief Tilemap Component
struct Tilemap
{
    // Dimensions of our TileMap and individual tiles.
    // Used for spiltting up the sprite sheet
    int m_Rows;
    int m_Cols;    
    // the gap between each tile (ex. 1 pixel)  
    int m_gap;
    // How big each tile is.
    int m_TileWidth;
    int m_TileHeight;
    // size of our tilemap
    int m_MapX;
    int m_MapY; 
    /// @brief how much bigger do you want the tile to, for example, 5x bigger than before
    int m_ScaleFactor;
    /// @brief the max width of the tilemap
    int m_MaxMapWidth;
    // Stores our tile types
    vector<int> m_Tiles;
    set<pair<int,int>>  m_UpdatedPairs;
    // Where our TileMap is rendered
    // An SDL Surface contains pixel data to draw our TileMap
    SDL_Surface *m_TileSpriteSheet;
    SDL_Texture *m_Texture;
    /// @brief draw a tilemap in a texture and render it to our screen
    SDL_Texture *m_FullTexture;
    /// @brief position to cut tilemap in src image
    SDL_Rect    m_Src;
    /// @brief position to render tilemap in screen/texture
    SDL_Rect    m_Dest;
};

