#ifndef MINIGAMES_H
#define MINIGAMES_H

#include "raylib.h"
#include "rlgl.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    Font bold;
    Font regular;
    bool carregada;
} FontesUI;

static inline FontesUI CarregarFontesUI(void) {
    FontesUI f = {0};
    int codepoints[224];
    int count = 0;
    for (int c = 32; c <= 126; c++) codepoints[count++] = c;
    for (int c = 160; c <= 255; c++) codepoints[count++] = c;

    const char *caminhosBold[] = {
        "src/fonts/Tektur-Regular.ttf",
        "jogo_ai.libi/src/fonts/Tektur-Regular.ttf",
        "../jogo_ai.libi/src/fonts/Tektur-Regular.ttf",
        "src/fonts/Rajdhani-Bold.ttf",
        "jogo_ai.libi/src/fonts/Rajdhani-Bold.ttf"
    };
    const char *caminhosReg[] = {
        "src/fonts/Tektur-Regular.ttf",
        "jogo_ai.libi/src/fonts/Tektur-Regular.ttf",
        "../jogo_ai.libi/src/fonts/Tektur-Regular.ttf",
        "src/fonts/Rajdhani-SemiBold.ttf",
        "jogo_ai.libi/src/fonts/Rajdhani-SemiBold.ttf"
    };

    for (int i = 0; i < 5; i++) {
        if (FileExists(caminhosBold[i])) {
            f.bold = LoadFontEx(caminhosBold[i], 36, codepoints, count);
            SetTextureFilter(f.bold.texture, TEXTURE_FILTER_BILINEAR);
            break;
        }
    }
    for (int i = 0; i < 5; i++) {
        if (FileExists(caminhosReg[i])) {
            f.regular = LoadFontEx(caminhosReg[i], 28, codepoints, count);
            SetTextureFilter(f.regular.texture, TEXTURE_FILTER_BILINEAR);
            break;
        }
    }

    if (f.bold.texture.id == 0) f.bold = GetFontDefault();
    else f.carregada = true;

    if (f.regular.texture.id == 0) f.regular = f.bold;
    return f;
}

static inline void DescarregarFontesUI(FontesUI *f) {
    if (f->carregada) {
        if (f->bold.texture.id != GetFontDefault().texture.id) UnloadFont(f->bold);
        if (f->regular.texture.id != GetFontDefault().texture.id &&
            f->regular.texture.id != f->bold.texture.id) {
            UnloadFont(f->regular);
        }
    }
}

static inline float MedirTextoUI(Font f, const char *texto, float tamanho) {
    Vector2 m = MeasureTextEx(f, texto, tamanho, 1.0f);
    return m.x;
}

static inline void DesenharTextoUI(Font f, const char *texto, float x, float y, float tamanho, Color cor) {
    DrawTextEx(f, texto, (Vector2){ x, y }, tamanho, 1.0f, cor);
}

static inline void DesenharTextoCentralizadoUI(Font f, const char *texto, Rectangle rect, float tamanho, Color cor) {
    Vector2 m = MeasureTextEx(f, texto, tamanho, 1.0f);
    float x = rect.x + (rect.width - m.x) * 0.5f;
    float y = rect.y + (rect.height - m.y) * 0.5f;
    DrawTextEx(f, texto, (Vector2){ x, y }, tamanho, 1.0f, cor);
}

typedef struct {
    int desafioAtual;   // 2, 4, 5, 6 ou 7
    int chances;        // 4 no jogo da memoria, 2 nos demais
    int chancesMax;
    float timerFeedback;
    bool feedbackErro;
    char msgFeedback[128];

    // Cena 2: Memória (4 pares = 8 cartas)
    int memValores[8];
    bool memEncontrada[8];
    int memSel1;
    int memSel2;
    float memTimerPreview;
    float memTimerMismatch;

    // Cena 4: Gaveta 3D Cyberpunk com 8 Pen Drives + Física 3D + Inspeção 360
    int penSimbolos[8];
    int penCores[8];
    int penAlvo;
    int penHover;
    bool penTestado[8];
    Vector3 penPos[8];
    Vector3 penVel[8];
    float penRot[8];
    float penVelRot[8];
    int penArrastando;
    Vector2 penClickPos;
    bool penArrastou;

    int penInspecionando;
    float penInspYaw;
    float penInspPitch;
    float penInspVelYaw;
    float penInspVelPitch;
    float penInspZoom;
    bool penInspArrastando;
    Vector2 penInspMouseAnt;

    // Cena 5: Interceptação de Sinal (barra de timing)
    float sinalPos;
    int sinalDir;
    float sinalAlvoMin;
    float sinalAlvoMax;
    int sinalAcertos;

    // Cena 6: Sequência de Segurança (Simon 4 passos)
    int seqAlvo[4];
    int seqProgresso;
    bool seqMostrando;
    int seqPassoShow;
    float seqTimer;
    int seqBotaoAtivo;
    float seqTimerClique;

    // Cena 7: Rastreamento de Rede (ligar 4 pares)
    int redeDireitaMap[4];
    bool redeConectada[4];
    int redeSelEsq;
} EstadoMiniGame;

