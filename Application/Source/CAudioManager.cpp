#include "CAudioManager.h"
#include <iostream>

// Initialize the static instance
CAudioManager* CAudioManager::instance = nullptr;

CAudioManager::CAudioManager() :
    isMusicPlaying(false),
    currentMusic(""),
    musicVolume(1.0f),
    nextSoundId(0)
{
}

CAudioManager* CAudioManager::GetInstance()
{
    if (instance == nullptr) {
        instance = new CAudioManager();
    }
    return instance;
}

void CAudioManager::PlayMusic(const std::string& musicFile, bool restart)
{
    // If the same music is already playing and restart is not requested, do nothing
    if (isMusicPlaying && currentMusic == musicFile && !restart) {
        return;
    }

    // Stop any currently playing music
    if (isMusicPlaying) {
        StopMusic();
    }

    // Play the new music file - converting std::string to wstring
    std::wstring wMusicFile(musicFile.begin(), musicFile.end());
    std::wstring command = L"open \"" + wMusicFile + L"\" type mpegvideo alias bgMusic";

    if (mciSendStringW(command.c_str(), NULL, 0, NULL) == 0) {
        mciSendStringW(L"play bgMusic repeat", NULL, 0, NULL);
        isMusicPlaying = true;
        currentMusic = musicFile;

        // Set volume
        SetMusicVolume(musicVolume);
    }
    else {
        std::cerr << "Failed to play music: " << musicFile << std::endl;
    }
}

void CAudioManager::StopMusic()
{
    if (isMusicPlaying) {
        mciSendStringW(L"stop bgMusic", NULL, 0, NULL);
        mciSendStringW(L"close bgMusic", NULL, 0, NULL);
        isMusicPlaying = false;
        currentMusic = "";
    }
}

void CAudioManager::PauseMusic()
{
    if (isMusicPlaying) {
        mciSendStringW(L"pause bgMusic", NULL, 0, NULL);
    }
}

void CAudioManager::ResumeMusic()
{
    if (isMusicPlaying) {
        mciSendStringW(L"resume bgMusic", NULL, 0, NULL);
    }
}

void CAudioManager::SetMusicVolume(float volume)
{
    // Clamp volume between 0 and 1
    musicVolume = (volume < 0.0f) ? 0.0f : (volume > 1.0f) ? 1.0f : volume;

    // Convert to Windows volume (0-1000)
    int windowsVolume = static_cast<int>(musicVolume * 1000);

    // Set the volume - using wide string version
    std::wstring command = L"setaudio bgMusic volume to " + std::to_wstring(windowsVolume);
    mciSendStringW(command.c_str(), NULL, 0, NULL);
}

void CAudioManager::PlaySound(const std::string& soundFile)
{
    // Create a unique alias for this sound
    std::string aliasStr = "sound" + std::to_string(nextSoundId++);
    std::wstring soundAlias(aliasStr.begin(), aliasStr.end());

    // Convert std::string to wstring
    std::wstring wSoundFile(soundFile.begin(), soundFile.end());

    // Open and play the sound asynchronously
    std::wstring command = L"open \"" + wSoundFile + L"\" type mpegvideo alias " + soundAlias;

    if (mciSendStringW(command.c_str(), NULL, 0, NULL) == 0) {
        // Store the alias for cleanup
        soundAliases[aliasStr] = soundFile;

        // Play the sound
        command = L"play " + soundAlias;
        mciSendStringW(command.c_str(), NULL, 0, NULL);
    }
    else {
        std::cerr << "Failed to play sound: " << soundFile << std::endl;
    }
}

void CAudioManager::Release()
{
    // Stop and close background music
    StopMusic();

    // Close any open sound effects
    for (const auto& pair : soundAliases) {
        std::wstring alias(pair.first.begin(), pair.first.end());
        std::wstring command = L"close " + alias;
        mciSendStringW(command.c_str(), NULL, 0, NULL);
    }

    soundAliases.clear();
}