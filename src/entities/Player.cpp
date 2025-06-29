/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Player
*/

#include "Player.hpp"
#include "../level/Level.hpp"
#include "../graphics/AnimationManager.hpp"
#include <raymath.h>
#include <iostream>
#include <cmath>

Player::Player()
    : position({0, 0, 0})
    , targetPosition({0, 0, 0})
    , gridPosition({0, 0})
    , rotation(0.0f)
    , state(PlayerState::IDLE)
    , moveSpeed(3.0f)
    , isMoving(false)
    , animationTime(0.0f)
    , animationDuration(0.3f)
    , animationStartPos({0, 0, 0})
    , animationEndPos({0, 0, 0})
    , currentLevel(nullptr)
    , animationManager(nullptr)
{
}

Player::~Player() {}

void Player::Initialize(Vector3 startPos)
{
    position = startPos;
    targetPosition = startPos;
    gridPosition = GetGridPositionFromWorld(startPos);
    rotation = 0.0f;
    state = PlayerState::IDLE;
    isMoving = false;
    animationTime = 0.0f;
}

void Player::Update(float deltaTime)
{
    if (!isMoving)
        ProcessInput();
    if (isMoving)
        UpdateAnimation(deltaTime);
    if (!isMoving && state != PlayerState::IDLE)
        state = PlayerState::IDLE;
}

void Player::ProcessInput()
{
    Vector2 direction;
    if (IsInputPressed(direction))
        TryMove(direction);
}

bool Player::IsInputPressed(Vector2 &outDirection)
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        outDirection = {0, -1};
        return true;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        outDirection = {0, 1};
        return true;
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
        outDirection = {-1, 0};
        return true;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
        outDirection = {1, 0};
        return true;
    }
    return false;
}

bool Player::TryMove(Vector2 direction)
{
    if (isMoving || !currentLevel)
        return false;
    Vector2 newGridPos = {gridPosition.x + direction.x, gridPosition.y + direction.y};
    if (!currentLevel->IsValidPosition((int)newGridPos.x, (int)newGridPos.y))
        return false;
    if (!currentLevel->CanMoveToTile((int)newGridPos.x, (int)newGridPos.y))
        return false;
    if (currentLevel->HasBox((int)newGridPos.x, (int)newGridPos.y)) {
        Vector2 boxNewPos = {newGridPos.x + direction.x, newGridPos.y + direction.y};
        if (!currentLevel->IsValidPosition((int)boxNewPos.x, (int)boxNewPos.y) ||
            !currentLevel->CanMoveToTile((int)boxNewPos.x, (int)boxNewPos.y) ||
            currentLevel->HasBox((int)boxNewPos.x, (int)boxNewPos.y)) {
            return false;
        }
        currentLevel->MoveBox((int)newGridPos.x, (int)newGridPos.y, (int)boxNewPos.x, (int)boxNewPos.y);
        state = PlayerState::PUSHING;
        if (onBoxPushed)
            onBoxPushed(boxNewPos);
    } else
        state = PlayerState::WALKING;
    rotation = CalculateRotationToDirection(direction);
    Vector3 fromPos = position;
    Vector3 toPos = GetWorldPositionFromGrid(newGridPos);
    float duration = (state == PlayerState::PUSHING) ? 0.4f : 0.3f;
    StartMoveAnimation(fromPos, toPos, duration);
    gridPosition = newGridPos;
    return true;
}

bool Player::TryMoveWithInput()
{
    Vector2 direction = GetDirectionFromInput();
    if (direction.x == 0 && direction.y == 0) {
        return false;
    }
    return TryMove(direction);
}

Vector2 Player::GetDirectionFromInput()
{
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) return {0, -1};
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) return {0, 1};
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) return {-1, 0};
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) return {1, 0};
    return {0, 0};
}

void Player::StartMoveAnimation(Vector3 from, Vector3 to, float duration)
{
    animationStartPos = from;
    animationEndPos = to;
    animationTime = 0.0f;
    animationDuration = duration;
    isMoving = true;
    if (animationManager) {
        animationManager->StartMoveAnimation(from, to, duration, EaseType::EASE_OUT, [this]() {
            CompleteMovement();
        });
    }
}

void Player::StartPushAnimation(Vector3 from, Vector3 to, float duration)
{
    state = PlayerState::PUSHING;
    StartMoveAnimation(from, to, duration);
}

