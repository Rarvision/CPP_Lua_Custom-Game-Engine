#pragma once

#include "System.hpp"
#include "Sprite.hpp"

using namespace std;

class SpriteSystem : public ISystem{
public:
	void Init() override;

	/// @brief Update a sprite animation if the frame is > 0
	void Update() override;

    void Clear() override;
    
    /// @brief create a sprite object
    /// @param filePath the sprite's file path
    /// @param ren SDL renderer 
    /// @param sizeX src sprite size x
    /// @param sizeY src sprite size y
    /// @param posX where the sprite x lands in screen
    /// @param posY where the sprite y lands in screen
    /// @param frame if the sprite can run animation, frame > 0, else frame = 0
    /// @param renderSizeX how big the sprite x will be rendered in screen
    /// @param renderSizeY how big the sprite y will be rendered in screen
    /// @return 
    Sprite CreateSprite(std::string filePath, SDL_Renderer* ren, int sizeX, int sizeY, int posX, int posY, int frame, int renderSizeX, int renderSizeY);

    Sprite CreateSprite(std::string filePath, SDL_Renderer* ren, int sizeX, int sizeY, int posX, int posY, int frame);
private:
    SDL_Renderer*   m_Render      =  nullptr;
};