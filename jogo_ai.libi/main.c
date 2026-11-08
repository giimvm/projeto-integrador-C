#include "raylib.h"
#include <stdio.h>
#include "progresso.h"

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
    STATE_DESAFIO_IA,

    // CENA 3
    STATE_C3_1,
    STATE_C3_2,
    STATE_C3_3,
    STATE_C3_4,
    STATE_C3_PERDIDA,
    STATE_C3_DESBLOQUEADA,
    STATE_C3_AUDIO,
    STATE_C3_PERGUNTA,

    // CENA 4
        // CENA 4
    STATE_C4_1,
    STATE_C4_2,
    STATE_C4_3,
    STATE_C4_4,
    STATE_C4_5,
    STATE_C4_6,
    STATE_C4_7,
    STATE_C4_8,
    STATE_C4_9,
    STATE_C4_10,
    STATE_C4_11,
    STATE_C4_12,
    STATE_C4_13,
    STATE_C4_14,        // <-- NOVO (c4.14 é diálogo)
    STATE_C4_EVIDENCIA, // c4.15 - nova evidência encontrada
    STATE_C4_DESAFIO,   // c4.16 - questão de lógica
    STATE_C4_PERDIDA,   // c4.17 - evidência perdida
    STATE_C4_DESBLOQUEADA, // c4.18 - evidência desbloqueada (NOVO)
    STATE_C4_ORIGEM,    // p4_1
    STATE_C4_PERGUNTA   // p4_2
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


// Tenta carregar uma imagem da Cena 3 em várias pastas e com dois nomes possíveis
Texture2D CarregarCena3(const char* nome1, const char* nome2) {
    const char* pastas[] = { "src/cena3/", "cena3/", "src/cena2/", "" };
    const char* nomes[2] = { nome1, nome2 };
    char caminho[256];

    for (int n = 0; n < 2; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "CENA3: imagem NAO encontrada: %s (nem %s)", nome1, nome2 ? nome2 : "-");
    return (Texture2D){ 0 };
}


// Tenta carregar uma imagem da Cena 4 em várias pastas e com até 3 nomes possíveis
Texture2D CarregarCena4(const char* nome1, const char* nome2, const char* nome3) {
    const char* pastas[] = { "src/cena4/", "cena4/", "src/cena3/", "" };
    const char* nomes[3] = { nome1, nome2, nome3 };
    char caminho[256];

    for (int n = 0; n < 3; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "CENA4: imagem NAO encontrada: %s", nome1);
    return (Texture2D){ 0 };
}


