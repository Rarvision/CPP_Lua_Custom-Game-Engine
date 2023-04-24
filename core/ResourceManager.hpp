#ifndef RESOURCEMANAGER_HPP 
#define RESOURCEMANAGER_HPP
#if defined(LINUX) || defined(MINGW)
	#include <SDL2/SDL.h>
#else
	#include <SDL.h>
#endif
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL_mixer.h>

#include "Common.hpp"
#include "Singleton.hpp"

using namespace std;

class ResourceManager: public Singleton<ResourceManager>{
public:
    /// @brief Get SDL_Surface from map if exists; otherwise load from disk and store to the map
    /// @param image_filename filename
    /// @return SDL_Surface*
    static SDL_Surface* GetSDL_Surface(string image_filename);

    /// @brief Get SDL_Texture from map if exists; otherwise load from disk and store to the map
    /// @param image_filename filename
    /// @return SDL_Texture*
    static SDL_Texture* GetSDL_Texture(string image_filename, SDL_Renderer*, SDL_Surface*);

    // static void DestroyResources();

    /// @brief Get Mix_Chunk from map if exists; otherwise load from disk and store to the map
    /// @param sound_filePath sound filepath
    /// @return Mix_Chunk*
    static Mix_Chunk* LoadSound(const string& sound_filePath);

private:
    friend class Singleton<ResourceManager>;
    
    ResourceManager();
	~ResourceManager();

    ResourceManager(ResourceManager const&)         = delete;
    void operator=(ResourceManager const&)          = delete;

    /// @brief LoadResource from folder
    /// @param image_filename your filename
    static void LoadResource(string image_filename, SDLType type);

    static SDL_Surface*                             m_SpriteSheet;
    static SDL_Texture*                             m_Texture;
    static unordered_map<string, SDL_Surface*>      m_SurfaceMap;
    static unordered_map<string, SDL_Texture*>      m_TextureMap;
    static unordered_map<string, Mix_Chunk*>        m_SoundCache;

    // vector<Sprite>                                  m_Sprites;
};



#endif
