/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Level
*/

#include "Level.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

class SimpleJSONParser {
public:
    static std::string extractString(const std::string &json, const std::string &key)
    {
        std::string searchKey = "\"" + key + "\"";
        size_t keyPos = json.find(searchKey);
        if (keyPos == std::string::npos)
            return "";
        size_t colonPos = json.find(":", keyPos);
        if (colonPos == std::string::npos)
            return "";
        size_t startQuote = json.find("\"", colonPos);
        if (startQuote == std::string::npos)
            return "";
        size_t endQuote = json.find("\"", startQuote + 1);
        if (endQuote == std::string::npos)
            return "";
        return json.substr(startQuote + 1, endQuote - startQuote - 1);
    }

    static int extractInt(const std::string &json, const std::string &key)
    {
        std::string searchKey = "\"" + key + "\"";
        size_t keyPos = json.find(searchKey);
        if (keyPos == std::string::npos)
            return 0;
        size_t colonPos = json.find(":", keyPos);
        if (colonPos == std::string::npos)
            return 0;
        size_t numStart = colonPos + 1;
        while (numStart < json.length() && std::isspace(json[numStart])) numStart++;
        size_t numEnd = numStart;
        while (numEnd < json.length() && (std::isdigit(json[numEnd]) || json[numEnd] == '-')) numEnd++;
        if (numEnd <= numStart)
            return 0;
        std::string numStr = json.substr(numStart, numEnd - numStart);
        return std::stoi(numStr);
    }

    static Vector2 extractVector2(const std::string &json, const std::string &key)
    {
        std::string searchKey = "\"" + key + "\"";
        size_t keyPos = json.find(searchKey);
        if (keyPos == std::string::npos)
            return {0, 0};
        size_t colonPos = json.find(":", keyPos);
        size_t braceStart = json.find("{", colonPos);
        size_t braceEnd = json.find("}", braceStart);
        if (braceStart == std::string::npos || braceEnd == std::string::npos)
            return {0, 0};
        std::string objStr = json.substr(braceStart, braceEnd - braceStart + 1);
        int x = extractInt(objStr, "x");
        int y = extractInt(objStr, "y");
        return {(float)x, (float)y};
    }

    static std::vector<std::string> extractStringArray(const std::string &json, const std::string &key)
    {
        std::vector<std::string> result;
        std::string searchKey = "\"" + key + "\"";
        size_t keyPos = json.find(searchKey);
        if (keyPos == std::string::npos)
            return result;
        size_t colonPos = json.find(":", keyPos);
        size_t arrayStart = json.find("[", colonPos);
        size_t arrayEnd = json.find("]", arrayStart);
        if (arrayStart == std::string::npos || arrayEnd == std::string::npos)
            return result;
        std::string arrayContent = json.substr(arrayStart + 1, arrayEnd - arrayStart - 1);
        size_t pos = 0;
        while (pos < arrayContent.length()) {
            size_t quoteStart = arrayContent.find("\"", pos);
            if (quoteStart == std::string::npos)
                break;
            size_t quoteEnd = arrayContent.find("\"", quoteStart + 1);
            if (quoteEnd == std::string::npos)
                break;
            std::string str = arrayContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
            result.push_back(str);
            pos = quoteEnd + 1;
        }
        return result;
    }
};

Level::Level()
    : isCompleted(false) {
    data.width = 0;
    data.height = 0;
    data.totalBoxes = 0;
    data.boxesOnTargets = 0;
    data.playerStart = {0, 0};
}

Level::~Level() {}

bool Level::LoadFromFile(const std::string &filename)
{
    std::cout << "Loading level from file: " << filename << std::endl;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open level file: " << filename << std::endl;
        return false;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonContent = buffer.str();
    file.close();
    return LoadFromJSON(jsonContent);
}

