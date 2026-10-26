#include "raylib.h"


typedef enum {
    STATE_MENU = 0,
    STATE_REGRAS,
    STATE_CONFIG,
    STATE_CREDITOS,


    // Subtelas da CENA 1
    STATE_C1_1,
    STATE_C1_2,
    STATE_C1_3,
    STATE_C1_4,
    STATE_C1_5,
    STATE_C1_6,
    STATE_C1_7,
    STATE_C1_8,


    // CENA 2
    STATE_C2_1,
    STATE_C2_2,
    STATE_C2_3,
    STATE_C2_4,
    STATE_C2_5,
    STATE_C2_6,
    STATE_C2_7,
    STATE_C2_8,
    STATE_C2_9,
    STATE_C2_10,
    STATE_C2_11,
    STATE_C2_12,
    STATE_C2_13,
    STATE_C2_14,
    STATE_C2_15,
    STATE_C2_16,
    STATE_C2_17,
    STATE_C2_18,
    STATE_C2_19,
    STATE_C2_20,
    STATE_C2_21,
    STATE_C2_22,
    STATE_C2_23,
    STATE_C2_24,
    STATE_C2_25,         // C2.24.png
    STATE_C2_26,         // C2.25.png
    STATE_CONFIRMA_CAIO, // C2.26.png
    STATE_C2_28,         // C2.27.png


    // RAMIFICAÇÃO JUÍZA E VEREDITO
    STATE_ACUSA_CAIO_1,          
    STATE_ACUSA_CAIO_2,          
    STATE_ACUSA_CAIO_3,          
    STATE_VEREDITO_INCORRETO,    
    STATE_EVIDENCIA_DESBLOQUEADA,
    STATE_DESAFIO_IA              
} GameState;


bool BotaoClicado(Rectangle btn, Vector2 posMouse) {
    return CheckCollisionPointRec(posMouse, btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}


float AtualizarSlider(Rectangle slider, Vector2 posMouse, float valorAtual) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(posMouse, slider)) {
        float novoValor = (posMouse.x - slider.x) / slider.width;
        if (novoValor < 0.0f) novoValor = 0.0f;
        if (novoValor > 1.0f) novoValor = 1.0f;
        return novoValor;
    }
    return valorAtual;
}


void DesenharEfeitoHover(Rectangle btn, Vector2 posMouse, Color cor) {
    // Quadrado vermelho removido
}


