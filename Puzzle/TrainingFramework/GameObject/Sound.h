#pragma once
#include "../TrainingFramework/stdafx.h"
#include "SDL.h"
#include "SDL_mixer.h"

class Sound
{
public:
    Sound();
    Sound(const char* soundPath);
    ~Sound();

    bool Load(const char* soundPath);
    void Play(int channel, int loop); // Chơi âm thanh trên kênh được chỉ định
    void Pause();
    void Resume();
    void Stop(int channel);
    bool isPaused;

private:
    Mix_Chunk* soundChunk; // Sử dụng Mix_Chunk thay vì Mix_Music
};
