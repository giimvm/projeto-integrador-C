#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "IA.LIBI");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("CONSEGUI CARALHO CHUPA RAYLIB", 200, 200, 40, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}