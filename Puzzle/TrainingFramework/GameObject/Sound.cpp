#include "Sound.h"
#include<string>

Sound::Sound()
{
    soundChunk = nullptr;
    isPaused = false;
}

Sound::Sound(const char* soundPath)
{
    soundChunk = nullptr;
    Load(soundPath);
}

Sound::~Sound()
{
    Stop(-1);
    Mix_FreeChunk(soundChunk); // Giải phóng Mix_Chunk thay vì Mix_Music
}

bool Sound::Load(const char* soundName)
{
    std::string path = "../Resources/Sounds/" + std::string(soundName) + ".wav"; // Đảm bảo sử dụng tệp âm thanh WAV hoặc định dạng khác tương thích

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        // Xử lý lỗi khởi tạo SDL
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        // Xử lý lỗi mở thiết bị âm thanh
        return 1;
    }

    if (Mix_Init(MIX_INIT_MP3) == -1) {
        // Xử lý lỗi SDL_mixer
        const char* error = Mix_GetError();
        printf("SDL_mixer initialization error: %s\n", error);
        return false;
    }

    // Cấu hình số kênh âm thanh (ví dụ: 16 kênh)
    Mix_AllocateChannels(16);

    soundChunk = Mix_LoadWAV(path.c_str()); // Sử dụng Mix_LoadWAV thay vì Mix_LoadMUS
    if (soundChunk == NULL) {
        // Xử lý lỗi
        printf("Cannot load sound: %s\n", path);
        return false;
    }

    return true;
}

void Sound::Play(int channel, int loop)
{
    if (soundChunk != nullptr) {
        Mix_PlayChannel(channel, soundChunk, loop); // Chơi âm thanh trên kênh được chỉ định, -1 để chơi liên tục
        Mix_Volume(channel, MIX_MAX_VOLUME); // Tăng âm lượng âm thanh trên kênh
    }
}

void Sound::Pause()
{
    Mix_Pause(-1); // Tạm dừng tất cả âm thanh trên tất cả các kênh
    isPaused = true;
}

void Sound::Resume()
{
    Mix_Resume(-1); // Tiếp tục tất cả âm thanh trên tất cả các kênh
    isPaused = false;
}

void Sound::Stop(int channel)
{
    Mix_HaltChannel(channel); // Dừng tất cả âm thanh trên tất cả các kênh
}

