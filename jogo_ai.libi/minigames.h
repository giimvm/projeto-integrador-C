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
        "src/fonts/Rajdhani-Bold.ttf",
        "jogo_ai.libi/src/fonts/Rajdhani-Bold.ttf"
    };
    const char *caminhosReg[] = {
        "src/fonts/Rajdhani-SemiBold.ttf",
        "jogo_ai.libi/src/fonts/Rajdhani-SemiBold.ttf"
    };

    for (int i = 0; i < 2; i++) {
        if (FileExists(caminhosBold[i])) {
            f.bold = LoadFontEx(caminhosBold[i], 36, codepoints, count);
            SetTextureFilter(f.bold.texture, TEXTURE_FILTER_BILINEAR);
            break;
        }
    }
    for (int i = 0; i < 2; i++) {
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

    // Cena 4: Gaveta 3D Cyberpunk com 8 Pen Drives
    int penSimbolos[8];
    int penCores[8];
    int penAlvo;
    int penHover;
    bool penTestado[8];

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
        int idxCorreto = GetRandomValue(0, 7);
        for (int i = 0; i < 8; i++) {
            if (i == idxCorreto) {
                mg->penSimbolos[i] = mg->penAlvo;
            } else {
                int s = (mg->penAlvo + 1 + GetRandomValue(0, 2)) % 4;
                mg->penSimbolos[i] = s;
            }
            mg->penCores[i] = GetRandomValue(0, 3);
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
    cam.position = (Vector3){ 0.0f, 8.8f, 5.2f };
    cam.target   = (Vector3){ 0.0f, 0.0f, 0.35f };
    cam.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    cam.fovy     = 42.0f;
    cam.projection = CAMERA_PERSPECTIVE;
    return cam;
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
        // Gaveta 3D Cyberpunk: raycasting nos 8 pen drives
        Camera3D cam = MGGavetaCamera();
        Ray ray = GetScreenToWorldRay(mouse, cam);
        mg->penHover = -1;
        float melhorDist = 9999.0f;
        for (int i = 0; i < 8; i++) {
            if (mg->penTestado[i]) continue;
            Vector3 p;
            float rot;
            MGPenDrivePosRot(i, &p, &rot);
            BoundingBox box = {
                (Vector3){ p.x - 0.62f, p.y - 0.20f, p.z - 0.62f },
                (Vector3){ p.x + 0.62f, p.y + 0.35f, p.z + 0.62f }
            };
            RayCollision hit = GetRayCollisionBox(ray, box);
            if (hit.hit && hit.distance < melhorDist) {
                melhorDist = hit.distance;
                mg->penHover = i;
            }
        }
        if (mg->penHover >= 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int i = mg->penHover;
            if (mg->penSimbolos[i] == mg->penAlvo) {
                return 1;
            } else {
                mg->penTestado[i] = true;
                MGRegistrarErro(mg, "Pen drive errado! Resta 1 chance.");
                if (mg->chances <= 0) return -1;
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

        // Renderização 3D da gaveta bagunçada estilo Cyberpunk
        Camera3D cam = MGGavetaCamera();
        BeginScissorMode(86, 228, 628, 266);
        BeginMode3D(cam);

        // Fundo e paredes da gaveta cyberpunk
        DrawCube((Vector3){ 0.0f, -0.35f, 0.0f }, 8.6f, 0.30f, 5.2f, (Color){ 16, 10, 30, 255 });
        for (float gx = -4.0f; gx <= 4.0f; gx += 0.8f) {
            DrawLine3D((Vector3){ gx, -0.19f, -2.5f }, (Vector3){ gx, -0.19f, 2.5f }, (Color){ 110, 25, 130, 120 });
        }
        for (float gz = -2.4f; gz <= 2.4f; gz += 0.8f) {
            DrawLine3D((Vector3){ -4.2f, -0.19f, gz }, (Vector3){ 4.2f, -0.19f, gz }, (Color){ 0, 180, 220, 90 });
        }
        // Bordas da gaveta + puxador frontal neon
        DrawCube((Vector3){ -4.25f, 0.15f,  0.0f }, 0.25f, 0.90f, 5.2f, (Color){ 28, 18, 52, 255 });
        DrawCube((Vector3){  4.25f, 0.15f,  0.0f }, 0.25f, 0.90f, 5.2f, (Color){ 28, 18, 52, 255 });
        DrawCube((Vector3){  0.0f,  0.15f, -2.55f }, 8.6f, 0.90f, 0.25f, (Color){ 28, 18, 52, 255 });
        DrawCube((Vector3){  0.0f,  0.15f,  2.55f }, 8.6f, 0.90f, 0.25f, (Color){ 36, 22, 66, 255 });
        DrawCubeWires((Vector3){ 0.0f, 0.15f, 0.0f }, 8.55f, 0.92f, 5.15f, (Color){ 0, 215, 255, 190 });
        DrawCube((Vector3){ 0.0f, 0.35f, 2.74f }, 1.9f, 0.14f, 0.16f, (Color){ 255, 45, 175, 255 });

        // Bagunça na gaveta (cabos soltos e chips/cartões inclinados)
        DrawCylinderEx((Vector3){ -3.6f, -0.10f, -2.0f }, (Vector3){ -1.4f, -0.05f, -0.2f }, 0.045f, 0.045f, 6, (Color){ 255, 40, 165, 220 });
        DrawCylinderEx((Vector3){ -1.4f, -0.05f, -0.2f }, (Vector3){  0.2f, -0.10f,  1.8f }, 0.045f, 0.045f, 6, (Color){ 255, 40, 165, 220 });
        DrawCylinderEx((Vector3){  3.5f, -0.10f, -1.9f }, (Vector3){  1.8f, -0.04f,  0.1f }, 0.045f, 0.045f, 6, (Color){ 0, 220, 255, 210 });

        rlPushMatrix();
        rlTranslatef(-3.4f, -0.08f, 1.55f);
        rlRotatef(24.0f, 0.0f, 1.0f, 0.0f);
        DrawCube((Vector3){ 0, 0, 0 }, 0.95f, 0.08f, 0.65f, (Color){ 42, 32, 68, 255 });
        DrawCubeWires((Vector3){ 0, 0, 0 }, 0.96f, 0.09f, 0.66f, (Color){ 245, 210, 60, 180 });
        rlPopMatrix();

        rlPushMatrix();
        rlTranslatef(3.45f, -0.08f, -0.15f);
        rlRotatef(-40.0f, 0.0f, 1.0f, 0.0f);
        DrawCube((Vector3){ 0, 0, 0 }, 0.72f, 0.08f, 0.52f, (Color){ 28, 42, 66, 255 });
        DrawCubeWires((Vector3){ 0, 0, 0 }, 0.73f, 0.09f, 0.53f, (Color){ 0, 220, 255, 160 });
        rlPopMatrix();

        // 8 Pen Drives 3D com paleta Cyberpunk
        Color paletaCorpo[4] = {
            { 42, 20, 68, 255 },   // roxo cyberpunk
            { 18, 36, 62, 255 },   // azul escuro
            { 52, 18, 48, 255 },   // vinho neon
            { 28, 32, 46, 255 }    // grafite
        };
        Color paletaNeon[4] = {
            { 255, 55, 185, 255 }, // magenta
            { 0, 230, 255, 255 },  // ciano
            { 245, 215, 55, 255 }, // amarelo ácido
            { 170, 90, 255, 255 }  // violeta
        };

        for (int i = 0; i < 8; i++) {
            Vector3 p;
            float rot;
            MGPenDrivePosRot(i, &p, &rot);
            bool testado = mg->penTestado[i];
            bool hover = (mg->penHover == i) && !testado;

            Color corpo = testado ? (Color){ 48, 20, 26, 220 } : paletaCorpo[mg->penCores[i]];
            Color borda = testado ? (Color){ 220, 60, 75, 200 }
                        : hover   ? RAYWHITE
                                  : paletaNeon[mg->penCores[i]];

            rlPushMatrix();
            rlTranslatef(p.x, p.y + (hover ? 0.10f : 0.0f), p.z);
            rlRotatef(rot, 0.0f, 1.0f, 0.0f);

            // Corpo do pen drive 3D
            DrawCube((Vector3){ 0.0f, 0.0f, 0.0f }, 0.62f, 0.22f, 1.10f, corpo);
            DrawCubeWires((Vector3){ 0.0f, 0.0f, 0.0f }, 0.63f, 0.23f, 1.11f, borda);

            // Conector USB metálico na ponta
            DrawCube((Vector3){ 0.0f, -0.01f, -0.72f }, 0.38f, 0.14f, 0.36f,
                     testado ? (Color){ 100, 95, 105, 220 } : (Color){ 185, 200, 220, 255 });
            DrawCubeWires((Vector3){ 0.0f, -0.01f, -0.72f }, 0.39f, 0.15f, 0.37f, (Color){ 80, 95, 120, 255 });

            // LED neon na traseira
            DrawCube((Vector3){ 0.0f, 0.08f, 0.44f }, 0.22f, 0.08f, 0.10f, borda);
            rlPopMatrix();
        }

        EndMode3D();
        EndScissorMode();

        // Projeção 2D dos símbolos em cima de cada pen drive 3D para leitura nítida
        for (int i = 0; i < 8; i++) {
            Vector3 p;
            float rot;
            MGPenDrivePosRot(i, &p, &rot);
            bool testado = mg->penTestado[i];
            bool hover = (mg->penHover == i) && !testado;
            p.y += hover ? 0.26f : 0.16f;
            Vector2 sc = GetWorldToScreen(p, cam);

            Rectangle tag = { sc.x - 16.0f, sc.y - 16.0f, 32.0f, 32.0f };
            DrawRectangleRounded(tag, 0.25f, 4, testado ? (Color){ 55, 18, 26, 230 } : (Color){ 12, 16, 32, 230 });
            DrawRectangleRoundedLinesEx(tag, 0.25f, 4, 1.5f,
                                        testado ? (Color){ 235, 70, 85, 220 }
                                      : hover   ? RAYWHITE
                                                : (Color){ 0, 225, 255, 210 });
            DesenharSimboloPenDrive(mg->penSimbolos[i], sc.x, sc.y, 9.5f,
                                    testado ? (Color){ 220, 90, 100, 200 } : (Color){ 170, 245, 255, 255 });
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
