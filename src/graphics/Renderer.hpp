/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Renderer
*/

#ifndef RENDERER_HPP_
#define RENDERER_HPP_
#pragma once

#include <raylib.h>
#include <vector>
#include <memory>
#include <string>

struct RenderObject {
    Model model;
    Vector3 position;
    Vector3 scale;
    Vector3 rotation;
    Color tint;
    bool visible;
    RenderObject() : position({0, 0, 0}), scale({1, 1, 1}), rotation({0, 0, 0}), 
                     tint(WHITE), visible(true) {}
};

class Renderer {
private:
    Camera3D camera;
    std::vector<std::unique_ptr<RenderObject>> objects;
    Model playerModel;
    Model boxModel;
    Model wallModel;
    Model floorModel;
    Model targetModel;
    Texture2D playerTexture;
    Texture2D boxTexture;
    Texture2D wallTexture;
    Texture2D floorTexture;
    Texture2D targetTexture;
    Material playerMaterial;
    Material boxMaterial;
    Material wallMaterial;
    Material floorMaterial;
    Material targetMaterial;
    bool modelsLoaded;
    bool texturesLoaded;
    Vector3 lightPosition;
    void LoadModels();
    void LoadTextures();
    void LoadMaterials();
    void UnloadModels();
    void UnloadTextures();
    Model CreateCubeModel();
    Model CreatePlaneModel();
    Texture2D CreateColorTexture(Color color);
    bool FileExists(const std::string& path);

public:
    Renderer();
    ~Renderer();
    bool Initialize();
    void Shutdown();
    void BeginFrame();
    void EndFrame();
    void Clear(Color color = {20, 20, 40, 255});
    void SetCamera(Vector3 position, Vector3 target, Vector3 up);
    void UpdateCamera();
    void SetCameraPosition(Vector3 position);
    void SetCameraTarget(Vector3 target);
    const Camera3D& GetCamera() const { return camera; }
    void DrawPlayer(Vector3 position, float rotation = 0.0f);
    void DrawBox(Vector3 position, bool onTarget = false);
    void DrawWall(Vector3 position);
    void DrawFloor(Vector3 position);
    void DrawTarget(Vector3 position);
    void DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis, 
                     float rotationAngle, Vector3 scale, Color tint);
    void DrawBoundingBox(Vector3 position, Vector3 size, Color color);
    void DrawGrid(int width, int height, float spacing = 1.0f);
    void DrawText3D(const char* text, Vector3 position, float fontSize, Color color);
    void DrawCoordinateSystem(Vector3 position, float size = 1.0f);
    void SetLightPosition(Vector3 position) { lightPosition = position; }
    Vector3 GetLightPosition() const { return lightPosition; }
    bool IsInitialized() const { return modelsLoaded && texturesLoaded; }
};

#endif /* !RENDERER_HPP_ */