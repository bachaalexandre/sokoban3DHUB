/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** MenuScreen
*/

#ifndef MENUSCREEN_HPP_
#define MENUSCREEN_HPP_
#pragma once

#include <raylib.h>
#include <vector>
#include <string>

enum class MenuOption {
    START_GAME,
    SELECT_LEVEL,
    OPTIONS,
    QUIT
};

class MenuScreen {
private:
    int selectedOption;
    std::vector<std::string> menuItems;
    Font font;
    Music backgroundMusic;
    bool musicLoaded;
    bool fontLoaded;
    float titleAnimation;
    float menuAnimation;
    float backgroundRotation;
    Color backgroundColor;
    Color titleColor;
    Color selectedColor;
    Color normalColor;
    Vector2 titlePos;
    Vector2 menuStartPos;
    float menuSpacing;
    float inputCooldown;
    const float INPUT_DELAY = 0.15f;
    void InitializeMenuItems();
    void CalculatePositions();
    void UpdateAnimations(float deltaTime);
    void DrawBackground();
    void DrawTitle();
    void DrawMenuItems();
    void DrawFooter();

public:
    MenuScreen();
    ~MenuScreen();
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void Draw();
    void MoveUp();
    void MoveDown();
    MenuOption GetSelectedOption() const;
    bool IsOptionSelected() const;
    void PlayBackgroundMusic();
    void StopBackgroundMusic();
    void Reset();
};

#endif /* !MENUSCREEN_HPP_ */