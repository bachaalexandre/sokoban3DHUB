/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** AnimationManager
*/

#include "AnimationManager.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>

AnimationManager::AnimationManager()
{
    animations.reserve(50);
}

AnimationManager::~AnimationManager()
{
    StopAllAnimations();
}

void AnimationManager::Update(float deltaTime)
{
    for (auto& animation : animations) {
        if (!animation.isActive || animation.isPaused) {
            continue;
        }
        animation.currentTime += deltaTime;
        float progress = animation.currentTime / animation.duration;
        if (progress >= 1.0f) {
            if (animation.loop) {
                animation.currentTime = 0.0f;
                progress = 0.0f;
            } else {
                progress = 1.0f;
                animation.isActive = false;
            }
        }
        float easedProgress = ApplyEasing(progress, animation.easeType);
        switch (animation.type) {
            case AnimationType::MOVE:
            case AnimationType::PUSH:
            case AnimationType::BOUNCE:
                animation.currentPos = InterpolateVector3(animation.startPos, animation.endPos, easedProgress);
                break;
            case AnimationType::SCALE:
            case AnimationType::FADE:
            case AnimationType::ROTATE:
                animation.currentValue = animation.startValue + (animation.endValue - animation.startValue) * easedProgress;
                break;
            case AnimationType::IDLE:
                break;
        }
        if (animation.onUpdate)
            animation.onUpdate(easedProgress);
        if (!animation.isActive && animation.onComplete)
            animation.onComplete();
    }
    ClearCompletedAnimations();
}

float AnimationManager::LinearInterpolation(float t)
{
    return t;
}

float AnimationManager::EaseIn(float t)
{
    return t * t;
}

float AnimationManager::EaseOut(float t)
{
    return 1.0f - (1.0f - t) * (1.0f - t);
}

float AnimationManager::EaseInOut(float t)
{
    if (t < 0.5f)
        return 2.0f * t * t;
    else
        return 1.0f - 2.0f * (1.0f - t) * (1.0f - t);
}

float AnimationManager::BounceEase(float t)
{
    if (t < 1.0f / 2.75f) {
        return 7.5625f * t * t;
    } else if (t < 2.0f / 2.75f) {
        t -= 1.5f / 2.75f;
        return 7.5625f * t * t + 0.75f;
    } else if (t < 2.5f / 2.75f) {
        t -= 2.25f / 2.75f;
        return 7.5625f * t * t + 0.9375f;
    } else {
        t -= 2.625f / 2.75f;
        return 7.5625f * t * t + 0.984375f;
    }
}

float AnimationManager::ElasticEase(float t)
{
    if (t == 0.0f || t == 1.0f) return t;
    float p = 0.3f;
    float s = p / 4.0f;
    return -(powf(2.0f, 10.0f * (t - 1.0f)) * sinf((t - 1.0f - s) * (2.0f * PI) / p));
}

float AnimationManager::ApplyEasing(float t, EaseType easeType)
{
    switch (easeType) {
        case EaseType::LINEAR:
            return LinearInterpolation(t);
        case EaseType::EASE_IN:
            return EaseIn(t);
        case EaseType::EASE_OUT:
            return EaseOut(t);
        case EaseType::EASE_IN_OUT:
            return EaseInOut(t);
        case EaseType::BOUNCE:
            return BounceEase(t);
        case EaseType::ELASTIC:
            return ElasticEase(t);
        default:
            return t;
    }
}

Vector3 AnimationManager::InterpolateVector3(Vector3 start, Vector3 end, float t)
{
    return {
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t,
        start.z + (end.z - start.z) * t
    };
}

int AnimationManager::FindAnimationByTag(const std::string &tag) const
{
    auto it = taggedAnimations.find(tag);
    if (it != taggedAnimations.end())
        return it->second;
    return -1;
}

void AnimationManager::StartMoveAnimation(Vector3 from, Vector3 to, float duration, EaseType easeType, std::function<void()> callback)
{
    Animation animation;
    animation.type = AnimationType::MOVE;
    animation.startPos = from;
    animation.endPos = to;
    animation.currentPos = from;
    animation.duration = duration;
    animation.currentTime = 0.0f;
    animation.isActive = true;
    animation.easeType = easeType;
    animation.onComplete = callback;
    animations.push_back(animation);
}

void AnimationManager::StartPushAnimation(Vector3 from, Vector3 to, float duration,
    std::function<void()> callback) {
StartMoveAnimation(from, to, duration, EaseType::EASE_IN_OUT, callback);
}

void AnimationManager::StartBounceAnimation(Vector3 position, float height, float duration,
      std::function<void()> callback)
{
    Animation animation;
    animation.type = AnimationType::BOUNCE;
    animation.startPos = position;
    animation.endPos = {position.x, position.y + height, position.z};
    animation.currentPos = position;
    animation.duration = duration;
    animation.currentTime = 0.0f;
    animation.isActive = true;
    animation.easeType = EaseType::BOUNCE;
    animation.onComplete = callback;
    animations.push_back(animation);
}

void AnimationManager::StartScaleAnimation(float fromScale, float toScale, float duration, EaseType easeType, std::function<void()> callback)
{
    Animation animation;
    animation.type = AnimationType::SCALE;
    animation.startValue = fromScale;
    animation.endValue = toScale;
    animation.currentValue = fromScale;
    animation.duration = duration;
    animation.currentTime = 0.0f;
    animation.isActive = true;
    animation.easeType = easeType;
    animation.onComplete = callback;
    animations.push_back(animation);
}

