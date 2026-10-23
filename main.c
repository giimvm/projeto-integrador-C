// ./main.exe
//FAZENDO AS TRANSICOES ENTRE AS TELAS!!

// ./main.exe
// FAZENDO AS TRANSICOES ENTRE AS TELAS!!

#include "raylib.h"

//Variaveis para o switch case
typedef enum TelaAtual {
    TELA_MENU = 0,
    TELA_JOGO,
    TELA_REGRAS,
    TELA_CONFIG,
    TELA_CREDITOS
} TelaAtual;

// Função para verificação dos cliques
bool BotaoClicado(Rectangle btn, Vector2 posMouse) {
    return CheckCollisionPointRec(posMouse, btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

// Função para atualizar o valor dos sliders de volume
float AtualizarSlider(Rectangle slider, Vector2 posMouse, float valorAtual) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(posMouse, slider)) {
        float novoValor = (posMouse.x - slider.x) / slider.width;
        if (novoValor < 0.0f) novoValor = 0.0f;
        if (novoValor > 1.0f) novoValor = 1.0f;
        return novoValor;
    }
    return valorAtual;
}

// Função de renderização do efeito de destaque
void DesenharEfeitoHover(Rectangle btn, Vector2 posMouse, Color cor) {
    if (CheckCollisionPointRec(posMouse, btn)) {
        DrawRectangleLinesEx(btn, 2, cor);
    }
}

