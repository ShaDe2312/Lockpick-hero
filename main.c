#include "raylib.h"
#include <math.h>
// Move these to a header file later
/* Utility Functions */
#define MIN(a,b)    (a < b) ? a : b

/* Constant Definitions */
#define MAIN_WINDOW_COLOR   0x453A49FF
#define LOCK_COLOR          0xDB7F8EFF
#define PICK_COLOR          0xF0EBD8FF
#define SCREEN_WIDTH        800
#define SCREEN_HEIGHT       400


#define ACCELERATE_FACTOR   180
#define MAX_ACCELERATION_FACTOR 10.0f
#define MAX_ANGULAR_VELOCITY    15.0f
#define ACCELERATION_DAMPENER   0.3f 


int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LockpickHero");  
    SetTargetFPS(60);

    const float LockRadius = (MIN(SCREEN_WIDTH,SCREEN_HEIGHT)/2) * 0.8f;
    /* X&Y coordinates of origin of both lock & pick is same*/
    const int PickLockPosX = SCREEN_WIDTH/2;
    const int PickLockPosY = SCREEN_HEIGHT/2;
    
    float CurrentPickAngle = 0.0f;
    /*Count of frames for which key is held, used for pick acceleration */
    float KeyDownFrameCount = 0.0f;
    float AccelerationMultiplier = 0.0f;
    float AngularVelocity = 0.0f;

    // --- Constants for Drawing Pick ---
    const float PICK_RADIUS_SCALE = 1.2f;
    const float PICK_WIDTH_DEGREES = 10.0f;
    const int PICK_SECTOR_SEGMENTS = 30;

    while(!WindowShouldClose())
    {
        bool IsUpPressed = IsKeyDown(KEY_UP) || IsKeyDown(KEY_W);
        bool IsDownPressed = IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S);
        /* Accelerate as long as key is pressed. */
        if(IsUpPressed || IsDownPressed ){
            KeyDownFrameCount++;
            AccelerationMultiplier = KeyDownFrameCount / ACCELERATE_FACTOR;
            if(AccelerationMultiplier > MAX_ACCELERATION_FACTOR)
                AccelerationMultiplier = MAX_ACCELERATION_FACTOR;

            float DeltaVelocity = (AccelerationMultiplier * ACCELERATION_DAMPENER);
            if(IsUpPressed){
            AngularVelocity += DeltaVelocity;
            if(AngularVelocity > MAX_ANGULAR_VELOCITY)
                AngularVelocity = MAX_ANGULAR_VELOCITY;
            } else {
            AngularVelocity -= DeltaVelocity;
            if(AngularVelocity < -MAX_ANGULAR_VELOCITY)
                AngularVelocity = -MAX_ANGULAR_VELOCITY;
            }
        }
        else {
            /* No longer accelerating, start slowing down */
            /*TODO: Handle abrupt stop.*/
            AngularVelocity = 0;
            KeyDownFrameCount = 0.0f;
            AccelerationMultiplier = 0.0f;
        }
        
        CurrentPickAngle += AngularVelocity;

        /* Normalising startAngle, keeping less than 360. */
        CurrentPickAngle = fmodf(CurrentPickAngle, 360.0f);
        if(CurrentPickAngle < 0.0f)
            CurrentPickAngle += 360.0f;

        BeginDrawing();
            ClearBackground(GetColor(MAIN_WINDOW_COLOR));
            DrawCircle(PickLockPosX,PickLockPosY,LockRadius,GetColor(LOCK_COLOR));
            DrawCircleSector((Vector2){PickLockPosX,PickLockPosY}, 
                (PICK_RADIUS_SCALE)*LockRadius, 
                CurrentPickAngle, CurrentPickAngle + PICK_WIDTH_DEGREES, 
                PICK_SECTOR_SEGMENTS, GetColor(PICK_COLOR));
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
