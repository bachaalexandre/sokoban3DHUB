/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** GameScreen
*/

#ifndef GAMESCREEN_HPP_
#define GAMESCREEN_HPP_
#pragma once

#include <raylib.h>
#include <memory>
#include "../level/Level.hpp"
#include "../entities/Player.hpp"
#include "../entities/Box.hpp"
#include "../graphics/Renderer.hpp"

enum class GameScreenState {
    LOADING,
    PLAYING,
    LEVEL_COMPLETE,
    PAUSED
};

class GameScreen {
private:
    GameScreenState state;
    int currentLevel;
    int moveCount;
    float gameTime;
    float levelCompleteTimer;
    bool shouldShowPause;
    bool shouldReturnToMenu;
    float movementTimer;
    Vector2 lastMoveDirection;
    bool isMovementPressed;
    std::unique_ptr<Level> level;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Box>> boxes;
    std::unique_ptr<Renderer> renderer;
    Font uiFont;
    bool fontLoaded;
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;
    std::vector<std::string> levelFiles;
    int totalLevels;
    Sound moveSound;
    Sound pushSound;
    Sound completeSound;
    bool soundsLoaded;
    float movementCooldown;
    float generalInputCooldown;
    void InitializeLevelFiles();
    void LoadSounds();
    bool LoadLevel(int levelIndex);
    void CreateBoxes();
    void ProcessInput();
    void UpdateGameLogic(float deltaTime);
    void CheckLevelCompletion();
    void HandlePlayerMovement();
    bool TryMovePlayer(Vector2 direction);
    bool TryPushBox(int boxIndex, Vector2 direction);
    void PlayMoveSound();
    void PlayPushSound();
    void PlayCompleteSound();
    void DrawUI();
    void DrawLevelCompleteOverlay();
    void DrawGameStats();
    void DrawInstructions();

public:
    GameScreen();
    ~GameScreen();
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void Draw();
    void SetCurrentLevel(int levelIndex);
    int GetCurrentLevel() const { return currentLevel; }
    bool LoadNextLevel();
    void RestartLevel();
    GameScreenState GetState() const { return state; }
    bool IsLevelComplete() const;
    bool ShouldShowPause() const { return shouldShowPause; }
    bool ShouldReturnToMenu() const { return shouldReturnToMenu; }
    void IncrementMoveCount() { moveCount++; }
    int GetMoveCount() const { return moveCount; }
    float GetGameTime() const { return gameTime; }
    void ResetStats();
    void CreateTestLevel();
    void SynchronizeBoxesWithLevel();
    void ClearFlags();
};

#endif /* !GAMESCREEN_HPP_ */