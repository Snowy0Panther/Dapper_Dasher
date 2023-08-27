#include"raylib.h"
#include<stdlib.h>

struct FAnimData
{ 
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(FAnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

FAnimData updateAnimData(FAnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime >= data.updateTime)
    {
        data.runningTime = 0;
        // update animation frame
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{
    // ===============================================================================================
    // Window dimensions
    // ===============================================================================================
    const int windowDementions[2] = {512, 320};
    const char* nameOfWindow{"DapperDasher"};
    // ===============================================================================================
    // Initialize the window
    // ===============================================================================================
    InitWindow(windowDementions[0], windowDementions[1], nameOfWindow);
    // ===============================================================================================
    // Acceleration due to gravity (pixel/seconds)/seconds
    // ===============================================================================================
    const int gravity{1'000};

    // ===============================================================================================
    // Nebula variables
    // ===============================================================================================
    Texture2D nebula = LoadTexture("Textures/12_nebula_spritesheet.png");
    // ===============================================================================================
    // Array Nabula's
    // ===============================================================================================
    int sizeOfNabulae{8};
    FAnimData nebulae[sizeOfNabulae];
    for (int i = 0; i < sizeOfNabulae; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8.0f;
        nebulae[i].rec.height = nebula.height/8.0f;
        nebulae[i].pos.y = windowDementions[1] - nebula.height/8.0f;
        nebulae[i].runningTime = 1.0f/12.0f;
        nebulae[i].frame = 0;
        nebulae[i].updateTime = 0.0f;
        nebulae[i].pos.x = windowDementions[0] + i * 300;
    }

    float finishLine{ nebulae[sizeOfNabulae - 1].pos.x };

    // ===============================================================================================
    // Player variables
    // ===============================================================================================
    Texture2D player = LoadTexture("Textures/MyCharPink.png");
    FAnimData playerData;
    playerData.rec.width = player.width/8;
    playerData.rec.height = player.height;
    playerData.rec.x = 0;
    playerData.rec.y = -15;
    playerData.pos.x = windowDementions[0]/2 - playerData.rec.width/2;
    playerData.pos.y = windowDementions[1] - playerData.rec.height;
    playerData.frame = 0;
    playerData.updateTime = 1.0f/12.0f;
    playerData.runningTime = 0.0f;

    // ===============================================================================================
    // player velocity
    // ===============================================================================================
    int velocity{0};
    // ===============================================================================================
    // nebula velocity
    // ===============================================================================================
    int nebulaVelocity{-200};
    // ===============================================================================================
    // Is the rectangle in the air?
    // ===============================================================================================
    bool bIsInAir{};
    // ===============================================================================================
    // animation frame
    // ===============================================================================================

    Texture2D background = LoadTexture("Textures/far-buildings.png");
    Texture2D midground = LoadTexture("Textures/back-buildings.png");
    Texture2D foreground = LoadTexture("Textures/foreground.png");
    float bgX{};
    float mgX{};
    float fgX{};
    // ===============================================================================================
    // Jump velocity (pixels/second)
    // ===============================================================================================
    float jumpVel{-600};

    bool collision{};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // ===============================================================================================
        // delta time (time since last frame)
        // ===============================================================================================
        float dT{GetFrameTime()};
        // ===============================================================================================
        // Start Drawing
        // ===============================================================================================
        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * dT;
        if (bgX <= -background.width * 2)
        {
            bgX = 0.0f;
        }
        mgX -= 40 * dT;
        if (mgX <= -midground.width * 2)
        {
            mgX = 0.0f;
        }
        fgX -= 80 * dT;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0.0f;
        }

        // Draw background
        Vector2 bg1Pos{bgX, 0.0f};
        Vector2 mg1Pos{mgX, -15.0f};
        Vector2 fg1Pos{fgX, -25.0f};

        Vector2 bg2Pos{bgX + background.width * 2, 0.0f};
        Vector2 mg2Pos{mgX + midground.width * 2, 15.0f};
        Vector2 fg2pos{fgX + foreground.width * 2, -25.0f};

        DrawTextureEx(background, bg1Pos, 0.0f, 2.0f, WHITE); 
        DrawTextureEx(background, bg2Pos, 0.0f, 2.0f, WHITE);
        DrawTextureEx(midground, mg1Pos, 0.0f, 2.0f, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0f, 2.0f, WHITE);
        DrawTextureEx(foreground, fg1Pos, 0.0f, 2.0f, WHITE);
        DrawTextureEx(foreground, fg2pos, 0.0f, 2.0f, WHITE);

        // ===============================================================================================
        // perform ground check
        // ===============================================================================================
        if (isOnGround(playerData, windowDementions[1]))
        {
            // ===============================================================================================
            // Regtangle is on the ground
            // ===============================================================================================
            velocity = 0;
            bIsInAir = false;
        }
        else
        {
            // ===============================================================================================
            // Regtangle is in air
            // Apply gravity
            // ===============================================================================================
            velocity += gravity * dT;
            bIsInAir = true;
        }

        // ===============================================================================================
        // jump check
        // ===============================================================================================
        if (IsKeyPressed(KEY_SPACE) && !bIsInAir)
        {
            velocity += jumpVel;
        }

        // ===============================================================================================
        // Update position nebula
        // ===============================================================================================
        for (int i = 0; i < sizeOfNabulae; i++)
        {
            nebulae[i].pos.x += nebulaVelocity * dT;
        }

        // update finis line
        finishLine += nebulaVelocity * dT;

        // ===============================================================================================
        // Update position player 
        // ===============================================================================================
        playerData.pos.y += velocity * dT;

        // ===============================================================================================
        // update animation frame player
        // ===============================================================================================
        playerData.runningTime += dT;
        if (!bIsInAir)
        {
            playerData = updateAnimData(playerData, dT, 7);
        }
        else
        {
            playerData.rec.x = 2 * playerData.rec.width;
        }

        // ===============================================================================================
        // Update nebula animation frame
        // ===============================================================================================
        for (int i = 0; i < sizeOfNabulae; i++)
        {
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        for (FAnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - 2 * pad,
                nebula.rec.height - 2 * pad
            };

            Rectangle playerRec{
                playerData.pos.x,
                playerData.pos.y,
                playerData.rec.width - pad/2,
                playerData.rec.width - pad/2
            };
            if (CheckCollisionRecs(nebRec, playerRec))
            {
                collision = true;
            }
            
        }

        if (collision)
        {
            DrawText("Game Over!", windowDementions[0]/4+25, windowDementions[1]/2, 40, RED);
        }
        else if (playerData.pos.x >= finishLine)
        {
            DrawText("You win!", windowDementions[0]/4+45, windowDementions[1]/2, 40, GREEN);
        }
        
        else
        {
            // ===============================================================================================
            // Draw nebula
            // ===============================================================================================
            for (int i = 0; i < sizeOfNabulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
 
            // ===============================================================================================
            // Draw player
            // ===============================================================================================
            DrawTextureRec(player, playerData.rec, playerData.pos, WHITE);
        }

        // ===============================================================================================
        // Stop Drawing
        // ===============================================================================================
        EndDrawing();
    }
    UnloadTexture(player);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
    return 0;
}