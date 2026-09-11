#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "IA.LIBI");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("APRENDI RAYLIB ESPERO", 201, 200, 24, PINK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}