static inline void EmbaralharInt(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

static inline void IniciarMiniGame(EstadoMiniGame *mg, int cena) {
    memset(mg, 0, sizeof(*mg));
    mg->desafioAtual = cena;
    mg->chancesMax = (cena == 2) ? 4 : 2;
    mg->chances = mg->chancesMax;

    if (cena == 2) {
        int base[8] = { 0, 0, 1, 1, 2, 2, 3, 3 };
        EmbaralharInt(base, 8);
        for (int i = 0; i < 8; i++) mg->memValores[i] = base[i];
        mg->memSel1 = -1;
        mg->memSel2 = -1;
        mg->memTimerPreview = 1.25f;
    } else if (cena == 4) {
        mg->penAlvo = GetRandomValue(0, 3);
        mg->penHover = -1;
        mg->penArrastando = -1;
        mg->penInspecionando = -1;
        mg->penInspZoom = 1.0f;
        int idxCorreto = GetRandomValue(0, 7);
        static const Vector3 posInit[8] = {
            { -2.65f, 0.06f, -1.05f },
            { -0.85f, 0.06f, -1.20f },
            {  1.15f, 0.06f, -0.95f },
            {  2.75f, 0.06f, -1.10f },
            { -2.35f, 0.06f,  0.85f },
            { -0.55f, 0.06f,  0.65f },
            {  1.05f, 0.06f,  1.05f },
            {  2.65f, 0.06f,  0.75f }
        };
        static const float rotInit[8] = { -28.0f, 19.0f, -52.0f, 34.0f, 48.0f, -14.0f, 72.0f, -38.0f };
        for (int i = 0; i < 8; i++) {
            if (i == idxCorreto) {
                mg->penSimbolos[i] = mg->penAlvo;
            } else {
                int s = (mg->penAlvo + 1 + GetRandomValue(0, 2)) % 4;
                mg->penSimbolos[i] = s;
            }
            mg->penCores[i] = GetRandomValue(0, 3);
            mg->penPos[i] = posInit[i];
            mg->penRot[i] = rotInit[i];
        }
    } else if (cena == 5) {
        mg->sinalPos = 0.1f;
        mg->sinalDir = 1;
        mg->sinalAlvoMin = 0.37f;
        mg->sinalAlvoMax = 0.63f;
        mg->sinalAcertos = 0;
    } else if (cena == 6) {
        for (int i = 0; i < 4; i++) mg->seqAlvo[i] = GetRandomValue(0, 3);
        mg->seqMostrando = true;
        mg->seqPassoShow = 0;
        mg->seqTimer = 0.45f;
        mg->seqBotaoAtivo = -1;
    } else if (cena == 7) {
        int mapa[4] = { 0, 1, 2, 3 };
        EmbaralharInt(mapa, 4);
        for (int i = 0; i < 4; i++) mg->redeDireitaMap[i] = mapa[i];
        mg->redeSelEsq = -1;
    }
}

static inline bool MGBotaoClicado(Rectangle r, Vector2 mouse) {
    return CheckCollisionPointRec(mouse, r) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

static inline void MGDesenharBotao(FontesUI *f, Rectangle r, const char *texto, Vector2 mouse, bool destaque, Color corBorda) {
    bool hover = CheckCollisionPointRec(mouse, r);
    Color fundo = destaque ? (Color){ 12, 65, 92, 245 } : (Color){ 22, 32, 54, 240 };
    if (hover) {
        fundo.r = (unsigned char)(fundo.r + 14);
        fundo.g = (unsigned char)(fundo.g + 22);
        fundo.b = (unsigned char)(fundo.b + 30);
    }
    DrawRectangleRounded(r, 0.2f, 6, fundo);
    DrawRectangleRoundedLinesEx(r, 0.2f, 6, hover ? 2.2f : 1.5f, hover ? (Color){ 120, 240, 255, 255 } : corBorda);
    DesenharTextoCentralizadoUI(f->bold, texto, r, 22, RAYWHITE);
}

static inline void MGRegistrarErro(EstadoMiniGame *mg, const char *msg) {
    mg->chances--;
    mg->feedbackErro = true;
    mg->timerFeedback = 1.6f;
    snprintf(mg->msgFeedback, sizeof(mg->msgFeedback), "%s", msg);
}

static inline Camera3D MGGavetaCamera(void) {
    Camera3D cam = { 0 };
    cam.position = (Vector3){ 0.0f, 7.6f, 4.4f };
    cam.target   = (Vector3){ 0.0f, 0.0f, 0.20f };
    cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    cam.fovy     = 38.0f;
    cam.projection = CAMERA_PERSPECTIVE;
    return cam;
}

static inline Rectangle MGGavetaArea(void) {
    return (Rectangle){ 86.0f, 228.0f, 628.0f, 266.0f };
}

static inline RenderTexture2D *MGObterRTGaveta(bool criar) {
    static RenderTexture2D rt = { 0 };
    if (criar && rt.id == 0) {
        rt = LoadRenderTexture(628, 266);
        SetTextureFilter(rt.texture, TEXTURE_FILTER_BILINEAR);
    }
    return &rt;
}

static inline void DescarregarMiniGame(void) {
    RenderTexture2D *rt = MGObterRTGaveta(false);
    if (rt->id != 0) {
        UnloadRenderTexture(*rt);
        *rt = (RenderTexture2D){ 0 };
    }
}

static inline void MGPenDrivePosRot(int i, Vector3 *pos, float *rotY) {
    static const Vector3 posicoes[8] = {
        { -2.65f, 0.06f, -1.05f },
        { -0.85f, 0.06f, -1.20f },
        {  1.15f, 0.06f, -0.95f },
        {  2.75f, 0.06f, -1.10f },
        { -2.35f, 0.06f,  0.85f },
        { -0.55f, 0.06f,  0.65f },
        {  1.05f, 0.06f,  1.05f },
        {  2.65f, 0.06f,  0.75f }
    };
    static const float angulos[8] = { -28.0f, 19.0f, -52.0f, 34.0f, 48.0f, -14.0f, 72.0f, -38.0f };
    *pos = posicoes[i];
    *rotY = angulos[i];
}

// Desenha o símbolo diretamente na geometria 3D do pen drive (em relevo neon na base)
static inline void DesenharSimboloPenDrive3D(int tipo, float yBase, float zCentro, float r, Color cor) {
    float esp = 0.011f;
    if (tipo == 0) {
        // Triângulo 3D
        Vector3 p1 = { 0.0f, yBase, zCentro - r };
        Vector3 p2 = { -r * 0.92f, yBase, zCentro + r * 0.78f };
        Vector3 p3 = {  r * 0.92f, yBase, zCentro + r * 0.78f };
        DrawCylinderEx(p1, p2, esp, esp, 6, cor);
        DrawCylinderEx(p2, p3, esp, esp, 6, cor);
        DrawCylinderEx(p3, p1, esp, esp, 6, cor);
    } else if (tipo == 1) {
        // Losango 3D
        Vector3 p1 = { 0.0f, yBase, zCentro - r };
        Vector3 p2 = {  r * 0.88f, yBase, zCentro };
        Vector3 p3 = { 0.0f, yBase, zCentro + r };
        Vector3 p4 = { -r * 0.88f, yBase, zCentro };
        DrawCylinderEx(p1, p2, esp, esp, 6, cor);
        DrawCylinderEx(p2, p3, esp, esp, 6, cor);
        DrawCylinderEx(p3, p4, esp, esp, 6, cor);
        DrawCylinderEx(p4, p1, esp, esp, 6, cor);
    } else if (tipo == 2) {
        // Alvo 3D (anel + núcleo)
        const int segs = 14;
        for (int i = 0; i < segs; i++) {
            float a0 = (2.0f * PI * i) / segs;
            float a1 = (2.0f * PI * (i + 1)) / segs;
            Vector3 p0 = { cosf(a0) * r, yBase, zCentro + sinf(a0) * r };
            Vector3 p1 = { cosf(a1) * r, yBase, zCentro + sinf(a1) * r };
            DrawCylinderEx(p0, p1, esp, esp, 5, cor);
        }
        DrawCylinder((Vector3){ 0.0f, yBase - 0.004f, zCentro }, r * 0.34f, r * 0.34f, 0.012f, 10, cor);
    } else {
        // Cruz-X 3D
        float d = r * 0.78f;
        DrawCylinderEx((Vector3){ -d, yBase, zCentro - d }, (Vector3){  d, yBase, zCentro + d }, esp, esp, 6, cor);
        DrawCylinderEx((Vector3){  d, yBase, zCentro - d }, (Vector3){ -d, yBase, zCentro + d }, esp, esp, 6, cor);
    }
}

// Desenha um pen drive 3D afiado, chanfrado e tecnológico no estilo cyberpunk
static inline void DesenharPenDriveCyberpunk3D(Vector3 pos, float rotY, float inspYaw, float inspPitch,
                                                Color corpo, Color neon, bool testado, bool hover,
                                                double tempo, int simbolo) {
    float lift = hover ? 0.12f : 0.0f;
    float pulso = testado ? 0.35f : (0.72f + 0.28f * (float)sin(tempo * 4.5 + pos.x * 1.7));
    Color neonAtivo = {
        neon.r,
        neon.g,
        neon.b,
        (unsigned char)(hover ? 255 : (int)(205 * pulso))
    };
    Color corpoEscuro = {
        (unsigned char)(corpo.r / 2),
        (unsigned char)(corpo.g / 2),
        (unsigned char)(corpo.b / 2),
        255
    };
    Color metal = testado ? (Color){ 95, 92, 102, 230 } : (Color){ 195, 212, 232, 255 };
    Color ouro  = testado ? (Color){ 115, 95, 55, 210 } : (Color){ 250, 195, 60, 255 };

    rlPushMatrix();
    rlTranslatef(pos.x, pos.y + lift, pos.z);
    if (inspYaw != 0.0f || inspPitch != 0.0f) {
        rlRotatef(inspPitch, 1.0f, 0.0f, 0.0f);
        rlRotatef(inspYaw, 0.0f, 1.0f, 0.0f);
    } else {
        rlRotatef(rotY, 0.0f, 1.0f, 0.0f);
    }

    // 1. Corpo principal chanfrado e afunilado (perfil hexágono-blade)
    // Seções ao longo de Z: nariz (-0.36), ombro (-0.10), cintura (+0.34), cauda (+0.58)
    const float zSec[4] = { -0.36f, -0.10f, 0.34f, 0.58f };
    const float wMid[4] = {  0.14f,  0.22f, 0.19f, 0.11f };
    const float wTop[4] = {  0.10f,  0.16f, 0.13f, 0.07f };
    const float wBot[4] = {  0.11f,  0.17f, 0.14f, 0.08f };
    const float yTop =  0.075f;
    const float yMid =  0.005f;
    const float yBot = -0.060f;

    rlDisableBackfaceCulling();
    rlBegin(RL_TRIANGLES);
    for (int s = 0; s < 3; s++) {
        float z0 = zSec[s], z1 = zSec[s + 1];
        float wt0 = wTop[s], wt1 = wTop[s + 1];
        float wm0 = wMid[s], wm1 = wMid[s + 1];
        float wb0 = wBot[s], wb1 = wBot[s + 1];
        Color cTop = (s == 1) ? corpo : corpoEscuro;
        Color cChanfro = { (unsigned char)(corpo.r * 0.72f), (unsigned char)(corpo.g * 0.72f), (unsigned char)(corpo.b * 0.72f), 255 };

        // Deck superior
        rlColor4ub(cTop.r, cTop.g, cTop.b, cTop.a);
        rlVertex3f(-wt0, yTop, z0); rlVertex3f( wt0, yTop, z0); rlVertex3f( wt1, yTop, z1);
        rlVertex3f(-wt0, yTop, z0); rlVertex3f( wt1, yTop, z1); rlVertex3f(-wt1, yTop, z1);

        // Chanfro superior direito e esquerdo (efeito afiado)
        rlColor4ub(cChanfro.r, cChanfro.g, cChanfro.b, cChanfro.a);
        rlVertex3f( wt0, yTop, z0); rlVertex3f( wm0, yMid, z0); rlVertex3f( wm1, yMid, z1);
        rlVertex3f( wt0, yTop, z0); rlVertex3f( wm1, yMid, z1); rlVertex3f( wt1, yTop, z1);

        rlVertex3f(-wm0, yMid, z0); rlVertex3f(-wt0, yTop, z0); rlVertex3f(-wt1, yTop, z1);
        rlVertex3f(-wm0, yMid, z0); rlVertex3f(-wt1, yTop, z1); rlVertex3f(-wm1, yMid, z1);

        // Chanfro inferior direito e esquerdo + base
        rlColor4ub(corpoEscuro.r, corpoEscuro.g, corpoEscuro.b, corpoEscuro.a);
        rlVertex3f( wm0, yMid, z0); rlVertex3f( wb0, yBot, z0); rlVertex3f( wb1, yBot, z1);
        rlVertex3f( wm0, yMid, z0); rlVertex3f( wb1, yBot, z1); rlVertex3f( wm1, yMid, z1);

        rlVertex3f(-wb0, yBot, z0); rlVertex3f(-wm0, yMid, z0); rlVertex3f(-wm1, yMid, z1);
        rlVertex3f(-wb0, yBot, z0); rlVertex3f(-wm1, yMid, z1); rlVertex3f(-wb1, yBot, z1);

        rlVertex3f(-wb0, yBot, z0); rlVertex3f( wb1, yBot, z1); rlVertex3f( wb0, yBot, z0);
        rlVertex3f(-wb0, yBot, z0); rlVertex3f(-wb1, yBot, z1); rlVertex3f( wb1, yBot, z1);
    }

    // Tampa frontal (nariz) e tampa traseira (cauda)
    rlColor4ub(corpoEscuro.r, corpoEscuro.g, corpoEscuro.b, corpoEscuro.a);
    rlVertex3f(-wTop[0], yTop, zSec[0]); rlVertex3f(wTop[0], yTop, zSec[0]); rlVertex3f(wBot[0], yBot, zSec[0]);
    rlVertex3f(-wTop[0], yTop, zSec[0]); rlVertex3f(wBot[0], yBot, zSec[0]); rlVertex3f(-wBot[0], yBot, zSec[0]);
    rlVertex3f(-wTop[3], yTop, zSec[3]); rlVertex3f(wBot[3], yBot, zSec[3]); rlVertex3f(wTop[3], yTop, zSec[3]);
    rlVertex3f(-wTop[3], yTop, zSec[3]); rlVertex3f(-wBot[3], yBot, zSec[3]); rlVertex3f(wBot[3], yBot, zSec[3]);

    // Pontas traseiras duplas (cyber-tail V-notch)
    rlColor4ub(corpo.r, corpo.g, corpo.b, corpo.a);
    rlVertex3f(-0.10f, yMid, 0.56f); rlVertex3f(-0.04f, yMid, 0.56f); rlVertex3f(-0.08f, yMid + 0.02f, 0.70f);
    rlVertex3f( 0.04f, yMid, 0.56f); rlVertex3f( 0.10f, yMid, 0.56f); rlVertex3f( 0.08f, yMid + 0.02f, 0.70f);
    rlEnd();
    rlEnableBackfaceCulling();

    // 2. Trilhos neon laterais + contorno afiado das arestas
    rlBegin(RL_LINES);
    rlColor4ub(neonAtivo.r, neonAtivo.g, neonAtivo.b, neonAtivo.a);
    for (int s = 0; s < 3; s++) {
        // Arestas superiores
        rlVertex3f(-wTop[s], yTop + 0.003f, zSec[s]); rlVertex3f(-wTop[s + 1], yTop + 0.003f, zSec[s + 1]);
        rlVertex3f( wTop[s], yTop + 0.003f, zSec[s]); rlVertex3f( wTop[s + 1], yTop + 0.003f, zSec[s + 1]);
        // Arestas da cintura (lâmina lateral)
        rlVertex3f(-wMid[s], yMid, zSec[s]); rlVertex3f(-wMid[s + 1], yMid, zSec[s + 1]);
        rlVertex3f( wMid[s], yMid, zSec[s]); rlVertex3f( wMid[s + 1], yMid, zSec[s + 1]);
    }
    // Contorno frontal, ombro e cauda
    rlVertex3f(-wTop[0], yTop + 0.003f, zSec[0]); rlVertex3f(wTop[0], yTop + 0.003f, zSec[0]);
    rlVertex3f(-wTop[1], yTop + 0.003f, zSec[1]); rlVertex3f(wTop[1], yTop + 0.003f, zSec[1]);
    rlVertex3f(-wTop[3], yTop + 0.003f, zSec[3]); rlVertex3f(wTop[3], yTop + 0.003f, zSec[3]);
    // Circuito central no deck
    rlVertex3f(0.0f, yTop + 0.005f, -0.30f); rlVertex3f(0.0f, yTop + 0.005f, 0.34f);
    rlEnd();

    // 3. Detalhes tecnológicos no deck: canaleta de carbono, aletas de dissipação e LED chevron
    DrawCube((Vector3){ 0.0f, yTop + 0.004f, 0.05f }, 0.055f, 0.012f, 0.44f, (Color){ 8, 12, 22, 255 });
    for (int r = 0; r < 3; r++) {
        float rz = -0.24f + r * 0.07f;
        DrawCube((Vector3){ 0.0f, yTop + 0.008f, rz }, 0.18f, 0.014f, 0.025f, corpoEscuro);
        DrawCube((Vector3){ 0.0f, yTop + 0.012f, rz }, 0.16f, 0.006f, 0.012f, neonAtivo);
    }
    // Núcleo LED traseiro pulsante
    DrawCube((Vector3){ 0.0f, yTop + 0.010f, 0.42f }, 0.09f, 0.018f, 0.11f, neonAtivo);

    // 4. Colar blindado + conector USB metálico com pinos dourados
    DrawCube((Vector3){ 0.0f, 0.0f, -0.395f }, 0.25f, 0.11f, 0.07f, (Color){ 18, 24, 38, 255 });
    DrawCube((Vector3){ 0.0f, 0.0f, -0.575f }, 0.22f, 0.082f, 0.30f, metal);
    DrawCubeWires((Vector3){ 0.0f, 0.0f, -0.575f }, 0.225f, 0.086f, 0.305f, (Color){ 70, 90, 120, 255 });
    // Pinos dourados no conector USB
    for (int p = -1; p <= 1; p++) {
        DrawCube((Vector3){ p * 0.052f, 0.042f, -0.62f }, 0.024f, 0.008f, 0.14f, ouro);
    }

    // 5. Placa inferior recuada com o símbolo gravado diretamente na geometria 3D
    DrawCube((Vector3){ 0.0f, yBot - 0.003f, 0.08f }, 0.24f, 0.008f, 0.30f, (Color){ 8, 12, 22, 255 });
    DrawCubeWires((Vector3){ 0.0f, yBot - 0.003f, 0.08f }, 0.245f, 0.010f, 0.305f, neonAtivo);
    Color corSimbolo = testado ? (Color){ 220, 75, 85, 220 } : (Color){ 0, 240, 255, 255 };
    DesenharSimboloPenDrive3D(simbolo, yBot - 0.008f, 0.08f, 0.085f, corSimbolo);

    rlPopMatrix();
}

static inline void PrepararMiniGame3D(EstadoMiniGame *mg, int cena) {
    if (cena != 4) return;
    if (mg->desafioAtual != cena) {
        IniciarMiniGame(mg, cena);
    }

    RenderTexture2D *rt = MGObterRTGaveta(true);
    double tempo = GetTime();

    Color paletaCorpo[4] = {
        { 46, 22, 74, 255 },   // roxo cyberpunk
        { 18, 40, 72, 255 },   // azul cyber
        { 62, 20, 56, 255 },   // vinho neon
        { 30, 36, 54, 255 }    // grafite tech
    };
    Color paletaNeon[4] = {
        { 255, 55, 185, 255 }, // magenta
        { 0, 230, 255, 255 },  // ciano
        { 245, 215, 55, 255 }, // amarelo ácido
        { 170, 90, 255, 255 }  // violeta
    };

    BeginTextureMode(*rt);
    ClearBackground((Color){ 8, 10, 22, 255 });

    if (mg->penInspecionando >= 0) {
        // Modo Inspeção 360° estilo Resident Evil
        int idx = mg->penInspecionando;
        Camera3D camInsp = { 0 };
        camInsp.position = (Vector3){ 0.0f, 0.35f, 2.25f / (mg->penInspZoom > 0.1f ? mg->penInspZoom : 1.0f) };
        camInsp.target   = (Vector3){ 0.0f, -0.08f, 0.0f };
        camInsp.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
        camInsp.fovy     = 36.0f;
        camInsp.projection = CAMERA_PERSPECTIVE;

        BeginMode3D(camInsp);
        // Grade holográfica de scanner ao fundo/base
        for (float gx = -1.6f; gx <= 1.6f; gx += 0.4f) {
            DrawLine3D((Vector3){ gx, -0.68f, -1.6f }, (Vector3){ gx, -0.68f, 1.6f }, (Color){ 0, 200, 240, 55 });
            DrawLine3D((Vector3){ -1.6f, -0.68f, gx }, (Vector3){ 1.6f, -0.68f, gx }, (Color){ 180, 40, 150, 45 });
        }
        bool testado = mg->penTestado[idx];
        Color corpo = testado ? (Color){ 52, 22, 28, 235 } : paletaCorpo[mg->penCores[idx]];
        Color neon  = testado ? (Color){ 220, 60, 75, 200 } : paletaNeon[mg->penCores[idx]];
        DesenharPenDriveCyberpunk3D((Vector3){ 0.0f, 0.04f, 0.0f }, 0.0f, mg->penInspYaw, mg->penInspPitch,
                                    corpo, neon, testado, false, tempo, mg->penSimbolos[idx]);
        EndMode3D();
        EndTextureMode();
        return;
    }

    Camera3D cam = MGGavetaCamera();
    BeginMode3D(cam);

    // Fundo e grade dual-tone da gaveta cyberpunk
    DrawCube((Vector3){ 0.0f, -0.35f, 0.0f }, 8.6f, 0.30f, 5.2f, (Color){ 12, 14, 28, 255 });
    for (float gx = -4.0f; gx <= 4.0f; gx += 0.8f) {
        DrawLine3D((Vector3){ gx, -0.19f, -2.5f }, (Vector3){ gx, -0.19f, 2.5f }, (Color){ 165, 30, 145, 110 });
    }
    for (float gz = -2.4f; gz <= 2.4f; gz += 0.8f) {
        DrawLine3D((Vector3){ -4.2f, -0.19f, gz }, (Vector3){ 4.2f, -0.19f, gz }, (Color){ 0, 200, 240, 95 });
    }

    // Bordas da gaveta + puxador frontal neon
    DrawCube((Vector3){ -4.25f, 0.15f,  0.0f }, 0.25f, 0.90f, 5.2f, (Color){ 20, 24, 44, 255 });
    DrawCube((Vector3){  4.25f, 0.15f,  0.0f }, 0.25f, 0.90f, 5.2f, (Color){ 20, 24, 44, 255 });
    DrawCube((Vector3){  0.0f,  0.15f, -2.55f }, 8.6f, 0.90f, 0.25f, (Color){ 20, 24, 44, 255 });
    DrawCube((Vector3){  0.0f,  0.15f,  2.55f }, 8.6f, 0.90f, 0.25f, (Color){ 26, 28, 52, 255 });
    DrawCubeWires((Vector3){ 0.0f, 0.15f, 0.0f }, 8.55f, 0.92f, 5.15f, (Color){ 0, 215, 255, 195 });
    DrawCube((Vector3){ 0.0f, 0.35f, 2.74f }, 1.9f, 0.14f, 0.16f, (Color){ 255, 45, 175, 255 });

    // Cabos de dados e microchips soltos na gaveta
    DrawCylinderEx((Vector3){ -3.6f, -0.10f, -2.0f }, (Vector3){ -1.4f, -0.05f, -0.2f }, 0.042f, 0.042f, 6, (Color){ 255, 40, 165, 210 });
    DrawCylinderEx((Vector3){ -1.4f, -0.05f, -0.2f }, (Vector3){  0.2f, -0.10f,  1.8f }, 0.042f, 0.042f, 6, (Color){ 255, 40, 165, 210 });
    DrawCylinderEx((Vector3){  3.5f, -0.10f, -1.9f }, (Vector3){  1.8f, -0.04f,  0.1f }, 0.042f, 0.042f, 6, (Color){ 0, 220, 255, 210 });

    rlPushMatrix();
    rlTranslatef(-3.4f, -0.08f, 1.55f);
    rlRotatef(24.0f, 0.0f, 1.0f, 0.0f);
    DrawCube((Vector3){ 0, 0, 0 }, 0.92f, 0.07f, 0.62f, (Color){ 28, 32, 58, 255 });
    DrawCubeWires((Vector3){ 0, 0, 0 }, 0.93f, 0.08f, 0.63f, (Color){ 245, 210, 60, 180 });
    rlPopMatrix();

    rlPushMatrix();
    rlTranslatef(3.45f, -0.08f, -0.15f);
    rlRotatef(-40.0f, 0.0f, 1.0f, 0.0f);
    DrawCube((Vector3){ 0, 0, 0 }, 0.70f, 0.07f, 0.50f, (Color){ 22, 36, 58, 255 });
    DrawCubeWires((Vector3){ 0, 0, 0 }, 0.71f, 0.08f, 0.51f, (Color){ 0, 220, 255, 165 });
    rlPopMatrix();

    for (int i = 0; i < 8; i++) {
        bool testado = mg->penTestado[i];
        bool hover = ((mg->penHover == i) || (mg->penArrastando == i)) && !testado;

        Color corpo = testado ? (Color){ 52, 22, 28, 235 } : paletaCorpo[mg->penCores[i]];
        Color neon  = testado ? (Color){ 220, 60, 75, 200 }
                    : hover   ? RAYWHITE
                              : paletaNeon[mg->penCores[i]];

        DesenharPenDriveCyberpunk3D(mg->penPos[i], mg->penRot[i], 0.0f, 0.0f, corpo, neon, testado, hover, tempo, mg->penSimbolos[i]);
    }

    EndMode3D();
    EndTextureMode();
}

// Retorna: 0 = em andamento, 1 = venceu, -1 = perdeu (sem chances)
static inline int AtualizarMiniGame(EstadoMiniGame *mg, int cena, Vector2 mouse, float dt) {
    if (mg->desafioAtual != cena) {
        IniciarMiniGame(mg, cena);
    }

    if (mg->timerFeedback > 0.0f) {
        mg->timerFeedback -= dt;
        if (mg->timerFeedback <= 0.0f) {
            mg->timerFeedback = 0.0f;
            mg->msgFeedback[0] = '\0';
        }
    }

    if (cena == 2) {
        // Memória de Dados
        if (mg->memTimerPreview > 0.0f) {
            mg->memTimerPreview -= dt;
            return 0;
        }
        if (mg->memTimerMismatch > 0.0f) {
            mg->memTimerMismatch -= dt;
            if (mg->memTimerMismatch <= 0.0f) {
                mg->memSel1 = -1;
                mg->memSel2 = -1;
                if (mg->chances <= 0) return -1;
            }
            return 0;
        }

        for (int i = 0; i < 8; i++) {
            if (mg->memEncontrada[i] || i == mg->memSel1) continue;
            int col = i % 4;
            int lin = i / 4;
            Rectangle card = { 145.0f + col * 132.0f, 185.0f + lin * 136.0f, 114.0f, 116.0f };
            if (MGBotaoClicado(card, mouse)) {
                if (mg->memSel1 < 0) {
                    mg->memSel1 = i;
                } else {
                    mg->memSel2 = i;
                    if (mg->memValores[mg->memSel1] == mg->memValores[mg->memSel2]) {
                        mg->memEncontrada[mg->memSel1] = true;
                        mg->memEncontrada[mg->memSel2] = true;
                        mg->memSel1 = -1;
                        mg->memSel2 = -1;
                        int pares = 0;
                        for (int k = 0; k < 8; k++) if (mg->memEncontrada[k]) pares++;
                        if (pares == 8) return 1;
                    } else {
                        mg->memTimerMismatch = 0.65f;
                        char msgErr[96];
                        snprintf(msgErr, sizeof(msgErr), "Par incorreto! Restam %d chance(s).", mg->chances - 1);
                        MGRegistrarErro(mg, msgErr);
                    }
                }
                break;
            }
        }
    } else if (cena == 4) {
        // Gaveta 3D Cyberpunk: Física 3D + Inspeção 360° estilo Resident Evil
        Rectangle areaGaveta = MGGavetaArea();

        if (mg->penInspecionando >= 0) {
            Rectangle btnSelecionar   = { 158.0f, 446.0f, 244.0f, 40.0f };
            Rectangle btnVoltarGaveta = { 420.0f, 446.0f, 222.0f, 40.0f };

            if (MGBotaoClicado(btnVoltarGaveta, mouse) || IsKeyPressed(KEY_BACKSPACE)) {
                mg->penInspecionando = -1;
                mg->penInspArrastando = false;
                return 0;
            }
            if (MGBotaoClicado(btnSelecionar, mouse)) {
                int i = mg->penInspecionando;
                if (mg->penSimbolos[i] == mg->penAlvo) {
                    return 1;
                } else {
                    mg->penTestado[i] = true;
                    mg->penInspecionando = -1;
                    mg->penInspArrastando = false;
                    MGRegistrarErro(mg, "Pen drive errado! Resta 1 chance.");
                    if (mg->chances <= 0) return -1;
                    return 0;
                }
            }

            Rectangle areaRotacao = { areaGaveta.x, areaGaveta.y, areaGaveta.width, areaGaveta.height - 50.0f };
            if (CheckCollisionPointRec(mouse, areaRotacao) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                mg->penInspArrastando = true;
                mg->penInspMouseAnt = mouse;
            }
            if (mg->penInspArrastando && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                Vector2 delta = { mouse.x - mg->penInspMouseAnt.x, mouse.y - mg->penInspMouseAnt.y };
                mg->penInspYaw += delta.x * 0.55f;
                mg->penInspPitch += delta.y * 0.55f;
                mg->penInspVelYaw = delta.x * 22.0f;
                mg->penInspVelPitch = delta.y * 22.0f;
                mg->penInspMouseAnt = mouse;
            } else {
                mg->penInspArrastando = false;
                mg->penInspYaw += mg->penInspVelYaw * dt;
                mg->penInspPitch += mg->penInspVelPitch * dt;
                float damp = expf(-6.0f * dt);
                mg->penInspVelYaw *= damp;
                mg->penInspVelPitch *= damp;
            }

            float wheel = GetMouseWheelMove();
            if (wheel != 0.0f) {
                mg->penInspZoom += wheel * 0.12f;
                if (mg->penInspZoom < 0.75f) mg->penInspZoom = 0.75f;
                if (mg->penInspZoom > 1.75f) mg->penInspZoom = 1.75f;
            }
            return 0;
        }

        // Passo de física 3D na gaveta (inércia, atrito, paredes e colisões entre pen drives)
        float damp = expf(-7.5f * dt);
        for (int i = 0; i < 8; i++) {
            if (mg->penArrastando != i) {
                mg->penPos[i].x += mg->penVel[i].x * dt;
                mg->penPos[i].z += mg->penVel[i].z * dt;
                mg->penVel[i].x *= damp;
                mg->penVel[i].z *= damp;
            }
            mg->penRot[i] += mg->penVelRot[i] * dt;
            mg->penVelRot[i] *= damp;

            if (mg->penPos[i].x < -3.45f) { mg->penPos[i].x = -3.45f; mg->penVel[i].x *= -0.4f; }
            if (mg->penPos[i].x >  3.45f) { mg->penPos[i].x =  3.45f; mg->penVel[i].x *= -0.4f; }
            if (mg->penPos[i].z < -1.85f) { mg->penPos[i].z = -1.85f; mg->penVel[i].z *= -0.4f; }
            if (mg->penPos[i].z >  1.85f) { mg->penPos[i].z =  1.85f; mg->penVel[i].z *= -0.4f; }
        }
        for (int i = 0; i < 8; i++) {
            for (int j = i + 1; j < 8; j++) {
                float dx = mg->penPos[j].x - mg->penPos[i].x;
                float dz = mg->penPos[j].z - mg->penPos[i].z;
                float dist2 = dx * dx + dz * dz;
                const float minDist = 0.88f;
                if (dist2 > 0.0001f && dist2 < minDist * minDist) {
                    float dist = sqrtf(dist2);
                    float nx = dx / dist, nz = dz / dist;
                    float overlap = (minDist - dist) * 0.5f;
                    if (mg->penArrastando != i) {
                        mg->penPos[i].x -= nx * overlap;
                        mg->penPos[i].z -= nz * overlap;
                        mg->penVel[i].x -= nx * 1.8f;
                        mg->penVel[i].z -= nz * 1.8f;
                        mg->penVelRot[i] += (nx - nz) * 18.0f;
                    }
                    if (mg->penArrastando != j) {
                        mg->penPos[j].x += nx * overlap;
                        mg->penPos[j].z += nz * overlap;
                        mg->penVel[j].x += nx * 1.8f;
                        mg->penVel[j].z += nz * 1.8f;
                        mg->penVelRot[j] -= (nx - nz) * 18.0f;
                    }
                }
            }
        }

        Camera3D cam = MGGavetaCamera();
        mg->penHover = -1;
        Ray ray = { 0 };
        bool mouseNaGaveta = CheckCollisionPointRec(mouse, areaGaveta);
        if (mouseNaGaveta) {
            Vector2 mouseLocal = { mouse.x - areaGaveta.x, mouse.y - areaGaveta.y };
            ray = GetScreenToWorldRayEx(mouseLocal, cam, (int)areaGaveta.width, (int)areaGaveta.height);
            float melhorDist = 9999.0f;

            for (int i = 0; i < 8; i++) {
                Vector3 p = mg->penPos[i];
                float rot = mg->penRot[i];
                float rad = -rot * DEG2RAD;
                float cs = cosf(rad), sn = sinf(rad);
                Vector3 relOrig = { ray.position.x - p.x, ray.position.y - p.y, ray.position.z - p.z };
                Ray rayLocal = {
                    (Vector3){ relOrig.x * cs - relOrig.z * sn, relOrig.y, relOrig.x * sn + relOrig.z * cs },
                    (Vector3){ ray.direction.x * cs - ray.direction.z * sn, ray.direction.y, ray.direction.x * sn + ray.direction.z * cs }
                };
                BoundingBox box = {
                    (Vector3){ -0.28f, -0.14f, -0.78f },
                    (Vector3){  0.28f,  0.22f,  0.74f }
                };
                RayCollision hit = GetRayCollisionBox(rayLocal, box);
                if (hit.hit && hit.distance < melhorDist) {
                    melhorDist = hit.distance;
                    mg->penHover = i;
                }
            }
        }

        if (mg->penHover >= 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            mg->penArrastando = mg->penHover;
            mg->penClickPos = mouse;
            mg->penArrastou = false;
        }
        if (mg->penArrastando >= 0 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            int idx = mg->penArrastando;
            float dx = mouse.x - mg->penClickPos.x;
            float dy = mouse.y - mg->penClickPos.y;
            if (dx * dx + dy * dy > 36.0f) mg->penArrastou = true;

            if (mg->penArrastou && mouseNaGaveta && fabsf(ray.direction.y) > 0.001f) {
                float t = (0.06f - ray.position.y) / ray.direction.y;
                if (t > 0.0f) {
                    float tx = ray.position.x + ray.direction.x * t;
                    float tz = ray.position.z + ray.direction.z * t;
                    if (tx < -3.45f) tx = -3.45f;
                    if (tx >  3.45f) tx =  3.45f;
                    if (tz < -1.85f) tz = -1.85f;
                    if (tz >  1.85f) tz =  1.85f;
                    float vx = (tx - mg->penPos[idx].x) / (dt > 0.001f ? dt : 0.016f);
                    float vz = (tz - mg->penPos[idx].z) / (dt > 0.001f ? dt : 0.016f);
                    mg->penVel[idx].x = vx * 0.45f;
                    mg->penVel[idx].z = vz * 0.45f;
                    mg->penVelRot[idx] += (vx - vz) * 0.35f * dt * 60.0f;
                    mg->penPos[idx].x = tx;
                    mg->penPos[idx].z = tz;
                    mg->penPos[idx].y = 0.20f;
                }
            }
        }
        if (mg->penArrastando >= 0 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            int idx = mg->penArrastando;
            mg->penPos[idx].y = 0.06f;
            mg->penArrastando = -1;
            if (!mg->penArrastou && !mg->penTestado[idx]) {
                mg->penInspecionando = idx;
                mg->penInspYaw = 28.0f;
                mg->penInspPitch = 32.0f;
                mg->penInspVelYaw = 0.0f;
                mg->penInspVelPitch = 0.0f;
                mg->penInspZoom = 1.0f;
                mg->penInspArrastando = false;
            }
        }
    } else if (cena == 5) {
        // Interceptação de Sinal
        float vel = (mg->sinalAcertos == 0) ? 0.85f : 1.20f;
        mg->sinalPos += mg->sinalDir * vel * dt;
        if (mg->sinalPos >= 1.0f) { mg->sinalPos = 1.0f; mg->sinalDir = -1; }
        if (mg->sinalPos <= 0.0f) { mg->sinalPos = 0.0f; mg->sinalDir = 1; }

        Rectangle btnSinc = { 275.0f, 380.0f, 250.0f, 52.0f };
        if (MGBotaoClicado(btnSinc, mouse) || IsKeyPressed(KEY_SPACE)) {
            if (mg->sinalPos >= mg->sinalAlvoMin && mg->sinalPos <= mg->sinalAlvoMax) {
                mg->sinalAcertos++;
                if (mg->sinalAcertos >= 2) return 1;
                mg->sinalAlvoMin = 0.42f;
                mg->sinalAlvoMax = 0.58f;
                mg->feedbackErro = false;
                mg->timerFeedback = 1.2f;
                snprintf(mg->msgFeedback, sizeof(mg->msgFeedback), "Sinal 1/2 capturado! Mais um!");
            } else {
                MGRegistrarErro(mg, "Fora da zona de sincronia! Resta 1 chance.");
                if (mg->chances <= 0) return -1;
            }
        }
    } else if (cena == 6) {
        // Sequência de Segurança (Simon)
        if (mg->seqTimerClique > 0.0f) {
            mg->seqTimerClique -= dt;
            if (mg->seqTimerClique <= 0.0f) mg->seqBotaoAtivo = -1;
        }
        if (mg->seqMostrando) {
            mg->seqTimer -= dt;
            if (mg->seqTimer <= 0.0f) {
                if (mg->seqBotaoAtivo >= 0) {
                    mg->seqBotaoAtivo = -1;
                    mg->seqPassoShow++;
                    if (mg->seqPassoShow >= 4) {
                        mg->seqMostrando = false;
                        mg->seqProgresso = 0;
                    } else {
                        mg->seqTimer = 0.20f;
                    }
                } else {
                    mg->seqBotaoAtivo = mg->seqAlvo[mg->seqPassoShow];
                    mg->seqTimer = 0.48f;
                }
            }
            return 0;
        }

        for (int i = 0; i < 4; i++) {
            int col = i % 2;
            int lin = i / 2;
            Rectangle mod = { 220.0f + col * 185.0f, 200.0f + lin * 120.0f, 170.0f, 102.0f };
            if (MGBotaoClicado(mod, mouse)) {
                mg->seqBotaoAtivo = i;
                mg->seqTimerClique = 0.22f;
                if (i == mg->seqAlvo[mg->seqProgresso]) {
                    mg->seqProgresso++;
                    if (mg->seqProgresso >= 4) return 1;
                } else {
                    MGRegistrarErro(mg, "Ordem incorreta! Observe novamente.");
                    if (mg->chances <= 0) return -1;
                    mg->seqMostrando = true;
                    mg->seqPassoShow = 0;
                    mg->seqProgresso = 0;
                    mg->seqBotaoAtivo = -1;
                    mg->seqTimer = 0.70f;
                }
                break;
            }
        }
    } else if (cena == 7) {
        // Rastreamento de Rede (ligar nós esquerda -> direita)
        for (int i = 0; i < 4; i++) {
            if (mg->redeConectada[i]) continue;
            Rectangle esq = { 135.0f, 195.0f + i * 74.0f, 175.0f, 54.0f };
            if (MGBotaoClicado(esq, mouse)) {
                mg->redeSelEsq = i;
                break;
            }
        }
        if (mg->redeSelEsq >= 0) {
            for (int j = 0; j < 4; j++) {
                int alvoReal = mg->redeDireitaMap[j];
                if (mg->redeConectada[alvoReal]) continue;
                Rectangle dir = { 490.0f, 195.0f + j * 74.0f, 175.0f, 54.0f };
                if (MGBotaoClicado(dir, mouse)) {
                    if (alvoReal == mg->redeSelEsq) {
                        mg->redeConectada[alvoReal] = true;
                        mg->redeSelEsq = -1;
                        int total = 0;
                        for (int k = 0; k < 4; k++) if (mg->redeConectada[k]) total++;
                        if (total == 4) return 1;
                    } else {
                        mg->redeSelEsq = -1;
                        MGRegistrarErro(mg, "Conexao invalida! Resta 1 chance.");
                        if (mg->chances <= 0) return -1;
                    }
                    break;
                }
            }
        }
    }

    return 0;
}

static inline void DesenharSimboloPenDrive(int tipo, float cx, float cy, float r, Color cor) {
    float esp = 3.2f;
    if (tipo == 0) {
        // Triângulo
        Vector2 p1 = { cx, cy - r };
        Vector2 p2 = { cx - r * 0.92f, cy + r * 0.78f };
        Vector2 p3 = { cx + r * 0.92f, cy + r * 0.78f };
        DrawLineEx(p1, p2, esp, cor);
        DrawLineEx(p2, p3, esp, cor);
        DrawLineEx(p3, p1, esp, cor);
    } else if (tipo == 1) {
        // Losango
        Vector2 p1 = { cx, cy - r };
        Vector2 p2 = { cx + r * 0.9f, cy };
        Vector2 p3 = { cx, cy + r };
        Vector2 p4 = { cx - r * 0.9f, cy };
        DrawLineEx(p1, p2, esp, cor);
        DrawLineEx(p2, p3, esp, cor);
        DrawLineEx(p3, p4, esp, cor);
        DrawLineEx(p4, p1, esp, cor);
    } else if (tipo == 2) {
        // Círculo duplo (Alvo)
        DrawRing((Vector2){ cx, cy }, r - esp, r, 0, 360, 32, cor);
        DrawCircle((int)cx, (int)cy, r * 0.32f, cor);
    } else {
        // Cruz / X
        float d = r * 0.78f;
        DrawLineEx((Vector2){ cx - d, cy - d }, (Vector2){ cx + d, cy + d }, esp + 0.5f, cor);
        DrawLineEx((Vector2){ cx + d, cy - d }, (Vector2){ cx - d, cy + d }, esp + 0.5f, cor);
    }
}

static inline void DesenharMiniGame(EstadoMiniGame *mg, FontesUI *f, int cena, Vector2 mouse, int larguraTela, int alturaTela) {
    // Fundo escuro + grade sutil combinando com o visual do jogo
    DrawRectangle(0, 0, larguraTela, alturaTela, (Color){ 6, 10, 20, 255 });
    for (int x = 0; x < larguraTela; x += 40) {
        DrawLine(x, 0, x, alturaTela, (Color){ 0, 210, 255, 14 });
    }
    for (int y = 0; y < alturaTela; y += 40) {
        DrawLine(0, y, larguraTela, y, (Color){ 0, 210, 255, 14 });
    }

    Rectangle painel = { 70.0f, 66.0f, 660.0f, 476.0f };
    DrawRectangleRounded(painel, 0.04f, 8, (Color){ 14, 20, 34, 245 });
    DrawRectangleRoundedLinesEx(painel, 0.04f, 8, 2.0f, (Color){ 0, 210, 255, 200 });
    // Cantoneiras tecnológicas do painel
    Color corCanto = (Color){ 120, 245, 255, 235 };
    DrawLineEx((Vector2){ painel.x + 6, painel.y + 20 }, (Vector2){ painel.x + 6, painel.y + 6 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + 6, painel.y + 6 }, (Vector2){ painel.x + 20, painel.y + 6 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + painel.width - 20, painel.y + 6 }, (Vector2){ painel.x + painel.width - 6, painel.y + 6 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + painel.width - 6, painel.y + 6 }, (Vector2){ painel.x + painel.width - 6, painel.y + 20 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + 6, painel.y + painel.height - 20 }, (Vector2){ painel.x + 6, painel.y + painel.height - 6 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + 6, painel.y + painel.height - 6 }, (Vector2){ painel.x + 20, painel.y + painel.height - 6 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + painel.width - 20, painel.y + painel.height - 6 }, (Vector2){ painel.x + painel.width - 6, painel.y + painel.height - 6 }, 2.5f, corCanto);
    DrawLineEx((Vector2){ painel.x + painel.width - 6, painel.y + painel.height - 20 }, (Vector2){ painel.x + painel.width - 6, painel.y + painel.height - 6 }, 2.5f, corCanto);

    const char *titulo = "";
    const char *instrucao = "";
    if (cena == 2) {
        titulo = "DESAFIO 1/5 - RECONSTRUCAO DE DADOS";
        instrucao = "Encontre os 4 pares de fragmentos de memoria";
    } else if (cena == 4) {
        titulo = "DESAFIO 2/5 - GAVETA DE DADOS 3D";
        instrucao = "Ache na gaveta o pen drive com o simbolo requerido";
    } else if (cena == 5) {
        titulo = "DESAFIO 3/5 - INTERCEPTACAO DE SINAL";
        instrucao = "Clique em SINCRONIZAR (ou ESPACO) na zona ciano (2x)";
    } else if (cena == 6) {
        titulo = "DESAFIO 4/5 - SEQUENCIA DE SEGURANCA";
        instrucao = "Observe a ordem dos 4 modulos e repita";
    } else if (cena == 7) {
        titulo = "DESAFIO 5/5 - RASTREAMENTO DE REDE";
        instrucao = "Clique em um no da esquerda e ligue ao seu par na direita";
    }

    DesenharTextoUI(f->bold, titulo, 98.0f, 88.0f, 26.0f, (Color){ 160, 245, 255, 255 });
    DesenharTextoUI(f->regular, instrucao, 98.0f, 122.0f, 20.0f, (Color){ 195, 215, 235, 255 });

    // Indicador de chances no topo direito do painel
    int maxC = (mg->chancesMax > 0) ? mg->chancesMax : 2;
    float labelX = (maxC > 2) ? 496.0f : 552.0f;
    float startDotX = (maxC > 2) ? 602.0f : 658.0f;
    DesenharTextoUI(f->bold, "CHANCES:", labelX, 92.0f, 20.0f, (Color){ 180, 210, 230, 255 });
    for (int c = 0; c < maxC; c++) {
        Color corChance = (c < mg->chances) ? (Color){ 0, 225, 255, 255 } : (Color){ 70, 35, 45, 220 };
        DrawCircle((int)(startDotX + c * 28), 102, 9.0f, corChance);
        DrawCircleLines((int)(startDotX + c * 28), 102, 9.0f, (Color){ 160, 245, 255, 200 });
    }
    DrawLine(96, 155, 704, 155, (Color){ 0, 210, 255, 70 });

    if (cena == 2) {
        const char *simbolos[4] = { "LOG", "ID", "KEY", "GPS" };
        for (int i = 0; i < 8; i++) {
            int col = i % 4;
            int lin = i / 4;
            Rectangle card = { 145.0f + col * 132.0f, 185.0f + lin * 136.0f, 114.0f, 116.0f };
            bool revelada = mg->memEncontrada[i] || i == mg->memSel1 || i == mg->memSel2 || mg->memTimerPreview > 0.0f;
            bool hover = CheckCollisionPointRec(mouse, card) && !revelada;
            Color fundo = mg->memEncontrada[i] ? (Color){ 14, 72, 74, 245 }
                        : revelada             ? (Color){ 24, 48, 82, 245 }
                        : hover                ? (Color){ 32, 44, 72, 245 }
                                               : (Color){ 20, 28, 48, 245 };
            Color borda = mg->memEncontrada[i] ? (Color){ 60, 245, 190, 230 }
                        : hover                ? (Color){ 120, 240, 255, 255 }
                                               : (Color){ 0, 190, 235, 170 };
            DrawRectangleRounded(card, 0.14f, 6, fundo);
            DrawRectangleRoundedLinesEx(card, 0.14f, 6, 2.0f, borda);
            if (revelada) {
                DesenharTextoCentralizadoUI(f->bold, simbolos[mg->memValores[i]], card, 28.0f, RAYWHITE);
            } else {
                DesenharTextoCentralizadoUI(f->bold, "?", card, 34.0f, (Color){ 90, 160, 200, 180 });
            }
        }
    } else if (cena == 4) {
        // Barra compacta do símbolo alvo no topo do painel
        const char *nomesSim[4] = { "TRIANGULO", "LOSANGO", "ALVO", "CRUZ-X" };
        Rectangle caixaAlvo = { 175.0f, 166.0f, 450.0f, 54.0f };
        DrawRectangleRounded(caixaAlvo, 0.14f, 6, (Color){ 20, 14, 40, 240 });
        DrawRectangleRoundedLinesEx(caixaAlvo, 0.14f, 6, 1.5f, (Color){ 255, 55, 175, 200 });
        DesenharTextoUI(f->bold, "SIMBOLO CORRETO:", 198.0f, 182.0f, 20.0f, (Color){ 255, 165, 225, 255 });
        DesenharTextoUI(f->regular, nomesSim[mg->penAlvo], 392.0f, 183.0f, 19.0f, (Color){ 0, 240, 255, 255 });
        Rectangle badgeAlvo = { 556.0f, 172.0f, 42.0f, 42.0f };
        DrawRectangleRounded(badgeAlvo, 0.2f, 6, (Color){ 10, 14, 28, 255 });
        DrawRectangleRoundedLinesEx(badgeAlvo, 0.2f, 6, 1.5f, (Color){ 0, 235, 255, 220 });
        DesenharSimboloPenDrive(mg->penAlvo, badgeAlvo.x + 21.0f, badgeAlvo.y + 21.0f, 12.0f, (Color){ 0, 240, 255, 255 });

        // Viewport 3D pré-renderizada da gaveta Cyberpunk (ou inspeção 360° Resident Evil)
        Rectangle areaGaveta = MGGavetaArea();
        RenderTexture2D *rt = MGObterRTGaveta(false);
        if (rt->id != 0) {
            Rectangle srcRT = { 0.0f, 0.0f, (float)rt->texture.width, -(float)rt->texture.height };
            DrawTexturePro(rt->texture, srcRT, areaGaveta, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
        } else {
            DrawRectangleRec(areaGaveta, (Color){ 8, 10, 22, 255 });
        }
        DrawRectangleLinesEx(areaGaveta, 1.5f, (Color){ 0, 210, 255, 175 });

        if (mg->penInspecionando >= 0) {
            DesenharTextoUI(f->regular, "INSPECAO 360 // ARRASTE PARA GIRAR  |  SCROLL: ZOOM",
                            areaGaveta.x + 10.0f, areaGaveta.y + 6.0f, 15.0f, (Color){ 0, 225, 255, 195 });
            Rectangle btnSelecionar   = { 158.0f, 446.0f, 244.0f, 40.0f };
            Rectangle btnVoltarGaveta = { 420.0f, 446.0f, 222.0f, 40.0f };
            MGDesenharBotao(f, btnSelecionar,   "SELECIONAR PEN DRIVE", mouse, true,  (Color){ 0, 225, 255, 230 });
            MGDesenharBotao(f, btnVoltarGaveta, "VOLTAR A GAVETA",      mouse, false, (Color){ 255, 65, 180, 210 });
        } else {
            DesenharTextoUI(f->regular, "GAVETA 3D // ARRASTE PARA MOVER  |  CLIQUE PARA INSPECIONAR EM 360",
                            areaGaveta.x + 10.0f, areaGaveta.y + 6.0f, 15.0f, (Color){ 0, 225, 255, 175 });
        }
    } else if (cena == 5) {
        char prog[32];
        snprintf(prog, sizeof(prog), "SINCRONIAS: %d / 2", mg->sinalAcertos);
        DesenharTextoUI(f->bold, prog, 318.0f, 195.0f, 24.0f, (Color){ 160, 245, 255, 255 });

        Rectangle barra = { 130.0f, 265.0f, 540.0f, 46.0f };
        DrawRectangleRounded(barra, 0.25f, 6, (Color){ 18, 28, 48, 255 });
        DrawRectangleRoundedLinesEx(barra, 0.25f, 6, 2.0f, (Color){ 80, 170, 220, 180 });

        Rectangle zona = {
            barra.x + mg->sinalAlvoMin * barra.width,
            barra.y + 4.0f,
            (mg->sinalAlvoMax - mg->sinalAlvoMin) * barra.width,
            barra.height - 8.0f
        };
        DrawRectangleRounded(zona, 0.2f, 6, (Color){ 0, 210, 255, 110 });
        DrawRectangleRoundedLinesEx(zona, 0.2f, 6, 2.0f, (Color){ 0, 240, 255, 255 });

        float cursorX = barra.x + mg->sinalPos * (barra.width - 10.0f);
        DrawRectangleRounded((Rectangle){ cursorX, barra.y - 6.0f, 10.0f, barra.height + 12.0f }, 0.4f, 4, RAYWHITE);

        Rectangle btnSinc = { 275.0f, 380.0f, 250.0f, 52.0f };
        MGDesenharBotao(f, btnSinc, "SINCRONIZAR", mouse, true, (Color){ 0, 210, 255, 220 });
    } else if (cena == 6) {
        const char *nomes[4] = { "ALPHA", "BETA", "GAMMA", "DELTA" };
        Color cores[4] = {
            { 0, 185, 235, 255 },
            { 60, 220, 160, 255 },
            { 240, 180, 50, 255 },
            { 225, 85, 125, 255 }
        };
        const char *status = mg->seqMostrando ? "OBSERVE A SEQUENCIA..." : "SUA VEZ! REPITA A ORDEM";
        DesenharTextoUI(f->bold, status, 285.0f, 166.0f, 22.0f,
                        mg->seqMostrando ? (Color){ 255, 215, 110, 255 } : (Color){ 140, 245, 210, 255 });

        for (int i = 0; i < 4; i++) {
            int col = i % 2;
            int lin = i / 2;
            Rectangle mod = { 220.0f + col * 185.0f, 200.0f + lin * 120.0f, 170.0f, 102.0f };
            bool aceso = (mg->seqBotaoAtivo == i);
            bool hover = !mg->seqMostrando && CheckCollisionPointRec(mouse, mod);
            Color fundo = aceso ? cores[i] : (Color){ cores[i].r / 5, cores[i].g / 5, cores[i].b / 5, 240 };
            if (hover && !aceso) {
                fundo.r = (unsigned char)(fundo.r + 25);
                fundo.g = (unsigned char)(fundo.g + 25);
                fundo.b = (unsigned char)(fundo.b + 25);
            }
            DrawRectangleRounded(mod, 0.16f, 6, fundo);
            DrawRectangleRoundedLinesEx(mod, 0.16f, 6, aceso ? 3.0f : 2.0f, cores[i]);
            DesenharTextoCentralizadoUI(f->bold, nomes[i], mod, 24.0f, aceso ? (Color){ 10, 14, 24, 255 } : RAYWHITE);
        }
    } else if (cena == 7) {
        const char *labelsEsq[4] = { "IP LOCAL", "PORTA HTTPS", "PROTOCOLO", "CRIPTOGRAFIA" };
        const char *labelsDir[4] = { "192.168.0.1", "443", "TCP/IP", "SHA-256" };

        // Linhas das conexões já feitas
        for (int i = 0; i < 4; i++) {
            if (!mg->redeConectada[i]) continue;
            int posDir = -1;
            for (int j = 0; j < 4; j++) if (mg->redeDireitaMap[j] == i) posDir = j;
            if (posDir >= 0) {
                Vector2 p1 = { 310.0f, 222.0f + i * 74.0f };
                Vector2 p2 = { 490.0f, 222.0f + posDir * 74.0f };
                DrawLineEx(p1, p2, 3.0f, (Color){ 60, 245, 190, 220 });
            }
        }

        for (int i = 0; i < 4; i++) {
            Rectangle esq = { 135.0f, 195.0f + i * 74.0f, 175.0f, 54.0f };
            Color bordaEsq = mg->redeConectada[i]   ? (Color){ 60, 245, 190, 230 }
                           : (mg->redeSelEsq == i)  ? (Color){ 255, 220, 90, 255 }
                                                    : (Color){ 0, 200, 245, 180 };
            MGDesenharBotao(f, esq, labelsEsq[i], mouse, mg->redeSelEsq == i, bordaEsq);

            Rectangle dir = { 490.0f, 195.0f + i * 74.0f, 175.0f, 54.0f };
            int alvo = mg->redeDireitaMap[i];
            Color bordaDir = mg->redeConectada[alvo] ? (Color){ 60, 245, 190, 230 }
                                                     : (Color){ 0, 200, 245, 180 };
            MGDesenharBotao(f, dir, labelsDir[alvo], mouse, false, bordaDir);
        }
    }

    // Barra de feedback na base do painel
    if (mg->msgFeedback[0] != '\0') {
        Color corMsg = mg->feedbackErro ? (Color){ 255, 110, 110, 255 } : (Color){ 90, 245, 195, 255 };
        Rectangle barMsg = { 90.0f, 496.0f, 620.0f, 32.0f };
        DesenharTextoCentralizadoUI(f->bold, mg->msgFeedback, barMsg, 22.0f, corMsg);
    }
}

#endif
