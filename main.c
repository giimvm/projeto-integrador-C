#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "IA.LIBI");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("CONSEGUI CARALHO CHUPAAAAA RAYLIB", 200, 200, 20, PINK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}