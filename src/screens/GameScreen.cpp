/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** GameScreen
*/

#include "GameScreen.hpp"
#include <iostream>
#include <filesystem>

GameScreen::GameScreen() :
    state(GameScreenState::LOADING),
    currentLevel(0),
    moveCount(0),
    gameTime(0.0f),
    levelCompleteTimer(0.0f),
    shouldShowPause(false),
    shouldReturnToMenu(false),
    fontLoaded(false),
    inputCooldown(0.0f),
    totalLevels(0),
    soundsLoaded(false),
    movementCooldown(0.0f),
    generalInputCooldown(0.0f)
{
    level = std::make_unique<Level>();
    player = std::make_unique<Player>();
    renderer = std::make_unique<Renderer>();
}

GameScreen::~GameScreen() {
    Shutdown();
}

bool GameScreen::Initialize()
{
    if (!renderer->Initialize()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return false;
    }
    if (FileExists("assets/fonts/ui_font.ttf")) {
        uiFont = LoadFont("assets/fonts/ui_font.ttf");
        fontLoaded = true;
    } else {
        uiFont = GetFontDefault();
        fontLoaded = false;
    }
    LoadSounds();
    InitializeLevelFiles();
    player->Initialize({1.0f, 0.0f, 1.0f});
    ResetStats();
    return true;
}

void GameScreen::LoadSounds()
{
    if (FileExists("assets/sounds/move.wav")) {
        moveSound = LoadSound("assets/sounds/move.wav");
        soundsLoaded = true;
    }
    if (FileExists("assets/sounds/push.wav")) {
        pushSound = LoadSound("assets/sounds/push.wav");
    }
    if (FileExists("assets/sounds/complete.wav")) {
        completeSound = LoadSound("assets/sounds/complete.wav");
    }
}

void GameScreen::Shutdown()
{
    if (fontLoaded) {
        UnloadFont(uiFont);
        fontLoaded = false;
    }
    if (soundsLoaded) {
        UnloadSound(moveSound);
        UnloadSound(pushSound);
        UnloadSound(completeSound);
        soundsLoaded = false;
    }
    if (renderer) {
        renderer->Shutdown();
    }
    boxes.clear();
}

void GameScreen::InitializeLevelFiles()
{
    levelFiles.clear();
    for (int i = 1; i <= 10; i++) {
        std::string filename = "assets/levels/level" + std::to_string(i) + ".json";
        levelFiles.push_back(filename);
    }
    totalLevels = static_cast<int>(levelFiles.size());
    std::cout << "Found " << totalLevels << " levels" << std::endl;
}

void GameScreen::Update(float deltaTime)
{
    if (movementCooldown > 0)
        movementCooldown -= deltaTime;
    if (generalInputCooldown > 0)
        generalInputCooldown -= deltaTime;
    if (state == GameScreenState::PLAYING)
        gameTime += deltaTime;
    if (state == GameScreenState::LEVEL_COMPLETE)
        levelCompleteTimer += deltaTime;
    ProcessInput();
    UpdateGameLogic(deltaTime);
    if (player)
        player->Update(deltaTime);
    for (auto& box : boxes) {
        if (box)
            box->Update(deltaTime);
    }
    if (state == GameScreenState::PLAYING)
        CheckLevelCompletion();
}

void GameScreen::ProcessInput()
{
    if (generalInputCooldown <= 0) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_P)) {
            if (state == GameScreenState::PLAYING) {
                shouldShowPause = true;
                generalInputCooldown = 0.2f;
            }
        }
        if (IsKeyPressed(KEY_M)) {
            shouldReturnToMenu = true;
            generalInputCooldown = 0.2f;
        }
        if (IsKeyPressed(KEY_R)) {
            RestartLevel();
            generalInputCooldown = 0.3f;
        }
    }
    if (state == GameScreenState::LEVEL_COMPLETE) {
        if (generalInputCooldown <= 0) {
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (currentLevel < totalLevels - 1) {
                    LoadNextLevel();
                } else {
                    shouldReturnToMenu = true;
                }
                generalInputCooldown = 0.2f;
            }
        }
        return;
    }
    if (state == GameScreenState::PLAYING)
        HandlePlayerMovement();
}