//======= INICIO DO LOOP PRINCIPAL =======
//========================================
int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;

    InitWindow(larguraTela, alturaTela, "Projeto Menu");
    SetTargetFPS(60);

    // ======= CARREGAMENTO DE TEXTURAS =======
    //=========================================
    Texture2D minhaImagem = LoadTexture("src/menu.png");
    Texture2D texConfigAtivado = LoadTexture("src/configuracoes 1.png");   
    Texture2D texConfigDesativado = LoadTexture("src/configuracoes.png"); 
    Texture2D texCreditos = LoadTexture("src/creditos.png");

    Texture2D texRegras[5];
    texRegras[0] = LoadTexture("src/REGRAS 1.png");
    texRegras[1] = LoadTexture("src/REGRAS 2.png");
    texRegras[2] = LoadTexture("src/REGRAS 3.png");
    texRegras[3] = LoadTexture("src/REGRAS 4.png");
    texRegras[4] = LoadTexture("src/REGRAS 5.png");

    int paginaRegras = 0;

    TraceLog(LOG_INFO, "Diretorio de Trabalho: %s", GetWorkingDirectory());

    if (minhaImagem.id == 0 || texConfigAtivado.id == 0 || texConfigDesativado.id == 0 || texCreditos.id == 0) {
        TraceLog(LOG_WARNING, "ERRO: Uma ou mais imagens nao foram encontradas no caminho especificado!");
    }

    Rectangle origMenu = { 0.0f, 0.0f, (float)minhaImagem.width, (float)minhaImagem.height };
    Rectangle origConfig = { 0.0f, 0.0f, (float)texConfigAtivado.width, (float)texConfigAtivado.height };
    Rectangle origCreditos = { 0.0f, 0.0f, (float)texCreditos.width, (float)texCreditos.height };
    Rectangle origRegras = { 0.0f, 0.0f, (float)texRegras[0].width, (float)texRegras[0].height };

    Rectangle dest = { 0.0f, 0.0f, (float)larguraTela, (float)alturaTela };
    Vector2 pontoZero = { 0.0f, 0.0f };

    // ===== DEFINIÇÃO DAS ÁREAS DOS BOTÕES =======
    //=============================================
    Rectangle btnJogar    = { 115, 271, 106, 35 };
    Rectangle btnRegras   = { 137, 321, 104, 37 };
    Rectangle btnConfig   = { 102, 372, 181, 43 };
    Rectangle btnCreditos = { 134, 424, 106, 39 };
   
    Rectangle btnVoltar           = { 100, 520, 90, 30 };
    Rectangle btnTelaCheia        = { 610, 415, 70, 25 };
    Rectangle sliderVolumeGeral   = { 250, 245, 385, 15 };
    Rectangle sliderMusica        = { 250, 283, 385, 15 };
    Rectangle sliderEfeitos       = { 250, 323, 385, 15 };

    Rectangle btnVoltarRegras     = { 90, 520, 110, 35 };
    Rectangle btnContinuarRegras  = { 630, 520, 130, 35 };

    // Variáveis de estado
    TelaAtual telaAtual = TELA_MENU;
    bool telaCheia = true;
    float volGeral = 0.75f;
    float volMusica = 0.55f;
    float volEfeitos = 0.65f;

    Vector2 posMouse = { 0.0f, 0.0f };

    while (!WindowShouldClose()) {
        posMouse = GetMousePosition();

        // ===== TRANSIÇÕES DAS TELAS =====
        //=================================
        switch (telaAtual) {

            //case do menu
            case TELA_MENU:
                if (BotaoClicado(btnJogar, posMouse))         telaAtual = TELA_JOGO;
                else if (BotaoClicado(btnRegras, posMouse))   telaAtual = TELA_REGRAS;
                else if (BotaoClicado(btnConfig, posMouse))   telaAtual = TELA_CONFIG;
                else if (BotaoClicado(btnCreditos, posMouse)) telaAtual = TELA_CREDITOS;
                break;
             
            //case das configurações
            case TELA_CONFIG:
                if (BotaoClicado(btnTelaCheia, posMouse)) {
                    telaCheia = !telaCheia;
                    ToggleFullscreen();
                }

                volGeral   = AtualizarSlider(sliderVolumeGeral, posMouse, volGeral);
                volMusica  = AtualizarSlider(sliderMusica, posMouse, volMusica);
                volEfeitos = AtualizarSlider(sliderEfeitos, posMouse, volEfeitos);

                SetMasterVolume(volGeral);

                if (BotaoClicado(btnVoltar, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    telaAtual = TELA_MENU;
                }
                break;

            
             //case dos créditos
            case TELA_CREDITOS:
                if (BotaoClicado(btnVoltar, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    telaAtual = TELA_MENU;
                }
                break;

            //case do jogo 
            case TELA_JOGO:
                if (BotaoClicado(btnVoltar, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    telaAtual = TELA_MENU;
                }
                break;
            
            //case das regras
            case TELA_REGRAS:
                if (BotaoClicado(btnContinuarRegras, posMouse)) {
                    if (paginaRegras < 4) {
                        paginaRegras++;
                    } else {
                        paginaRegras = 0;
                        telaAtual = TELA_JOGO;
                    }
                }

                if (BotaoClicado(btnVoltarRegras, posMouse)) {
                    if (paginaRegras > 0) {
                        paginaRegras--;
                    } else {
                        telaAtual = TELA_MENU;
                    }
                }

                if (IsKeyPressed(KEY_ESCAPE)) {
                    paginaRegras = 0;
                    telaAtual = TELA_MENU;
                }
                break;
        }

        // ========== RENDERIZAÇÃO DAS IMAGENS =========
        //==============================================
        BeginDrawing();
            ClearBackground(RAYWHITE);

            //switch para desenhar as telas
            switch (telaAtual) {

                //Desenho da tela do menu
                case TELA_MENU:
                    DrawTexturePro(minhaImagem, origMenu, dest, pontoZero, 0.0f, WHITE);
                    DesenharEfeitoHover(btnJogar, posMouse, RED);
                    DesenharEfeitoHover(btnRegras, posMouse, RED);
                    DesenharEfeitoHover(btnConfig, posMouse, RED);
                    DesenharEfeitoHover(btnCreditos, posMouse, RED);
                    break;
                
                    //desenho da tela de configuração
                case TELA_CONFIG: {
                    Texture2D texAtual = telaCheia ? texConfigAtivado : texConfigDesativado;
                    DrawTexturePro(texAtual, origConfig, dest, pontoZero, 0.0f, WHITE);
                    DesenharEfeitoHover(btnVoltar, posMouse, RED);
                    DesenharEfeitoHover(btnTelaCheia, posMouse, PURPLE);
                    break;
                }
                
                //desenho da tela de créditos
                case TELA_CREDITOS:
                    DrawTexturePro(texCreditos, origCreditos, dest, pontoZero, 0.0f, WHITE);
                    DesenharEfeitoHover(btnVoltar, posMouse, RED);
                    break;

               //desenho da tela do jogo, botem aqui os arquivos do jogo 
                case TELA_JOGO:
                    DrawText("--- TELA DO JOGO ---", 260, 200, 30, GREEN);
                    DesenharEfeitoHover(btnVoltar, posMouse, RED);
                    break;
                
                //desenho da tela de regras
                case TELA_REGRAS: 
                    DrawTexturePro(texRegras[paginaRegras], origRegras, dest, pontoZero, 0.0f, WHITE);
                    DesenharEfeitoHover(btnVoltarRegras, posMouse, RED);
                    DesenharEfeitoHover(btnContinuarRegras, posMouse, RED);
                    break;
            }
            
        EndDrawing();
    }

    // Liberação de memoria
    for (int i = 0; i < 5; i++) {
        UnloadTexture(texRegras[i]);
    }
    UnloadTexture(texConfigAtivado);
    UnloadTexture(texConfigDesativado);
    UnloadTexture(texCreditos);
    UnloadTexture(minhaImagem);

    CloseWindow();
    return 0;
}