void Player::UpdateAnimation(float deltaTime)
{
    if (!isMoving) return;
    animationTime += deltaTime;
    float t = animationTime / animationDuration;
    if (t >= 1.0f) {
        t = 1.0f;
        CompleteMovement();
    }
    float easedT = 1.0f - (1.0f - t) * (1.0f - t);
    position = Vector3Lerp(animationStartPos, animationEndPos, easedT);
    targetPosition = animationEndPos;
}

void Player::CompleteMovement()
{
    isMoving = false;
    position = animationEndPos;
    targetPosition = animationEndPos;
    animationTime = 0.0f;
    if (currentLevel)
        currentLevel->CheckCompletion();
    if (onMoveComplete)
        onMoveComplete();
    state = PlayerState::IDLE;
}

void Player::SetPosition(Vector3 pos)
{
    position = pos;
    targetPosition = pos;
    gridPosition = GetGridPositionFromWorld(pos);
}

void Player::SetGridPosition(Vector2 gridPos)
{
    gridPosition = gridPos;
    position = GetWorldPositionFromGrid(gridPos);
    targetPosition = position;
}

void Player::ForcePosition(Vector3 worldPos, Vector2 gridPos)
{
    position = worldPos;
    targetPosition = worldPos;
    gridPosition = gridPos;
    isMoving = false;
    state = PlayerState::IDLE;
}

float Player::CalculateRotationToDirection(Vector2 direction)
{
    if (direction.x > 0) return 90.0f;
    if (direction.x < 0) return -90.0f;
    if (direction.y > 0) return 180.0f;
    if (direction.y < 0) return 0.0f;
    return rotation;
}

bool Player::CanMoveInDirection(Vector2 direction)
{
    if (!currentLevel || isMoving)
        return false;
    Vector2 newPos = {gridPosition.x + direction.x, gridPosition.y + direction.y};
    return currentLevel->IsValidPosition((int)newPos.x, (int)newPos.y) &&
           currentLevel->CanMoveToTile((int)newPos.x, (int)newPos.y);
}

bool Player::CanPushBoxInDirection(Vector2 direction)
{
    if (!currentLevel || isMoving)
        return false;
    Vector2 boxPos = {gridPosition.x + direction.x, gridPosition.y + direction.y};
    Vector2 boxNewPos = {boxPos.x + direction.x, boxPos.y + direction.y};
    return currentLevel->HasBox((int)boxPos.x, (int)boxPos.y) &&
           currentLevel->IsValidPosition((int)boxNewPos.x, (int)boxNewPos.y) &&
           currentLevel->CanMoveToTile((int)boxNewPos.x, (int)boxNewPos.y) &&
           !currentLevel->HasBox((int)boxNewPos.x, (int)boxNewPos.y);
}

bool Player::CanPush() const
{
    return state == PlayerState::PUSHING ||
           (state == PlayerState::IDLE && !isMoving);
}

bool Player::CanMove() const
{
    return !isMoving;
}

void Player::StartPushInteraction()
{
    if (CanPush())
        state = PlayerState::PUSHING;
}

void Player::StopMovement()
{
    isMoving = false;
    state = PlayerState::IDLE;
    position = targetPosition;
    animationTime = 0.0f;
}

void Player::StopAnimation()
{
    StopMovement();
}

Vector3 Player::GetWorldPositionFromGrid(Vector2 gridPos) const
{
    if (currentLevel)
        return currentLevel->GridToWorld((int)gridPos.x, (int)gridPos.y);
    return {gridPos.x, 0.0f, gridPos.y};
}

Vector2 Player::GetGridPositionFromWorld(Vector3 worldPos) const
{
    if (currentLevel)
        return currentLevel->WorldToGrid(worldPos);
    return {worldPos.x, worldPos.z};
}

Vector2 Player::GetFacingDirection() const
{
    if (rotation >= -45.0f && rotation < 45.0f)
        return {0, -1};
    if (rotation >= 45.0f && rotation < 135.0f)
        return {1, 0};
    if (rotation >= 135.0f || rotation < -135.0f)
        return {0, 1};
    return {-1, 0};
}

void DrawText3D(const char* text, Vector3 position, float fontSize, Color color, const Camera3D& camera)
{
    Vector2 screenPos = GetWorldToScreen(position, camera);
    DrawText(text, (int)screenPos.x, (int)screenPos.y, (int)fontSize, color);
}