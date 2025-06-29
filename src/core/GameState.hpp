/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** GameState
*/

#ifndef GAMESTATE_HPP_
#define GAMESTATE_HPP_
#pragma once

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    LEVEL_COMPLETE,
    GAME_OVER
};

struct GameConfig {
    int windowWidth = 1024;
    int windowHeight = 768;
    const char* title = "Sokoban 3D";
    float masterVolume = 0.7f;
    bool vsync = true;
    bool fullscreen = false;
    int targetFPS = 60;
    bool showFPS = true;
    bool enableMSAA = true;
    bool enableVSync = true;
    float musicVolume = 0.5f;
    float sfxVolume = 0.8f;
    bool enableAnimations = true;
    float animationSpeed = 1.0f;
    bool showGrid = false;
    float inputDelay = 0.15f;
    bool allowDiagonalMovement = false;
};

inline const char* GameStateToString(GameState state)
{
    switch (state) {
        case GameState::MENU: return "MENU";
        case GameState::PLAYING: return "PLAYING";
        case GameState::PAUSED: return "PAUSED";
        case GameState::LEVEL_COMPLETE: return "LEVEL_COMPLETE";
        case GameState::GAME_OVER: return "GAME_OVER";
        default: return "UNKNOWN";
    }
}

inline bool IsGameplayState(GameState state)
{
    return state == GameState::PLAYING ||
           state == GameState::PAUSED ||
           state == GameState::LEVEL_COMPLETE;
}

inline bool IsMenuState(GameState state)
{
    return state == GameState::MENU ||
           state == GameState::GAME_OVER;
}

#endif /* !GAMESTATE_HPP_ */