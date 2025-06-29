/*
** EPITECH PROJECT, 2025
** sokoban3D
** File description:
** Renderer
*/

#include "Renderer.hpp"
#include <iostream>
#include <cmath>

Renderer::Renderer()
    : modelsLoaded(false)
    , texturesLoaded(false)
    , lightPosition({5.0f, 5.0f, 5.0f}) {
    camera.position = {10.0f, 12.0f, 10.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize()
{
    try {
        LoadModels();
        LoadTextures();
        LoadMaterials();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize Renderer: " << e.what() << std::endl;
        return false;
    }
}

void Renderer::Shutdown()
{
    UnloadTextures();
    UnloadModels();
    objects.clear();
    modelsLoaded = false;
    texturesLoaded = false;
}

void Renderer::LoadModels()
{
    playerModel = CreateCubeModel();
    boxModel = CreateCubeModel();
    wallModel = CreateCubeModel();
    floorModel = CreatePlaneModel();
    targetModel = CreatePlaneModel();
    modelsLoaded = true;
}

void Renderer::LoadTextures()
{
    floorTexture = CreateColorTexture(LIGHTGRAY);
    wallTexture = CreateColorTexture(GRAY);
    playerTexture = CreateColorTexture(BLUE);
    boxTexture = CreateColorTexture(BROWN);
    targetTexture = CreateColorTexture(GREEN);
    texturesLoaded = true;
    std::cout << "Default textures loaded successfully!" << std::endl;
}

void Renderer::LoadMaterials()
{
    playerMaterial = LoadMaterialDefault();
    SetMaterialTexture(&playerMaterial, MATERIAL_MAP_DIFFUSE, playerTexture);
    boxMaterial = LoadMaterialDefault();
    SetMaterialTexture(&boxMaterial, MATERIAL_MAP_DIFFUSE, boxTexture);
    wallMaterial = LoadMaterialDefault();
    SetMaterialTexture(&wallMaterial, MATERIAL_MAP_DIFFUSE, wallTexture);
    floorMaterial = LoadMaterialDefault();
    SetMaterialTexture(&floorMaterial, MATERIAL_MAP_DIFFUSE, floorTexture);
    targetMaterial = LoadMaterialDefault();
    SetMaterialTexture(&targetMaterial, MATERIAL_MAP_DIFFUSE, targetTexture);
    std::cout << "Default materials loaded successfully!" << std::endl;
}

void Renderer::UnloadModels()
{
    if (modelsLoaded) {
        UnloadModel(playerModel);
        UnloadModel(boxModel);
        UnloadModel(wallModel);
        UnloadModel(floorModel);
        UnloadModel(targetModel);
    }
}

void Renderer::UnloadTextures()
{
    if (texturesLoaded) {
        UnloadTexture(playerTexture);
        UnloadTexture(boxTexture);
        UnloadTexture(wallTexture);
        UnloadTexture(floorTexture);
        UnloadTexture(targetTexture);
    }
}

Model Renderer::CreateCubeModel()
{
    Mesh cubeMesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model model = LoadModelFromMesh(cubeMesh);
    return model;
}

Model Renderer::CreatePlaneModel()
{
    Mesh planeMesh = GenMeshPlane(1.0f, 1.0f, 1, 1);
    Model model = LoadModelFromMesh(planeMesh);
    return model;
}

Texture2D Renderer::CreateColorTexture(Color color)
{
    Image image = GenImageColor(64, 64, color);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);
    return texture;
}

void Renderer::BeginFrame()
{
    BeginDrawing();
    Clear({20, 30, 60, 255}); //fd couleur
}

void Renderer::EndFrame()
{
    EndMode3D();
    EndDrawing();
}

void Renderer::SetCamera(Vector3 position, Vector3 target, Vector3 up)
{
    camera.position = position;
    camera.target = target;
    camera.up = up;
}

void Renderer::UpdateCamera() {
    // Mise à jour automatique de la caméra
}

void Renderer::SetCameraPosition(Vector3 position)
{
    camera.position = position;
}

void Renderer::SetCameraTarget(Vector3 target)
{
    camera.target = target;
}

void Renderer::DrawPlayer(Vector3 position, float rotation)
{
    if (!modelsLoaded)
        return;
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    Vector3 playerPos = {position.x, 0.5f, position.z};
    Color workSuitColor = {70, 120, 180, 255};
    Color darkWorkSuit = {50, 90, 140, 255};
    Vector3 bodyScale = {0.6f, 0.8f, 0.4f};
    DrawModelEx(playerModel, playerPos, rotationAxis, rotation, bodyScale, workSuitColor);
    Vector3 leftLegPos = {playerPos.x - 0.15f, 0.2f, playerPos.z};
    Vector3 rightLegPos = {playerPos.x + 0.15f, 0.2f, playerPos.z};
    Vector3 legScale = {0.25f, 0.6f, 0.25f};
    DrawModelEx(playerModel, leftLegPos, rotationAxis, rotation, legScale, darkWorkSuit);
    DrawModelEx(playerModel, rightLegPos, rotationAxis, rotation, legScale, darkWorkSuit);
    Vector3 leftArmPos = {playerPos.x - 0.35f, playerPos.y + 0.1f, playerPos.z};
    Vector3 rightArmPos = {playerPos.x + 0.35f, playerPos.y + 0.1f, playerPos.z};
    Vector3 armScale = {0.25f, 0.5f, 0.25f};
    DrawModelEx(playerModel, leftArmPos, rotationAxis, rotation, armScale, workSuitColor);
    DrawModelEx(playerModel, rightArmPos, rotationAxis, rotation, armScale, workSuitColor);
    Vector3 headPos = {playerPos.x, playerPos.y + 0.5f, playerPos.z};
    Color skinColor = {220, 180, 140, 255};
    DrawSphere(headPos, 0.2f, skinColor);
    Vector3 helmetPos = {playerPos.x, playerPos.y + 0.55f, playerPos.z};
    Color helmetColor = {255, 220, 0, 255};
    Color helmetDark = {200, 170, 0, 255};
    DrawSphere(helmetPos, 0.24f, helmetColor);
    Vector3 visorPos = {playerPos.x, playerPos.y + 0.52f, playerPos.z + 0.22f};
    DrawCube(visorPos, 0.35f, 0.08f, 0.04f, helmetDark);
    Vector3 stripPos = {playerPos.x, playerPos.y + 0.45f, playerPos.z};
    DrawCylinder(stripPos, 0.25f, 0.25f, 0.03f, 8, {255, 255, 255, 255});
    Color strapColor = {80, 80, 80, 255};
    Vector3 strapLeft = {playerPos.x - 0.18f, playerPos.y + 0.4f, playerPos.z};
    Vector3 strapRight = {playerPos.x + 0.18f, playerPos.y + 0.4f, playerPos.z};
    DrawCube(strapLeft, 0.02f, 0.15f, 0.02f, strapColor);
    DrawCube(strapRight, 0.02f, 0.15f, 0.02f, strapColor);
    Vector3 leftShoePos = {playerPos.x - 0.15f, 0.05f, playerPos.z + 0.1f};
    Vector3 rightShoePos = {playerPos.x + 0.15f, 0.05f, playerPos.z + 0.1f};
    Color shoeColor = {40, 30, 20, 255};
    Vector3 shoeScale = {0.3f, 0.15f, 0.4f};
    DrawModelEx(playerModel, leftShoePos, rotationAxis, rotation, shoeScale, shoeColor);
    DrawModelEx(playerModel, rightShoePos, rotationAxis, rotation, shoeScale, shoeColor);
    Color metalColor = {120, 120, 130, 255};
    Vector3 leftCapPos = {playerPos.x - 0.15f, 0.08f, playerPos.z + 0.2f};
    Vector3 rightCapPos = {playerPos.x + 0.15f, 0.08f, playerPos.z + 0.2f};
    DrawCube(leftCapPos, 0.25f, 0.06f, 0.15f, metalColor);
    DrawCube(rightCapPos, 0.25f, 0.06f, 0.15f, metalColor);
    Vector3 chestStripPos = {playerPos.x, playerPos.y + 0.2f, playerPos.z + 0.35f};
    DrawCube(chestStripPos, 0.5f, 0.05f, 0.01f, {255, 255, 255, 255});
    Vector3 waistStripPos = {playerPos.x, playerPos.y - 0.1f, playerPos.z + 0.35f};
    DrawCube(waistStripPos, 0.5f, 0.05f, 0.01f, {255, 255, 255, 255});
    Vector3 badgePos = {playerPos.x - 0.25f, playerPos.y + 0.15f, playerPos.z + 0.35f};
    DrawCube(badgePos, 0.08f, 0.12f, 0.01f, {255, 255, 255, 255});
    DrawCube(badgePos, 0.06f, 0.1f, 0.012f, {200, 50, 50, 255});
}

void Renderer::DrawBox(Vector3 position, bool onTarget)
{
    if (!modelsLoaded)
        return;
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    Vector3 scale = {0.9f, 0.9f, 0.9f};
    Vector3 boxPos = {position.x, 0.5f, position.z};
    Color woodColor = onTarget ? Color{200, 150, 100, 255} : Color{160, 120, 80, 255};
    Color darkWoodColor = onTarget ? Color{180, 130, 80, 255} : Color{140, 100, 60, 255};
    Color metalColor = {80, 80, 90, 255};
    Color rustColor = {120, 80, 60, 255};
    Color boltColor = {60, 60, 70, 255};
    DrawModelEx(boxModel, boxPos, rotationAxis, 0.0f, scale, woodColor);
    float plankWidth = 0.15f;
    float plankHeight = 0.85f;
    float plankThickness = 0.02f;
    Color plankBorderColor = {80, 50, 30, 255};
    float borderThickness = 0.005f;
    for (int i = 0; i < 6; i++) {
        float offsetX = (i - 2.5f) * plankWidth;
        Vector3 plankPosFront = {boxPos.x + offsetX, boxPos.y, boxPos.z + 0.46f};
        DrawCube(plankPosFront, plankWidth - 0.01f, plankHeight, plankThickness,
                i % 2 == 0 ? woodColor : darkWoodColor);
        if (i > 0) {
            Vector3 leftBorderPos = {boxPos.x + offsetX - plankWidth/2, boxPos.y, boxPos.z + 0.465f};
            DrawCube(leftBorderPos, borderThickness, plankHeight + 0.02f, plankThickness + 0.01f, plankBorderColor);
        }
    float topPlankWidth = 0.15f;
    float topPlankLength = 0.9f;
    float topPlankThickness = 0.03f;
    for (int i = 0; i < 6; i++) {
        float offsetX = (i - 2.5f) * topPlankWidth;
        Vector3 topPlankPos = {boxPos.x + offsetX, boxPos.y + 0.44f, boxPos.z};
        DrawCube(topPlankPos, topPlankWidth - 0.01f, topPlankThickness, topPlankLength,
                i % 2 == 0 ? woodColor : darkWoodColor);
        if (i > 0) {
            Vector3 leftBorderPos = {boxPos.x + offsetX - topPlankWidth/2, boxPos.y + 0.445f, boxPos.z};
            DrawCube(leftBorderPos, borderThickness, topPlankThickness + 0.01f, topPlankLength + 0.02f, plankBorderColor);
        }
        if (i < 5) {
            Vector3 rightBorderPos = {boxPos.x + offsetX + topPlankWidth/2, boxPos.y + 0.445f, boxPos.z};
            DrawCube(rightBorderPos, borderThickness, topPlankThickness + 0.01f, topPlankLength + 0.02f, plankBorderColor);
        }
    }
        if (i < 5) {
            Vector3 rightBorderPos = {boxPos.x + offsetX + plankWidth/2, boxPos.y, boxPos.z + 0.465f};
            DrawCube(rightBorderPos, borderThickness, plankHeight + 0.02f, plankThickness + 0.01f, plankBorderColor);
        }
        Vector3 plankPosBack = {boxPos.x + offsetX, boxPos.y, boxPos.z - 0.46f};
        DrawCube(plankPosBack, plankWidth - 0.01f, plankHeight, plankThickness,
                i % 2 == 0 ? woodColor : darkWoodColor);
        if (i > 0) {
            Vector3 leftBorderPos = {boxPos.x + offsetX - plankWidth/2, boxPos.y, boxPos.z - 0.465f};
            DrawCube(leftBorderPos, borderThickness, plankHeight + 0.02f, plankThickness + 0.01f, plankBorderColor);
        }
        if (i < 5) {
            Vector3 rightBorderPos = {boxPos.x + offsetX + plankWidth/2, boxPos.y, boxPos.z - 0.465f};
            DrawCube(rightBorderPos, borderThickness, plankHeight + 0.02f, plankThickness + 0.01f, plankBorderColor);
        }
    }
    for (int i = 0; i < 6; i++) {
        float offsetZ = (i - 2.5f) * plankWidth;
        Vector3 plankPosLeft = {boxPos.x - 0.46f, boxPos.y, boxPos.z + offsetZ};
        DrawCube(plankPosLeft, plankThickness, plankHeight, plankWidth - 0.01f,
                i % 2 == 0 ? darkWoodColor : woodColor);
        if (i > 0) {
            Vector3 frontBorderPos = {boxPos.x - 0.465f, boxPos.y, boxPos.z + offsetZ - plankWidth/2};
            DrawCube(frontBorderPos, plankThickness + 0.01f, plankHeight + 0.02f, borderThickness, plankBorderColor);
        }
        if (i < 5) {
            Vector3 backBorderPos = {boxPos.x - 0.465f, boxPos.y, boxPos.z + offsetZ + plankWidth/2};
            DrawCube(backBorderPos, plankThickness + 0.01f, plankHeight + 0.02f, borderThickness, plankBorderColor);
        }
        Vector3 plankPosRight = {boxPos.x + 0.46f, boxPos.y, boxPos.z + offsetZ};
        DrawCube(plankPosRight, plankThickness, plankHeight, plankWidth - 0.01f,
                i % 2 == 0 ? darkWoodColor : woodColor);
        if (i > 0) {
            Vector3 frontBorderPos = {boxPos.x + 0.465f, boxPos.y, boxPos.z + offsetZ - plankWidth/2};
            DrawCube(frontBorderPos, plankThickness + 0.01f, plankHeight + 0.02f, borderThickness, plankBorderColor);
        }
        if (i < 5) {
            Vector3 backBorderPos = {boxPos.x + 0.465f, boxPos.y, boxPos.z + offsetZ + plankWidth/2};
            DrawCube(backBorderPos, plankThickness + 0.01f, plankHeight + 0.02f, borderThickness, plankBorderColor);
        }
    }
    float frameWidth = 0.04f;
    float frameThickness = 0.02f;
    DrawCube({boxPos.x, boxPos.y + 0.45f, boxPos.z - 0.45f}, 0.9f, frameWidth, frameThickness, metalColor);
    DrawCube({boxPos.x, boxPos.y + 0.45f, boxPos.z + 0.45f}, 0.9f, frameWidth, frameThickness, metalColor);
    DrawCube({boxPos.x - 0.45f, boxPos.y + 0.45f, boxPos.z}, frameThickness, frameWidth, 0.9f, metalColor);
    DrawCube({boxPos.x + 0.45f, boxPos.y + 0.45f, boxPos.z}, frameThickness, frameWidth, 0.9f, metalColor);
    DrawCube({boxPos.x, boxPos.y - 0.45f, boxPos.z - 0.45f}, 0.9f, frameWidth, frameThickness, metalColor);
    DrawCube({boxPos.x, boxPos.y - 0.45f, boxPos.z + 0.45f}, 0.9f, frameWidth, frameThickness, metalColor);
    DrawCube({boxPos.x - 0.45f, boxPos.y - 0.45f, boxPos.z}, frameThickness, frameWidth, 0.9f, metalColor);
    DrawCube({boxPos.x + 0.45f, boxPos.y - 0.45f, boxPos.z}, frameThickness, frameWidth, 0.9f, metalColor);
    Vector3 cornerPositions[4] = {
        {boxPos.x - 0.45f, boxPos.y, boxPos.z - 0.45f},
        {boxPos.x + 0.45f, boxPos.y, boxPos.z - 0.45f},
        {boxPos.x - 0.45f, boxPos.y, boxPos.z + 0.45f},
        {boxPos.x + 0.45f, boxPos.y, boxPos.z + 0.45f}
    };
    for (int i = 0; i < 4; i++) {
        DrawCube(cornerPositions[i], frameThickness, 0.9f, frameThickness, metalColor);
        Vector3 rustPos = {cornerPositions[i].x, cornerPositions[i].y - 0.2f, cornerPositions[i].z};
        DrawCube(rustPos, frameThickness + 0.005f, 0.1f, frameThickness + 0.005f, rustColor);
    }
    float boltRadius = 0.015f;
    float boltHeight = 0.02f;
    for (int side = 0; side < 4; side++) {
        for (int bolt = 0; bolt < 3; bolt++) {
            Vector3 boltPos;
            float boltOffset = (bolt - 1) * 0.3f;
            switch (side) {
                case 0:
                    boltPos = {boxPos.x + boltOffset, boxPos.y + 0.45f, boxPos.z + 0.47f};
                    break;
                case 1:
                    boltPos = {boxPos.x + boltOffset, boxPos.y + 0.45f, boxPos.z - 0.47f};
                    break;
                case 2:
                    boltPos = {boxPos.x + boltOffset, boxPos.y - 0.45f, boxPos.z + 0.47f};
                    break;
                case 3:
                    boltPos = {boxPos.x + boltOffset, boxPos.y - 0.45f, boxPos.z - 0.47f};
                    break;
            }
            DrawCylinder(boltPos, boltRadius, boltRadius, boltHeight, 8, boltColor);
            DrawCylinder(boltPos, boltRadius - 0.005f, boltRadius - 0.005f, boltHeight + 0.005f, 8, metalColor);
        }
    }
    int seed = static_cast<int>(position.x * 1000 + position.z * 100);
    srand(seed);
    for (int i = 0; i < 8; i++) {
        float offsetX = (rand() % 100 - 50) / 100.0f * 0.7f;
        float offsetY = (rand() % 100 - 50) / 100.0f * 0.7f;
        float offsetZ = (rand() % 100 - 50) / 100.0f * 0.7f;
        Vector3 woodDetailPos = {boxPos.x + offsetX, boxPos.y + offsetY, boxPos.z + offsetZ};
        Color woodDetailColor = {
            static_cast<unsigned char>(darkWoodColor.r - 20),
            static_cast<unsigned char>(darkWoodColor.g - 20),
            static_cast<unsigned char>(darkWoodColor.b - 10),
            255
        };
        if (i < 4) {
            DrawSphere(woodDetailPos, 0.02f, woodDetailColor);
        }
        else {
            DrawSphere(woodDetailPos, 0.04f, Fade(woodDetailColor, 0.8f));
        }
    }
    Vector3 labelPos = {boxPos.x, boxPos.y + 0.2f, boxPos.z + 0.48f};
    DrawCube(labelPos, 0.3f, 0.15f, 0.005f, BEIGE);
    DrawCube(labelPos, 0.28f, 0.13f, 0.008f, WHITE);
    Vector3 handleLeft = {boxPos.x - 0.48f, boxPos.y + 0.1f, boxPos.z};
    Vector3 handleRight = {boxPos.x + 0.48f, boxPos.y + 0.1f, boxPos.z};
    DrawCube(handleLeft, 0.02f, 0.08f, 0.15f, metalColor);
    DrawCube(handleRight, 0.02f, 0.08f, 0.15f, metalColor);
    DrawCylinder({handleLeft.x - 0.03f, handleLeft.y, handleLeft.z}, 0.02f, 0.02f, 0.15f, 8, boltColor);
    DrawCylinder({handleRight.x + 0.03f, handleRight.y, handleRight.z}, 0.02f, 0.02f, 0.15f, 8, boltColor);
    if (onTarget) {
        Vector3 glowScale = {1.05f, 1.05f, 1.05f};
        DrawModelEx(boxModel, boxPos, rotationAxis, 0.0f, glowScale, Fade(GOLD, 0.2f));
        static float particleTime = 0.0f;
        particleTime += GetFrameTime();
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * DEG2RAD + particleTime;
            Vector3 particlePos = {
                boxPos.x + cosf(angle) * 0.7f,
                boxPos.y + sinf(particleTime * 2.0f + i) * 0.2f,
                boxPos.z + sinf(angle) * 0.7f
            };
            DrawSphere(particlePos, 0.03f, Fade(GOLD, 0.6f));
        }
    }
}