void GameScreen::HandlePlayerMovement()
{
    if (movementCooldown > 0)
        return;
    Vector2 direction = {0, 0};
    bool moveRequested = false;
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        direction = {0, -1};
        moveRequested = true;
    }
    else if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) {
        direction = {0, 1};
        moveRequested = true;
    }
    else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        direction = {-1, 0};
        moveRequested = true;
    }
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        direction = {1, 0};
        moveRequested = true;
    }
    if (moveRequested) {
        bool success = TryMovePlayer(direction);
        if (success) {
            movementCooldown = 0.12f;
        } else {
            movementCooldown = 0.03f;
        }
    }
}

void GameScreen::SynchronizeBoxesWithLevel()
{
    if (!level)
        return;
    int width = level->GetWidth();
    int height = level->GetHeight();
    std::vector<Vector2> boxPositions;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (level->HasBox(x, y)) {
                boxPositions.push_back({static_cast<float>(x), static_cast<float>(y)});
            }
        }
    }
    size_t numBoxesToUpdate = std::min(boxes.size(), boxPositions.size());
    for (size_t i = 0; i < numBoxesToUpdate; i++) {
        if (boxes[i]) {
            Vector3 worldPos = level->GridToWorld(
                static_cast<int>(boxPositions[i].x),
                static_cast<int>(boxPositions[i].y)
            );
            boxes[i]->SetPosition(worldPos);
        }
    }
    if (boxPositions.size() > boxes.size()) {
        for (size_t i = boxes.size(); i < boxPositions.size(); i++) {
            Vector3 worldPos = level->GridToWorld(
                static_cast<int>(boxPositions[i].x),
                static_cast<int>(boxPositions[i].y)
            );
            auto newBox = std::make_unique<Box>();
            newBox->SetPosition(worldPos);
            boxes.push_back(std::move(newBox));
        }
    }
    if (boxPositions.size() < boxes.size())
        boxes.resize(boxPositions.size());
}

bool GameScreen::TryMovePlayer(Vector2 direction)
{
    if (!level || !player)
        return false;
    Vector2 playerPos = player->GetGridPosition();
    Vector2 newPos = {playerPos.x + direction.x, playerPos.y + direction.y};
    if (!level->IsValidPosition(static_cast<int>(newPos.x), static_cast<int>(newPos.y)))
        return false;
    TileType tileType = level->GetTileType(static_cast<int>(newPos.x), static_cast<int>(newPos.y));
    if (tileType == TileType::WALL)
        return false;
    if (level->HasBox(static_cast<int>(newPos.x), static_cast<int>(newPos.y))) {
        Vector2 boxNewPos = {newPos.x + direction.x, newPos.y + direction.y};
        if (!level->IsValidPosition(static_cast<int>(boxNewPos.x), static_cast<int>(boxNewPos.y))) {
            return false;
        }
        if (!level->CanMoveToTile(static_cast<int>(boxNewPos.x), static_cast<int>(boxNewPos.y))) {
            return false;
        }
        level->MoveBox(
            static_cast<int>(newPos.x), static_cast<int>(newPos.y),
            static_cast<int>(boxNewPos.x), static_cast<int>(boxNewPos.y)
        );
        PlayPushSound();
    } else {
        PlayMoveSound();
    }
    player->SetGridPosition(newPos);
    Vector3 worldPos = level->GridToWorld(static_cast<int>(newPos.x), static_cast<int>(newPos.y));
    player->SetPosition(worldPos);
    SynchronizeBoxesWithLevel();
    IncrementMoveCount();
    return true;
}

void GameScreen::UpdateGameLogic(float deltaTime)
{
    if (renderer && player) {
        Vector3 playerPos = player->GetPosition();
        Vector3 cameraPos = {playerPos.x + 5, playerPos.y + 8, playerPos.z + 5};
        Vector3 cameraTarget = playerPos;
        Vector3 cameraUp = {0, 1, 0};
        renderer->SetCamera(cameraPos, cameraTarget, cameraUp);
    }
}

void GameScreen::CheckLevelCompletion()
{
    if (level && level->IsCompleted()) {
        state = GameScreenState::LEVEL_COMPLETE;
        levelCompleteTimer = 0.0f;
        PlayCompleteSound();
        std::cout << "Level completed!" << std::endl;
    }
}

