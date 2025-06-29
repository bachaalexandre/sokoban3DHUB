/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Box
*/

#include "Box.hpp"
#include "../level/Level.hpp"
#include "../graphics/AnimationManager.hpp"
#include <raymath.h>
#include <iostream>
#include <cmath>

Box::Box()
    : position({0, 0, 0})
    , targetPosition({0, 0, 0})
    , gridPosition({0, 0})
    , state(BoxState::NORMAL)
    , isMoving(false)
    , moveSpeed(2.5f)
    , animationTime(0.0f)
    , animationDuration(0.3f)
    , animationStartPos({0, 0, 0})
    , animationEndPos({0, 0, 0})
    , glowIntensity(0.0f)
    , bounceOffset(0.0f)
    , scaleMultiplier(1.0f)
    , currentTint(WHITE)
    , currentLevel(nullptr)
    , animationManager(nullptr)
{
}

Box::~Box() {}

void Box::Initialize(Vector3 startPos)
{
    position = startPos;
    targetPosition = startPos;
    gridPosition = GetGridPositionFromWorld(startPos);
    state = BoxState::NORMAL;
    isMoving = false;
    glowIntensity = 0.0f;
    bounceOffset = 0.0f;
    scaleMultiplier = 1.0f;
    UpdateTint();
}

void Box::Initialize(Vector2 gridPos, Level *level)
{
    currentLevel = level;
    gridPosition = gridPos;
    position = GetWorldPositionFromGrid(gridPos);
    targetPosition = position;
    state = BoxState::NORMAL;
    isMoving = false;
    glowIntensity = 0.0f;
    bounceOffset = 0.0f;
    scaleMultiplier = 1.0f;
    CheckTargetStatus();
    UpdateTint();
}

void Box::Update(float deltaTime)
{
    if (isMoving)
        UpdateAnimation(deltaTime);
    UpdateVisualEffects(deltaTime);
    if (!isMoving)
        CheckTargetStatus();
}

void Box::UpdateAnimation(float deltaTime)
{
    if (!isMoving)
        return;
    animationTime += deltaTime;
    float t = animationTime / animationDuration;
    if (t >= 1.0f) {
        t = 1.0f;
        CompleteMovement();
    }
    float easedT = 1.0f - (1.0f - t) * (1.0f - t);
    position = Vector3Lerp(animationStartPos, animationEndPos, easedT);
    float bounceT = sinf(t * PI);
    position.y += bounceT * 0.1f;
}

void Box::UpdateVisualEffects(float deltaTime)
{
    if (state == BoxState::ON_TARGET) {
        glowIntensity = 0.5f + 0.3f * sinf(GetTime() * 3.0f);
        bounceOffset = 0.05f * sinf(GetTime() * 2.0f);
        scaleMultiplier = 1.0f + 0.05f * sinf(GetTime() * 2.5f);
    } else {
        glowIntensity = Lerp(glowIntensity, 0.0f, deltaTime * 3.0f);
        bounceOffset = Lerp(bounceOffset, 0.0f, deltaTime * 4.0f);
        scaleMultiplier = Lerp(scaleMultiplier, 1.0f, deltaTime * 4.0f);
    }
    UpdateTint();
}

void Box::CompleteMovement()
{
    isMoving = false;
    position = animationEndPos;
    targetPosition = animationEndPos;
    animationTime = 0.0f;
    CheckTargetStatus();
    if (state == BoxState::ON_TARGET) {
        StartBounceAnimation();
        StartGlowAnimation();
    }
    if (onMoveComplete)
        onMoveComplete();
}

void Box::CheckTargetStatus()
{
    if (!currentLevel) return;
    bool onTarget = currentLevel->IsTarget((int)gridPosition.x, (int)gridPosition.y);
    BoxState newState = onTarget ? BoxState::ON_TARGET : BoxState::NORMAL;
    if (newState != state) {
        SetState(newState);
        if (newState == BoxState::ON_TARGET)
            OnPlacedOnTarget();
        else
            OnRemovedFromTarget();
    }
}

void Box::SetState(BoxState newState)
{
    if (state != newState) {
        BoxState oldState = state;
        state = newState;
        if (onStateChanged)
            onStateChanged(newState);
        UpdateTint();
    }
}

void Box::UpdateTint()
{
    switch (state) {
        case BoxState::NORMAL:
            currentTint = WHITE;
            break;
        case BoxState::ON_TARGET:
            currentTint = {0, 255, 0, (unsigned char)(200 + 55 * glowIntensity)};
            break;
        case BoxState::MOVING:
            currentTint = LIGHTGRAY;
            break;
        case BoxState::HIGHLIGHTED:
            currentTint = YELLOW;
            break;
        default:
            currentTint = WHITE;
            break;
    }
}

void Box::MoveTo(Vector2 newGridPos)
{
    if (isMoving) {
        std::cout << "Box is already moving, cannot move!" << std::endl;
        return;
    }
    if (!CanMoveTo(newGridPos)) {
        std::cout << "Cannot move to position (" << newGridPos.x << "," << newGridPos.y << ")" << std::endl;
        return;
    }
    Vector3 fromPos = position;
    Vector3 toPos = GetWorldPositionFromGrid(newGridPos);
    gridPosition = newGridPos;
    if (animationDuration > 0.0f) {
        StartMoveAnimation(fromPos, toPos, animationDuration);
        std::cout << "Started move animation" << std::endl;
    } else {
        position = toPos;
        targetPosition = toPos;
        CheckTargetStatus();
        std::cout << "Instant movement completed" << std::endl;
    }
}