void Renderer::DrawWall(Vector3 position)
{
    if (!modelsLoaded)
        return;
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    Vector3 scale = {1.0f, 2.0f, 1.0f};
    Vector3 wallPos = {position.x, 1.0f, position.z};
    Color brickColor = {180, 100, 80, 255};
    DrawModelEx(wallModel, wallPos, rotationAxis, 0.0f, scale, brickColor);
    Color mortarColor = {220, 220, 210, 255};
    float brickHeight = 0.25f;
    for (float y = wallPos.y - 1.0f; y <= wallPos.y + 1.0f; y += brickHeight) {
        Vector3 startFront = {wallPos.x - 0.51f, y, wallPos.z + 0.51f};
        Vector3 endFront = {wallPos.x + 0.51f, y, wallPos.z + 0.51f};
        DrawLine3D(startFront, endFront, mortarColor);
        Vector3 startBack = {wallPos.x - 0.51f, y, wallPos.z - 0.51f};
        Vector3 endBack = {wallPos.x + 0.51f, y, wallPos.z - 0.51f};
        DrawLine3D(startBack, endBack, mortarColor);
        Vector3 startLeft = {wallPos.x - 0.51f, y, wallPos.z - 0.51f};
        Vector3 endLeft = {wallPos.x - 0.51f, y, wallPos.z + 0.51f};
        DrawLine3D(startLeft, endLeft, mortarColor);
        Vector3 startRight = {wallPos.x + 0.51f, y, wallPos.z - 0.51f};
        Vector3 endRight = {wallPos.x + 0.51f, y, wallPos.z + 0.51f};
        DrawLine3D(startRight, endRight, mortarColor);
    }
    float brickWidth = 0.4f;
    bool offsetRow = false;
    for (float y = wallPos.y - 1.0f; y <= wallPos.y + 1.0f; y += brickHeight) {
        float startOffset = offsetRow ? brickWidth * 0.5f : 0.0f;
        for (float x = wallPos.x - 0.5f + startOffset; x <= wallPos.x + 0.5f; x += brickWidth) {
            if (x >= wallPos.x - 0.5f && x <= wallPos.x + 0.5f) {
                Vector3 startFront = {x, y, wallPos.z + 0.51f};
                Vector3 endFront = {x, y + brickHeight, wallPos.z + 0.51f};
                DrawLine3D(startFront, endFront, mortarColor);
                Vector3 startBack = {x, y, wallPos.z - 0.51f};
                Vector3 endBack = {x, y + brickHeight, wallPos.z - 0.51f};
                DrawLine3D(startBack, endBack, mortarColor);
            }
        }
        for (float z = wallPos.z - 0.5f + startOffset; z <= wallPos.z + 0.5f; z += brickWidth) {
            if (z >= wallPos.z - 0.5f && z <= wallPos.z + 0.5f) {
                Vector3 startLeft = {wallPos.x - 0.51f, y, z};
                Vector3 endLeft = {wallPos.x - 0.51f, y + brickHeight, z};
                DrawLine3D(startLeft, endLeft, mortarColor);
                Vector3 startRight = {wallPos.x + 0.51f, y, z};
                Vector3 endRight = {wallPos.x + 0.51f, y + brickHeight, z};
                DrawLine3D(startRight, endRight, mortarColor);
            }
        }
        offsetRow = !offsetRow;
    }
    Color borderColor = {100, 60, 50, 255};
    DrawCubeWires(wallPos, scale.x + 0.02f, scale.y + 0.02f, scale.z + 0.02f, borderColor);
    std::cout << "Drawing brick wall at: " << wallPos.x << ", " << wallPos.y << ", " << wallPos.z << std::endl;
}


