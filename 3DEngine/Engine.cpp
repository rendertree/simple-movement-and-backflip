// Copyright (c) 2024 Wildan R Wijanarko (@wildan9)

// This software is provided "as-is", without any express or implied warranty. In no event 
// will the authors be held liable for any damages arising from the use of this software.

// Permission is granted to anyone to use this software for any purpose, including commercial 
// applications, and to alter it and redistribute it freely, subject to the following restrictions:

//   1. The origin of this software must not be misrepresented; you must not claim that you 
//   wrote the original software. If you use this software in a product, an acknowledgment 
//   in the product documentation would be appreciated but is not required.

//   2. Altered source versions must be plainly marked as such, and must not be misrepresented
//   as being the original software.

//   3. This notice may not be removed or altered from any source distribution.

#include "Engine.h"

int main(void)
{
    const int screenWidth = 1080;
    const int screenHeight = 780;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "3D Engine");
    SetTargetFPS(60);

    RLCamera camera{};
    Player player{};

    Shader groundPlaneShader = LoadShader("./resources/checkerboard.vs", "./resources/checkerboard.fs");
    Mesh groundPlaneMesh = GenMeshPlane(40.0f, 40.0f, 10, 10);
    Model groundPlaneModel = LoadModelFromMesh(groundPlaneMesh);
    groundPlaneModel.materials[0].shader = groundPlaneShader;

    while (!WindowShouldClose())
    {
        const Vector3& playerPos = player.GetPosition();
        const Vector3& targetPos = player.GetDestination();

        if (IsKeyPressed(KEY_F10)) ToggleBorderlessWindowed();

        camera.Update(playerPos);
        player.Update(camera);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            camera.BeginMode();
                player.Draw();
                DrawGrid(40, 1.0f);
                DrawModel(groundPlaneModel, Vector3{ 0.0f, -0.01f, 0.0f }, 1.0f, WHITE);
                DrawLine3D(Vector3{ playerPos.x, playerPos.y + 0.5f, playerPos.z }, Vector3{ targetPos.x, targetPos.y + 0.5f, targetPos.z }, BLUE);
                DrawSphere(Vector3{ targetPos.x, targetPos.y + 0.5f, targetPos.z }, 0.2f, GREEN);
            camera.EndMode();

            DrawFPS(10, 10);
        EndDrawing();
    }

    UnloadModel(groundPlaneModel);
    UnloadShader(groundPlaneShader);         

    CloseWindow();

    return 0;
}