void AnimationManager::StartRotationAnimation(float fromAngle, float toAngle, float duration, EaseType easeType, std::function<void()> callback)
{
    Animation animation;
    animation.type = AnimationType::ROTATE;
    animation.startValue = fromAngle;
    animation.endValue = toAngle;
    animation.currentValue = fromAngle;
    animation.duration = duration;
    animation.currentTime = 0.0f;
    animation.isActive = true;
    animation.easeType = easeType;
    animation.onComplete = callback;
    animations.push_back(animation);
}

void AnimationManager::StartFadeAnimation(float fromAlpha, float toAlpha, float duration, std::function<void()> callback)
{
    Animation animation;
    animation.type = AnimationType::FADE;
    animation.startValue = fromAlpha;
    animation.endValue = toAlpha;
    animation.currentValue = fromAlpha;
    animation.duration = duration;
    animation.currentTime = 0.0f;
    animation.isActive = true;
    animation.easeType = EaseType::LINEAR;
    animation.onComplete = callback;
    animations.push_back(animation);
}

void AnimationManager::StartTaggedAnimation(const std::string& tag, AnimationType type, Vector3 from, Vector3 to, float duration, EaseType easeType, std::function<void()> callback)
{
    int existingIndex = FindAnimationByTag(tag);
    if (existingIndex != -1)
    animations[existingIndex].isActive = false;
    Animation animation;
    animation.type = type;
    animation.startPos = from;
    animation.endPos = to;
    animation.currentPos = from;
    animation.duration = duration;
    animation.currentTime = 0.0f;
    animation.isActive = true;
    animation.easeType = easeType;
    animation.tag = tag;
    animation.onComplete = callback;
    taggedAnimations[tag] = animations.size();
    animations.push_back(animation);
}

void AnimationManager::PauseAnimation(const std::string& tag)
{
    int index = FindAnimationByTag(tag);
    if (index != -1)
    animations[index].isPaused = true;
}

void AnimationManager::ResumeAnimation(const std::string& tag)
{
    int index = FindAnimationByTag(tag);
    if (index != -1)
    animations[index].isPaused = false;
}

void AnimationManager::StopAnimation(const std::string& tag)
{
    int index = FindAnimationByTag(tag);
    if (index != -1)
    animations[index].isActive = false;
}

void AnimationManager::StopAllAnimations()
{
    for (auto& animation : animations)
        animation.isActive = false;
}

void AnimationManager::ClearCompletedAnimations()
{
    animations.erase(
    std::remove_if(animations.begin(), animations.end(), [](const Animation& a) { return !a.isActive;}), animations.end());
    taggedAnimations.clear();
    for (size_t i = 0; i < animations.size(); ++i) {
        if (!animations[i].tag.empty())
            taggedAnimations[animations[i].tag] = static_cast<int>(i);
    }
}

bool AnimationManager::IsAnimating() const
{
    return std::any_of(animations.begin(), animations.end(), [](const Animation& a) {
    return a.isActive; });
}

bool AnimationManager::IsAnimating(const std::string& tag) const
{
    int index = FindAnimationByTag(tag);
    return index != -1 && animations[index].isActive;
}

bool AnimationManager::IsAnimating(AnimationType type) const
{
    return std::any_of(animations.begin(), animations.end(), [type](const Animation& a) {
    return a.isActive && a.type == type; });
}

Vector3 AnimationManager::GetAnimatedPosition(const std::string& tag) const
{
    int index = FindAnimationByTag(tag);
    if (index != -1)
        return animations[index].currentPos;
    return {0, 0, 0};
}

Vector3 AnimationManager::GetAnimatedPosition(AnimationType type) const
{
    for (const auto& a : animations) {
        if (a.isActive && a.type == type)
            return a.currentPos;
    }
    return {0, 0, 0};
}

float AnimationManager::GetAnimatedValue(const std::string& tag) const {
int index = FindAnimationByTag(tag);
if (index != -1) {
return animations[index].currentValue;
}
return 0.0f;
}

float AnimationManager::GetAnimatedScale(const std::string& tag) const {
return GetAnimatedValue(tag);
}

float AnimationManager::GetAnimatedRotation(const std::string& tag) const {
return GetAnimatedValue(tag);
}

float AnimationManager::GetAnimatedAlpha(const std::string& tag) const {
return GetAnimatedValue(tag);
}

int AnimationManager::GetActiveAnimationCount() const
{
    return static_cast<int>(std::count_if(animations.begin(), animations.end(), [](const Animation& a) {
    return a.isActive;}));
}

void AnimationManager::SetGlobalTimeScale(float scale)
{
    for (auto& animation : animations) {
        animation.duration /= scale;
    }
}

void AnimationManager::AnimatePlayerMove(Vector3 from, Vector3 to, std::function<void()> callback)
{
    StartMoveAnimation(from, to, 0.5f, EaseType::EASE_IN_OUT, callback);
}

void AnimationManager::AnimateBoxPush(Vector3 from, Vector3 to, std::function<void()> callback)
{
    StartPushAnimation(from, to, 0.4f, callback);
}

void AnimationManager::AnimateBoxOnTarget(Vector3 position)
{
    StartBounceAnimation(position, 0.2f, 0.3f);
}

void AnimationManager::AnimateLevelComplete()
{
    StartScaleAnimation(1.0f, 1.2f, 0.5f, EaseType::ELASTIC);
}

void AnimationManager::AnimateGameStart()
{
    StartFadeAnimation(0.0f, 1.0f, 1.0f);
}

