/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** MenuScreen
*/

#include "MenuScreen.hpp"
#include <cmath>

MenuScreen::MenuScreen() :
    selectedOption(0),
    musicLoaded(false),
    fontLoaded(false),
    titleAnimation(0.0f),
    menuAnimation(0.0f),
    backgroundRotation(0.0f),
    backgroundColor({20, 25, 40, 255}),
    titleColor({255, 215, 0, 255}),
    selectedColor({255, 100, 100, 255}),
    normalColor({200, 200, 200, 255}),
    menuSpacing(60.0f),
    inputCooldown(0.0f)
{
    InitializeMenuItems();
}

MenuScreen::~MenuScreen()
{
    Shutdown();
}

void MenuScreen::InitializeMenuItems()
{
    menuItems.clear();
    menuItems.push_back("Start Game");
    menuItems.push_back("Select Level");
    menuItems.push_back("Options");
    menuItems.push_back("Quit");
}

bool MenuScreen::Initialize()
{
    if (FileExists("assets/fonts/menu_font.ttf")) {
        font = LoadFont("assets/fonts/menu_font.ttf");
        fontLoaded = true;
    } else {
        font = GetFontDefault();
        fontLoaded = false;
    }
    if (FileExists("assets/sounds/menu_music.mp3")) {
        backgroundMusic = LoadMusicStream("assets/sounds/menu_music.mp3");
        musicLoaded = true;
        SetMusicVolume(backgroundMusic, 0.3f);
    }
    CalculatePositions();
    Reset();
    return true;
}

void MenuScreen::Shutdown()
{
    if (musicLoaded) {
        StopBackgroundMusic();
        UnloadMusicStream(backgroundMusic);
        musicLoaded = false;
    }
    if (fontLoaded) {
        UnloadFont(font);
        fontLoaded = false;
    }
}

void MenuScreen::CalculatePositions()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    titlePos.x = screenWidth / 2.0f;
    titlePos.y = screenHeight * 0.2f;
    menuStartPos.x = screenWidth / 2.0f;
    menuStartPos.y = screenHeight * 0.45f;
}

void MenuScreen::Update(float deltaTime)
{
    if (inputCooldown > 0)
        inputCooldown -= deltaTime;
    UpdateAnimations(deltaTime);
    if (musicLoaded && IsMusicStreamPlaying(backgroundMusic))
        UpdateMusicStream(backgroundMusic);
    if (inputCooldown <= 0) {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            MoveUp();
            inputCooldown = INPUT_DELAY;
        }
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            MoveDown();
            inputCooldown = INPUT_DELAY;
        }
    }
}

void MenuScreen::UpdateAnimations(float deltaTime)
{
    titleAnimation += deltaTime * 2.0f;
    menuAnimation += deltaTime * 1.5f;
    backgroundRotation += deltaTime * 10.0f;
    if (titleAnimation > 360.0f)
        titleAnimation -= 360.0f;
    if (menuAnimation > 360.0f)
        menuAnimation -= 360.0f;
    if (backgroundRotation > 360.0f)
        backgroundRotation -= 360.0f;
}

void MenuScreen::Draw()
{
    DrawBackground();
    DrawTitle();
    DrawMenuItems();
    DrawFooter();
}

void MenuScreen::DrawBackground()
{
    ClearBackground(backgroundColor);
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    for (int i = 0; i < 5; i++) {
        float x = (screenWidth / 6.0f) * (i + 1);
        float y = screenHeight * 0.7f + sin(backgroundRotation * DEG2RAD + i) * 50.0f;
        float size = 20.0f + sin(backgroundRotation * DEG2RAD + i * 2) * 10.0f;
        Color cubeColor = {
            static_cast<unsigned char>(100 + sin(backgroundRotation * DEG2RAD + i) * 50),
            static_cast<unsigned char>(100 + cos(backgroundRotation * DEG2RAD + i) * 50),
            static_cast<unsigned char>(150 + sin(backgroundRotation * DEG2RAD + i * 1.5) * 50),
            100
        };
        DrawRectangle(x - size/2, y - size/2, size, size, cubeColor);
    }
}

void MenuScreen::DrawTitle()
{
    const char* title = "SOKOBAN 3D";
    int fontSize = 72;
    Vector2 titleSize = MeasureTextEx(font, title, fontSize, 2);
    float animOffset = sin(titleAnimation) * 10.0f;
    Vector2 drawPos = {
        titlePos.x - titleSize.x / 2.0f,
        titlePos.y + animOffset
    };
    DrawTextEx(font, title, {drawPos.x + 3, drawPos.y + 3}, fontSize, 2, {0, 0, 0, 100});
    Color animatedTitleColor = {
        titleColor.r,
        static_cast<unsigned char>(titleColor.g + sin(titleAnimation * 2) * 50),
        titleColor.b,
        titleColor.a
    };
    DrawTextEx(font, title, drawPos, fontSize, 2, animatedTitleColor);
}

void MenuScreen::DrawMenuItems()
{
    int fontSize = 36;
    for (size_t i = 0; i < menuItems.size(); i++) {
        const char* item = menuItems[i].c_str();
        Vector2 textSize = MeasureTextEx(font, item, fontSize, 1);
        Vector2 itemPos = {
            menuStartPos.x - textSize.x / 2.0f,
            menuStartPos.y + i * menuSpacing
        };
        if (static_cast<int>(i) == selectedOption) {
            float bounce = sin(menuAnimation * 3) * 5.0f;
            itemPos.x += bounce;
            DrawRectangle(itemPos.x - 20, itemPos.y - 5, textSize.x + 40, textSize.y + 10, {255, 255, 255, 50});
        }
        Color itemColor = (static_cast<int>(i) == selectedOption) ? selectedColor : normalColor;
        DrawTextEx(font, item, {itemPos.x + 2, itemPos.y + 2}, fontSize, 1, {0, 0, 0, 100});
        DrawTextEx(font, item, itemPos, fontSize, 1, itemColor);
    }
}

void MenuScreen::DrawFooter()
{
    const char* instructions = "Use ARROW KEYS or WASD to navigate, ENTER to select, ESC to quit";
    int fontSize = 20;
    Vector2 textSize = MeasureTextEx(font, instructions, fontSize, 1);
    Vector2 footerPos = {
        GetScreenWidth() / 2.0f - textSize.x / 2.0f,
        GetScreenHeight() - 50.0f
    };
    DrawTextEx(font, instructions, footerPos, fontSize, 1, {150, 150, 150, 255});
}

void MenuScreen::MoveUp() {
    selectedOption--;
    if (selectedOption < 0) {
        selectedOption = static_cast<int>(menuItems.size()) - 1;
    }
}

void MenuScreen::MoveDown() {
    selectedOption++;
    if (selectedOption >= static_cast<int>(menuItems.size())) {
        selectedOption = 0;
    }
}

MenuOption MenuScreen::GetSelectedOption() const {
    return static_cast<MenuOption>(selectedOption);
}

bool MenuScreen::IsOptionSelected() const {
    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
}

void MenuScreen::PlayBackgroundMusic() {
    if (musicLoaded && !IsMusicStreamPlaying(backgroundMusic)) {
        PlayMusicStream(backgroundMusic);
    }
}

void MenuScreen::StopBackgroundMusic() {
    if (musicLoaded && IsMusicStreamPlaying(backgroundMusic)) {
        StopMusicStream(backgroundMusic);
    }
}

void MenuScreen::Reset() {
    selectedOption = 0;
    titleAnimation = 0.0f;
    menuAnimation = 0.0f;
    backgroundRotation = 0.0f;
    inputCooldown = 0.0f;
}