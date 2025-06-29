/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Player
*/

#ifndef PLAYER_HPP_
#define PLAYER_HPP_
#pragma once

#include <raylib.h>
#include <functional>

enum class PlayerState {
    IDLE,
    WALKING,
    PUSHING
};

class Level;
class AnimationManager;

class Player {
private:
    Vector3 position;
    Vector3 targetPosition;
    Vector2 gridPosition;
    float rotation;
    PlayerState state;
    float moveSpeed;
    bool isMoving;
    float animationTime;
    float animationDuration;
    Vector3 animationStartPos;
    Vector3 animationEndPos;
    Level *currentLevel;
    AnimationManager* animationManager;
    std::function<void()> onMoveComplete;
    std::function<void(Vector2)> onBoxPushed;
    void UpdateAnimation(float deltaTime);
    void CompleteMovement();
    float CalculateRotationToDirection(Vector2 direction);
    Vector2 GetDirectionFromInput();
    bool CanMoveInDirection(Vector2 direction);
    bool CanPushBoxInDirection(Vector2 direction);

public:
    Player();
    ~Player();
    void Initialize(Vector3 startPos);
    void Update(float deltaTime);
    void SetLevel(Level* level) { currentLevel = level; }
    void SetAnimationManager(AnimationManager* manager) { animationManager = manager; }
    bool TryMove(Vector2 direction);
    bool TryMoveWithInput();
    void SetPosition(Vector3 pos);
    void SetGridPosition(Vector2 gridPos);
    void ForcePosition(Vector3 worldPos, Vector2 gridPos);
    void StartMoveAnimation(Vector3 from, Vector3 to, float duration = 0.3f);
    void StartPushAnimation(Vector3 from, Vector3 to, float duration = 0.4f);
    void StopAnimation();
    PlayerState GetState() const { return state; }
    void SetState(PlayerState newState) { state = newState; }
    Vector3 GetPosition() const { return position; }
    Vector2 GetGridPosition() const { return gridPosition; }
    float GetRotation() const { return rotation; }
    bool IsMoving() const { return isMoving; }
    bool CanPush() const;
    bool CanMove() const;
    void StartPushInteraction();
    void StopMovement();
    void SetOnMoveCompleteCallback(std::function<void()> callback) { onMoveComplete = callback; }
    void SetOnBoxPushedCallback(std::function<void(Vector2)> callback) { onBoxPushed = callback; }
    void ProcessInput();
    bool IsInputPressed(Vector2 &outDirection);
    Vector3 GetWorldPositionFromGrid(Vector2 gridPos) const;
    Vector2 GetGridPositionFromWorld(Vector3 worldPos) const;
    Vector2 GetFacingDirection() const;
};

#endif /* !PLAYER_HPP_ */