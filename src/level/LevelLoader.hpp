/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** LevelLoader
*/

#ifndef LEVELLOADER_HPP_
#define LEVELLOADER_HPP_
#pragma once

#include "Level.hpp"
#include <vector>
#include <string>

class LevelLoader {
private:
    static std::vector<std::string> levelFiles;
    static int currentLevelIndex;

public:
    static bool LoadLevelList(const std::string &levelDirectory = "assets/levels/");
    static bool LoadLevel(Level &level, int levelIndex);
    static bool LoadNextLevel(Level &level);
    static bool LoadPreviousLevel(Level &level);
    static bool ReloadCurrentLevel(Level &level);
    static int GetCurrentLevelIndex() { return currentLevelIndex; }
    static int GetTotalLevels() { return static_cast<int>(levelFiles.size()); }
    static std::string GetCurrentLevelName();
    static std::string GetLevelFileName(int index);
    static bool HasNextLevel();
    static bool HasPreviousLevel();
    static void SetCurrentLevel(int index);
    static Level CreateTestLevel();
    static std::string CreateTestLevelJSON();
    static void CreateTestLevelFiles(const std::string &directory = "assets/levels/");
};

#endif /* !LEVELLOADER_HPP_ */