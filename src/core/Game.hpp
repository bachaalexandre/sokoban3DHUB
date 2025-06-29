/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_
#pragma once

#include <raylib.h>
#include "GameState.hpp"

class MenuScreen;
class GameScreen;
class PauseScreen;

class Game {
private:
    GameState currentState;
    GameConfig config;
    bool shouldClose;
    float deltaTime;

    MenuScreen* menuScreen;
    GameScreen* gameScreen;
    PauseScreen* pauseScreen;

    void UpdateCurrentScreen();
    void DrawCurrentScreen();
    void HandleStateTransitions();
    void CleanupScreens();

public:
    Game();
    ~Game();

    bool Initialize();
    void Run();
    void Shutdown();
    void ChangeState(GameState newState);
    GameState GetCurrentState() const { return currentState; }
    float GetDeltaTime() const { return deltaTime; }
    const GameConfig& GetConfig() const { return config; }
    void SetConfig(const GameConfig& newConfig) { config = newConfig; }
    bool ShouldClose() const { return shouldClose; }
    void RequestClose() { shouldClose = true; }
    MenuScreen* GetMenuScreen() const { return menuScreen; }
    GameScreen* GetGameScreen() const { return gameScreen; }
    PauseScreen* GetPauseScreen() const { return pauseScreen; }
};

#endif /* !GAME_HPP_ */