void Renderer::DrawFloor(Vector3 position)
{
    if (!modelsLoaded)
        return;
    Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};
    Vector3 scale = {1.0f, 1.0f, 1.0f};
    Vector3 floorPos = {position.x, 0.0f, position.z};
    Color concreteBase = {140, 140, 130, 255};
    DrawModelEx(floorModel, floorPos, rotationAxis, 0.0f, scale, concreteBase);
    Color jointColor = {80, 80, 75, 255};
    float jointWidth = 0.01f;
    int tileX = static_cast<int>(position.x);
    int tileZ = static_cast<int>(position.z);
    if (tileZ % 2 == 0) {
        Vector3 jointStart = {floorPos.x - 0.5f, 0.01f, floorPos.z - 0.5f};
        Vector3 jointEnd = {floorPos.x + 0.5f, 0.01f, floorPos.z - 0.5f};
        for (float offset = -jointWidth; offset <= jointWidth; offset += jointWidth/3) {
            Vector3 start = {jointStart.x, jointStart.y, jointStart.z + offset};
            Vector3 end = {jointEnd.x, jointEnd.y, jointEnd.z + offset};
            DrawLine3D(start, end, jointColor);
        }
    }
    if (tileX % 2 == 0) {
        Vector3 jointStart = {floorPos.x - 0.5f, 0.01f, floorPos.z - 0.5f};
        Vector3 jointEnd = {floorPos.x - 0.5f, 0.01f, floorPos.z + 0.5f};
        for (float offset = -jointWidth; offset <= jointWidth; offset += jointWidth/3) {
            Vector3 start = {jointStart.x + offset, jointStart.y, jointStart.z};
            Vector3 end = {jointEnd.x + offset, jointEnd.y, jointEnd.z};
            DrawLine3D(start, end, jointColor);
        }
    }
    Color microJointColor = {120, 120, 115, 255};
    for (float z = -0.4f; z <= 0.4f; z += 0.2f) {
        Vector3 start = {floorPos.x - 0.45f, 0.005f, floorPos.z + z};
        Vector3 end = {floorPos.x + 0.45f, 0.005f, floorPos.z + z};
        DrawLine3D(start, end, microJointColor);
    }
    for (float x = -0.4f; x <= 0.4f; x += 0.2f) {
        Vector3 start = {floorPos.x + x, 0.005f, floorPos.z - 0.45f};
        Vector3 end = {floorPos.x + x, 0.005f, floorPos.z + 0.45f};
        DrawLine3D(start, end, microJointColor);
    }
    Color stainColor = {100, 100, 95, 180};
    int seed = static_cast<int>(position.x * 1000 + position.z * 100);
    srand(seed);
    for (int i = 0; i < 3; i++) {
        float offsetX = (rand() % 100 - 50) / 100.0f * 0.6f;
        float offsetZ = (rand() % 100 - 50) / 100.0f * 0.6f;
        float stainSize = (rand() % 30 + 10) / 1000.0f;
        Vector3 stainPos = {floorPos.x + offsetX, 0.003f, floorPos.z + offsetZ};
        for (int j = 0; j < 8; j++) {
            float angle = j * 45.0f * DEG2RAD;
            Vector3 point1 = {
                stainPos.x + cosf(angle) * stainSize,
                stainPos.y,
                stainPos.z + sinf(angle) * stainSize
            };
            Vector3 point2 = {
                stainPos.x + cosf(angle + 45.0f * DEG2RAD) * stainSize,
                stainPos.y,
                stainPos.z + sinf(angle + 45.0f * DEG2RAD) * stainSize
            };
            DrawLine3D(stainPos, point1, Fade(stainColor, 0.3f));
        }
    }
    Color markingColor = {200, 180, 60, 255};
    if ((tileX + tileZ) % 7 == 0) {
        Vector3 diagStart = {floorPos.x - 0.3f, 0.008f, floorPos.z - 0.3f};
        Vector3 diagEnd = {floorPos.x + 0.3f, 0.008f, floorPos.z + 0.3f};
        for (float t = 0.0f; t <= 1.0f; t += 0.1f) {
            if (static_cast<int>(t * 10) % 2 == 0) {
                Vector3 segStart = {
                    diagStart.x + t * (diagEnd.x - diagStart.x),
                    diagStart.y,
                    diagStart.z + t * (diagEnd.z - diagStart.z)
                };
                Vector3 segEnd = {
                    diagStart.x + (t + 0.05f) * (diagEnd.x - diagStart.x),
                    diagStart.y,
                    diagStart.z + (t + 0.05f) * (diagEnd.z - diagStart.z)
                };
                DrawLine3D(segStart, segEnd, markingColor);
            }
        }
    }
    Color edgeColor = {160, 160, 150, 255};
    Vector3 corners[4] = {
        {floorPos.x - 0.48f, 0.006f, floorPos.z - 0.48f},
        {floorPos.x + 0.48f, 0.006f, floorPos.z - 0.48f},
        {floorPos.x + 0.48f, 0.006f, floorPos.z + 0.48f},
        {floorPos.x - 0.48f, 0.006f, floorPos.z + 0.48f}
    };
    for (int i = 0; i < 4; i++) {
        Vector3 start = corners[i];
        Vector3 end = corners[(i + 1) % 4];
        DrawLine3D(start, end, Fade(edgeColor, 0.4f));
    }
    Color textureColor = {130, 130, 125, 100};
    for (int i = 0; i < 5; i++) {
        float texX = (i * 37 % 100 - 50) / 100.0f * 0.8f;
        float texZ = (i * 73 % 100 - 50) / 100.0f * 0.8f;
        Vector3 texPoint = {floorPos.x + texX, 0.004f, floorPos.z + texZ};
        for (int j = 0; j < 4; j++) {
            Vector3 microPoint = {
                texPoint.x + (j % 2 - 0.5f) * 0.02f,
                texPoint.y,
                texPoint.z + (j / 2 - 0.5f) * 0.02f
            };
            DrawPoint3D(microPoint, Fade(textureColor, 0.6f));
        }
    }
}

