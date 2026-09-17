#include "raylib.h"

typedef enum GameState { //enum define variaveis de tipo texto para o estados do jogo.
 STATE_MENU,  //funcionam para o switch case, ao invés de usar numeros, usamos os nomes dos estados.
 STATE_GAME,
 STATE_EXIT

} GameState;

int main(void) {
    InitWindow(800, 450, "IA.LIBI.MENUTESTE");
    SetTargetFPS(60);
     
    GameState currentState = STATE_MENU; //variavel de estado do menu, funcionando TIPO um switch case, mas so TIPO um mesmo
    
    //Declaração dos botoes do menu, os numeros ditam onde ele fica, sua altura e largura.
    Rectangle btnPlay = {300, 180, 200, 50};
    Rectangle btnExit = {300, 250, 200, 50};

   //laço principal que vai rodar até o usuario fechar a aba (WindowShouldClose) ou(&&) o estado do jogo ser igual a STATE_EXIT.
    while (!WindowShouldClose() && currentState != STATE_EXIT) {
    Vector2 mousePoint = GetMousePosition();
    
    //AS LINHAS A SEGUIR REALIZAM A CHECAGEM DO CLICK DO USUARIO NOS BOTOES E ONDE ELE SE ENCONTRA

    if (currentState == STATE_MENU) {


       //Condicional criada para a checagem do click no botão de jogar
       //Se o ponto de colisão do mouse(CheckCollisionPointRec) do mouse
       if (CheckCollisionPointRec(mousePoint, btnPlay) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        currentState = STATE_GAME;
    }
      //Condicional criada para checagem do click no botão de sair 
      //se o ponto de colisão do mouse (CheckCollisionPointRec) 
       if (CheckCollisionPointRec(mousePoint, btnExit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
       currentState = STATE_EXIT;
        
       }
  




    }
















   }



    return 0;
}