int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;


    InitWindow(larguraTela, alturaTela, "Projeto Investigativo");
    SetTargetFPS(60);


    GameState estadoAtual = STATE_MENU;

    // ======= CARREGAMENTO DE TEXTURAS COM SUPORTE MULTI-CAMINHO =======
    Texture2D imgMenu = LoadTexture("src/menu_inicial/menu.png");
    if (imgMenu.id == 0) imgMenu = LoadTexture("jogo_ai.libi/src/menu_inicial/menu.png");
    if (imgMenu.id == 0) imgMenu = LoadTexture("menu_inicial/menu.png");

    Texture2D texConfigAtivado = LoadTexture("src/menu_inicial/configuracoes1.png");
    if (texConfigAtivado.id == 0) texConfigAtivado = LoadTexture("jogo_ai.libi/src/menu_inicial/configuracoes1.png");
    if (texConfigAtivado.id == 0) texConfigAtivado = LoadTexture("menu_inicial/configuracoes1.png");

    Texture2D texConfigDesativado = LoadTexture("src/menu_inicial/configuracoes.png");
    if (texConfigDesativado.id == 0) texConfigDesativado = LoadTexture("jogo_ai.libi/src/menu_inicial/configuracoes.png");
    if (texConfigDesativado.id == 0) texConfigDesativado = LoadTexture("menu_inicial/configuracoes.png");

    Texture2D texCreditos = LoadTexture("src/menu_inicial/creditos.png");
    if (texCreditos.id == 0) texCreditos = LoadTexture("jogo_ai.libi/src/menu_inicial/creditos.png");
    if (texCreditos.id == 0) texCreditos = LoadTexture("menu_inicial/creditos.png");

    Texture2D texRegras[5];
    texRegras[0] = LoadTexture("src/menu_inicial/REGRAS1.png");
    if (texRegras[0].id == 0) texRegras[0] = LoadTexture("jogo_ai.libi/src/menu_inicial/REGRAS1.png");
    if (texRegras[0].id == 0) texRegras[0] = LoadTexture("menu_inicial/REGRAS1.png");

    texRegras[1] = LoadTexture("src/menu_inicial/REGRAS2.png");
    if (texRegras[1].id == 0) texRegras[1] = LoadTexture("jogo_ai.libi/src/menu_inicial/REGRAS2.png");
    if (texRegras[1].id == 0) texRegras[1] = LoadTexture("menu_inicial/REGRAS2.png");

    texRegras[2] = LoadTexture("src/menu_inicial/REGRAS3.png");
    if (texRegras[2].id == 0) texRegras[2] = LoadTexture("jogo_ai.libi/src/menu_inicial/REGRAS3.png");
    if (texRegras[2].id == 0) texRegras[2] = LoadTexture("menu_inicial/REGRAS3.png");

    texRegras[3] = LoadTexture("src/menu_inicial/REGRAS4.png");
    if (texRegras[3].id == 0) texRegras[3] = LoadTexture("jogo_ai.libi/src/menu_inicial/REGRAS4.png");
    if (texRegras[3].id == 0) texRegras[3] = LoadTexture("menu_inicial/REGRAS4.png");

    texRegras[4] = LoadTexture("src/menu_inicial/REGRAS5.png");
    if (texRegras[4].id == 0) texRegras[4] = LoadTexture("jogo_ai.libi/src/menu_inicial/REGRAS5.png");
    if (texRegras[4].id == 0) texRegras[4] = LoadTexture("menu_inicial/REGRAS5.png");

    int paginaRegras = 0;


    // ======= TEXTURAS DA CENA 1 =======
    Texture2D imgC1_1 = LoadTexture("src/cena1/C1.1.png");
    if (imgC1_1.id == 0) imgC1_1 = LoadTexture("cena1/C1.1.png");
   
    Texture2D imgC1_2 = LoadTexture("src/cena1/C1.2.png");
    if (imgC1_2.id == 0) imgC1_2 = LoadTexture("src/cena1/C1 .2.png");
    if (imgC1_2.id == 0) imgC1_2 = LoadTexture("cena1/C1.2.png");


    Texture2D imgC1_3 = LoadTexture("src/cena1/C1.3.png");
    if (imgC1_3.id == 0) imgC1_3 = LoadTexture("cena1/C1.3.png");


    Texture2D imgC1_4 = LoadTexture("src/cena1/C1.4.png");
    if (imgC1_4.id == 0) imgC1_4 = LoadTexture("cena1/C1.4.png");


    Texture2D imgC1_5 = LoadTexture("src/cena1/C1.5.png");
    if (imgC1_5.id == 0) imgC1_5 = LoadTexture("cena1/C1.5.png");


    Texture2D imgC1_6 = LoadTexture("src/cena1/C1.6.png");
    if (imgC1_6.id == 0) imgC1_6 = LoadTexture("cena1/C1.6.png");


    Texture2D imgC1_7 = LoadTexture("src/cena1/C1.7.png");
    if (imgC1_7.id == 0) imgC1_7 = LoadTexture("cena1/C1.7.png");


    Texture2D imgC1_8 = LoadTexture("src/cena1/C1.8.png");
    if (imgC1_8.id == 0) imgC1_8 = LoadTexture("cena1/C1.8.png");


    // PERSONAGENS
    Texture2D imgGabriel = LoadTexture("src/cena1/fotogabriel.jpeg");
    if (imgGabriel.id == 0) imgGabriel = LoadTexture("cena1/fotogabriel.jpeg");


    Texture2D imgCaio = LoadTexture("src/cena1/fotocaio.jpeg");
    if (imgCaio.id == 0) imgCaio = LoadTexture("cena1/fotocaio.jpeg");


    // ======= TEXTURAS DA CENA 2 =======
    Texture2D imgC2_1    = LoadTexture("src/cena2/C2.0.png");
    Texture2D imgC2_2    = LoadTexture("src/cena2/C2.1.png");
    Texture2D imgC2_3    = LoadTexture("src/cena2/C2.2.png");
    Texture2D imgC2_4    = LoadTexture("src/cena2/C2.3.png");
    Texture2D imgC2_5    = LoadTexture("src/cena2/C2.4.png");
    Texture2D imgC2_6    = LoadTexture("src/cena2/C2.5.png");
    Texture2D imgC2_7    = LoadTexture("src/cena2/C2.6.png");
    Texture2D imgC2_8    = LoadTexture("src/cena2/C2.7.png");
    Texture2D imgC2_9    = LoadTexture("src/cena2/C2.8.png");
    Texture2D imgC2_10   = LoadTexture("src/cena2/C2.9.png");
    Texture2D imgC2_11   = LoadTexture("src/cena2/C2.10.png");
    Texture2D imgC2_12   = LoadTexture("src/cena2/C2.11.png");
    Texture2D imgC2_13   = LoadTexture("src/cena2/C2.12.png");
    Texture2D imgC2_14   = LoadTexture("src/cena2/C2.13.png");
    Texture2D imgC2_15   = LoadTexture("src/cena2/C2.14.png");
    Texture2D imgC2_16   = LoadTexture("src/cena2/C2.15.png");
    Texture2D imgC2_17   = LoadTexture("src/cena2/C2.16.png");
    Texture2D imgC2_18   = LoadTexture("src/cena2/C2.17.png");
    Texture2D imgC2_19   = LoadTexture("src/cena2/C2.18.png");    
    Texture2D imgC2_20   = LoadTexture("src/cena2/C2.19.png");
    Texture2D imgC2_21   = LoadTexture("src/cena2/C2.20.png");
    Texture2D imgC2_22   = LoadTexture("src/cena2/C2.21.png");
    Texture2D imgC2_23   = LoadTexture("src/cena2/C2.22.png");
    Texture2D imgC2_24   = LoadTexture("src/cena2/C2.23.png");
    Texture2D imgC2_25   = LoadTexture("src/cena2/C2.24.png");
    Texture2D imgC2_26   = LoadTexture("src/cena2/C2.25.png");
    Texture2D imgC2_27   = LoadTexture("src/cena2/C2.26.png");
    Texture2D imgC2_28   = LoadTexture("src/cena2/C2.27.png");


    // RAMIFICAÇÃO E RESULTADOS
    Texture2D imgF1_1    = LoadTexture("src/cena2/F1.1.png");
    Texture2D imgF1_2    = LoadTexture("src/cena2/F1.2.png");
    Texture2D imgF1_3    = LoadTexture("src/cena2/F1.3.png");
    Texture2D imgP1      = LoadTexture("src/cena2/P1.png");
    Texture2D imgP1_1    = LoadTexture("src/cena2/P1.1.png");
    Texture2D imgP1_2    = LoadTexture("src/cena2/P1.2.png");


    Texture2D* todasTexturas[] = {
        &imgMenu, &texConfigAtivado, &texConfigDesativado, &texCreditos,
        &texRegras[0], &texRegras[1], &texRegras[2], &texRegras[3], &texRegras[4],
        &imgC1_1, &imgC1_2, &imgC1_3, &imgC1_4, &imgC1_5, &imgC1_6, &imgC1_7, &imgC1_8,
        &imgGabriel, &imgCaio, &imgC2_1, &imgC2_2, &imgC2_3, &imgC2_4, &imgC2_5, &imgC2_6, &imgC2_7, &imgC2_8,
        &imgC2_9, &imgC2_10, &imgC2_11, &imgC2_12, &imgC2_13, &imgC2_14, &imgC2_15, &imgC2_16, &imgC2_17,
        &imgC2_18, &imgC2_19, &imgC2_20, &imgC2_21, &imgC2_22, &imgC2_23, &imgC2_24, &imgC2_25, &imgC2_26, &imgC2_27, &imgC2_28,
        &imgF1_1, &imgF1_2, &imgF1_3, &imgP1, &imgP1_1, &imgP1_2
    };
    int totalTexturas = sizeof(todasTexturas) / sizeof(todasTexturas[0]);


    for (int i = 0; i < totalTexturas; i++) {
        if (todasTexturas[i]->id != 0) {
            GenTextureMipmaps(todasTexturas[i]);
            SetTextureFilter(*todasTexturas[i], TEXTURE_FILTER_BILINEAR);
        }
    }


    Rectangle orig         = { 0.0f, 0.0f, 0.0f, 0.0f };
    Rectangle origMenu     = { 0.0f, 0.0f, (float)imgMenu.width, (float)imgMenu.height };
    Rectangle origConfig   = { 0.0f, 0.0f, (float)texConfigAtivado.width, (float)texConfigAtivado.height };
    Rectangle origCreditos = { 0.0f, 0.0f, (float)texCreditos.width, (float)texCreditos.height };
    Rectangle origRegras   = { 0.0f, 0.0f, (float)texRegras[0].width, (float)texRegras[0].height };


    Rectangle dest = { 0.0f, 0.0f, (float)larguraTela, (float)alturaTela };
    Vector2 pontoZero = { 0.0f, 0.0f };


    // ======= BOTÕES E ÁREAS DE CLIQUE =======
    Rectangle btnJogar    = { 115, 271, 106, 35 };
    Rectangle btnRegras   = { 137, 321, 104, 37 };
    Rectangle btnConfig   = { 102, 372, 181, 43 };
    Rectangle btnCreditos = { 134, 424, 106, 39 };


    Rectangle btnVoltarMenu = { 100, 520, 90, 30 };
    Rectangle btnTelaCheia  = { 610, 415, 70, 25 };
    Rectangle sliderVolumeGeral = { 250, 245, 385, 15 };
    Rectangle sliderMusica      = { 250, 283, 385, 15 };
    Rectangle sliderEfeitos     = { 250, 323, 385, 15 };


    Rectangle btnVoltarRegras   = { 90, 520, 110, 35 };
    Rectangle btnContinuarRegras = { 630, 520, 130, 35 };


    Rectangle btnLogin  = { 340, 400, 120, 45 };
    Rectangle btnVoltar = { 15, 20, 160, 60 }; // Botão de Voltar no canto superior esquerdo


    Rectangle destFotoGabriel = { 440, 160, 230, 270 };
    Rectangle destFotoCaio    = { 440, 160, 230, 270 };
    Rectangle btnFotoCaio     = { 440, 160, 230, 270 };


    Rectangle btnAvancarC2 = { 660, 450, 100, 100 };


    Rectangle btnContinuarInvestigacao = { 280, 370, 250, 70 };
    Rectangle btnResolverDesafio       = { 280, 370, 250, 70 };
    Rectangle btnAcessarEvidencia      = { 280, 370, 250, 70 };
    Rectangle btnTentarNovamente = { 320, 425, 160, 50 };


    Rectangle btnProvaVerdadeira = { 250, 420, 140, 40 };
    Rectangle btnProvaManipulada = { 405, 420, 140, 40 };


    Rectangle btnOpcaoA = { 180, 270, 440, 40 };
    Rectangle btnOpcaoB = { 180, 325, 440, 40 };
    Rectangle btnOpcaoC = { 180, 380, 440, 40 };
    Rectangle btnOpcaoD = { 180, 435, 440, 40 };


    Rectangle btnAcusaCaioSim = { 180, 270, 440, 40 };
    Rectangle btnAcusaCaioNao = { 180, 325, 440, 40 };


    bool telaCheia = true;
    float volGeral = 0.75f;
    float volMusica = 0.55f;
    float volEfeitos = 0.65f;


    Vector2 posMouse = { 0.0f, 0.0f };


    while (!WindowShouldClose()) {
        posMouse = GetMousePosition();


        switch (estadoAtual) {
            case STATE_MENU:
                if (BotaoClicado(btnJogar, posMouse)) estadoAtual = STATE_C1_1;
                else if (BotaoClicado(btnRegras, posMouse)) {
                    paginaRegras = 0;
                    estadoAtual = STATE_REGRAS;
                }
                else if (BotaoClicado(btnConfig, posMouse)) estadoAtual = STATE_CONFIG;
                else if (BotaoClicado(btnCreditos, posMouse)) estadoAtual = STATE_CREDITOS;
                break;


            case STATE_CONFIG:
                if (BotaoClicado(btnTelaCheia, posMouse)) {
                    telaCheia = !telaCheia;
                    ToggleFullscreen();
                }


                volGeral   = AtualizarSlider(sliderVolumeGeral, posMouse, volGeral);
                volMusica  = AtualizarSlider(sliderMusica, posMouse, volMusica);
                volEfeitos = AtualizarSlider(sliderEfeitos, posMouse, volEfeitos);


                SetMasterVolume(volGeral);


                if (BotaoClicado(btnVoltarMenu, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_CREDITOS:
                if (BotaoClicado(btnVoltarMenu, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_REGRAS:
                if (BotaoClicado(btnContinuarRegras, posMouse)) {
                    if (paginaRegras < 4) {
                        paginaRegras++;
                    } else {
                        paginaRegras = 0;
                        estadoAtual = STATE_C1_1;
                    }
                }


                if (BotaoClicado(btnVoltarRegras, posMouse)) {
                    if (paginaRegras > 0) {
                        paginaRegras--;
                    } else {
                        estadoAtual = STATE_MENU;
                    }
                }


                if (IsKeyPressed(KEY_ESCAPE)) {
                    paginaRegras = 0;
                    estadoAtual = STATE_MENU;
                }
                break;


            // --- CENA 1 ---
            case STATE_C1_1:
                // Seta de voltar na tela C1.1 retorna ao Menu Principal
                if (BotaoClicado(btnVoltar, posMouse) || IsKeyPressed(KEY_ESCAPE)) estadoAtual = STATE_MENU;
                else if (BotaoClicado(btnLogin, posMouse)) estadoAtual = STATE_C1_2;
                break;


            case STATE_C1_2:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_1;
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C1_3;
                break;


            case STATE_C1_3:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_2;
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C1_4;
                break;


            case STATE_C1_4:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_3;
                else if (BotaoClicado(btnFotoCaio, posMouse)) estadoAtual = STATE_C1_5;
                break;


            case STATE_C1_5:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_4;
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C1_6;
                break;


            case STATE_C1_6:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_5;
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C1_7;
                break;


            case STATE_C1_7:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_6;
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C1_8;
                break;


            case STATE_C1_8:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_7;
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C2_1;
                break;


            // --- CENA 2 ---
            case STATE_C2_1:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_8;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_2;
                break;


            case STATE_C2_2:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_1;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_3;
                break;


            case STATE_C2_3:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_2;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_4;
                break;


            case STATE_C2_4:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_3;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_5;
                break;


            case STATE_C2_5:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_4;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_6;
                break;


            case STATE_C2_6:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_5;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_7;
                break;


            case STATE_C2_7:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_6;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_8;
                break;


            case STATE_C2_8:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_7;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_9;
                break;


            case STATE_C2_9:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_8;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_10;
                break;


            case STATE_C2_10:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_9;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_11;
                break;


            case STATE_C2_11:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_10;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_12;
                break;


            case STATE_C2_12:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_11;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_13;
                break;


            case STATE_C2_13:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_12;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_14;
                break;


            case STATE_C2_14:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_13;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_15;
                break;


            case STATE_C2_15:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_14;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_16;
                break;


            case STATE_C2_16:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_15;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_17;
                break;


            case STATE_C2_17:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_16;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_18;
                break;


            case STATE_C2_18:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_17;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_19;
                break;


            case STATE_C2_19:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_18;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_20;
                break;


            case STATE_C2_20:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_19;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_21;
                break;


            case STATE_C2_21:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_20;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_22;
                break;


            case STATE_C2_22:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_21;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_23;
                break;


            case STATE_C2_23:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_22;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_24;
                break;


            case STATE_C2_24:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C2_23;
                else if (BotaoClicado(btnAvancarC2, posMouse)) estadoAtual = STATE_C2_25;
                break;


            case STATE_C2_25:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C2_24;
                }
                else if (BotaoClicado(btnResolverDesafio, posMouse)) {
                    estadoAtual = STATE_C2_26;
                }
                break;


            case STATE_C2_26:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C2_25;
                }
                else if (BotaoClicado(btnOpcaoA, posMouse)) {
                    estadoAtual = STATE_EVIDENCIA_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnOpcaoB, posMouse) || BotaoClicado(btnOpcaoC, posMouse)) {
                    estadoAtual = STATE_C2_28;
                }
                else if (BotaoClicado(btnOpcaoD, posMouse)) {
                    estadoAtual = STATE_CONFIRMA_CAIO;
                }
                break;


            case STATE_CONFIRMA_CAIO:
                if (BotaoClicado(btnVoltar, posMouse) || BotaoClicado(btnAcusaCaioNao, posMouse)) {
                    estadoAtual = STATE_C2_26;
                }
                else if (BotaoClicado(btnAcusaCaioSim, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_1;
                }
                break;


            case STATE_EVIDENCIA_DESBLOQUEADA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C2_26;
                }
                else if (BotaoClicado(btnAcessarEvidencia, posMouse)) {
                    estadoAtual = STATE_DESAFIO_IA;
                }
                break;


            case STATE_DESAFIO_IA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_EVIDENCIA_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnProvaVerdadeira, posMouse) || BotaoClicado(btnProvaManipulada, posMouse)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_ACUSA_CAIO_1:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_CONFIRMA_CAIO;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_2;
                }
                break;


            case STATE_ACUSA_CAIO_2:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_1;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_3;
                }
                break;


            case STATE_ACUSA_CAIO_3:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_2;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_VEREDITO_INCORRETO;
                }
                break;


            case STATE_VEREDITO_INCORRETO:
                if (BotaoClicado(btnVoltar, posMouse) || BotaoClicado(btnTentarNovamente, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_C2_28:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C2_26;
                }
                else if (BotaoClicado(btnContinuarInvestigacao, posMouse) || BotaoClicado(btnTentarNovamente, posMouse) || BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_MENU;
                }
                break;
        }
        // ======= RENDERIZAÇÃO DAS TELAS =======
        BeginDrawing();
        ClearBackground(BLACK);


        switch (estadoAtual) {
            case STATE_MENU:
                if (imgMenu.id != 0) {
                    origMenu = (Rectangle){ 0.0f, 0.0f, (float)imgMenu.width, (float)imgMenu.height };
                    DrawTexturePro(imgMenu, origMenu, dest, pontoZero, 0.0f, WHITE);
                } else {
                    DrawText("AI.LIBE - MENU", 310, 150, 24, RAYWHITE);
                    DrawText("JOGAR", btnJogar.x, btnJogar.y, 22, RAYWHITE);
                    DrawText("REGRAS", btnRegras.x, btnRegras.y, 22, RAYWHITE);
                    DrawText("CONFIGURACOES", btnConfig.x, btnConfig.y, 22, RAYWHITE);
                    DrawText("CREDITOS", btnCreditos.x, btnCreditos.y, 22, RAYWHITE);
                }
                break;


            case STATE_CONFIG: {
                Texture2D texAtual = telaCheia ? texConfigAtivado : texConfigDesativado;
                if (texAtual.id != 0) {
                    origConfig = (Rectangle){ 0.0f, 0.0f, (float)texAtual.width, (float)texAtual.height };
                    DrawTexturePro(texAtual, origConfig, dest, pontoZero, 0.0f, WHITE);
                } else {
                    DrawText("CONFIGURACOES", 300, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", btnVoltarMenu.x, btnVoltarMenu.y, 20, RAYWHITE);
                }
                break;
            }


            case STATE_CREDITOS:
                if (texCreditos.id != 0) {
                    origCreditos = (Rectangle){ 0.0f, 0.0f, (float)texCreditos.width, (float)texCreditos.height };
                    DrawTexturePro(texCreditos, origCreditos, dest, pontoZero, 0.0f, WHITE);
                } else {
                    DrawText("CREDITOS", 340, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", btnVoltarMenu.x, btnVoltarMenu.y, 20, RAYWHITE);
                }
                break;


            case STATE_REGRAS:
                if (texRegras[paginaRegras].id != 0) {
                    origRegras = (Rectangle){ 0.0f, 0.0f, (float)texRegras[paginaRegras].width, (float)texRegras[paginaRegras].height };
                    DrawTexturePro(texRegras[paginaRegras], origRegras, dest, pontoZero, 0.0f, WHITE);
                } else {
                    DrawText("REGRAS DO JOGO", 300, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", btnVoltarRegras.x, btnVoltarRegras.y, 20, RAYWHITE);
                    DrawText("CONTINUAR", btnContinuarRegras.x, btnContinuarRegras.y, 20, RAYWHITE);
                }
                break;


            // --- CENA 1 ---
            case STATE_C1_1:
                if (imgC1_1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_1.width, (float)imgC1_1.height };
                    DrawTexturePro(imgC1_1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_2:
                if (imgC1_2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_2.width, (float)imgC1_2.height };
                    DrawTexturePro(imgC1_2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_3:
                if (imgC1_3.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_3.width, (float)imgC1_3.height };
                    DrawTexturePro(imgC1_3, orig, dest, pontoZero, 0.0f, WHITE);
                }
                if (imgGabriel.id != 0) {
                    Rectangle origGabriel = { 0.0f, 0.0f, (float)imgGabriel.width, (float)imgGabriel.height };
                    DrawTexturePro(imgGabriel, origGabriel, destFotoGabriel, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_4:
                if (imgC1_4.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_4.width, (float)imgC1_4.height };
                    DrawTexturePro(imgC1_4, orig, dest, pontoZero, 0.0f, WHITE);
                }
                if (imgCaio.id != 0) {
                    Rectangle origCaio = { 0.0f, 0.0f, (float)imgCaio.width, (float)imgCaio.height };
                    DrawTexturePro(imgCaio, origCaio, destFotoCaio, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_5:
                if (imgC1_5.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_5.width, (float)imgC1_5.height };
                    DrawTexturePro(imgC1_5, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_6:
                if (imgC1_6.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_6.width, (float)imgC1_6.height };
                    DrawTexturePro(imgC1_6, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_7:
                if (imgC1_7.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_7.width, (float)imgC1_7.height };
                    DrawTexturePro(imgC1_7, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C1_8:
                if (imgC1_8.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC1_8.width, (float)imgC1_8.height };
                    DrawTexturePro(imgC1_8, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            // --- CENA 2 ---
            case STATE_C2_1:
                if (imgC2_1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_1.width, (float)imgC2_1.height };
                    DrawTexturePro(imgC2_1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_2:
                if (imgC2_2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_2.width, (float)imgC2_2.height };
                    DrawTexturePro(imgC2_2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_3:
                if (imgC2_3.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_3.width, (float)imgC2_3.height };
                    DrawTexturePro(imgC2_3, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_4:
                if (imgC2_4.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_4.width, (float)imgC2_4.height };
                    DrawTexturePro(imgC2_4, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_5:
                if (imgC2_5.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_5.width, (float)imgC2_5.height };
                    DrawTexturePro(imgC2_5, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_6:
                if (imgC2_6.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_6.width, (float)imgC2_6.height };
                    DrawTexturePro(imgC2_6, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_7:
                if (imgC2_7.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_7.width, (float)imgC2_7.height };
                    DrawTexturePro(imgC2_7, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_8:
                if (imgC2_8.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_8.width, (float)imgC2_8.height };
                    DrawTexturePro(imgC2_8, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_9:
                if (imgC2_9.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_9.width, (float)imgC2_9.height };
                    DrawTexturePro(imgC2_9, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_10:
                if (imgC2_10.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_10.width, (float)imgC2_10.height };
                    DrawTexturePro(imgC2_10, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_11:
                if (imgC2_11.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_11.width, (float)imgC2_11.height };
                    DrawTexturePro(imgC2_11, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_12:
                if (imgC2_12.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_12.width, (float)imgC2_12.height };
                    DrawTexturePro(imgC2_12, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_13:
                if (imgC2_13.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_13.width, (float)imgC2_13.height };
                    DrawTexturePro(imgC2_13, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_14:
                if (imgC2_14.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_14.width, (float)imgC2_14.height };
                    DrawTexturePro(imgC2_14, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_15:
                if (imgC2_15.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_15.width, (float)imgC2_15.height };
                    DrawTexturePro(imgC2_15, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_16:
                if (imgC2_16.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_16.width, (float)imgC2_16.height };
                    DrawTexturePro(imgC2_16, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_17:
                if (imgC2_17.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_17.width, (float)imgC2_17.height };
                    DrawTexturePro(imgC2_17, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_18:
                if (imgC2_18.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_18.width, (float)imgC2_18.height };
                    DrawTexturePro(imgC2_18, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_19:
                if (imgC2_19.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_19.width, (float)imgC2_19.height };
                    DrawTexturePro(imgC2_19, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_20:
                if (imgC2_20.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_20.width, (float)imgC2_20.height };
                    DrawTexturePro(imgC2_20, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_21:
                if (imgC2_21.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_21.width, (float)imgC2_21.height };
                    DrawTexturePro(imgC2_21, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_22:
                if (imgC2_22.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_22.width, (float)imgC2_22.height };
                    DrawTexturePro(imgC2_22, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_23:
                if (imgC2_23.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_23.width, (float)imgC2_23.height };
                    DrawTexturePro(imgC2_23, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_24:
                if (imgC2_24.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_24.width, (float)imgC2_24.height };
                    DrawTexturePro(imgC2_24, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_25:
                if (imgC2_25.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_25.width, (float)imgC2_25.height };
                    DrawTexturePro(imgC2_25, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_26:
                if (imgC2_26.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_26.width, (float)imgC2_26.height };
                    DrawTexturePro(imgC2_26, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_CONFIRMA_CAIO:
                if (imgC2_27.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_27.width, (float)imgC2_27.height };
                    DrawTexturePro(imgC2_27, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_ACUSA_CAIO_1:
                if (imgF1_1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgF1_1.width, (float)imgF1_1.height };
                    DrawTexturePro(imgF1_1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_ACUSA_CAIO_2:
                if (imgF1_2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgF1_2.width, (float)imgF1_2.height };
                    DrawTexturePro(imgF1_2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_ACUSA_CAIO_3:
                if (imgF1_3.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgF1_3.width, (float)imgF1_3.height };
                    DrawTexturePro(imgF1_3, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_VEREDITO_INCORRETO:
                if (imgP1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP1.width, (float)imgP1.height };
                    DrawTexturePro(imgP1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_EVIDENCIA_DESBLOQUEADA:
                if (imgP1_1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP1_1.width, (float)imgP1_1.height };
                    DrawTexturePro(imgP1_1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_DESAFIO_IA:
                if (imgP1_2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP1_2.width, (float)imgP1_2.height };
                    DrawTexturePro(imgP1_2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C2_28:
                if (imgC2_28.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC2_28.width, (float)imgC2_28.height };
                    DrawTexturePro(imgC2_28, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;
        }


        EndDrawing();
    }


    // ======= UNLOAD TEXTURES =======
    for (int i = 0; i < 5; i++) {
        UnloadTexture(texRegras[i]);
    }
    UnloadTexture(texConfigAtivado);
    UnloadTexture(texConfigDesativado);
    UnloadTexture(texCreditos);
    UnloadTexture(imgMenu);


    UnloadTexture(imgC1_1);
    UnloadTexture(imgC1_2);
    UnloadTexture(imgC1_3);
    UnloadTexture(imgC1_4);
    UnloadTexture(imgC1_5);
    UnloadTexture(imgC1_6);
    UnloadTexture(imgC1_7);
    UnloadTexture(imgC1_8);


    UnloadTexture(imgGabriel);
    UnloadTexture(imgCaio);


    UnloadTexture(imgC2_1);
    UnloadTexture(imgC2_2);
    UnloadTexture(imgC2_3);
    UnloadTexture(imgC2_4);
    UnloadTexture(imgC2_5);
    UnloadTexture(imgC2_6);
    UnloadTexture(imgC2_7);
    UnloadTexture(imgC2_8);
    UnloadTexture(imgC2_9);
    UnloadTexture(imgC2_10);
    UnloadTexture(imgC2_11);
    UnloadTexture(imgC2_12);
    UnloadTexture(imgC2_13);
    UnloadTexture(imgC2_14);
    UnloadTexture(imgC2_15);
    UnloadTexture(imgC2_16);
    UnloadTexture(imgC2_17);
    UnloadTexture(imgC2_18);
    UnloadTexture(imgC2_19);
    UnloadTexture(imgC2_20);
    UnloadTexture(imgC2_21);
    UnloadTexture(imgC2_22);
    UnloadTexture(imgC2_23);
    UnloadTexture(imgC2_24);
    UnloadTexture(imgC2_25);
    UnloadTexture(imgC2_26);
    UnloadTexture(imgC2_27);
    UnloadTexture(imgC2_28);


    UnloadTexture(imgF1_1);
    UnloadTexture(imgF1_2);
    UnloadTexture(imgF1_3);
    UnloadTexture(imgP1);
    UnloadTexture(imgP1_1);
    UnloadTexture(imgP1_2);


    CloseWindow();
    return 0;
}