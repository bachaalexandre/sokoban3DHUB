/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** PauseScreen
*/

#include "PauseScreen.hpp"
#include <cmath>

PauseScreen::PauseScreen() :
    selectedOption(0),
    menuItems(),
    isVisible(false),
    font(),
    fontLoaded(false),
    menuAnimation(0.0f),
    overlayAlpha(0.0f),
    targetAlpha(180.0f),
    inputCooldown(0.0f),
    titlePos({0}),
    menuStartPos({0}),
    menuSpacing(50.0f),
    backgroundColor({0, 0, 0, 180}),
    selectedColor({255, 100, 100, 255}),
    normalColor({255, 255, 255, 255}),
    titleColor({255, 215, 0, 255})
{
    InitializeMenuItems();
}


PauseScreen::~PauseScreen() {
    Shutdown();
}

void PauseScreen::InitializeMenuItems() {
    menuItems.clear();
    menuItems.push_back("Resume");
    menuItems.push_back("Restart Level");
    menuItems.push_back("Main Menu");
}

bool PauseScreen::Initialize()
{
    if (FileExists("assets/fonts/ui_font.ttf")) {
        font = LoadFont("assets/fonts/ui_font.ttf");
        fontLoaded = true;
    } else {
        font = GetFontDefault();
        fontLoaded = false;
    }
    CalculatePositions();
    Reset();
    return true;
}

void PauseScreen::Shutdown()
{
    if (fontLoaded) {
        UnloadFont(font);
        fontLoaded = false;
    }
}

void PauseScreen::CalculatePositions()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    titlePos.x = screenWidth / 2.0f;
    titlePos.y = screenHeight * 0.3f;
    menuStartPos.x = screenWidth / 2.0f;
    menuStartPos.y = screenHeight * 0.5f;
}

void PauseScreen::Update(float deltaTime)
{
    if (!isVisible)
        return;
    if (inputCooldown > 0)
        inputCooldown -= deltaTime;
    UpdateAnimations(deltaTime);
    if (inputCooldown <= 0) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            MoveUp();
            inputCooldown = INPUT_DELAY;
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            MoveDown();
            inputCooldown = INPUT_DELAY;
        }
        else if (IsKeyPressed(KEY_ESCAPE)) {
            selectedOption = 0;
            inputCooldown = INPUT_DELAY;
        }
    }
}

void PauseScreen::UpdateAnimations(float deltaTime)
{
    menuAnimation += deltaTime * 2.0f;
    if (isVisible && overlayAlpha < targetAlpha) {
        overlayAlpha += deltaTime * 400.0f;
        if (overlayAlpha > targetAlpha) {
            overlayAlpha = targetAlpha;
        }
    } else if (!isVisible && overlayAlpha > 0) {
        overlayAlpha -= deltaTime * 600.0f;
        if (overlayAlpha < 0) {
            overlayAlpha = 0;
        }
    }
    if (menuAnimation > 360.0f)
        menuAnimation -= 360.0f;
}

void PauseScreen::Draw()
{
    if (!isVisible && overlayAlpha <= 0)
        return;
    DrawOverlay();
    DrawTitle();
    DrawMenuItems();
    DrawInstructions();
}

void PauseScreen::DrawOverlay()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    Color overlayColor = backgroundColor;
    overlayColor.a = static_cast<unsigned char>(overlayAlpha);
    DrawRectangle(0, 0, screenWidth, screenHeight, overlayColor);
    int panelWidth = 400;
    int panelHeight = 300;
    int panelX = (screenWidth - panelWidth) / 2;
    int panelY = (screenHeight - panelHeight) / 2;
    DrawRectangle(panelX + 5, panelY + 5, panelWidth, panelHeight, {0, 0, 0, 100});
    DrawRectangle(panelX, panelY, panelWidth, panelHeight, {40, 50, 70, 220});
    DrawRectangleLines(panelX, panelY, panelWidth, panelHeight, {100, 120, 150, 255});
}

