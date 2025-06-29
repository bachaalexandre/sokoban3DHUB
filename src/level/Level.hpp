/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Level
*/

#ifndef LEVEL_HPP_
#define LEVEL_HPP_
#pragma once

#include <vector>
#include <string>
#include <raylib.h>

enum class TileType {
    WALL,
    FLOOR,
    TARGET,
    PLAYER_START
};

struct Tile {
    TileType type;
    Vector3 position;
    bool hasBox = false;
    bool isTarget = false;
};

struct LevelData {
    std::string name;
    int width;
    int height;
    Vector2 playerStart;
    std::vector<std::vector<Tile>> grid;
    int totalBoxes;
    int boxesOnTargets;
};

class Level {
private:
    LevelData data;
    bool isCompleted;
    std::vector<Vector2> boxPositions;
    std::vector<Vector2> targetPositions;

    void InitializeGrid();
    void ParseGridFromStrings(const std::vector<std::string> &gridStrings);
    void CountBoxesAndTargets();

public:
    Level();
    ~Level();

    bool LoadFromFile(const std::string &filename);
    bool LoadFromJSON(const std::string &jsonContent);
    void Reset();

    const LevelData& GetData() const { return data; }
    bool IsCompleted() const { return isCompleted; }
    TileType GetTileType(int x, int y) const;
    bool CanMoveToTile(int x, int y) const;
    bool HasBox(int x, int y) const;
    bool IsTarget(int x, int y) const;
    bool IsValidPosition(int x, int y) const;
    void MoveBox(int fromX, int fromY, int toX, int toY);
    void CheckCompletion();
    void PlaceBox(int x, int y);
    void RemoveBox(int x, int y);
    Vector3 GridToWorld(int x, int y) const;
    Vector2 WorldToGrid(Vector3 worldPos) const;
    int GetWidth() const;
    int GetHeight() const;
};

#endif /* !LEVEL_HPP_ */