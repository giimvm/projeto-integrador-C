#include "raylib.h"
#include <stdio.h>

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
    STATE_C2_0,
    STATE_C2_1,
    STATE_C2_2,
    STATE_C2_3,
    STATE_C2_4,
    STATE_C2_5,
    STATE_C2_6,
    STATE_C2_7,

    STATE_C2_EVIDENCIA,   // C2.8  - nova evidência encontrada
    STATE_C2_DESAFIO,     // C2.9  - desafio de lógica
    STATE_CONFIRMA_CAIO,  // C2.10 - "Você selecionou: Caio é o culpado"
    STATE_C2_PERDIDA,     // C2.11 - evidência perdida

    // RAMIFICAÇÃO DELEGADO E VEREDITO
    STATE_ACUSA_CAIO_1,   // F1.1
    STATE_ACUSA_CAIO_2,   // F1.2
    STATE_ACUSA_CAIO_3,   // F1.3
    STATE_ACUSA_CAIO_4,   // F1.4
    STATE_VEREDITO_INCORRETO,     // P1
    STATE_EVIDENCIA_DESBLOQUEADA, // P1.1
    STATE_DESAFIO_IA,             // P1.2

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
    STATE_C4_14,           // c4.14 é diálogo
    STATE_C4_EVIDENCIA,    // c4.15 - nova evidência encontrada
    STATE_C4_DESAFIO,      // c4.16 - questão de lógica
    STATE_C4_PERDIDA,      // c4.17 - evidência perdida
    STATE_C4_DESBLOQUEADA, // c4.18 - evidência desbloqueada
    STATE_C4_ORIGEM,       // p4_1
    STATE_C4_PERGUNTA      // p4_2
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


// Desenha uma textura esticada na área "dest" (se a textura existir)
void DesenharTela(Texture2D t, Rectangle dest) {
    if (t.id == 0) return;
    Rectangle orig = { 0.0f, 0.0f, (float)t.width, (float)t.height };
    Vector2 zero = { 0.0f, 0.0f };
    DrawTexturePro(t, orig, dest, zero, 0.0f, WHITE);
}


// Gera mipmaps e filtro bilinear (se a textura existir)
void PrepararTextura(Texture2D* t) {
    if (t->id != 0) {
        GenTextureMipmaps(t);
        SetTextureFilter(*t, TEXTURE_FILTER_BILINEAR);
    }
}