void GameScreen::Draw()
{
    if (!renderer)
        return;
    renderer->BeginFrame();
    BeginMode3D(renderer->GetCamera());
    if (level) {
        const LevelData& levelData = level->GetData();
        for (int y = 0; y < levelData.height; y++) {
            for (int x = 0; x < levelData.width; x++) {
                Vector3 worldPos = level->GridToWorld(x, y);
                TileType tileType = level->GetTileType(x, y);

                switch (tileType) {
                    case TileType::WALL:
                        renderer->DrawWall(worldPos);
                        break;
                    case TileType::FLOOR:
                    case TileType::PLAYER_START:
                        renderer->DrawFloor(worldPos);
                        if (level->IsTarget(x, y)) {
                            renderer->DrawTarget(worldPos);
                        }
                        break;
                    case TileType::TARGET:
                        renderer->DrawFloor(worldPos);
                        renderer->DrawTarget(worldPos);
                        break;
                }
            }
        }
        for (const auto& box : boxes) {
            if (box) {
                Vector3 boxPos = box->GetPosition();
                Vector2 gridPos = box->GetGridPosition();
                bool onTarget = level->IsTarget(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y));
                renderer->DrawBox(boxPos, onTarget);
            }
        }
        if (player) {
            Vector3 playerPos = player->GetPosition();
            renderer->DrawPlayer(playerPos, player->GetRotation());
        }
    } else {
        std::cout << "No level loaded!" << std::endl;
        DrawCube({0, 1, 0}, 2, 2, 2, RED);
    }
    EndMode3D();
    DrawUI();
    if (state == GameScreenState::LEVEL_COMPLETE)
        DrawLevelCompleteOverlay();
    renderer->EndFrame();
}

void GameScreen::DrawUI() {
    DrawGameStats();
    DrawInstructions();
}

void GameScreen::DrawGameStats()
{
    int fontSize = 20;
    int y = 20;
    std::string levelText = "Level: " + std::to_string(currentLevel + 1) + "/" + std::to_string(totalLevels);
    DrawTextEx(uiFont, levelText.c_str(), {20, static_cast<float>(y)}, fontSize, 1, WHITE);
    y += 30;
    std::string moveText = "Moves: " + std::to_string(moveCount);
    DrawTextEx(uiFont, moveText.c_str(), {20, static_cast<float>(y)}, fontSize, 1, WHITE);
    y += 30;
    int minutes = static_cast<int>(gameTime) / 60;
    int seconds = static_cast<int>(gameTime) % 60;
    std::string timeText = "Time: " + std::to_string(minutes) + ":" + 
                          (seconds < 10 ? "0" : "") + std::to_string(seconds);
    DrawTextEx(uiFont, timeText.c_str(), {20, static_cast<float>(y)}, fontSize, 1, WHITE);
}

void GameScreen::DrawInstructions()
{
    int fontSize = 16;
    int screenHeight = GetScreenHeight();
    int y = screenHeight - 80;
    DrawTextEx(uiFont, "WASD/Arrows: Move  |  R: Restart  |  ESC: Pause  |  M: Menu",
               {20, static_cast<float>(y)}, fontSize, 1, LIGHTGRAY);
}

void GameScreen::DrawLevelCompleteOverlay()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 150});
    const char* title = "LEVEL COMPLETE!";
    int titleSize = 48;
    Vector2 titleTextSize = MeasureTextEx(uiFont, title, titleSize, 2);
    Vector2 titlePos = {
        screenWidth / 2.0f - titleTextSize.x / 2.0f,
        screenHeight / 2.0f - 100
    };
    DrawTextEx(uiFont, title, titlePos, titleSize, 2, GREEN);
    int statsSize = 24;
    int statsY = static_cast<int>(titlePos.y) + 80;
    std::string statsText = "Moves: " + std::to_string(moveCount);
    Vector2 statsTextSize = MeasureTextEx(uiFont, statsText.c_str(), statsSize, 1);
    Vector2 statsPos = {
        screenWidth / 2.0f - statsTextSize.x / 2.0f,
        static_cast<float>(statsY)
    };
    DrawTextEx(uiFont, statsText.c_str(), statsPos, statsSize, 1, WHITE);
    const char* instruction = (currentLevel < totalLevels - 1) ?
        "Press ENTER for next level" : "Press ENTER to return to menu";
    int instructionSize = 20;
    Vector2 instructionTextSize = MeasureTextEx(uiFont, instruction, instructionSize, 1);
    Vector2 instructionPos = {
        screenWidth / 2.0f - instructionTextSize.x / 2.0f,
        static_cast<float>(statsY + 60)
    };
    DrawTextEx(uiFont, instruction, instructionPos, instructionSize, 1, LIGHTGRAY);
}

