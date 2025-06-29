/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** PauseScreen
*/

#pragma once
#include <raylib.h>
#include <vector>
#include <string>

enum class PauseOption {
    RESUME,
    RESTART_LEVEL,
    MAIN_MENU
};

class PauseScreen {
private:
    static constexpr float INPUT_DELAY = 0.15f;
    int selectedOption;
    std::vector<std::string> menuItems;
    bool isVisible;
    Font font;
    bool fontLoaded;
    float menuAnimation;
    float overlayAlpha;
    float targetAlpha;
    float inputCooldown;
    Vector2 titlePos;
    Vector2 menuStartPos;
    float menuSpacing;
    Color backgroundColor;
    Color selectedColor;
    Color normalColor;
    Color titleColor;
    void InitializeMenuItems();
    void CalculatePositions();
    void UpdateAnimations(float deltaTime);
    void DrawOverlay();
    void DrawTitle();
    void DrawMenuItems();
    void DrawInstructions();

public:
    PauseScreen();
    ~PauseScreen();
    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);
    void Draw();
    void Show();
    void Hide();
    bool IsVisible() const { return isVisible; }
    void MoveUp();
    void MoveDown();
    PauseOption GetSelectedOption() const;
    bool IsOptionSelected() const;
    void Reset();
};