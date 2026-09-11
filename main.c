#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "IA.LIBI");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("TESTANDO O COMMIT RAYLIB", 201, 200, 24, GREY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}