void Renderer::DrawTarget(Vector3 position)
{
    if (!modelsLoaded)
        return;
    Vector3 targetPos = {position.x, 0.0f, position.z};
    Color concreteBase = {160, 160, 155, 255};
    Color concreteDark = {120, 120, 115, 255};
    Vector3 basePos = {targetPos.x, 0.02f, targetPos.z};
    DrawCube(basePos, 0.9f, 0.04f, 0.9f, concreteBase);
    Color borderColor = {100, 100, 95, 255};
    DrawCube({targetPos.x, 0.04f, targetPos.z - 0.43f}, 0.86f, 0.04f, 0.04f, borderColor);
    DrawCube({targetPos.x, 0.04f, targetPos.z + 0.43f}, 0.86f, 0.04f, 0.04f, borderColor);
    DrawCube({targetPos.x - 0.43f, 0.04f, targetPos.z}, 0.04f, 0.04f, 0.86f, borderColor);
    DrawCube({targetPos.x + 0.43f, 0.04f, targetPos.z}, 0.04f, 0.04f, 0.86f, borderColor);
    Color markingPaint = {255, 220, 0, 255};
    float lineWidth = 0.05f;
    float lineLength = 0.6f;
    Vector3 hLinePos = {targetPos.x, 0.025f, targetPos.z};
    DrawCube(hLinePos, lineLength, 0.005f, lineWidth, markingPaint);
    Vector3 vLinePos = {targetPos.x, 0.025f, targetPos.z};
    DrawCube(vLinePos, lineWidth, 0.005f, lineLength, markingPaint);
    Color centerColor = {255, 50, 50, 255};
    DrawCylinder(targetPos, 0.08f, 0.08f, 0.03f, 8, centerColor);
    Color plotOrange = {255, 140, 0, 255};
    Color plotWhite = {255, 255, 255, 255};
    Color plotReflector = {255, 255, 0, 255};
    Vector3 plotPositions[4] = {
        {targetPos.x - 0.35f, 0.0f, targetPos.z - 0.35f},
        {targetPos.x + 0.35f, 0.0f, targetPos.z - 0.35f},
        {targetPos.x - 0.35f, 0.0f, targetPos.z + 0.35f},
        {targetPos.x + 0.35f, 0.0f, targetPos.z + 0.35f}
    };
    static float blinkTime = 0.0f;
    blinkTime += GetFrameTime();
    float blinkIntensity = 0.7f + 0.3f * sinf(blinkTime * 4.0f);
    for (int i = 0; i < 4; i++) {
        Vector3 plotPos = plotPositions[i];
        Vector3 baseplotPos = {plotPos.x, 0.05f, plotPos.z};
        DrawCylinder(baseplotPos, 0.04f, 0.04f, 0.1f, 8, concreteDark);
        Vector3 mainplotPos = {plotPos.x, 0.25f, plotPos.z};
        DrawCylinder(mainplotPos, 0.035f, 0.035f, 0.4f, 8, plotOrange);
        for (int j = 0; j < 3; j++) {
            Vector3 bandPos = {plotPos.x, 0.15f + j * 0.1f, plotPos.z};
            DrawCylinder(bandPos, 0.037f, 0.037f, 0.03f, 8, plotWhite);
        }
        Vector3 topPos = {plotPos.x, 0.47f, plotPos.z};
        Color blinkColor = {
            static_cast<unsigned char>(plotReflector.r * blinkIntensity),
            static_cast<unsigned char>(plotReflector.g * blinkIntensity),
            static_cast<unsigned char>(plotReflector.b * blinkIntensity),
            255
        };
        DrawCylinder(topPos, 0.04f, 0.04f, 0.06f, 8, blinkColor);
        if (blinkIntensity > 0.9f) {
            DrawSphere(topPos, 0.08f, Fade(plotReflector, 0.3f));
        }
    }
    static int parcelNumber = static_cast<int>(position.x * 10 + position.z * 100) % 99 + 1;
    Color arrowColor = {255, 100, 100, 255};
    Vector3 arrowPositions[4] = {
        {targetPos.x, 0.026f, targetPos.z - 0.3f},
        {targetPos.x + 0.3f, 0.026f, targetPos.z},
        {targetPos.x, 0.026f, targetPos.z + 0.3f},
        {targetPos.x - 0.3f, 0.026f, targetPos.z}
    };

    for (int i = 0; i < 4; i++) {
        Vector3 arrowPos = arrowPositions[i];
        float angle = i * 90.0f;

        Vector3 arrowBody = arrowPos;
        if (i % 2 == 0) {
            DrawCube(arrowBody, 0.03f, 0.005f, 0.12f, arrowColor);
        } else {
            DrawCube(arrowBody, 0.12f, 0.005f, 0.03f, arrowColor);
        }
        Vector3 arrowTip = {
            arrowPos.x + cosf((angle + 180.0f) * DEG2RAD) * 0.08f,
            arrowPos.y,
            arrowPos.z + sinf((angle + 180.0f) * DEG2RAD) * 0.08f
        };
        DrawSphere(arrowTip, 0.02f, arrowColor);
    }
    Color chainColor = {180, 180, 180, 200};
    for (int i = 0; i < 4; i++) {
        Vector3 start = {plotPositions[i].x, 0.3f, plotPositions[i].z};
        Vector3 end = {plotPositions[(i + 1) % 4].x, 0.3f, plotPositions[(i + 1) % 4].z};
        int segments = 8;
        for (int j = 0; j < segments; j++) {
            float t1 = static_cast<float>(j) / segments;
            float t2 = static_cast<float>(j + 1) / segments;
            Vector3 segStart = {
                start.x + t1 * (end.x - start.x),
                start.y - 0.02f * sinf(t1 * 3.14159f),
                start.z + t1 * (end.z - start.z)
            };
            Vector3 segEnd = {
                start.x + t2 * (end.x - start.x),
                start.y - 0.02f * sinf(t2 * 3.14159f),
                start.z + t2 * (end.z - start.z)
            };
            if (j % 2 == 0) {
                DrawLine3D(segStart, segEnd, chainColor);
            }
        }
    }
    Vector3 signPos = {targetPos.x, 0.08f, targetPos.z - 0.2f};
    DrawCube(signPos, 0.15f, 0.1f, 0.02f, plotOrange);
    DrawCube(signPos, 0.13f, 0.08f, 0.025f, plotWhite);
    Vector3 polePos = {signPos.x, 0.04f, signPos.z - 0.015f};
    DrawCylinder(polePos, 0.008f, 0.008f, 0.08f, 6, {100, 100, 100, 255});
    static float shineTime = 0.0f;
    shineTime += GetFrameTime() * 2.0f;
    if (sinf(shineTime) > 0.7f) {
        Color shineColor = {255, 255, 255, 100};
        for (int i = 0; i < 4; i++) {
            Vector3 shinePos = {plotPositions[i].x, 0.47f, plotPositions[i].z};
            DrawSphere(shinePos, 0.06f, Fade(shineColor, 0.4f));
        }
    }
}

