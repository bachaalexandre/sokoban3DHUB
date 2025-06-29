/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Main entry point for Sokoban 3D game
*/

#include <iostream>
#include <raylib.h>
#include "core/Game.hpp"

void SetupGameConfig(GameConfig& config)
{
    config.windowWidth = 1200;
    config.windowHeight = 800;
    config.title = "Sokoban 3D - EPITECH Project 2025";
    config.targetFPS = 60;
    config.vsync = true;
    config.fullscreen = false;
    config.showFPS = true;
    config.enableMSAA = true;
    config.masterVolume = 0.7f;
    config.musicVolume = 0.5f;
    config.sfxVolume = 0.8f;
    config.enableAnimations = true;
    config.animationSpeed = 1.0f;
    config.showGrid = false;
    config.inputDelay = 0.15f;
    config.allowDiagonalMovement = false;
}
void ProcessCommandLineArgs(int argc, char* argv[], GameConfig& config)
{
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--fullscreen" || arg == "-f") {
            config.fullscreen = true;
            std::cout << "Fullscreen mode enabled" << std::endl;
        }
        else if (arg == "--no-vsync") {
            config.vsync = false;
            config.enableVSync = false;
            std::cout << "VSync disabled" << std::endl;
        }
        else if (arg == "--no-msaa") {
            config.enableMSAA = false;
            std::cout << "MSAA disabled" << std::endl;
        }
        else if (arg == "--show-fps") {
            config.showFPS = true;
            std::cout << "FPS display enabled" << std::endl;
        }
        else if (arg == "--no-animations") {
            config.enableAnimations = false;
            std::cout << "Animations disabled" << std::endl;
        }
        else if (arg == "--help" || arg == "-h") {
            std::cout << "Sokoban 3D - Usage:" << std::endl;
            std::cout << "  --fullscreen, -f     Enable fullscreen mode" << std::endl;
            std::cout << "  --no-vsync           Disable vertical sync" << std::endl;
            std::cout << "  --no-msaa            Disable anti-aliasing" << std::endl;
            std::cout << "  --show-fps           Show FPS counter" << std::endl;
            std::cout << "  --no-animations      Disable animations" << std::endl;
            std::cout << "  --show-grid          Show debug grid" << std::endl;
            std::cout << "  --help, -h           Show this help message" << std::endl;
            exit(0);
        }
        else if (arg.find("--resolution=") == 0) {
            std::string resolution = arg.substr(13);
            size_t xPos = resolution.find('x');
            if (xPos != std::string::npos) {
                try {
                    config.windowWidth = std::stoi(resolution.substr(0, xPos));
                    config.windowHeight = std::stoi(resolution.substr(xPos + 1));
                    std::cout << "Resolution set to " << config.windowWidth << "x" << config.windowHeight << std::endl;
                } catch (const std::exception& e) {
                    std::cerr << "Invalid resolution format. Using default." << std::endl;
                }
            }
        }
        else {
            std::cout << "Unknown argument: " << arg << std::endl;
            std::cout << "Use --help for available options." << std::endl;
        }
    }
}

bool CheckSystemRequirements()
{
    std::cout << "Raylib version: " << RAYLIB_VERSION << std::endl;
    return true;
}

int main(int argc, char* argv[])
{
    GameConfig config;
    SetupGameConfig(config);
    ProcessCommandLineArgs(argc, argv, config);
    if (!CheckSystemRequirements()) {
        std::cerr << "System requirements not met!" << std::endl;
        return 1;
    }
    Game game;
    try {
        if (!game.Initialize()) {
            std::cerr << "Failed to initialize game!" << std::endl;
            return 1;
        }
        game.SetConfig(config);
        std::cout << "Game initialized successfully. Starting main loop..." << std::endl;
        game.Run();
        std::cout << "Game ended normally." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        game.Shutdown();
        return 1;
    } catch (...) {
        std::cerr << "Unknown exception caught!" << std::endl;
        game.Shutdown();
        return 1;
    }
    game.Shutdown();
    std::cout << "Thanks for playing Sokoban 3D!" << std::endl;
    return 0;
}