void GameScreen::SetCurrentLevel(int levelIndex)
{
    if (levelIndex >= 0 && levelIndex < totalLevels) {
        currentLevel = levelIndex;
        LoadLevel(currentLevel);
    }
}

bool GameScreen::LoadLevel(int levelIndex)
{
    if (levelIndex < 0 || levelIndex >= totalLevels) {
        return false;
    }
    std::cout << "Loading level " << (levelIndex + 1) << "..." << std::endl;
    boxes.clear();
    bool levelLoaded = false;
    if (std::filesystem::exists(levelFiles[levelIndex])) {
        levelLoaded = level->LoadFromFile(levelFiles[levelIndex]);
        std::cout << "Attempted to load: " << levelFiles[levelIndex] << std::endl;
    }
    if (!levelLoaded) {
        std::cerr << "Could not load level, creating test level!" << std::endl;
        CreateTestLevel();
        levelLoaded = true;
    }
    const LevelData& levelData = level->GetData();
    Vector3 playerStartPos = level->GridToWorld(
        static_cast<int>(levelData.playerStart.x),
        static_cast<int>(levelData.playerStart.y)
    );
    player->Initialize(playerStartPos);
    player->SetGridPosition(levelData.playerStart);
    CreateBoxes();
    if (renderer) {
        float centerX = levelData.width * 0.5f;
        float centerZ = levelData.height * 0.5f;
        Vector3 cameraPos = {centerX + 8.0f, 10.0f, centerZ + 8.0f};
        Vector3 cameraTarget = {centerX, 0.0f, centerZ};
        Vector3 cameraUp = {0.0f, 1.0f, 0.0f};
        renderer->SetCamera(cameraPos, cameraTarget, cameraUp);
    }
    ResetStats();
    state = GameScreenState::PLAYING;
    std::cout << "Level " << (levelIndex + 1) << " loaded successfully!" << std::endl;
    std::cout << "Level size: " << levelData.width << "x" << levelData.height << std::endl;
    std::cout << "Player start: " << levelData.playerStart.x << "," << levelData.playerStart.y << std::endl;
    std::cout << "Total boxes: " << boxes.size() << std::endl;
    return true;
}

void GameScreen::CreateBoxes()
{
    boxes.clear();
    if (!level) {
        std::cout << "No level available for creating boxes!" << std::endl;
        return;
    }
    const LevelData& levelData = level->GetData();
    for (int y = 0; y < levelData.height; y++) {
        for (int x = 0; x < levelData.width; x++) {
            if (level->HasBox(x, y)) {
                auto box = std::make_unique<Box>();
                Vector2 gridPos = {static_cast<float>(x), static_cast<float>(y)};
                box->Initialize(gridPos, level.get());
                boxes.push_back(std::move(box));
            }
        }
    }
}

void GameScreen::CreateTestLevel()
{
    std::string testLevelJSON = R"({
        "name": "Test Level",
        "width": 7,
        "height": 5,
        "playerStart": {"x": 1, "y": 1},
        "grid": [
            "#######",
            "#@  $.#",
            "#     #",
            "#     #",
            "#######"
        ]
    })";

    level->LoadFromJSON(testLevelJSON);
}

bool GameScreen::LoadNextLevel()
{
    if (currentLevel < totalLevels - 1) {
        currentLevel++;
        return LoadLevel(currentLevel);
    }
    return false;
}

void GameScreen::RestartLevel()
{
    LoadLevel(currentLevel);
}

bool GameScreen::IsLevelComplete() const
{
    return state == GameScreenState::LEVEL_COMPLETE;
}

void GameScreen::ResetStats()
{
    moveCount = 0;
    gameTime = 0.0f;
    levelCompleteTimer = 0.0f;
}

void GameScreen::ClearFlags()
{
    shouldShowPause = false;
    shouldReturnToMenu = false;
}

void GameScreen::PlayMoveSound()
{
    if (soundsLoaded) {
        PlaySound(moveSound);
    }
}

void GameScreen::PlayPushSound()
{
    if (soundsLoaded) {
        PlaySound(pushSound);
    }
}

void GameScreen::PlayCompleteSound()
{
    if (soundsLoaded) {
        PlaySound(completeSound);
    }
}