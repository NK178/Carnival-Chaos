// AudioManager.h
#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <Windows.h>
#include <mmsystem.h>
#include <string>
#include <unordered_map>

#pragma comment(lib, "winmm.lib")

class CAudioManager {
private:
    static CAudioManager* instance;

    // Music control variables
    bool isMusicPlaying;
    std::string currentMusic;
    float musicVolume;

    // Sound effect tracking
    std::unordered_map<std::string, std::string> soundAliases;
    int nextSoundId;

    // Private constructor for singleton
    CAudioManager();

public:
    // Singleton access
    static CAudioManager* GetInstance();

    // Music methods
    void PlayMusic(const std::string& musicFile, bool restart = false);
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    void SetMusicVolume(float volume); // 0.0 to 1.0
    std::string GetCurrentMusic() const { return currentMusic; }

    // Sound effects methods
    void PlaySound(const std::string& soundFile);

    // Cleanup
    void Release();
};

#endif // AUDIO_MANAGER_H