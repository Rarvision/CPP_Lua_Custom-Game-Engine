#include "AudioManager.hpp"

void AudioManager::PlaySound(const string &filePath, int volume, int loop)
{
    Mix_Chunk* sound = ResourceManager::GetInstance().LoadSound(filePath);
    if (sound != nullptr)
    {
        Mix_VolumeChunk(sound, volume);
        int availableChannel = -1;
        for (int i = 0; i < Mix_AllocateChannels(-1); ++i)
        {
            if (Mix_Playing(i) == 0)
            {
                availableChannel = i;
                break;
            }
        }
        if (availableChannel != -1)
        {
            if (Mix_PlayChannel(availableChannel, sound, 0) == -1)
            {
                std::cerr << "Error playing sound: " << Mix_GetError() << std::endl;
            }
        }
        else
        {
            std::cerr << "No available channels for playing sound" << std::endl;
        }
        Mix_PlayChannel(availableChannel, sound, loop);
        // cout << "availableChannel " << availableChannel << endl;
    }
}