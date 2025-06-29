/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Game
*/

#include "Game.hpp"
#include "../screens/MenuScreen.hpp"
#include "../screens/GameScreen.hpp"
#include "../screens/PauseScreen.hpp"
#include <iostream>

Game::Game()
    : currentState(GameState::MENU)
    , shouldClose(false)
    , deltaTime(0.0f)
    , menuScreen(nullptr)
    , gameScreen(nullptr)
    , pauseScreen(nullptr) {
}

Game::~Game() {
    Shutdown();
}

bool Game::Initialize()
{
    std::cout << "Initializing Sokoban 3D..." << std::endl;
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
    if (config.vsync)
        SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(config.windowWidth, config.windowHeight, config.title);
    if (!IsWindowReady()) {
        std::cerr << "Failed to create window!" << std::endl;
        return false;
    }
    InitAudioDevice();
    if (!IsAudioDeviceReady())
        std::cerr << "Warning: Audio device not available!" << std::endl;
    else
        SetMasterVolume(config.masterVolume);
    SetTargetFPS(60);
    try {
        menuScreen = new MenuScreen();
        if (!menuScreen->Initialize()) {
            std::cerr << "Failed to initialize MenuScreen!" << std::endl;
            return false;
        }
        gameScreen = new GameScreen();
        if (!gameScreen->Initialize()) {
            std::cerr << "Failed to initialize GameScreen!" << std::endl;
            return false;
        }
        pauseScreen = new PauseScreen();
        if (!pauseScreen->Initialize()) {
            std::cerr << "Failed to initialize PauseScreen!" << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to create screens: " << e.what() << std::endl;
        return false;
    }
    if (menuScreen)
        menuScreen->PlayBackgroundMusic();
    std::cout << "Game initialized successfully!" << std::endl;
    return true;
}

void Game::Run()
{
    while (!WindowShouldClose() && !shouldClose) {
        deltaTime = GetFrameTime();
        UpdateCurrentScreen();
        HandleStateTransitions();
        BeginDrawing();
        ClearBackground(DARKBLUE);
        DrawCurrentScreen();
        DrawFPS(10, 10);
        EndDrawing();
    }
}

void Game::Shutdown()
{
    std::cout << "Shutting down game..." << std::endl;
    CleanupScreens();
    if (IsAudioDeviceReady())
        CloseAudioDevice();
    if (IsWindowReady())
        CloseWindow();
    std::cout << "Game shutdown complete." << std::endl;
}

void Game::CleanupScreens()
{
    if (menuScreen) {
        menuScreen->Shutdown();
        delete menuScreen;
        menuScreen = nullptr;
    }
    if (gameScreen) {
        gameScreen->Shutdown();
        delete gameScreen;
        gameScreen = nullptr;
    }
    if (pauseScreen) {
        pauseScreen->Shutdown();
        delete pauseScreen;
        pauseScreen = nullptr;
    }
}

void Game::ChangeState(GameState newState)
{
    if (currentState != newState) {
        GameState previousState = currentState;
        currentState = newState;
        switch (newState) {
            case GameState::MENU:
                if (menuScreen) {
                    menuScreen->Reset();
                    menuScreen->PlayBackgroundMusic();
                }
                break;
            case GameState::PLAYING:
                if (menuScreen)
                    menuScreen->StopBackgroundMusic();
                if (previousState == GameState::MENU && gameScreen)
                    gameScreen->SetCurrentLevel(0);
                break;
            case GameState::PAUSED:
                if (pauseScreen)
                    pauseScreen->Show();
                break;
            default:
                break;
        }
        if (previousState == GameState::PAUSED && pauseScreen)
            pauseScreen->Hide();
    }
}

void Game::UpdateCurrentScreen()
{
    switch (currentState) {
        case GameState::MENU:
            if (menuScreen)
                menuScreen->Update(deltaTime);
            break;
        case GameState::PLAYING:
            if (gameScreen)
                gameScreen->Update(deltaTime);
            break;
        case GameState::PAUSED:
            if (gameScreen) {
            }
            if (pauseScreen)
                pauseScreen->Update(deltaTime);
            break;
        case GameState::LEVEL_COMPLETE:
            if (gameScreen)
                gameScreen->Update(deltaTime);
            break;
        case GameState::GAME_OVER:
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
                ChangeState(GameState::MENU);
            break;
    }
}

void Game::DrawCurrentScreen()
{
    switch (currentState) {
        case GameState::MENU:
            if (menuScreen)
                menuScreen->Draw();
            break;
        case GameState::PLAYING:
            if (gameScreen)
                gameScreen->Draw();
            break;
        case GameState::PAUSED:
            if (gameScreen)
                gameScreen->Draw();
            if (pauseScreen)
                pauseScreen->Draw();
            break;
        case GameState::LEVEL_COMPLETE:
            if (gameScreen)
                gameScreen->Draw();
            break;
        case GameState::GAME_OVER:
            DrawText("GAME OVER", GetScreenWidth()/2 - 200, GetScreenHeight()/2 - 50, 60, RED);
            DrawText("Press ENTER to return to menu", GetScreenWidth()/2 - 180, GetScreenHeight()/2 + 20, 20, LIGHTGRAY);
            break;
    }
}

void Game::HandleStateTransitions()
{
    switch (currentState) {
        case GameState::MENU:
            if (menuScreen) {
                if (menuScreen->IsOptionSelected()) {
                    MenuOption option = menuScreen->GetSelectedOption();
                    switch (option) {
                        case MenuOption::START_GAME:
                            ChangeState(GameState::PLAYING);
                            break;
                        case MenuOption::SELECT_LEVEL:
                            // TODO: Implement level selection
                            std::cout << "Level selection not implemented yet" << std::endl;
                            break;
                        case MenuOption::OPTIONS:
                            // TODO: Implement options menu
                            std::cout << "Options menu not implemented yet" << std::endl;
                            break;
                        case MenuOption::QUIT:
                            RequestClose();
                            break;
                    }
                }
            }
            break;

        case GameState::PLAYING:
            if (gameScreen) {
                if (gameScreen->ShouldShowPause()) {
                    ChangeState(GameState::PAUSED);
                }
                else if (gameScreen->IsLevelComplete()) {
                    ChangeState(GameState::LEVEL_COMPLETE);
                }
                else if (gameScreen->ShouldReturnToMenu()) {
                    ChangeState(GameState::MENU);
                }
            }
            break;

        case GameState::PAUSED:
            if (pauseScreen && pauseScreen->IsOptionSelected()) {
                PauseOption option = pauseScreen->GetSelectedOption();
                switch (option) {
                    case PauseOption::RESUME:
                        ChangeState(GameState::PLAYING);
                        break;
                    case PauseOption::RESTART_LEVEL:
                        if (gameScreen) {
                            gameScreen->RestartLevel();
                        }
                        ChangeState(GameState::PLAYING);
                        break;
                    case PauseOption::MAIN_MENU:
                        ChangeState(GameState::MENU);
                        break;
                }
            }
            break;

        case GameState::LEVEL_COMPLETE:
            if (gameScreen) {
                if (gameScreen->GetState() == GameScreenState::PLAYING) {
                    ChangeState(GameState::PLAYING);
                }
                else if (gameScreen->ShouldReturnToMenu()) {
                    ChangeState(GameState::MENU);
                }
            }
            break;

        case GameState::GAME_OVER:
            // Handled in UpdateCurrentScreen
            break;
    }
}