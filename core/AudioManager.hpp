#pragma once

#include <SDL_mixer.h>
#include "Common.hpp"
#include "Singleton.hpp"
#include "ResourceManager.hpp"

/// @brief AudioManager is used to play sound
class AudioManager: public Singleton<AudioManager>{
public:
    /// @brief Play Sound based on the file name
    /// @param filePath set path in Common.hpp
    /// @param volume 0 - 100
    /// @param loop -1 means infinite loop, other means one time
    void PlaySound(const string& filePath, int volume, int loop);
private:
    ~AudioManager() = default;
    
    static const int MAX_PENDING = 16;
};