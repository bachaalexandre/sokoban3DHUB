/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Box
*/

#ifndef BOX_HPP_
#define BOX_HPP_
#pragma once

#include <raylib.h>
#include <functional>

enum class BoxState {
    NORMAL,
    ON_TARGET,
    MOVING,
    HIGHLIGHTED
};

class Level;
class AnimationManager;

class Box {
private:
    Vector3 position;
    Vector3 targetPosition;
    Vector2 gridPosition;
    BoxState state;
    bool isMoving;
    float moveSpeed;
    float animationTime;
    float animationDuration;
    Vector3 animationStartPos;
    Vector3 animationEndPos;
    float glowIntensity;
    float bounceOffset;
    float scaleMultiplier;
    Color currentTint;
    Level *currentLevel;
    AnimationManager *animationManager;
    std::function<void()> onMoveComplete;
    std::function<void(BoxState)> onStateChanged;
    void UpdateAnimation(float deltaTime);
    void UpdateVisualEffects(float deltaTime);
    void CompleteMovement();
    void CheckTargetStatus();
    void UpdateTint();

public:
    Box();
    ~Box();
    void Initialize(Vector3 startPos);
    void Initialize(Vector2 gridPos, Level* level);
    void Update(float deltaTime);
    void SetLevel(Level* level) { currentLevel = level; }
    void SetAnimationManager(AnimationManager* manager) { animationManager = manager; }
    void MoveTo(Vector2 newGridPos);
    void MoveToWorld(Vector3 newWorldPos);
    void SetPosition(Vector3 pos);
    void SetGridPosition(Vector2 gridPos);
    void ForcePosition(Vector3 worldPos, Vector2 gridPos);
    void StartMoveAnimation(Vector3 from, Vector3 to, float duration = 0.3f);
    void StartBounceAnimation(float intensity = 0.2f);
    void StartGlowAnimation();
    void StopAnimation();
    BoxState GetState() const { return state; }
    void SetState(BoxState newState);
    Vector3 GetPosition() const { return position; }
    Vector2 GetGridPosition() const { return gridPosition; }
    bool IsMoving() const { return isMoving; }
    bool IsOnTarget() const { return state == BoxState::ON_TARGET; }
    bool CanBePushed() const;
    bool CanMoveTo(Vector2 gridPos) const;
    bool OnPushed(Vector2 direction);
    void OnPlacedOnTarget();
    void OnRemovedFromTarget();
    float GetGlowIntensity() const { return glowIntensity; }
    float GetBounceOffset() const { return bounceOffset; }
    float GetScaleMultiplier() const { return scaleMultiplier; }
    Color GetCurrentTint() const { return currentTint; }
    Vector3 GetRenderPosition() const;
    Vector3 GetRenderScale() const;
    void SetOnMoveCompleteCallback(std::function<void()> callback) { onMoveComplete = callback; }
    void SetOnStateChangedCallback(std::function<void(BoxState)> callback) { onStateChanged = callback; }
    Vector3 GetWorldPositionFromGrid(Vector2 gridPos) const;
    Vector2 GetGridPositionFromWorld(Vector3 worldPos) const;
    float GetDistanceToTarget() const;
    void SetPositionImmediate(Vector2 newGridPos);
    void SetTargetPosition(Vector3 newTargetPos);
    void SyncWithLevel(Vector2 newGridPos, Vector3 newWorldPos);
    struct BoxSaveData {
        Vector2 gridPosition;
        BoxState state;
    };
    BoxSaveData GetSaveData() const;
    void LoadSaveData(const BoxSaveData& data);
};

#endif /* !BOX_HPP_ */