bool Level::LoadFromJSON(const std::string &jsonContent)
{
    try {
        data.name = SimpleJSONParser::extractString(jsonContent, "name");
        data.width = SimpleJSONParser::extractInt(jsonContent, "width");
        data.height = SimpleJSONParser::extractInt(jsonContent, "height");
        data.playerStart = SimpleJSONParser::extractVector2(jsonContent, "playerStart");
        std::vector<std::string> gridStrings = SimpleJSONParser::extractStringArray(jsonContent, "grid");
        if (data.width <= 0 || data.height <= 0) {
            std::cerr << "Error: Invalid level dimensions" << std::endl;
            return false;
        }
        if (gridStrings.size() != static_cast<size_t>(data.height)) {
            std::cerr << "Error: Grid height mismatch" << std::endl;
            return false;
        }
        InitializeGrid();
        ParseGridFromStrings(gridStrings);
        CountBoxesAndTargets();
        std::cout << "Level loaded successfully: " << data.name << std::endl;
        std::cout << "Dimensions: " << data.width << "x" << data.height << std::endl;
        std::cout << "Total boxes: " << data.totalBoxes << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error parsing JSON: " << e.what() << std::endl;
        return false;
    }
}

void Level::InitializeGrid()
{
    data.grid.clear();
    data.grid.resize(data.height);
    for (int y = 0; y < data.height; ++y) {
        data.grid[y].resize(data.width);
        for (int x = 0; x < data.width; ++x) {
            data.grid[y][x].type = TileType::FLOOR;
            data.grid[y][x].position = GridToWorld(x, y);
            data.grid[y][x].hasBox = false;
            data.grid[y][x].isTarget = false;
        }
    }
}

void Level::ParseGridFromStrings(const std::vector<std::string> &gridStrings)
{
    boxPositions.clear();
    targetPositions.clear();
    for (int y = 0; y < data.height && y < static_cast<int>(gridStrings.size()); ++y) {
        const std::string& row = gridStrings[y];
        for (int x = 0; x < data.width && x < static_cast<int>(row.length()); ++x) {
            char c = row[x];
            switch (c) {
                case '#': // Wall
                    data.grid[y][x].type = TileType::WALL;
                    break;
                case ' ': // Floor
                    data.grid[y][x].type = TileType::FLOOR;
                    break;
                case '@': // Player start
                    data.grid[y][x].type = TileType::PLAYER_START;
                    data.playerStart = {(float)x, (float)y};
                    break;
                case '$': // Box
                    data.grid[y][x].type = TileType::FLOOR;
                    data.grid[y][x].hasBox = true;
                    boxPositions.push_back({(float)x, (float)y});
                    break;
                case '.': // Target
                    data.grid[y][x].type = TileType::TARGET;
                    data.grid[y][x].isTarget = true;
                    targetPositions.push_back({(float)x, (float)y});
                    break;
                case '*': // Box on target
                    data.grid[y][x].type = TileType::TARGET;
                    data.grid[y][x].hasBox = true;
                    data.grid[y][x].isTarget = true;
                    boxPositions.push_back({(float)x, (float)y});
                    targetPositions.push_back({(float)x, (float)y});
                    break;
                case '+': // Player on target
                    data.grid[y][x].type = TileType::TARGET;
                    data.grid[y][x].isTarget = true;
                    data.playerStart = {(float)x, (float)y};
                    targetPositions.push_back({(float)x, (float)y});
                    break;
                default:
                    data.grid[y][x].type = TileType::FLOOR;
                    break;
            }
        }
    }
}

void Level::CountBoxesAndTargets()
{
    data.totalBoxes = static_cast<int>(boxPositions.size());
    data.boxesOnTargets = 0;
    for (const auto &boxPos : boxPositions) {
        if (IsTarget((int)boxPos.x, (int)boxPos.y)) {
            data.boxesOnTargets++;
        }
    }
    isCompleted = (data.boxesOnTargets == data.totalBoxes && data.totalBoxes > 0);
}