int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;


    InitWindow(larguraTela, alturaTela, "Projeto Investigativo");
    InitAudioDevice();
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


    // ======= TEXTURAS DA CENA 3 =======
    // (coloque as imagens em src/cena3/ - o jogo também procura em cena3/, src/cena2/ e na raiz)
    Texture2D imgC3_1 = CarregarCena3("C3.1.png", "C3_1.png");  // Chefe: dispositivo
    Texture2D imgC3_2 = CarregarCena3("C3.2.png", "C3_2.png");  // Chefe: mensagem
    Texture2D imgC3_3 = CarregarCena3("C3.3.png", "C3_3.png");  // Nova evidência encontrada
    Texture2D imgC3_4 = CarregarCena3("C3.4.png", "C3_4.png");  // Desafio de lógica
    Texture2D imgC3_5 = CarregarCena3("C3.5.png", "C3_5.png");  // Evidência perdida
    Texture2D imgP2   = CarregarCena3("P2.png", "P2.1.png");    // Evidência desbloqueada

    // Player de áudio (4 combinações de ícones)
    Texture2D imgAudioPausaSom  = CarregarCena3("p2.(2.1).png", "p2_2_1_.png");  // pausado + com som
    Texture2D imgAudioPausaMudo = CarregarCena3("p2.(2.2).png", "p2__2_2_.png"); // pausado + mudo
    Texture2D imgAudioPlaySom   = CarregarCena3("p2.(2.3).png", "p2_2__3_.png"); // tocando + com som
    Texture2D imgAudioPlayMudo  = CarregarCena3("p2.(2.4).png", "p2_2_4_.png");  // tocando + mudo
    Texture2D imgP2Pergunta     = CarregarCena3("p2.3.png",     "p2_3.png");     // Verdadeira ou manipulada?

    // Áudio opcional da prova (se o arquivo não existir, o jogo só simula o tempo)
    Music audioProva = { 0 };
    bool temAudio = false;
    if (FileExists("src/cena3/audio_prova.mp3")) audioProva = LoadMusicStream("src/cena3/audio_prova.mp3");
    else if (FileExists("src/cena3/audio_prova.wav")) audioProva = LoadMusicStream("src/cena3/audio_prova.wav");
    else if (FileExists("src/cena3/audio_prova.ogg")) audioProva = LoadMusicStream("src/cena3/audio_prova.ogg");
    if (audioProva.stream.buffer != NULL) {
        temAudio = true;
        audioProva.looping = false;
    }

    // ======= TEXTURAS DA CENA 4 =======
    // c4.1 ... c4.14 = diálogos (CORRIGIDO: eram 13)
    Texture2D imgC4[14];
    char n1[64], n2[64];
    for (int i = 0; i < 14; i++) {
        snprintf(n1, sizeof(n1), "c4.%d.png", i + 1);
        snprintf(n2, sizeof(n2), "C4.%d.png", i + 1);
        imgC4[i] = CarregarCena4(n1, n2, NULL);
    }
    // CORRIGIDO: numeração nova (c4.15 evidência, c4.16 desafio, c4.17 perdida)
    Texture2D imgC4Evidencia = CarregarCena4("c4.15.png", "C4.15.png", "c4_15.png"); // Nova evidência
    Texture2D imgC4Desafio   = CarregarCena4("c4.16.png", "C4.16.png", "c4_16.png"); // Questão de lógica
    Texture2D imgC4Perdida   = CarregarCena4("c4.17.png", "C4.17.png", "c4_17.png"); // Evidência perdida
    Texture2D imgC4Desbloqueada = CarregarCena4("c4.18.png", "C4.18.png", "c4_18.png"); // Evidência desbloqueada (NOVO)
    Texture2D imgP4_1        = CarregarCena4("p4_1.png",  "p4.1.png",  "P4.1.png");  // Origem da prova
    Texture2D imgP4_2        = CarregarCena4("p4_2.png",  "p4.2.png",  "P4.2.png");  // Verdadeira ou manipulada


    Texture2D* todasTexturas[] = {
        &imgMenu, &texConfigAtivado, &texConfigDesativado, &texCreditos,
        &texRegras[0], &texRegras[1], &texRegras[2], &texRegras[3], &texRegras[4],
        &imgC1_1, &imgC1_2, &imgC1_3, &imgC1_4, &imgC1_5, &imgC1_6, &imgC1_7, &imgC1_8,
        &imgGabriel, &imgCaio, &imgC2_1, &imgC2_2, &imgC2_3, &imgC2_4, &imgC2_5, &imgC2_6, &imgC2_7, &imgC2_8,
        &imgC2_9, &imgC2_10, &imgC2_11, &imgC2_12, &imgC2_13, &imgC2_14, &imgC2_15, &imgC2_16, &imgC2_17,
        &imgC2_18, &imgC2_19, &imgC2_20, &imgC2_21, &imgC2_22, &imgC2_23, &imgC2_24, &imgC2_25, &imgC2_26, &imgC2_27, &imgC2_28,
        &imgF1_1, &imgF1_2, &imgF1_3, &imgP1, &imgP1_1, &imgP1_2,
        &imgC3_1, &imgC3_2, &imgC3_3, &imgC3_4, &imgC3_5, &imgP2,
        &imgAudioPlayMudo, &imgAudioPlaySom, &imgAudioPausaMudo, &imgAudioPausaSom, &imgP2Pergunta,
        &imgC4[0], &imgC4[1], &imgC4[2], &imgC4[3], &imgC4[4], &imgC4[5], &imgC4[6],
        &imgC4[7], &imgC4[8], &imgC4[9], &imgC4[10], &imgC4[11], &imgC4[12], &imgC4[13],
        &imgC4Evidencia, &imgC4Desafio, &imgC4Perdida, &imgC4Desbloqueada, &imgP4_1, &imgP4_2
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


    Rectangle btnProvaVerdadeira = { 265, 480, 125, 44 };  // botão "Verdadeira"
    Rectangle btnProvaManipulada = { 422, 480, 125, 44 };  // botão "Manipulada"


    Rectangle btnOpcaoA = { 180, 270, 440, 40 };
    Rectangle btnOpcaoB = { 180, 325, 440, 40 };
    Rectangle btnOpcaoC = { 180, 380, 440, 40 };
    Rectangle btnOpcaoD = { 180, 435, 440, 40 };


    Rectangle btnAcusaCaioSim = { 180, 270, 440, 40 };
    Rectangle btnAcusaCaioNao = { 180, 325, 440, 40 };


    // ---- Botões da CENA 3 (coordenadas na janela 800x600) ----
    Rectangle btnAvancarC3 = { 660, 450, 100, 100 };      // botão redondo "Avançar" (mesma posição do btnAvancarC2)

    Rectangle btnResolverC3 = { 309, 384, 182, 38 };      // "Resolver desafio" (C3_3)

    // CORRIGIDO: posições das opções da C3.4 conforme o print
    Rectangle btnC3OpcaoA = { 139, 298, 497, 40 };        // a) não é ... e ... não é
    Rectangle btnC3OpcaoB = { 139, 353, 497, 40 };        // b) não é ... OU ... não é  (CORRETA)
    Rectangle btnC3OpcaoC = { 139, 407, 497, 40 };        // c) é ... ou ... não é
    Rectangle btnC3OpcaoD = { 139, 456, 497, 40 };        // d) é ... e ... não é

    // CORRIGIDO: y era 422, o botão fica em y ~383
    Rectangle btnContinuarPerdida = { 301, 383, 197, 38 }; // "Continuar investigação" (C3_5)
    Rectangle btnAcessarEvidC3    = { 309, 390, 182, 38 }; // "Acessar evidência" (P2)

    Rectangle btnAudioPlay = { 150, 398, 46, 46 };        // botão play/pause
    Rectangle btnAudioMudo = { 587, 398, 46, 46 };        // botão mudo/som

    Rectangle btnC3Verdadeira = { 269, 398, 116, 39 };
    Rectangle btnC3Manipulada = { 423, 398, 121, 39 };

    Rectangle btnContinuarC3  = { 495, 462, 175, 34 };    // "CLIQUE PARA CONTINUAR" (player de áudio e pergunta final)

    // Estado do player de áudio
    bool  audioTocando = true;
    bool  audioMudo = false;
    float audioTempo = 0.0f;
    const float audioDuracao = 18.0f;

    // De onde o jogador veio ao entrar na Cena 3 (para o botão VOLTAR da C3_1)
    GameState origemC3 = STATE_MENU;


    // ---- Botões da CENA 4 (coordenadas na janela 800x600) ----
    Rectangle btnAvancarC4 = { 615, 553, 130, 40 };       // "AVANÇAR" dos diálogos

    Rectangle btnResolverC4 = { 309, 384, 182, 38 };      // "Desbloquear/Resolver desafio" (c4.15)

    Rectangle btnC4OpcaoA = { 142, 326, 496, 40 };        // a) Gabriel vai contar
    Rectangle btnC4OpcaoB = { 142, 381, 496, 40 };        // b) Gabriel NÃO vai contar (CORRETA)
    Rectangle btnC4OpcaoC = { 142, 434, 496, 40 };        // c) É indeterminado
    Rectangle btnC4OpcaoD = { 142, 484, 496, 40 };        // d) Gabriel já contou

    // CORRIGIDO: y era 422
    Rectangle btnContinuarPerdidaC4 = { 301, 383, 197, 38 }; // "Continuar investigação" (c4.17)
    // CORRIGIDO: posição do "CLIQUE PARA CONTINUAR" da p4_1
    Rectangle btnContinuarOrigemC4  = { 565, 538, 185, 36 };

    // NOVO: botão "Acessar evidência" da c4.18 (mesma posição do botão da C3 desbloqueada)
    Rectangle btnAcessarEvidC4 = { 309, 390, 182, 38 };

    Rectangle btnC4Verdadeira = { 380, 311, 128, 40 };    // p4_2
    Rectangle btnC4Manipulada = { 534, 311, 134, 40 };    // p4_2

    // De onde o jogador veio ao entrar na Cena 4 (para o VOLTAR da primeira tela)
    GameState origemC4 = STATE_MENU;


    bool telaCheia = true;
    float volGeral = 0.75f;
    float volMusica = 0.55f;
    float volEfeitos = 0.65f;


    Vector2 posMouse = { 0.0f, 0.0f };

    //SALVAR PROGRESSO
     PlayerStats jogador = { 0, 0, "Nenhuma" };
    CarregarProgresso(&jogador);


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
                    jogador.acertos++;
                    SalvarProgresso(jogador);
                    estadoAtual = STATE_EVIDENCIA_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnOpcaoB, posMouse) || BotaoClicado(btnOpcaoC, posMouse)) {
                    jogador.erros++;
                    strcpy(jogador.evidenciaPerdida, "Registro da Câmera da Saída");
                    SalvarProgresso(jogador);
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
                    // Antes voltava ao MENU; agora segue para a CENA 3
                    origemC3 = STATE_DESAFIO_IA;
                    estadoAtual = STATE_C3_1;
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
                    // Antes voltava ao MENU; agora segue para a CENA 3
                    origemC3 = STATE_C2_28;
                    estadoAtual = STATE_C3_1;
                }
                break;


            // --- CENA 3 ---
            case STATE_C3_1:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = origemC3;
                else if (BotaoClicado(btnAvancarC3, posMouse)) estadoAtual = STATE_C3_2;
                break;


            case STATE_C3_2:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C3_1;
                else if (BotaoClicado(btnAvancarC3, posMouse)) estadoAtual = STATE_C3_3;
                break;


            case STATE_C3_3:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C3_2;
                else if (BotaoClicado(btnResolverC3, posMouse)) estadoAtual = STATE_C3_4;
                break;


            case STATE_C3_4:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C3_3;
                }
                else if (BotaoClicado(btnC3OpcaoB, posMouse)) {
                    // Resposta correta: a negação de (A e B) é (não A ou não B)
                    estadoAtual = STATE_C3_DESBLOQUEADA;
                    jogador.acertos++;
                    SalvarProgresso(jogador);
                    estadoAtual = STATE_C3_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC3OpcaoA, posMouse) ||
                         BotaoClicado(btnC3OpcaoC, posMouse) ||
                         BotaoClicado(btnC3OpcaoD, posMouse)) {
                    jogador.erros++;
                    strcpy(jogador.evidenciaPerdida, "Gravação de audio ");
                    SalvarProgresso(jogador);        
                    estadoAtual = STATE_C3_PERDIDA;
                }
                break;


            case STATE_C3_PERDIDA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C3_4;
                }
                else if (BotaoClicado(btnContinuarPerdida, posMouse)) {
                    // Segue para a CENA 4
                    origemC4 = STATE_C3_PERDIDA;
                    estadoAtual = STATE_C4_1;
                }
                break;


            case STATE_C3_DESBLOQUEADA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C3_4;
                }
                else if (BotaoClicado(btnAcessarEvidC3, posMouse)) {
                    // Entra no player de áudio
                    audioTocando = true;
                    audioMudo = false;
                    audioTempo = 0.0f;
                    if (temAudio) {
                        StopMusicStream(audioProva);
                        SetMusicVolume(audioProva, 1.0f);
                        PlayMusicStream(audioProva);
                    }
                    estadoAtual = STATE_C3_AUDIO;
                }
                break;


            case STATE_C3_AUDIO:
                if (temAudio) UpdateMusicStream(audioProva);

                // Tempo da faixa (só avança enquanto está tocando)
                if (audioTocando) audioTempo += GetFrameTime();

                if (BotaoClicado(btnVoltar, posMouse)) {
                    if (temAudio) StopMusicStream(audioProva);
                    estadoAtual = STATE_C3_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnAudioPlay, posMouse)) {
                    audioTocando = !audioTocando;
                    if (temAudio) {
                        if (audioTocando) ResumeMusicStream(audioProva);
                        else PauseMusicStream(audioProva);
                    }
                }
                else if (BotaoClicado(btnAudioMudo, posMouse)) {
                    audioMudo = !audioMudo;
                    if (temAudio) SetMusicVolume(audioProva, audioMudo ? 0.0f : 1.0f);
                }
                else if (BotaoClicado(btnContinuarC3, posMouse) || IsKeyPressed(KEY_ENTER)) {
                    // Clique em "CLIQUE PARA CONTINUAR" (ou ENTER) -> pergunta final
                    if (temAudio) StopMusicStream(audioProva);
                    estadoAtual = STATE_C3_PERGUNTA;
                }
                break;


            case STATE_C3_PERGUNTA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    audioTempo = 0.0f;
                    audioTocando = true;
                    audioMudo = false;
                    if (temAudio) {
                        SetMusicVolume(audioProva, 1.0f);
                        PlayMusicStream(audioProva);
                    }
                    estadoAtual = STATE_C3_AUDIO;
                }
                else if (BotaoClicado(btnC3Verdadeira, posMouse) || BotaoClicado(btnC3Manipulada, posMouse) ||
                         BotaoClicado(btnContinuarC3, posMouse)) {
                    // Segue para a CENA 4
                    origemC4 = STATE_C3_PERGUNTA;
                    estadoAtual = STATE_C4_1;
                }
                break;


            // --- CENA 4 ---
            case STATE_C4_EVIDENCIA:   // c4.15
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_14;                 // CORRIGIDO: volta para o diálogo c4.14
                }
                else if (BotaoClicado(btnResolverC4, posMouse)) {   // CORRIGIDO: usa o botão central
                    estadoAtual = STATE_C4_DESAFIO;            // vai para c4.16
                }
                break;

            case STATE_C4_DESAFIO:     // c4.16
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_EVIDENCIA;
                }
                else if (BotaoClicado(btnC4OpcaoB, posMouse)) {
                    // Única resposta correta -> tela "evidência desbloqueada" (c4.18)
                    jogador.acertos++;
                    SalvarProgresso(jogador);
                    estadoAtual = STATE_C4_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC4OpcaoA, posMouse) ||
                         BotaoClicado(btnC4OpcaoC, posMouse) ||
                         BotaoClicado(btnC4OpcaoD, posMouse)) {
                    // Qualquer erro -> evidência perdida (c4.17)
                    jogador.erros++;
                    strcpy(jogador.evidenciaPerdida, "Print da tela do celular de Gabriel");
                    SalvarProgresso(jogador);
                    estadoAtual = STATE_C4_PERDIDA;
                }
                break;


            case STATE_C4_PERDIDA:     // c4.17
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_DESAFIO;
                }
                else if (BotaoClicado(btnContinuarPerdidaC4, posMouse)) {
                    // TODO: trocar pelo próximo estado quando a próxima cena existir
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_C4_DESBLOQUEADA: // c4.18
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_DESAFIO;
                }
                else if (BotaoClicado(btnAcessarEvidC4, posMouse)) {
                    estadoAtual = STATE_C4_ORIGEM;         // vai para a prova (p4_1)
                }
                break;


            case STATE_C4_ORIGEM:      // p4_1
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C4_DESBLOQUEADA;
                else if (BotaoClicado(btnContinuarOrigemC4, posMouse)) estadoAtual = STATE_C4_PERGUNTA;
                break;


            case STATE_C4_PERGUNTA:    // p4_2
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_ORIGEM;
                }
                else if (BotaoClicado(btnC4Verdadeira, posMouse) || BotaoClicado(btnC4Manipulada, posMouse)) {
                    // TODO: definir qual é a correta e a próxima cena
                    estadoAtual = STATE_MENU;
                }
                break;


            // Telas de diálogo C4_1 ... C4_14 (voltar / avançar)
            default:
                if (estadoAtual >= STATE_C4_1 && estadoAtual <= STATE_C4_14) {   // CORRIGIDO: até C4_14
                    if (BotaoClicado(btnVoltar, posMouse)) {
                        if (estadoAtual == STATE_C4_1) {
                            estadoAtual = origemC4;
                        }
                        else {
                            estadoAtual = (GameState)(estadoAtual - 1);
                        }
                    }
                    else if (BotaoClicado(btnAvancarC4, posMouse)) {
                        estadoAtual = (GameState)(estadoAtual + 1);   // C4_14 -> C4_EVIDENCIA (c4.15)
                    }
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


            // --- CENA 3 ---
            case STATE_C3_1:
                if (imgC3_1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC3_1.width, (float)imgC3_1.height };
                    DrawTexturePro(imgC3_1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C3_2:
                if (imgC3_2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC3_2.width, (float)imgC3_2.height };
                    DrawTexturePro(imgC3_2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C3_3:
                if (imgC3_3.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC3_3.width, (float)imgC3_3.height };
                    DrawTexturePro(imgC3_3, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C3_4:
                if (imgC3_4.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC3_4.width, (float)imgC3_4.height };
                    DrawTexturePro(imgC3_4, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C3_PERDIDA:
                if (imgC3_5.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC3_5.width, (float)imgC3_5.height };
                    DrawTexturePro(imgC3_5, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C3_DESBLOQUEADA:
                if (imgP2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP2.width, (float)imgP2.height };
                    DrawTexturePro(imgP2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C3_AUDIO: {
                Texture2D texAudio;
                if (audioTocando) texAudio = audioMudo ? imgAudioPlayMudo  : imgAudioPlaySom;
                else              texAudio = audioMudo ? imgAudioPausaMudo : imgAudioPausaSom;
                if (texAudio.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)texAudio.width, (float)texAudio.height };
                    DrawTexturePro(texAudio, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;
            }


            case STATE_C3_PERGUNTA:
                if (imgP2Pergunta.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP2Pergunta.width, (float)imgP2Pergunta.height };
                    DrawTexturePro(imgP2Pergunta, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            // --- CENA 4 ---
            case STATE_C4_EVIDENCIA:
                if (imgC4Evidencia.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC4Evidencia.width, (float)imgC4Evidencia.height };
                    DrawTexturePro(imgC4Evidencia, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C4_DESAFIO:
                if (imgC4Desafio.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC4Desafio.width, (float)imgC4Desafio.height };
                    DrawTexturePro(imgC4Desafio, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C4_PERDIDA:
                if (imgC4Perdida.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC4Perdida.width, (float)imgC4Perdida.height };
                    DrawTexturePro(imgC4Perdida, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C4_DESBLOQUEADA:
                if (imgC4Desbloqueada.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgC4Desbloqueada.width, (float)imgC4Desbloqueada.height };
                    DrawTexturePro(imgC4Desbloqueada, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C4_ORIGEM:
                if (imgP4_1.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP4_1.width, (float)imgP4_1.height };
                    DrawTexturePro(imgP4_1, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            case STATE_C4_PERGUNTA:
                if (imgP4_2.id != 0) {
                    orig = (Rectangle){ 0.0f, 0.0f, (float)imgP4_2.width, (float)imgP4_2.height };
                    DrawTexturePro(imgP4_2, orig, dest, pontoZero, 0.0f, WHITE);
                }
                break;


            // Telas de diálogo C4_1 ... C4_14

            default:
                if (estadoAtual >= STATE_C4_1 && estadoAtual <= STATE_C4_14) {   // CORRIGIDO: até C4_14
                    Texture2D t = imgC4[estadoAtual - STATE_C4_1];

                    if (t.id != 0) {
                        orig = (Rectangle){ 0.0f, 0.0f, (float)t.width, (float)t.height };
                        DrawTexturePro(t, orig, dest, pontoZero, 0.0f, WHITE);
                    }
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


    UnloadTexture(imgC3_1);
    UnloadTexture(imgC3_2);
    UnloadTexture(imgC3_3);
    UnloadTexture(imgC3_4);
    UnloadTexture(imgC3_5);
    UnloadTexture(imgP2);
    UnloadTexture(imgAudioPlayMudo);
    UnloadTexture(imgAudioPlaySom);
    UnloadTexture(imgAudioPausaMudo);
    UnloadTexture(imgAudioPausaSom);
    UnloadTexture(imgP2Pergunta);


    for (int i = 0; i < 14; i++) UnloadTexture(imgC4[i]);   // CORRIGIDO: 14
    UnloadTexture(imgC4Evidencia);
    UnloadTexture(imgC4Desafio);
    UnloadTexture(imgC4Perdida);
    UnloadTexture(imgC4Desbloqueada);
    UnloadTexture(imgP4_1);
    UnloadTexture(imgP4_2);

    if (temAudio) UnloadMusicStream(audioProva);
    CloseAudioDevice();


    CloseWindow();
    return 0;
}