void PauseScreen::DrawTitle()
{
    const char* title = "PAUSED";
    int fontSize = 48;
    Vector2 titleSize = MeasureTextEx(font, title, fontSize, 2);
    float pulseScale = 1.0f + sin(menuAnimation * 3) * 0.05f;
    float scaledFontSize = fontSize * pulseScale;
    Vector2 drawPos = {
        titlePos.x - titleSize.x / 2.0f,
        titlePos.y
    };
    DrawTextEx(font, title, {drawPos.x + 2, drawPos.y + 2}, scaledFontSize, 2, {0, 0, 0, 150});
    DrawTextEx(font, title, drawPos, scaledFontSize, 2, titleColor);
}

void PauseScreen::DrawMenuItems()
{
    int fontSize = 28;
    for (size_t i = 0; i < menuItems.size(); i++) {
        const char* item = menuItems[i].c_str();
        Vector2 textSize = MeasureTextEx(font, item, fontSize, 1);
        Vector2 itemPos = {
            menuStartPos.x - textSize.x / 2.0f,
            menuStartPos.y + i * menuSpacing
        };
        if (static_cast<int>(i) == selectedOption) {
            float bounce = sin(menuAnimation * 4) * 3.0f;
            itemPos.x += bounce;
            DrawRectangle(
                itemPos.x - 15,
                itemPos.y - 3,
                textSize.x + 30,
                textSize.y + 6,
                {255, 255, 255, 80}
            );
            DrawText(">", itemPos.x - 30, itemPos.y, fontSize, selectedColor);
            DrawText("<", itemPos.x + textSize.x + 10, itemPos.y, fontSize, selectedColor);
        }
        Color itemColor = (static_cast<int>(i) == selectedOption) ? selectedColor : normalColor;
        if (static_cast<int>(i) == selectedOption) {
            float glowIntensity = (sin(menuAnimation * 5) + 1.0f) * 0.5f;
            Color glowColor = {
                selectedColor.r,
                selectedColor.g,
                selectedColor.b,
                static_cast<unsigned char>(50 * glowIntensity)
            };
            for (int offset = 1; offset <= 3; offset++) {
                DrawTextEx(font, item, {itemPos.x + offset, itemPos.y}, fontSize, 1, glowColor);
                DrawTextEx(font, item, {itemPos.x - offset, itemPos.y}, fontSize, 1, glowColor);
                DrawTextEx(font, item, {itemPos.x, itemPos.y + offset}, fontSize, 1, glowColor);
                DrawTextEx(font, item, {itemPos.x, itemPos.y - offset}, fontSize, 1, glowColor);
            }
        }
        DrawTextEx(font, item, {itemPos.x + 1, itemPos.y + 1}, fontSize, 1, {0, 0, 0, 150});
        DrawTextEx(font, item, itemPos, fontSize, 1, itemColor);
    }
}

void PauseScreen::DrawInstructions()
{
    const char* instructions = "Use ARROW KEYS or WASD to navigate, ENTER to select, ESC to resume";
    int fontSize = 16;
    Vector2 textSize = MeasureTextEx(font, instructions, fontSize, 1);
    Vector2 instructionPos = {
        GetScreenWidth() / 2.0f - textSize.x / 2.0f,
        menuStartPos.y + menuItems.size() * menuSpacing + 40
    };
    DrawTextEx(font, instructions, {instructionPos.x + 1, instructionPos.y + 1}, fontSize, 1, {0, 0, 0, 100});
    DrawTextEx(font, instructions, instructionPos, fontSize, 1, {180, 180, 180, 255});
}

void PauseScreen::Show()
{
    isVisible = true;
    overlayAlpha = 0;
    Reset();
}

void PauseScreen::Hide()
{
    isVisible = false;
}

void PauseScreen::MoveUp()
{
    selectedOption--;
    if (selectedOption < 0) {
        selectedOption = static_cast<int>(menuItems.size()) - 1;
    }
}

void PauseScreen::MoveDown()
{
    selectedOption++;
    if (selectedOption >= static_cast<int>(menuItems.size())) {
        selectedOption = 0;
    }
}

PauseOption PauseScreen::GetSelectedOption() const
{
    return static_cast<PauseOption>(selectedOption);
}

bool PauseScreen::IsOptionSelected() const
{
    return (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && isVisible;
}

void PauseScreen::Reset() {
    selectedOption = 0;
    menuAnimation = 0.0f;
    inputCooldown = 0.0f;
    CalculatePositions();
}