void Renderer::DrawModelEx(Model model, Vector3 position, Vector3 rotationAxis,
                          float rotationAngle, Vector3 scale, Color tint) {
    ::DrawModelEx(model, position, rotationAxis, rotationAngle, scale, tint);
}

void Renderer::DrawBoundingBox(Vector3 position, Vector3 size, Color color)
{
    Vector3 min = {position.x - size.x/2, position.y - size.y/2, position.z - size.z/2};
    Vector3 max = {position.x + size.x/2, position.y + size.y/2, position.z + size.z/2};
    BoundingBox box = {min, max};
    ::DrawBoundingBox(box, color);
}

void Renderer::DrawGrid(int width, int height, float spacing)
{
    for (int x = 0; x <= width; x++) {
        Vector3 start = {x * spacing, 0.0f, 0.0f};
        Vector3 end = {x * spacing, 0.0f, height * spacing};
        DrawLine3D(start, end, DARKGRAY);
    }
    for (int z = 0; z <= height; z++) {
        Vector3 start = {0.0f, 0.0f, z * spacing};
        Vector3 end = {width * spacing, 0.0f, z * spacing};
        DrawLine3D(start, end, DARKGRAY);
    }
}

void Renderer::DrawText3D(const char* text, Vector3 position, float fontSize, Color color)
{
    Vector2 screenPos = GetWorldToScreen(position, camera);
    DrawText(text, (int)screenPos.x, (int)screenPos.y, (int)fontSize, color);
}

void Renderer::Clear(Color color)
{
    ClearBackground(color.r == 0 && color.g == 0 && color.b == 0 ? RAYWHITE : color);
}