void Box::MoveToWorld(Vector3 newWorldPos)
{
    Vector2 newGridPos = GetGridPositionFromWorld(newWorldPos);
    MoveTo(newGridPos);
}

void Box::SetPosition(Vector3 pos)
{
    position = pos;
    targetPosition = pos;
    gridPosition = GetGridPositionFromWorld(pos);
    CheckTargetStatus();
}

void Box::SetPositionImmediate(Vector2 newGridPos)
{
    gridPosition = newGridPos;
    position = GetWorldPositionFromGrid(newGridPos);
    targetPosition = position;
    isMoving = false;
    animationTime = 0.0f;
    CheckTargetStatus();
    UpdateTint();
}

void Box::SetGridPosition(Vector2 gridPos)
{
    gridPosition = gridPos;
    position = GetWorldPositionFromGrid(gridPos);
    targetPosition = position;
    CheckTargetStatus();
}

void Box::ForcePosition(Vector3 worldPos, Vector2 gridPos)
{
    position = worldPos;
    targetPosition = worldPos;
    gridPosition = gridPos;
    isMoving = false;
    CheckTargetStatus();
}
void Box::StartMoveAnimation(Vector3 from, Vector3 to, float duration)
{
    animationStartPos = from;
    animationEndPos = to;
    animationTime = 0.0f;
    animationDuration = duration;
    isMoving = true;
    SetState(BoxState::MOVING);
}

void Box::StartBounceAnimation(float intensity)
{
    bounceOffset = intensity;
}

void Box::StartGlowAnimation()
{
    glowIntensity = 1.0f;
}

void Box::StopAnimation()
{
    isMoving = false;
    animationTime = 0.0f;
}

bool Box::CanBePushed() const
{
    return !isMoving;
}

bool Box::CanMoveTo(Vector2 gridPos) const
{
    if (!currentLevel)
        return false;
    int x = static_cast<int>(gridPos.x);
    int y = static_cast<int>(gridPos.y);
    bool validPos = currentLevel->IsValidPosition(x, y);
    bool canMove = currentLevel->CanMoveToTile(x, y);
    bool hasBox = currentLevel->HasBox(x, y);
    return validPos && canMove && !hasBox;
}

bool Box::OnPushed(Vector2 direction)
{
    Vector2 newGridPos = {
        gridPosition.x + direction.x,
        gridPosition.y + direction.y
    };
    if (!currentLevel) {
        std::cout << "No level reference!" << std::endl;
        return false;
    }
    int newX = static_cast<int>(newGridPos.x);
    int newY = static_cast<int>(newGridPos.y);
    if (!currentLevel->IsValidPosition(newX, newY)) {
        std::cout << "New position out of bounds!" << std::endl;
        return false;
    }
    if (currentLevel->GetTileType(newX, newY) == TileType::WALL) {
        std::cout << "Cannot push box into wall!" << std::endl;
        return false;
    }
    if (currentLevel->HasBox(newX, newY)) {
        std::cout << "Another box already at destination!" << std::endl;
        return false;
    }
    currentLevel->RemoveBox(static_cast<int>(gridPosition.x), static_cast<int>(gridPosition.y));
    Vector3 oldPos = position;
    Vector3 newWorldPos = currentLevel->GridToWorld(newX, newY);
    gridPosition = newGridPos;
    position = newWorldPos;
    targetPosition = newWorldPos;
    currentLevel->PlaceBox(newX, newY);
    if (animationDuration > 0.0f)
        StartMoveAnimation(oldPos, newWorldPos, animationDuration);
    CheckTargetStatus();
    return true;
}

void Box::OnPlacedOnTarget()
{
    // Placeholder pour effet ou score ou autre
}

void Box::OnRemovedFromTarget()
{
    // Placeholder pour effet ou retrait de score
}

Vector3 Box::GetRenderPosition() const
{
    return { position.x, position.y + bounceOffset, position.z };
}

Vector3 Box::GetRenderScale() const
{
    return Vector3Scale(Vector3{1.0f, 1.0f, 1.0f}, scaleMultiplier);
}

Vector3 Box::GetWorldPositionFromGrid(Vector2 gridPos) const
{
    if (currentLevel)
        return currentLevel->GridToWorld(static_cast<int>(gridPos.x), static_cast<int>(gridPos.y));
    float tileSize = 1.0f;
    return { gridPos.x * tileSize, 0.0f, gridPos.y * tileSize };
}

Vector2 Box::GetGridPositionFromWorld(Vector3 worldPos) const
{
    if (currentLevel)
        return currentLevel->WorldToGrid(worldPos);
    float tileSize = 1.0f;
    return { (float)(int)(worldPos.x / tileSize), (float)(int)(worldPos.z / tileSize) };
}

float Box::GetDistanceToTarget() const
{
    return Vector3Distance(position, targetPosition);
}

Box::BoxSaveData Box::GetSaveData() const
{
    return { gridPosition, state };
}

void Box::LoadSaveData(const BoxSaveData& data)
{
    gridPosition = data.gridPosition;
    state = data.state;
    position = GetWorldPositionFromGrid(gridPosition);
    targetPosition = position;
    isMoving = false;
    UpdateTint();
}

void Box::SetTargetPosition(Vector3 newTargetPos)
{
    targetPosition = newTargetPos;
}