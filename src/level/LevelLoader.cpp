/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** LevelLoader
*/

#include "LevelLoader.hpp"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <sstream>

std::vector<std::string> LevelLoader::levelFiles;
int LevelLoader::currentLevelIndex = 0;

bool LevelLoader::LoadLevelList(const std::string &levelDirectory)
{
    levelFiles.clear();
    currentLevelIndex = 0;
    std::cout << "Loading level list from: " << levelDirectory << std::endl;
    try {
        if (!std::filesystem::exists(levelDirectory)) {
            std::cout << "Level directory doesn't exist, creating test levels..." << std::endl;
            CreateTestLevelFiles(levelDirectory);
        }
        for (const auto& entry : std::filesystem::directory_iterator(levelDirectory)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                levelFiles.push_back(entry.path().string());
            }
        }
        std::sort(levelFiles.begin(), levelFiles.end());
        std::cout << "Found " << levelFiles.size() << " level files:" << std::endl;
        for (size_t i = 0; i < levelFiles.size(); ++i) {
            std::cout << "  " << i << ": " << levelFiles[i] << std::endl;
        }
        return !levelFiles.empty();
    } catch (const std::exception& e) {
        std::cerr << "Error loading level list: " << e.what() << std::endl;
        std::cout << "Creating fallback test levels..." << std::endl;
        CreateTestLevelFiles(levelDirectory);
        try {
            for (const auto& entry : std::filesystem::directory_iterator(levelDirectory)) {
                if (entry.is_regular_file() && entry.path().extension() == ".json") {
                    levelFiles.push_back(entry.path().string());
                }
            }
            std::sort(levelFiles.begin(), levelFiles.end());
        } catch (...) {
            return false;
        }
        return !levelFiles.empty();
    }
}

bool LevelLoader::LoadLevel(Level &level, int levelIndex)
{
    if (levelIndex < 0 || levelIndex >= static_cast<int>(levelFiles.size())) {
        std::cerr << "Error: Level index " << levelIndex << " out of range (0-"
                  << (levelFiles.size() - 1) << ")" << std::endl;
        return false;
    }
    currentLevelIndex = levelIndex;
    std::string filename = levelFiles[levelIndex];
    std::cout << "Loading level " << levelIndex << ": " << filename << std::endl;
    if (level.LoadFromFile(filename)) {
        std::cout << "Successfully loaded level: " << level.GetData().name << std::endl;
        return true;
    } else {
        std::cerr << "Failed to load level: " << filename << std::endl;
        return false;
    }
}

bool LevelLoader::LoadNextLevel(Level &level)
{
    if (!HasNextLevel()) {
        std::cout << "No next level available" << std::endl;
        return false;
    }
    return LoadLevel(level, currentLevelIndex + 1);
}

bool LevelLoader::LoadPreviousLevel(Level &level)
{
    if (!HasPreviousLevel()) {
        std::cout << "No previous level available" << std::endl;
        return false;
    }
    return LoadLevel(level, currentLevelIndex - 1);
}

bool LevelLoader::ReloadCurrentLevel(Level &level)
{
    if (levelFiles.empty()) {
        std::cout << "No levels loaded" << std::endl;
        return false;
    }
    return LoadLevel(level, currentLevelIndex);
}

std::string LevelLoader::GetCurrentLevelName()
{
    if (currentLevelIndex < 0 || currentLevelIndex >= static_cast<int>(levelFiles.size())) {
        return "Unknown";
    }
    std::string filename = levelFiles[currentLevelIndex];
    size_t lastSlash = filename.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        filename = filename.substr(lastSlash + 1);
    }
    size_t lastDot = filename.find_last_of(".");
    if (lastDot != std::string::npos) {
        filename = filename.substr(0, lastDot);
    }
    return filename;
}

std::string LevelLoader::GetLevelFileName(int index)
{
    if (index < 0 || index >= static_cast<int>(levelFiles.size())) {
        return "";
    }
    return levelFiles[index];
}

bool LevelLoader::HasNextLevel()
{
    return currentLevelIndex < static_cast<int>(levelFiles.size()) - 1;
}

