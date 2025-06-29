/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** AnimationManager
*/

#ifndef ANIMATIONMANAGER_HPP_
#define ANIMATIONMANAGER_HPP_
#pragma once

#include <raylib.h>
#include <functional>
#include <vector>
#include <unordered_map>
#include <string>

enum class AnimationType {
    MOVE,
    PUSH,
    IDLE,
    BOUNCE,
    FADE,
    SCALE,
    ROTATE
};

enum class EaseType {
    LINEAR,
    EASE_IN,
    EASE_OUT,
    EASE_IN_OUT,
    BOUNCE,
    ELASTIC
};

struct Animation {
    AnimationType type;
    Vector3 startPos;
    Vector3 endPos;
    Vector3 currentPos;
    float startValue;
    float endValue;
    float currentValue;
    float duration;
    float currentTime;
    bool isActive;
    bool isPaused;
    bool loop;
    EaseType easeType;
    std::string tag;
    std::function<void()> onComplete;
    std::function<void(float)> onUpdate;
    Animation() : type(AnimationType::MOVE), startPos({0,0,0}), endPos({0,0,0}),
                  currentPos({0,0,0}), startValue(0.0f), endValue(1.0f),
                  currentValue(0.0f), duration(1.0f), currentTime(0.0f),
                  isActive(false), isPaused(false), loop(false),
                  easeType(EaseType::LINEAR), tag("") {}
};

class AnimationManager {
private:
    std::vector<Animation> animations;
    std::unordered_map<std::string, int> taggedAnimations;
    float LinearInterpolation(float t);
    float EaseIn(float t);
    float EaseOut(float t);
    float EaseInOut(float t);
    float BounceEase(float t);
    float ElasticEase(float t);
    float ApplyEasing(float t, EaseType easeType);
    Vector3 InterpolateVector3(Vector3 start, Vector3 end, float t);
    int FindAnimationByTag(const std::string& tag) const;

public:
    AnimationManager();
    ~AnimationManager();
    void Update(float deltaTime);
    void StartMoveAnimation(Vector3 from, Vector3 to, float duration,
                           EaseType easeType = EaseType::EASE_OUT,
                           std::function<void()> callback = nullptr);
    void StartPushAnimation(Vector3 from, Vector3 to, float duration,
                           std::function<void()> callback = nullptr);
    void StartBounceAnimation(Vector3 position, float height, float duration,
                             std::function<void()> callback = nullptr);
    void StartScaleAnimation(float fromScale, float toScale, float duration,
                            EaseType easeType = EaseType::EASE_IN_OUT,
                            std::function<void()> callback = nullptr);
    void StartRotationAnimation(float fromAngle, float toAngle, float duration,
                               EaseType easeType = EaseType::LINEAR,
                               std::function<void()> callback = nullptr);
    void StartFadeAnimation(float fromAlpha, float toAlpha, float duration,
                           std::function<void()> callback = nullptr);
    void StartTaggedAnimation(const std::string& tag, AnimationType type,
                             Vector3 from, Vector3 to, float duration,
                             EaseType easeType = EaseType::LINEAR,
                             std::function<void()> callback = nullptr);
    void PauseAnimation(const std::string& tag);
    void ResumeAnimation(const std::string& tag);
    void StopAnimation(const std::string& tag);
    void StopAllAnimations();
    void ClearCompletedAnimations();
    bool IsAnimating() const;
    bool IsAnimating(const std::string& tag) const;
    bool IsAnimating(AnimationType type) const;
    Vector3 GetAnimatedPosition(const std::string& tag) const;
    Vector3 GetAnimatedPosition(AnimationType type) const;
    float GetAnimatedValue(const std::string& tag) const;
    float GetAnimatedScale(const std::string& tag) const;
    float GetAnimatedRotation(const std::string& tag) const;
    float GetAnimatedAlpha(const std::string& tag) const;
    int GetActiveAnimationCount() const;
    void SetGlobalTimeScale(float scale);
    void AnimatePlayerMove(Vector3 from, Vector3 to, std::function<void()> callback = nullptr);
    void AnimateBoxPush(Vector3 from, Vector3 to, std::function<void()> callback = nullptr);
    void AnimateBoxOnTarget(Vector3 position);
    void AnimateLevelComplete();
    void AnimateGameStart();
};

#endif /* !ANIMATIONMANAGER_HPP_ */