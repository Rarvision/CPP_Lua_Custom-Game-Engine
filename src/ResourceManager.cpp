// I recommend an unordered_map for filling in the resource manager
#include "ResourceManager.hpp"
#include <iterator>

unordered_map<string, SDL_Surface*> ResourceManager::m_SurfaceMap;
unordered_map<string, SDL_Texture*> ResourceManager::m_TextureMap;
unordered_map<string, Mix_Chunk*>   ResourceManager::m_SoundCache;
SDL_Surface* ResourceManager::m_SpriteSheet = nullptr;
SDL_Texture* ResourceManager::m_Texture = nullptr;


ResourceManager::ResourceManager(){
}


ResourceManager::~ResourceManager(){
    // cout << "call ~ResourceManager()!" << endl;
	for(auto& pair : m_SoundCache){
		Mix_FreeChunk(pair.second);
	}
}

void ResourceManager::LoadResource(std::string image_filename, SDLType type){
	switch (type)
	{
	case SDLType::Surface:
		if (m_SurfaceMap.find(image_filename) != m_SurfaceMap.end())
		{
			// std::cout << "Resource " << image_filename << " is loaded into the memory\n";
			return;
		}
		else
		{
			m_SpriteSheet = SDL_LoadBMP(image_filename.c_str());
			m_SurfaceMap.insert(make_pair(image_filename, m_SpriteSheet));
			// SDL_FreeSurface(m_SpriteSheet);
			// std::cout << "New copy of DL_Surface: " << image_filename << " has been loaded\n";
		}
		break;
	
	// case SDLType::Texture:
	// 	if (m_TextureMap.find(image_filename) != m_TextureMap.end())
	// 	{
	// 		std::cout << "Resource " << image_filename << " is loaded into the memory\n";
	// 		return;
	// 	}
	// 	else
	// 	{
	// 		m_Texture = SDL_CreateTextureFromSurface(ren, tileMap.m_TileSpriteSheet);
	// 		m_TextureMap.insert(make_pair(image_filename, m_Texture));
	// 		// SDL_FreeSurface(m_SpriteSheet);
	// 		std::cout << "New copy of " << image_filename << " has been loaded\n";
	// 	}
	// 	break;
	break;

	default:
		break;
	}
}

SDL_Surface* ResourceManager::GetSDL_Surface(std::string image_filename){
    if (m_SurfaceMap.count(image_filename) == 0)
	{
		// std::cout << "Resource " << image_filename << " is not found in the dictionary\n";
		LoadResource(image_filename, SDLType::Surface);
	}

	// std::cout << "Resource " << image_filename << " is loaded from the memory(unordered_map)\n";
    return m_SurfaceMap[image_filename];
}

SDL_Texture* ResourceManager::GetSDL_Texture(string image_filename, SDL_Renderer* ren, SDL_Surface* spriteSheet){
	if (m_TextureMap.find(image_filename) != m_TextureMap.end())
	{
		// std::cout << "Resource " << image_filename << " is loaded into the memory\n";
		return m_TextureMap[image_filename];
	}
	else{
		m_Texture = SDL_CreateTextureFromSurface(ren, spriteSheet);
		m_TextureMap.insert(make_pair(image_filename, m_Texture));
		// std::cout << "New copy of SDL_Texture: " << image_filename << " has been loaded\n";
		return m_TextureMap[image_filename];
	}

	// std::cout << "Resource " << image_filename << " is not found in the dictionary\n";
	return nullptr;
}

Mix_Chunk *ResourceManager::LoadSound(const string &sound_filePath)
{
	// load sound to the map
	if (m_SoundCache.find(sound_filePath) == m_SoundCache.end())
	{
		Mix_Chunk* sound = Mix_LoadWAV(sound_filePath.c_str());
		if (sound == nullptr)
		{
			std::cerr << "Error loading sound: " << Mix_GetError() << std::endl;
			return nullptr;
		}
		// cout << "insert " << sound_filePath << " to map" << endl;
		m_SoundCache[sound_filePath] = sound;
	}
	
	// cout << "get " << sound_filePath << " from map" << endl;
    return m_SoundCache[sound_filePath];
}

// void ResourceManager::DestroyResources(){

// 	for (auto& surface : m_SurfaceMap)
// 	{
// 		SDL_FreeSurface(surface.second);
// 	}

// 	if(m_SpriteSheet != nullptr){
// 		SDL_FreeSurface(m_SpriteSheet);
// 	}

// 	cout << "call DestoryResources()!" << endl;
// 	m_SurfaceMap.clear();
// }