bool LevelLoader::HasPreviousLevel()
{
    return currentLevelIndex > 0;
}

void LevelLoader::SetCurrentLevel(int index)
{
    if (index >= 0 && index < static_cast<int>(levelFiles.size())) {
        currentLevelIndex = index;
    }
}

Level LevelLoader::CreateTestLevel()
{
    Level level;
    std::string testJSON = CreateTestLevelJSON();
    if (!level.LoadFromJSON(testJSON)) {
        std::cerr << "Failed to create test level!" << std::endl;
    }
    return level;
}

std::string LevelLoader::CreateTestLevelJSON()
{
    return R"({
    "name": "Test Level 1",
    "width": 8,
    "height": 6,
    "playerStart": {"x": 1, "y": 1},
    "grid": [
        "########",
        "#@  .  #",
        "#  $   #",
        "#      #",
        "#   .$ #",
        "########"
    ],
    "legend": {
        "#": "wall",
        " ": "floor",
        "@": "player",
        "$": "box",
        ".": "target"
    }
})";
}

void LevelLoader::CreateTestLevelFiles(const std::string& directory) {
    try {
        std::filesystem::create_directories(directory);
        std::vector<std::string> testLevels = {
            // Level 1 - Basic tutorial
            R"({
    "name": "Tutorial",
    "width": 6,
    "height": 5,
    "playerStart": {"x": 1, "y": 2},
    "grid": [
        "######",
        "#    #",
        "#@$ .#",
        "#    #",
        "######"
    ],
    "legend": {
        "#": "wall",
        " ": "floor",
        "@": "player",
        "$": "box",
        ".": "target"
    }
})",

            // Level 2 - Two boxes
            R"({
    "name": "Double Push",
    "width": 8,
    "height": 6,
    "playerStart": {"x": 1, "y": 2},
    "grid": [
        "########",
        "#      #",
        "#@$$..##",
        "#      #",
        "#      #",
        "########"
    ],
    "legend": {
        "#": "wall",
        " ": "floor",
        "@": "player",
        "$": "box",
        ".": "target"
    }
})",

            // Level 3 - L-Shape challenge
            R"({
    "name": "L-Shape",
    "width": 7,
    "height": 7,
    "playerStart": {"x": 1, "y": 1},
    "grid": [
        "#######",
        "#@    #",
        "#  $  #",
        "#     #",
        "#  $  #",
        "#  .. #",
        "#######"
    ],
    "legend": {
        "#": "wall",
        " ": "floor",
        "@": "player",
        "$": "box",
        ".": "target"
    }
})",

            // Level 4 - Corner puzzle
            R"({
    "name": "Corner Trap",
    "width": 8,
    "height": 8,
    "playerStart": {"x": 6, "y": 6},
    "grid": [
        "########",
        "#.     #",
        "#      #",
        "#   $  #",
        "#      #",
        "#  $   #",
        "#     @#",
        "########"
    ],
    "legend": {
        "#": "wall",
        " ": "floor",
        "@": "player",
        "$": "box",
        ".": "target"
    }
})",

            // Level 5 - Advanced puzzle
            R"({
    "name": "Advanced Puzzle",
    "width": 10,
    "height": 8,
    "playerStart": {"x": 1, "y": 1},
    "grid": [
        "##########",
        "#@       #",
        "#  ###   #",
        "#  #.#   #",
        "#  #$#  ##",
        "#  #.# $ #",
        "#    #   #",
        "##########"
    ],
    "legend": {
        "#": "wall",
        " ": "floor",
        "@": "player",
        "$": "box",
        ".": "target"
    }
})"
        };
        for (size_t i = 0; i < testLevels.size(); ++i) {
            std::string filename = directory + "/level" + std::to_string(i + 1) + ".json";
            std::ofstream file(filename);
            if (file.is_open()) {
                file << testLevels[i];
                file.close();
                std::cout << "Created test level: " << filename << std::endl;
            } else {
                std::cerr << "Failed to create test level: " << filename << std::endl;
            }
        }
        std::cout << "Created " << testLevels.size() << " test level files in " << directory << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error creating test level files: " << e.what() << std::endl;
    }
}