// Tenta carregar uma imagem da Cena 2 em várias pastas e com dois nomes possíveis
// (ex.: "C2.0.png" ou "C2_0.png")
Texture2D CarregarCena2(const char* nome1, const char* nome2) {
    const char* pastas[] = { "src/cena2/", "cena2/", "jogo_ai.libi/src/cena2/", "" };
    const char* nomes[2] = { nome1, nome2 };
    char caminho[256];

    for (int n = 0; n < 2; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "CENA2: imagem NAO encontrada: %s (nem %s)", nome1, nome2 ? nome2 : "-");
    return (Texture2D){ 0 };
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
    // (aceita nomes com ponto ou underline: C2.0.png / C2_0.png)
    // 0..7  = diálogos
    // 8     = nova evidência encontrada
    // 9     = desafio de lógica
    // 10    = confirmação "Caio é o culpado"
    // 11    = evidência perdida
    Texture2D imgC2[12];
    {
        char a[64], b[64];
        for (int i = 0; i < 12; i++) {
            snprintf(a, sizeof(a), "C2.%d.png", i);
            snprintf(b, sizeof(b), "C2_%d.png", i);
            imgC2[i] = CarregarCena2(a, b);
        }
    }

    // RAMIFICAÇÃO DELEGADO (F1.1 ... F1.4)
    Texture2D imgF1[4];
    {
        char a[64], b[64];
        for (int i = 0; i < 4; i++) {
            snprintf(a, sizeof(a), "F1.%d.png", i + 1);
            snprintf(b, sizeof(b), "F1_%d.png", i + 1);
            imgF1[i] = CarregarCena2(a, b);
        }
    }

    Texture2D imgP1   = CarregarCena2("P1.png",   "P1_0.png");   // Veredito incorreto
    Texture2D imgP1_1 = CarregarCena2("P1.1.png", "P1_1.png");   // Evidência desbloqueada
    Texture2D imgP1_2 = CarregarCena2("P1.2.png", "P1_2.png");   // Verdadeira ou manipulada por IA?


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
    // c4.1 ... c4.14 = diálogos
    Texture2D imgC4[14];
    char n1[64], n2[64];
    for (int i = 0; i < 14; i++) {
        snprintf(n1, sizeof(n1), "c4.%d.png", i + 1);
        snprintf(n2, sizeof(n2), "C4.%d.png", i + 1);
        imgC4[i] = CarregarCena4(n1, n2, NULL);
    }
    Texture2D imgC4Evidencia    = CarregarCena4("c4.15.png", "C4.15.png", "c4_15.png"); // Nova evidência
    Texture2D imgC4Desafio      = CarregarCena4("c4.16.png", "C4.16.png", "c4_16.png"); // Questão de lógica
    Texture2D imgC4Perdida      = CarregarCena4("c4.17.png", "C4.17.png", "c4_17.png"); // Evidência perdida
    Texture2D imgC4Desbloqueada = CarregarCena4("c4.18.png", "C4.18.png", "c4_18.png"); // Evidência desbloqueada
    Texture2D imgP4_1           = CarregarCena4("p4_1.png",  "p4.1.png",  "P4.1.png");  // Origem da prova
    Texture2D imgP4_2           = CarregarCena4("p4_2.png",  "p4.2.png",  "P4.2.png");  // Verdadeira ou manipulada


    // ======= MIPMAPS + FILTRO =======
    Texture2D* todasTexturas[] = {
        &imgMenu, &texConfigAtivado, &texConfigDesativado, &texCreditos,
        &texRegras[0], &texRegras[1], &texRegras[2], &texRegras[3], &texRegras[4],
        &imgC1_1, &imgC1_2, &imgC1_3, &imgC1_4, &imgC1_5, &imgC1_6, &imgC1_7, &imgC1_8,
        &imgGabriel, &imgCaio,
        &imgP1, &imgP1_1, &imgP1_2,
        &imgC3_1, &imgC3_2, &imgC3_3, &imgC3_4, &imgC3_5, &imgP2,
        &imgAudioPlayMudo, &imgAudioPlaySom, &imgAudioPausaMudo, &imgAudioPausaSom, &imgP2Pergunta,
        &imgC4Evidencia, &imgC4Desafio, &imgC4Perdida, &imgC4Desbloqueada, &imgP4_1, &imgP4_2
    };
    int totalTexturas = sizeof(todasTexturas) / sizeof(todasTexturas[0]);

    for (int i = 0; i < totalTexturas; i++) PrepararTextura(todasTexturas[i]);
    for (int i = 0; i < 12; i++) PrepararTextura(&imgC2[i]);
    for (int i = 0; i < 4;  i++) PrepararTextura(&imgF1[i]);
    for (int i = 0; i < 14; i++) PrepararTextura(&imgC4[i]);


    Rectangle dest = { 0.0f, 0.0f, (float)larguraTela, (float)alturaTela };


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


    Rectangle btnVoltarRegras    = { 90, 520, 110, 35 };
    Rectangle btnContinuarRegras = { 630, 520, 130, 35 };


    Rectangle btnLogin  = { 340, 400, 120, 45 };
    Rectangle btnVoltar = { 15, 20, 160, 60 }; // Botão de Voltar no canto superior esquerdo


    Rectangle destFotoGabriel = { 440, 160, 230, 270 };
    Rectangle destFotoCaio    = { 440, 160, 230, 270 };
    Rectangle btnFotoCaio     = { 440, 160, 230, 270 };


    // ---- Botões da CENA 2 (coordenadas na janela 800x600) ----
    Rectangle btnAvancarC2  = { 615, 553, 130, 40 };   // "AVANÇAR" dos diálogos (C2.0 ... C2.7 e F1.1 ... F1.4)
    Rectangle btnResolverC2 = { 309, 384, 182, 38 };   // "Resolver desafio" (C2.8)

    // Opções do desafio (C2.9)
    Rectangle btnC2OpcaoA = { 140, 296, 497, 40 };     // a) A câmera deve ter registrado Caio saindo. (CORRETA)
    Rectangle btnC2OpcaoB = { 140, 350, 497, 40 };     // b) A câmera não registrou Caio.
    Rectangle btnC2OpcaoC = { 140, 403, 497, 40 };     // c) Nada se conclui.
    Rectangle btnC2OpcaoD = { 140, 454, 497, 40 };     // d) Caio é o culpado.

    // Confirmação (C2.10)
    Rectangle btnAcusaCaioSim = { 142, 319, 496, 40 }; // a) Sim, quero acusar Caio.
    Rectangle btnAcusaCaioNao = { 142, 373, 496, 40 }; // b) Não, quero continuar a investigação.

    // Evidência perdida (C2.11)
    Rectangle btnContinuarPerdidaC2 = { 297, 410, 197, 38 }; // "Continuar investigação"

    // Veredito incorreto (P1)
    Rectangle btnTentarNovamente = { 311, 462, 182, 38 };

    // Evidência desbloqueada (P1.1)
    Rectangle btnAcessarEvidencia = { 309, 390, 182, 38 };

    // Prova verdadeira ou manipulada (P1.2)
    Rectangle btnProvaVerdadeira = { 268, 482, 117, 40 };
    Rectangle btnProvaManipulada = { 423, 482, 120, 40 };


    // ---- Botões da CENA 3 (coordenadas na janela 800x600) ----
    Rectangle btnAvancarC3 = { 615, 553, 130, 40 };

    Rectangle btnResolverC3 = { 309, 384, 182, 38 };      // "Resolver desafio" (C3_3)

    Rectangle btnC3OpcaoA = { 139, 298, 497, 40 };        // a) não é ... e ... não é
    Rectangle btnC3OpcaoB = { 139, 353, 497, 40 };        // b) não é ... OU ... não é  (CORRETA)
    Rectangle btnC3OpcaoC = { 139, 407, 497, 40 };        // c) é ... ou ... não é
    Rectangle btnC3OpcaoD = { 139, 456, 497, 40 };        // d) é ... e ... não é

    Rectangle btnContinuarPerdida = { 301, 383, 197, 38 }; // "Continuar investigação" (C3_5)
    Rectangle btnAcessarEvidC3    = { 309, 390, 182, 38 }; // "Acessar evidência" (P2)

    Rectangle btnAudioPlay = { 150, 398, 46, 46 };        // botão play/pause
    Rectangle btnAudioMudo = { 587, 398, 46, 46 };        // botão mudo/som

    Rectangle btnC3Verdadeira = { 269, 398, 116, 39 };
    Rectangle btnC3Manipulada = { 423, 398, 121, 39 };

    Rectangle btnContinuarC3  = { 495, 462, 175, 34 };    // "CLIQUE PARA CONTINUAR"

    // Estado do player de áudio
    bool  audioTocando = true;
    bool  audioMudo = false;
    float audioTempo = 0.0f;

    // De onde o jogador veio ao entrar na Cena 3 (para o botão VOLTAR da C3_1)
    GameState origemC3 = STATE_MENU;


    // ---- Botões da CENA 4 (coordenadas na janela 800x600) ----
    Rectangle btnAvancarC4 = { 615, 553, 130, 40 };       // "AVANÇAR" dos diálogos

    Rectangle btnResolverC4 = { 309, 384, 182, 38 };      // "Resolver desafio" (c4.15)

    Rectangle btnC4OpcaoA = { 142, 326, 496, 40 };        // a) Gabriel vai contar
    Rectangle btnC4OpcaoB = { 142, 381, 496, 40 };        // b) Gabriel NÃO vai contar (CORRETA)
    Rectangle btnC4OpcaoC = { 142, 434, 496, 40 };        // c) É indeterminado
    Rectangle btnC4OpcaoD = { 142, 484, 496, 40 };        // d) Gabriel já contou

    Rectangle btnContinuarPerdidaC4 = { 301, 383, 197, 38 }; // "Continuar investigação" (c4.17)
    Rectangle btnContinuarOrigemC4  = { 565, 538, 185, 36 }; // "CLIQUE PARA CONTINUAR" da p4_1

    Rectangle btnAcessarEvidC4 = { 309, 390, 182, 38 };   // "Acessar evidência" da c4.18

    Rectangle btnC4Verdadeira = { 380, 311, 128, 40 };    // p4_2
    Rectangle btnC4Manipulada = { 534, 311, 134, 40 };    // p4_2

    // De onde o jogador veio ao entrar na Cena 4 (para o VOLTAR da primeira tela)
    GameState origemC4 = STATE_MENU;


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
                else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) estadoAtual = STATE_C2_0;
                break;


            // --- CENA 2 ---

            case STATE_C2_EVIDENCIA:   // C2.8 - nova evidência encontrada
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C2_7;
                }
                else if (BotaoClicado(btnResolverC2, posMouse)) {
                    estadoAtual = STATE_C2_DESAFIO;
                }
                break;


            case STATE_C2_DESAFIO:     // C2.9 - desafio de lógica
                if (BotaoClicado(btnC2OpcaoA, posMouse)) {
                    // Resposta correta (modus ponens) -> evidência desbloqueada
                    estadoAtual = STATE_EVIDENCIA_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC2OpcaoB, posMouse) || BotaoClicado(btnC2OpcaoC, posMouse)) {
                    // Erro -> evidência perdida
                    estadoAtual = STATE_C2_PERDIDA;
                }
                else if (BotaoClicado(btnC2OpcaoD, posMouse)) {
                    // "Caio é o culpado" -> pede confirmação
                    estadoAtual = STATE_CONFIRMA_CAIO;
                }
                break;


            case STATE_CONFIRMA_CAIO:  // C2.10 - "Você selecionou: Caio é o culpado"
                if (BotaoClicado(btnAcusaCaioNao, posMouse)) {
                    estadoAtual = STATE_C2_DESAFIO;
                }
                else if (BotaoClicado(btnAcusaCaioSim, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_1;
                }
                break;


            case STATE_C2_PERDIDA:     // C2.11 - evidência perdida
                if (BotaoClicado(btnContinuarPerdidaC2, posMouse)) {
                    // Segue para a CENA 3
                    origemC3 = STATE_C2_PERDIDA;
                    estadoAtual = STATE_C3_1;
                }
                break;


            case STATE_ACUSA_CAIO_1:   // F1.1
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_CONFIRMA_CAIO;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_2;
                }
                break;


            case STATE_ACUSA_CAIO_2:   // F1.2
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_1;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_3;
                }
                break;


            case STATE_ACUSA_CAIO_3:   // F1.3
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_2;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_4;
                }
                break;


            case STATE_ACUSA_CAIO_4:   // F1.4
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_3;
                }
                else if (BotaoClicado(btnAvancarC2, posMouse)) {
                    estadoAtual = STATE_VEREDITO_INCORRETO;
                }
                break;


            case STATE_VEREDITO_INCORRETO:   // P1
                if (BotaoClicado(btnTentarNovamente, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_EVIDENCIA_DESBLOQUEADA:   // P1.1
                if (BotaoClicado(btnAcessarEvidencia, posMouse)) {
                    estadoAtual = STATE_DESAFIO_IA;
                }
                break;


            case STATE_DESAFIO_IA:   // P1.2
                if (BotaoClicado(btnProvaVerdadeira, posMouse) || BotaoClicado(btnProvaManipulada, posMouse)) {
                    // Segue para a CENA 3
                    origemC3 = STATE_DESAFIO_IA;
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
                }
                else if (BotaoClicado(btnC3OpcaoA, posMouse) ||
                         BotaoClicado(btnC3OpcaoC, posMouse) ||
                         BotaoClicado(btnC3OpcaoD, posMouse)) {
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
                    estadoAtual = STATE_C4_14;
                }
                else if (BotaoClicado(btnResolverC4, posMouse)) {
                    estadoAtual = STATE_C4_DESAFIO;
                }
                break;


            case STATE_C4_DESAFIO:     // c4.16
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_EVIDENCIA;
                }
                else if (BotaoClicado(btnC4OpcaoB, posMouse)) {
                    // Única resposta correta -> evidência desbloqueada (c4.18)
                    estadoAtual = STATE_C4_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC4OpcaoA, posMouse) ||
                         BotaoClicado(btnC4OpcaoC, posMouse) ||
                         BotaoClicado(btnC4OpcaoD, posMouse)) {
                    // Qualquer erro -> evidência perdida (c4.17)
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
                    estadoAtual = STATE_C4_ORIGEM;
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


            // Telas de diálogo: C2_0 ... C2_7 e C4_1 ... C4_14 (voltar / avançar)
            default:
                if (estadoAtual >= STATE_C2_0 && estadoAtual <= STATE_C2_7) {
                    if (BotaoClicado(btnVoltar, posMouse)) {
                        if (estadoAtual == STATE_C2_0) {
                            estadoAtual = STATE_C1_8;
                        }
                        else {
                            estadoAtual = (GameState)(estadoAtual - 1);
                        }
                    }
                    else if (BotaoClicado(btnAvancarC2, posMouse)) {
                        estadoAtual = (GameState)(estadoAtual + 1);   // C2_7 -> STATE_C2_EVIDENCIA
                    }
                }
                else if (estadoAtual >= STATE_C4_1 && estadoAtual <= STATE_C4_14) {
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
                    DesenharTela(imgMenu, dest);
                } else {
                    DrawText("AI.LIBE - MENU", 310, 150, 24, RAYWHITE);
                    DrawText("JOGAR", (int)btnJogar.x, (int)btnJogar.y, 22, RAYWHITE);
                    DrawText("REGRAS", (int)btnRegras.x, (int)btnRegras.y, 22, RAYWHITE);
                    DrawText("CONFIGURACOES", (int)btnConfig.x, (int)btnConfig.y, 22, RAYWHITE);
                    DrawText("CREDITOS", (int)btnCreditos.x, (int)btnCreditos.y, 22, RAYWHITE);
                }
                break;


            case STATE_CONFIG: {
                Texture2D texAtual = telaCheia ? texConfigAtivado : texConfigDesativado;
                if (texAtual.id != 0) {
                    DesenharTela(texAtual, dest);
                } else {
                    DrawText("CONFIGURACOES", 300, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", (int)btnVoltarMenu.x, (int)btnVoltarMenu.y, 20, RAYWHITE);
                }
                break;
            }


            case STATE_CREDITOS:
                if (texCreditos.id != 0) {
                    DesenharTela(texCreditos, dest);
                } else {
                    DrawText("CREDITOS", 340, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", (int)btnVoltarMenu.x, (int)btnVoltarMenu.y, 20, RAYWHITE);
                }
                break;


            case STATE_REGRAS:
                if (texRegras[paginaRegras].id != 0) {
                    DesenharTela(texRegras[paginaRegras], dest);
                } else {
                    DrawText("REGRAS DO JOGO", 300, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", (int)btnVoltarRegras.x, (int)btnVoltarRegras.y, 20, RAYWHITE);
                    DrawText("CONTINUAR", (int)btnContinuarRegras.x, (int)btnContinuarRegras.y, 20, RAYWHITE);
                }
                break;


            // --- CENA 1 ---
            case STATE_C1_1: DesenharTela(imgC1_1, dest); break;
            case STATE_C1_2: DesenharTela(imgC1_2, dest); break;

            case STATE_C1_3:
                DesenharTela(imgC1_3, dest);
                DesenharTela(imgGabriel, destFotoGabriel);
                break;

            case STATE_C1_4:
                DesenharTela(imgC1_4, dest);
                DesenharTela(imgCaio, destFotoCaio);
                break;

            case STATE_C1_5: DesenharTela(imgC1_5, dest); break;
            case STATE_C1_6: DesenharTela(imgC1_6, dest); break;
            case STATE_C1_7: DesenharTela(imgC1_7, dest); break;
            case STATE_C1_8: DesenharTela(imgC1_8, dest); break;


            // --- CENA 2 (as telas de diálogo C2_0 ... C2_7 são desenhadas no "default") ---
            case STATE_C2_EVIDENCIA:  DesenharTela(imgC2[8],  dest); break;
            case STATE_C2_DESAFIO:    DesenharTela(imgC2[9],  dest); break;
            case STATE_CONFIRMA_CAIO: DesenharTela(imgC2[10], dest); break;
            case STATE_C2_PERDIDA:    DesenharTela(imgC2[11], dest); break;

            case STATE_ACUSA_CAIO_1: DesenharTela(imgF1[0], dest); break;
            case STATE_ACUSA_CAIO_2: DesenharTela(imgF1[1], dest); break;
            case STATE_ACUSA_CAIO_3: DesenharTela(imgF1[2], dest); break;
            case STATE_ACUSA_CAIO_4: DesenharTela(imgF1[3], dest); break;

            case STATE_VEREDITO_INCORRETO:     DesenharTela(imgP1,   dest); break;
            case STATE_EVIDENCIA_DESBLOQUEADA: DesenharTela(imgP1_1, dest); break;
            case STATE_DESAFIO_IA:             DesenharTela(imgP1_2, dest); break;


            // --- CENA 3 ---
            case STATE_C3_1: DesenharTela(imgC3_1, dest); break;
            case STATE_C3_2: DesenharTela(imgC3_2, dest); break;
            case STATE_C3_3: DesenharTela(imgC3_3, dest); break;
            case STATE_C3_4: DesenharTela(imgC3_4, dest); break;
            case STATE_C3_PERDIDA: DesenharTela(imgC3_5, dest); break;
            case STATE_C3_DESBLOQUEADA: DesenharTela(imgP2, dest); break;

            case STATE_C3_AUDIO: {
                Texture2D texAudio;
                if (audioTocando) texAudio = audioMudo ? imgAudioPlayMudo  : imgAudioPlaySom;
                else              texAudio = audioMudo ? imgAudioPausaMudo : imgAudioPausaSom;
                DesenharTela(texAudio, dest);
                break;
            }

            case STATE_C3_PERGUNTA: DesenharTela(imgP2Pergunta, dest); break;


            // --- CENA 4 ---
            case STATE_C4_EVIDENCIA:    DesenharTela(imgC4Evidencia, dest); break;
            case STATE_C4_DESAFIO:      DesenharTela(imgC4Desafio, dest); break;
            case STATE_C4_PERDIDA:      DesenharTela(imgC4Perdida, dest); break;
            case STATE_C4_DESBLOQUEADA: DesenharTela(imgC4Desbloqueada, dest); break;
            case STATE_C4_ORIGEM:       DesenharTela(imgP4_1, dest); break;
            case STATE_C4_PERGUNTA:     DesenharTela(imgP4_2, dest); break;


            // Telas de diálogo: C2_0 ... C2_7 e C4_1 ... C4_14
            default:
                if (estadoAtual >= STATE_C2_0 && estadoAtual <= STATE_C2_7) {
                    DesenharTela(imgC2[estadoAtual - STATE_C2_0], dest);
                }
                else if (estadoAtual >= STATE_C4_1 && estadoAtual <= STATE_C4_14) {
                    DesenharTela(imgC4[estadoAtual - STATE_C4_1], dest);
                }
                break;
        }


        EndDrawing();
    }


    // ======= UNLOAD TEXTURES =======
    for (int i = 0; i < 5; i++) UnloadTexture(texRegras[i]);
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

    for (int i = 0; i < 12; i++) UnloadTexture(imgC2[i]);
    for (int i = 0; i < 4;  i++) UnloadTexture(imgF1[i]);
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

    for (int i = 0; i < 14; i++) UnloadTexture(imgC4[i]);
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