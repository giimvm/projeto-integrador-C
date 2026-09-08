#include "raylib.h"

int main(void) {
    InitWindow(800, 450, "Meu Jogo em C");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Raylib rodando com sucesso!", 220, 200, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}