void Level::Reset()
{
    std::cout << "Resetting level..." << std::endl;
    for (int y = 0; y < data.height; ++y) {
        for (int x = 0; x < data.width; ++x) {
            data.grid[y][x].hasBox = false;
        }
    }
    for (const auto &boxPos : boxPositions) {
        int x = (int)boxPos.x;
        int y = (int)boxPos.y;
        if (IsValidPosition(x, y)) {
            data.grid[y][x].hasBox = true;
        }
    }
    CountBoxesAndTargets();
}

TileType Level::GetTileType(int x, int y) const
{
    if (!IsValidPosition(x, y)) {
        return TileType::WALL;
    }
    return data.grid[y][x].type;
}

bool Level::CanMoveToTile(int x, int y) const
{
    if (!IsValidPosition(x, y)) {
        return false;
    }
    TileType type = data.grid[y][x].type;
    return (type == TileType::FLOOR ||
            type == TileType::TARGET ||
            type == TileType::PLAYER_START) &&
           !data.grid[y][x].hasBox;
}

bool Level::HasBox(int x, int y) const
{
    if (!IsValidPosition(x, y)) {
        return false;
    }
    return data.grid[y][x].hasBox;
}

bool Level::IsTarget(int x, int y) const
{
    if (!IsValidPosition(x, y)) {
        return false;
    }
    return data.grid[y][x].isTarget;
}

bool Level::IsValidPosition(int x, int y) const
{
    return x >= 0 && x < data.width && y >= 0 && y < data.height;
}

void Level::MoveBox(int fromX, int fromY, int toX, int toY)
{
    if (!IsValidPosition(fromX, fromY) || !IsValidPosition(toX, toY)) {
        std::cerr << "Error: Invalid position for box move" << std::endl;
        return;
    }
    if (!HasBox(fromX, fromY)) {
        std::cerr << "Error: No box at source position" << std::endl;
        return;
    }
    if (HasBox(toX, toY) || GetTileType(toX, toY) == TileType::WALL) {
        std::cerr << "Error: Cannot move box to occupied/wall position" << std::endl;
        return;
    }
    data.grid[fromY][fromX].hasBox = false;
    data.grid[toY][toX].hasBox = true;
    std::cout << "Box moved from (" << fromX << "," << fromY << ") to (" << toX << "," << toY << ")" << std::endl;
    CheckCompletion();
}

void Level::CheckCompletion()
{
    int oldBoxesOnTargets = data.boxesOnTargets;
    data.boxesOnTargets = 0;
    for (int y = 0; y < data.height; ++y) {
        for (int x = 0; x < data.width; ++x) {
            if (data.grid[y][x].hasBox && data.grid[y][x].isTarget) {
                data.boxesOnTargets++;
            }
        }
    }
    bool wasCompleted = isCompleted;
    isCompleted = (data.boxesOnTargets == data.totalBoxes && data.totalBoxes > 0);
    if (data.boxesOnTargets != oldBoxesOnTargets) {
        std::cout << "Boxes on targets: " << data.boxesOnTargets << "/" << data.totalBoxes << std::endl;
    }
    if (isCompleted && !wasCompleted) {
        std::cout << "🎉 Level completed! 🎉" << std::endl;
    }
}

void Level::PlaceBox(int x, int y)
{
    if (IsValidPosition(x, y) && !HasBox(x, y)) {
        data.grid[y][x].hasBox = true;
        CheckCompletion();
    }
}

void Level::RemoveBox(int x, int y)
{
    if (IsValidPosition(x, y) && HasBox(x, y)) {
        data.grid[y][x].hasBox = false;
        CheckCompletion();
    }
}

Vector3 Level::GridToWorld(int x, int y) const
{
    float worldX = (float)x;
    float worldZ = (float)y;
    Vector3 result = {worldX, 0.0f, worldZ};
    return result;
}

Vector2 Level::WorldToGrid(Vector3 worldPos) const
{
    float gridX = worldPos.x;
    float gridY = worldPos.z;
    return {gridX, gridY};
}

int Level::GetWidth() const
{
    return data.width;
}

int Level::GetHeight() const
{
    return data.height;
}
