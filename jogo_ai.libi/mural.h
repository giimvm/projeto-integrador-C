#ifndef MURAL_H
#define MURAL_H

#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define MURAL_NUM_PESSOAS 3 // 0=Caio, 1=Juliana, 2=Gabriel
#define MURAL_NUM_EV_OFIC 5 // 0..4 -> Cenas 3..7 (provas de logica proposicional)
#define MURAL_NUM_AMB     4 // 5..8 -> Pistas ambiguas extras (A1..A4)
#define MURAL_NUM_TOTAL   9 // 0..4 oficiais + 5..8 ambiguas

typedef struct {
    int abaEsquerda;        // 0=Caso, 1=Pessoas, 2=Provas & Pistas
    int itemEsquerda;       // Selecao dentro da aba (0..2 pessoas ou 0..7 evidencias)

    // Camera 2.5D do Mural
    float zoom;
    Vector2 pan;
    bool arrastandoPan;
    Vector2 panMouseInicio;

    // Posicoes no espaco do mural (0..880 x 0..520)
    Vector2 posPessoa[MURAL_NUM_PESSOAS];
    Vector2 posEv[MURAL_NUM_TOTAL];
    bool evDesbloqueada[MURAL_NUM_TOTAL];
    bool evPosicionada[MURAL_NUM_TOTAL];
    int evLigadaA[MURAL_NUM_TOTAL];       // -1 = sem ligacao, 0=Caio, 1=Juliana, 2=Gabriel
    int evLigadaB[MURAL_NUM_TOTAL];       // Segunda ponta (para evidencias ambiguas de ligacao dupla)
    bool evAutoLigadaA[MURAL_NUM_TOTAL];  // true se a primeira ponta foi ligada automaticamente
    bool evExigeDupla[MURAL_NUM_TOTAL];   // true se exige conectar a outra pessoa (ex: Mensagens de Gabriel)
    bool evProcessada[MURAL_NUM_TOTAL];

    // Interacao de arrastar e conectar
    int arrastandoTipo;     // 0=nenhum, 1=pessoa, 2=evidencia no mural, 3=evidencia do inventario
    int arrastandoIdx;
    Vector2 dragOffset;
    Vector2 dragInicioMouse;
    bool clicouPessoaSemArrastar;
    int pessoaSelecionada;  // -1 = nenhuma, 0..2 = aguardando clique em uma evidencia para ligar

    // Ferramenta limitada de "IA" Falsa (2 usos por run)
    int iaUsosRestantes;
    char iaAviso[96];
    float iaAvisoTimer;

    // Audio player rapido na aba Provas
    bool audioTocando;
    float audioTempo;

    // Caches para os mugshots no mural
    CacheTextura cachePessoas[MURAL_NUM_PESSOAS];
    CacheTextura cacheEvImg;
    float hoverAbas[6];     // 3 abas + Fechar + IA + Auto-Organizar
    bool muralAbertoUmaVez;
} EstadoMural;

static inline void DescarregarMuralCaches(EstadoMural *m) {
    for (int i = 0; i < MURAL_NUM_PESSOAS; i++) {
        if (m->cachePessoas[i].tex.id != 0) {
            UnloadTexture(m->cachePessoas[i].tex);
            m->cachePessoas[i].tex = (Texture2D){0};
        }
        m->cachePessoas[i].caminho[0] = '\0';
    }
    if (m->cacheEvImg.tex.id != 0) {
        UnloadTexture(m->cacheEvImg.tex);
        m->cacheEvImg.tex = (Texture2D){0};
    }
    m->cacheEvImg.caminho[0] = '\0';
}

static inline void IniciarMural(EstadoMural *m) {
    DescarregarMuralCaches(m);
    memset(m, 0, sizeof(*m));
    m->zoom = 1.0f;
    m->pan = (Vector2){ 0.0f, 0.0f };
    m->arrastandoTipo = 0;
    m->arrastandoIdx = -1;
    m->pessoaSelecionada = -1;
    m->iaUsosRestantes = 2;

    // Fotos em formato de COLUNA a esquerda do Mural (sem sobrepor evidencias)
    m->posPessoa[0] = (Vector2){ 120.0f, 95.0f };   // Caio
    m->posPessoa[1] = (Vector2){ 120.0f, 258.0f };  // Juliana
    m->posPessoa[2] = (Vector2){ 120.0f, 421.0f };  // Gabriel

    // Posicoes padrao
    m->posEv[0] = (Vector2){ 425.0f, 95.0f };   // P1 Audio
    m->posEv[1] = (Vector2){ 425.0f, 258.0f };  // P2 Foto
    m->posEv[2] = (Vector2){ 425.0f, 421.0f };  // P3 GPS
    m->posEv[3] = (Vector2){ 695.0f, 115.0f };  // P4 Nuvem
    m->posEv[4] = (Vector2){ 695.0f, 235.0f };  // P5 Mobi / Video
    m->posEv[5] = (Vector2){ 695.0f, 395.0f };  // A1 Msgs apagadas
    m->posEv[6] = (Vector2){ 425.0f, 176.0f };  // A2 Chamada 02:47
    m->posEv[7] = (Vector2){ 695.0f, 315.0f };  // A3 Fibra e barro
    m->posEv[8] = (Vector2){ 695.0f, 465.0f };  // A4 Arquivo de empresa (Projeto de IA)

    for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
        m->evLigadaA[i] = -1;
        m->evLigadaB[i] = -1;
    }
    m->evExigeDupla[5] = true; // Mensagens de Gabriel: ligada a Gabriel, falta conectar outra pessoa
}

// Sincroniza provas de logica (mesmo se erradas -> aparecem como Nao Validadas) e evidencias ambiguas
static inline void SincronizarMuralComProvas(EstadoMural *m, int culpado, int cenaAtual, const int statusProva[8]) {
    bool ehJu = (culpado == 0);

    // 1. Provas de Logica Proposicional (0..4 -> Cenas 3..7)
    for (int i = 0; i < MURAL_NUM_EV_OFIC; i++) {
        int cena = i + 3;
        if ((statusProva[cena] == 1 || statusProva[cena] == -1) && !m->evProcessada[i]) {
            m->evProcessada[i] = true;
            m->evDesbloqueada[i] = true;
            if (i == 1) {
                // Prova 2: Foto de Juliana no carro -> ligada a Juliana (1)
                m->evPosicionada[i] = true;
                m->evLigadaA[i] = 1;
                m->evAutoLigadaA[i] = true;
                m->posEv[i] = (Vector2){ 425.0f, 258.0f };
            } else if (i == 2) {
                // Prova 3: Celular de Gabriel -> ligado a Gabriel (2)
                m->evPosicionada[i] = true;
                m->evLigadaA[i] = 2;
                m->evAutoLigadaA[i] = true;
                m->posEv[i] = (Vector2){ 425.0f, 421.0f };
            } else if (i == 4) {
                // Prova 5: Corrida Mobi (Juliana=1) ou Camera Estacionamento (Caio=0)
                int alvo = ehJu ? 1 : 0;
                m->evPosicionada[i] = true;
                m->evLigadaA[i] = alvo;
                m->evAutoLigadaA[i] = true;
                m->posEv[i] = (Vector2){ 695.0f, ehJu ? 235.0f : 95.0f };
            }
        }
    }

    // 2. Evidencias Ambiguas (automaticas por cena)
    // Index 5 (A1): Mensagens apagadas de Gabriel (Cena >= 3) -> ligada a Gabriel (2), falta conectar outra pessoa
    if (cenaAtual >= 3 && !m->evProcessada[5]) {
        m->evProcessada[5] = true;
        m->evDesbloqueada[5] = true;
        m->evPosicionada[5] = true;
        m->evLigadaA[5] = 2;
        m->evAutoLigadaA[5] = true;
        m->posEv[5] = (Vector2){ 695.0f, 395.0f };
    }
    // Index 8 (A4): Arquivo de empresa - Projeto de IA (Cena >= 4) -> vai para o inventario
    if (cenaAtual >= 4 && !m->evProcessada[8]) {
        m->evProcessada[8] = true;
        m->evDesbloqueada[8] = true;
    }
    // Index 6 (A2): Chamada curta 02:47 (Cena >= 5) -> vai para o inventario
    if (cenaAtual >= 5 && !m->evProcessada[6]) {
        m->evProcessada[6] = true;
        m->evDesbloqueada[6] = true;
    }
    // Index 7 (A3): Barro e fibra de tecido no barranco (Cena >= 6) -> vai para o inventario
    if (cenaAtual >= 6 && !m->evProcessada[7]) {
        m->evProcessada[7] = true;
        m->evDesbloqueada[7] = true;
    }
}

static inline Vector2 MuralParaTela(Vector2 world, Rectangle area, float zoom, Vector2 pan) {
    float cx = area.x + area.width * 0.5f;
    float cy = area.y + area.height * 0.5f;
    float wx = world.x - area.width * 0.5f + pan.x;
    float wy = world.y - area.height * 0.5f + pan.y;
    return (Vector2){ cx + wx * zoom, cy + wy * zoom };
}

static inline Vector2 TelaParaMural(Vector2 screen, Rectangle area, float zoom, Vector2 pan) {
    float cx = area.x + area.width * 0.5f;
    float cy = area.y + area.height * 0.5f;
    float wx = (screen.x - cx) / zoom - pan.x + area.width * 0.5f;
    float wy = (screen.y - cy) / zoom - pan.y + area.height * 0.5f;
    return (Vector2){ wx, wy };
}

static inline const char *NomeCurtoEvidenciaMural(int idx, int culpado) {
    bool ehJu = (culpado == 0);
    switch (idx) {
        case 0: return "P1: Áudio Anônimo";
        case 1: return "P2: Foto no Carro";
        case 2: return "P3: GPS Celular";
        case 3: return "P4: Backup Nuvem";
        case 4: return ehJu ? "P5: Corrida Mobi" : "P5: Câmera Saída";
        case 5: return "A1: Msgs Apagadas";
        case 6: return "A2: Chamada 02:47";
        case 7: return "A3: Fibra e Barro";
        case 8: return "A4: Projeto IA";
    }
    return "Evidência";
}

static inline const char *SlotCurtoMural(int idx, int culpado) {
    bool ehJu = (culpado == 0);
    switch (idx) {
        case 0: return "P1: Áudio";
        case 1: return "P2: Foto";
        case 2: return "P3: GPS";
        case 3: return "P4: Nuvem";
        case 4: return ehJu ? "P5: Mobi" : "P5: Vídeo";
        case 5: return "A1: Msgs";
        case 6: return "A2: 02:47";
        case 7: return "A3: Fibra";
        case 8: return "A4: Proj IA";
    }
    return "Pista";
}

static inline const char *OndeAchadaEvidenciaMural(int idx, int culpado) {
    bool ehJu = (culpado == 0);
    switch (idx) {
        case 0: return "Entrega anônima na delegacia (Cena 3)";
        case 1: return "Enviada por Juliana à mãe / interrogatório (Cena 4)";
        case 2: return "Torres de celular — barranco a 200m do galpão (Cena 5)";
        case 3: return "Backup automático na nuvem de Gabriel (Cena 6)";
        case 4: return ehJu ? "Registro oficial da Mobi — estrada de serviço (Cena 7)"
                        : "Câmera da cancela do estacionamento da festa (Cena 7)";
        case 5: return "Logs temporários recuperados do aparelho de Gabriel (02:15)";
        case 6: return "Antena da região do galpão — chamada de 14s às 02:47";
        case 7: return "Perícia de campo na borda do barranco (Cena 5)";
        case 8: return "Servidor acadêmico / NEXARA — autoria corrompida (Cena 4)";
    }
    return "";
}

static inline const char *ConteudoAmbiguoMural(int idx, int culpado) {
    bool ehJu = (culpado == 0);
    if (idx == 5) {
        return ehJu
            ? "\"Preciso falar com você fora da pista. Me encontra perto dos fundos antes das 3h. É sobre o projeto.\"\n[FALTA CONECTAR AO DESTINATÁRIO]"
            : "\"Não vou mais segurar essa história sozinho. Me encontra atrás do galpão antes das 3h, precisamos resolver hoje.\"\n[FALTA CONECTAR AO DESTINATÁRIO]";
    }
    if (idx == 6) {
        return "Ligação de 14 segundos às 02:47 para número bloqueado na torre do galpão. Nenhum suspeito assumiu a chamada.";
    }
    if (idx == 7) {
        return "Fibras sintéticas escuras e barro úmido na borda do barranco. Compatível com jaqueta universitária padrão.";
    }
    if (idx == 8) {
        return "Arquivo de empresa: rascunho de arquitetura de IA ligado à NEXARA, mas os metadados de autoria foram apagados. Não está claro se foi Juliana ou Caio quem escreveu. De quem é o projeto?";
    }
    return "";
}

static inline int AtualizarMural(EstadoMural *m, int culpado, int cenaAtual, const int statusProva[8], Vector2 mouse, float dt, Sound sfxClick, bool temSfx, Sound sfxLinkOk, bool temSfxLinkOk) {
    if (m->audioTocando) m->audioTempo += dt;
    if (m->iaAvisoTimer > 0.0f) m->iaAvisoTimer -= dt;

    Rectangle painelEsq = { 24, 66, 340, 630 };
    Rectangle areaMural = { 378, 66, 878, 516 };
    Rectangle areaInv   = { 378, 592, 878, 104 };

    Rectangle btnOrg    = { 620, 16, 200, 38 };
    Rectangle btnIA     = { 835, 16, 245, 38 };
    Rectangle btnFechar = { 1095, 16, 161, 38 };
    Rectangle abas[3] = {
        { painelEsq.x + 12, painelEsq.y + 12, 100, 34 },
        { painelEsq.x + 120, painelEsq.y + 12, 100, 34 },
        { painelEsq.x + 228, painelEsq.y + 12, 100, 34 }
    };
    Rectangle btnsHover[6] = { abas[0], abas[1], abas[2], btnFechar, btnIA, btnOrg };
    AtualizarHover(btnsHover, m->hoverAbas, 6, mouse, dt);

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_D)) {
        m->audioTocando = false;
        return 1;
    }

    Rectangle btnZoomMais  = { areaMural.x + areaMural.width - 126, areaMural.y + 10, 34, 32 };
    Rectangle btnZoomMenos = { areaMural.x + areaMural.width - 88,  areaMural.y + 10, 34, 32 };
    Rectangle btnZoomReset = { areaMural.x + areaMural.width - 48,  areaMural.y + 10, 38, 32 };

    if (CheckCollisionPointRec(mouse, areaMural)) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0.0f) {
            m->zoom = fmaxf(0.60f, fminf(1.75f, m->zoom + wheel * 0.12f));
        }
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if (CheckCollisionPointRec(mouse, btnOrg)) {
            if (temSfx) PlaySound(sfxClick);
            // Reposiciona as 3 fotos em colunas no topo (Gabriel no centro, pois liga aos dois)
            m->posPessoa[0] = (Vector2){ 115.0f, 92.0f };  // Caio (Esquerda)
            m->posPessoa[2] = (Vector2){ 440.0f, 92.0f };  // Gabriel (Centro)
            m->posPessoa[1] = (Vector2){ 765.0f, 92.0f };  // Juliana (Direita)
            int cntP[3] = { 0, 0, 0 }; // 0=Caio, 1=Juliana, 2=Gabriel
            int cntCG = 0, cntGJ = 0, cntCJ = 0, cntSolto = 0;
            for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
                if (!m->evPosicionada[i]) continue;
                int a = m->evLigadaA[i];
                int b = m->evLigadaB[i];
                if (a >= 0 && b >= 0) {
                    if ((a == 0 && b == 2) || (a == 2 && b == 0)) {
                        m->posEv[i] = (Vector2){ 277.0f, 198.0f + cntCG * 88.0f };
                        cntCG++;
                    } else if ((a == 2 && b == 1) || (a == 1 && b == 2)) {
                        m->posEv[i] = (Vector2){ 602.0f, 198.0f + cntGJ * 88.0f };
                        cntGJ++;
                    } else {
                        m->posEv[i] = (Vector2){ 440.0f, 462.0f - cntCJ * 82.0f };
                        cntCJ++;
                    }
                } else if (a == 0) {
                    m->posEv[i] = (Vector2){ 115.0f, 222.0f + cntP[0] * 84.0f };
                    cntP[0]++;
                } else if (a == 1) {
                    m->posEv[i] = (Vector2){ 765.0f, 222.0f + cntP[1] * 84.0f };
                    cntP[1]++;
                } else if (a == 2) {
                    m->posEv[i] = (Vector2){ 440.0f, 222.0f + cntP[2] * 84.0f };
                    cntP[2]++;
                } else {
                    m->posEv[i] = (Vector2){ 277.0f + (cntSolto % 2) * 325.0f, 360.0f + (cntSolto / 2) * 84.0f };
                    cntSolto++;
                }
            }
            m->zoom = 1.0f;
            m->pan = (Vector2){ 0.0f, 0.0f };
            snprintf(m->iaAviso, sizeof(m->iaAviso), "Mural organizado automaticamente!");
            m->iaAvisoTimer = 2.0f;
            return 0;
        }
        if (CheckCollisionPointRec(mouse, btnIA) && m->iaUsosRestantes > 0) {
            int totalDesb = 0;
            for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
                if (m->evDesbloqueada[i]) totalDesb++;
            }
            if (totalDesb == 0) {
                snprintf(m->iaAviso, sizeof(m->iaAviso), "Eu não tenho artefatos suficientes para ajudar!");
                m->iaAvisoTimer = 3.0f;
                return 0;
            }
            if (cenaAtual < 5) {
                snprintf(m->iaAviso, sizeof(m->iaAviso), "Não tenho contexto suficiente do caso para ajudar!");
                m->iaAvisoTimer = 3.5f;
                return 0;
            }
            int candidatos[MURAL_NUM_AMB];
            int nCand = 0;
            // IA trabalha apenas sobre evidencias extras (A1..A4) ja desbloqueadas
            for (int i = MURAL_NUM_EV_OFIC; i < MURAL_NUM_TOTAL; i++) {
                if (!m->evDesbloqueada[i]) continue;
                if (i == 5) {
                    if (!m->evPosicionada[i] || m->evLigadaB[i] < 0) candidatos[nCand++] = i;
                } else {
                    if (!m->evPosicionada[i] || m->evLigadaA[i] < 0) candidatos[nCand++] = i;
                }
            }
            if (nCand > 0) {
                if (temSfx) PlaySound(sfxClick);
                int esc = candidatos[GetRandomValue(0, nCand - 1)];
                // Randomiza entre Caio (0) e Juliana (1) para nao entregar o culpado
                int alvoHip = GetRandomValue(0, 1);
                m->evPosicionada[esc] = true;
                if (esc == 5) {
                    m->evLigadaA[esc] = 2; // Gabriel
                    m->evLigadaB[esc] = alvoHip;
                } else {
                    m->evLigadaA[esc] = alvoHip;
                }
                m->iaUsosRestantes--;
                snprintf(m->iaAviso, sizeof(m->iaAviso), "IA: Hipótese aplicada em uma evidência extra!");
                m->iaAvisoTimer = 3.0f;
            } else {
                snprintf(m->iaAviso, sizeof(m->iaAviso), "Não tenho contexto suficiente do caso para ajudar!");
                m->iaAvisoTimer = 3.5f;
            }
            return 0;
        }
        if (CheckCollisionPointRec(mouse, btnFechar)) {
            if (temSfx) PlaySound(sfxClick);
            m->audioTocando = false;
            return 1;
        }
        for (int i = 0; i < 3; i++) {
            if (CheckCollisionPointRec(mouse, abas[i])) {
                if (temSfx) PlaySound(sfxClick);
                m->abaEsquerda = i;
                m->itemEsquerda = 0;
                m->audioTocando = false;
                return 0;
            }
        }
        if (CheckCollisionPointRec(mouse, btnZoomMais)) {
            if (temSfx) PlaySound(sfxClick);
            m->zoom = fminf(1.75f, m->zoom + 0.15f);
            return 0;
        }
        if (CheckCollisionPointRec(mouse, btnZoomMenos)) {
            if (temSfx) PlaySound(sfxClick);
            m->zoom = fmaxf(0.60f, m->zoom - 0.15f);
            return 0;
        }
        if (CheckCollisionPointRec(mouse, btnZoomReset)) {
            if (temSfx) PlaySound(sfxClick);
            m->zoom = 1.0f;
            m->pan = (Vector2){ 0.0f, 0.0f };
            return 0;
        }

        if (CheckCollisionPointRec(mouse, painelEsq)) {
            int maxI = (m->abaEsquerda == 1) ? 3 : (m->abaEsquerda == 2) ? MURAL_NUM_TOTAL : 0;
            float rowH = (m->abaEsquerda == 2) ? 28.0f : 42.0f;
            for (int i = 0; i < maxI; i++) {
                Rectangle rItem = { painelEsq.x + 14, painelEsq.y + 60 + i * rowH, painelEsq.width - 28, rowH - 3 };
                if (CheckCollisionPointRec(mouse, rItem)) {
                    if (temSfx) PlaySound(sfxClick);
                    m->itemEsquerda = i;
                    m->audioTocando = false;
                }
            }
            if (m->abaEsquerda == 2 && m->itemEsquerda == 0 && statusProva[3] == 1) {
                Rectangle btnPlay = { painelEsq.x + 18, painelEsq.y + 620 - 46, 130, 34 };
                if (CheckCollisionPointRec(mouse, btnPlay)) {
                    if (temSfx) PlaySound(sfxClick);
                    m->audioTocando = !m->audioTocando;
                }
            }
            return 0;
        }

        if (CheckCollisionPointRec(mouse, areaInv)) {
            for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
                if (!m->evDesbloqueada[i] || m->evPosicionada[i]) continue;
                Rectangle slot = { areaInv.x + 10 + i * 96, areaInv.y + 32, 91, 60 };
                if (CheckCollisionPointRec(mouse, slot)) {
                    if (temSfx) PlaySound(sfxClick);
                    m->arrastandoTipo = 3;
                    m->arrastandoIdx = i;
                    m->pessoaSelecionada = -1;
                    return 0;
                }
            }
        }

        if (CheckCollisionPointRec(mouse, areaMural)) {
            Vector2 mw = TelaParaMural(mouse, areaMural, m->zoom, m->pan);
            bool acertouAlgo = false;

            for (int i = MURAL_NUM_TOTAL - 1; i >= 0; i--) {
                if (!m->evPosicionada[i]) continue;
                Rectangle rEv = { m->posEv[i].x - 82, m->posEv[i].y - 36, 164, 72 };
                if (CheckCollisionPointRec(mw, rEv)) {
                    acertouAlgo = true;
                    if (m->pessoaSelecionada >= 0) {
                        if (m->evExigeDupla[i] && m->evLigadaA[i] >= 0) {
                            if (m->pessoaSelecionada != m->evLigadaA[i]) {
                                m->evLigadaB[i] = m->pessoaSelecionada;
                            }
                        } else {
                            m->evLigadaA[i] = m->pessoaSelecionada;
                            m->evAutoLigadaA[i] = false;
                        }
                        int alvoCulp = (culpado == 0) ? 1 : 0; // 0=Caio, 1=Juliana
                        bool ligouCerto = false;
                        if (i < MURAL_NUM_EV_OFIC) {
                            int alvoCerto = (i == 1) ? 1 : (i == 2) ? 2 : alvoCulp;
                            ligouCerto = (m->evLigadaA[i] == alvoCerto);
                        } else if (i == 5) {
                            ligouCerto = (m->evLigadaB[5] == alvoCulp);
                        } else if (i == 8) {
                            ligouCerto = (m->evLigadaA[8] == 1); // Projeto sempre da Juliana
                        } else {
                            ligouCerto = (m->evLigadaA[i] == alvoCulp);
                        }
                        if (ligouCerto && temSfxLinkOk) PlaySound(sfxLinkOk);
                        else if (temSfx) PlaySound(sfxClick);
                        m->pessoaSelecionada = -1;
                    } else {
                        if (temSfx) PlaySound(sfxClick);
                        m->arrastandoTipo = 2;
                        m->arrastandoIdx = i;
                        m->dragOffset = (Vector2){ mw.x - m->posEv[i].x, mw.y - m->posEv[i].y };
                    }
                    m->abaEsquerda = 2;
                    m->itemEsquerda = i;
                    break;
                }
            }

            if (!acertouAlgo) {
                for (int p = MURAL_NUM_PESSOAS - 1; p >= 0; p--) {
                    Rectangle rPes = { m->posPessoa[p].x - 58, m->posPessoa[p].y - 68, 116, 136 };
                    if (CheckCollisionPointRec(mw, rPes)) {
                        acertouAlgo = true;
                        if (temSfx) PlaySound(sfxClick);
                        m->arrastandoTipo = 1;
                        m->arrastandoIdx = p;
                        m->dragInicioMouse = mouse;
                        m->clicouPessoaSemArrastar = true;
                        m->dragOffset = (Vector2){ mw.x - m->posPessoa[p].x, mw.y - m->posPessoa[p].y };
                        m->abaEsquerda = 1;
                        m->itemEsquerda = p;
                        break;
                    }
                }
            }

            if (!acertouAlgo) {
                m->pessoaSelecionada = -1;
                m->arrastandoPan = true;
                m->panMouseInicio = mouse;
            }
        }
    }

    // Clique direito desfaz ligacao (primeiro B, depois A, ou devolve ao inventario)
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) && CheckCollisionPointRec(mouse, areaMural)) {
        Vector2 mw = TelaParaMural(mouse, areaMural, m->zoom, m->pan);
        for (int i = MURAL_NUM_TOTAL - 1; i >= 0; i--) {
            if (!m->evPosicionada[i]) continue;
            Rectangle rEv = { m->posEv[i].x - 82, m->posEv[i].y - 36, 164, 72 };
            if (CheckCollisionPointRec(mw, rEv)) {
                if (temSfx) PlaySound(sfxClick);
                if (m->evLigadaB[i] >= 0) {
                    m->evLigadaB[i] = -1;
                } else if (m->evLigadaA[i] >= 0 && !m->evAutoLigadaA[i]) {
                    m->evLigadaA[i] = -1;
                } else if (m->evLigadaA[i] < 0) {
                    m->evPosicionada[i] = false;
                }
                break;
            }
        }
        m->pessoaSelecionada = -1;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (m->arrastandoPan) {
            Vector2 delta = { (mouse.x - m->panMouseInicio.x) / m->zoom, (mouse.y - m->panMouseInicio.y) / m->zoom };
            m->pan.x += delta.x;
            m->pan.y += delta.y;
            m->panMouseInicio = mouse;
        } else if (m->arrastandoTipo == 1 && m->arrastandoIdx >= 0) {
            float dx = mouse.x - m->dragInicioMouse.x;
            float dy = mouse.y - m->dragInicioMouse.y;
            if (dx * dx + dy * dy > 36.0f) {
                m->clicouPessoaSemArrastar = false;
            }
            Vector2 mw = TelaParaMural(mouse, areaMural, m->zoom, m->pan);
            m->posPessoa[m->arrastandoIdx] = (Vector2){ mw.x - m->dragOffset.x, mw.y - m->dragOffset.y };
        } else if (m->arrastandoTipo == 2 && m->arrastandoIdx >= 0) {
            Vector2 mw = TelaParaMural(mouse, areaMural, m->zoom, m->pan);
            m->posEv[m->arrastandoIdx] = (Vector2){ mw.x - m->dragOffset.x, mw.y - m->dragOffset.y };
        }
    }

    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
        if (m->arrastandoTipo == 1 && m->arrastandoIdx >= 0 && m->clicouPessoaSemArrastar) {
            int p = m->arrastandoIdx;
            m->pessoaSelecionada = (m->pessoaSelecionada == p) ? -1 : p;
        } else if (m->arrastandoTipo == 3 && m->arrastandoIdx >= 0 && CheckCollisionPointRec(mouse, areaMural)) {
            Vector2 mw = TelaParaMural(mouse, areaMural, m->zoom, m->pan);
            m->evPosicionada[m->arrastandoIdx] = true;
            m->posEv[m->arrastandoIdx] = mw;
            if (temSfx) PlaySound(sfxClick);
        }
        m->arrastandoTipo = 0;
        m->arrastandoIdx = -1;
        m->clicouPessoaSemArrastar = false;
        m->arrastandoPan = false;
    }

    return 0;
}

static inline void DesenharLinhaTecnologicaMural(Vector2 p1, Vector2 p2, Color cor, double tempo, bool autoLigada) {
    DrawLineEx((Vector2){ p1.x + 3, p1.y + 4 }, (Vector2){ p2.x + 3, p2.y + 4 }, 4.0f, (Color){ 0, 0, 0, 120 });
    DrawLineEx(p1, p2, 6.0f, ColorAlpha(cor, 0.22f));
    DrawLineEx(p1, p2, 2.2f, ColorAlpha(cor, 0.90f));

    float t = fmodf((float)tempo * (autoLigada ? 0.65f : 0.95f), 1.0f);
    Vector2 pulso = { p1.x + (p2.x - p1.x) * t, p1.y + (p2.y - p1.y) * t };
    DrawCircleV(pulso, 5.5f, ColorAlpha(cor, 0.35f));
    DrawCircleV(pulso, 3.0f, WHITE);
    DrawCircleV(p1, 5.0f, cor);
    DrawCircleV(p2, 5.0f, cor);
}

static inline void DesenharMural(EstadoMural *m, GerenciadorAssets *assets, int culpado, const int statusProva[8], const int respostaProva[8],
                                  bool notaCaio, bool notaJu, bool notaGab, bool notaNuvem, bool notaConfronto,
                                  int provasValidadas, Vector2 mouse, double tempo, float camZoom) {
    DesenharFundoCyberGrid(tempo, COR_CIANO_NEON);

    DesenharBadgeCyber(assets->fontes.titulo, (Vector2){ 24, 16 }, "MURAL TÁTICO.IA", 23, COR_CIANO_NEON);
    DesenharBadgeCyber(assets->fontes.bold, (Vector2){ 310, 18 },
        TextFormat("PROVAS: %d/5 VALIDADAS", provasValidadas), 18,
        (provasValidadas >= 4) ? COR_VERDE_OK : COR_AMARELO_AVISO);
    Rectangle btnOrg = { 620, 16, 200, 38 };
    DesenharBotaoCyber(assets->fontes.bold, btnOrg, "AUTO-ORGANIZAR", 17, m->hoverAbas[5], tempo, COR_CIANO_NEON);
    Rectangle btnIA = { 835, 16, 245, 38 };
    DesenharBotaoCyber(assets->fontes.bold, btnIA,
        TextFormat("ASSISTENTE IA (%d/2)", m->iaUsosRestantes), 18,
        (m->iaUsosRestantes > 0) ? m->hoverAbas[4] : 0.0f, tempo,
        (m->iaUsosRestantes > 0) ? COR_ROXO_NEON : GRAY);
    Rectangle btnFechar = { 1095, 16, 161, 38 };
    DesenharBotaoCyber(assets->fontes.bold, btnFechar, "FECHAR [D]", 20, m->hoverAbas[3], tempo, COR_ROSA_NEON);

    // =========================================================================
    // 1. PAINEL ESQUERDO
    // =========================================================================
    Rectangle painelEsq = { 24, 66, 340, 630 };
    DesenharPainelCyber(painelEsq, COR_ROXO_NEON);

    const char *titAbas[3] = { "CASO", "PESSOAS", "PROVAS" };
    for (int i = 0; i < 3; i++) {
        Rectangle ab = { painelEsq.x + 12 + i * 108, painelEsq.y + 12, 100, 34 };
        bool ativa = (m->abaEsquerda == i);
        DesenharBotaoCyber(assets->fontes.bold, ab, titAbas[i], 18, ativa ? 0.9f : m->hoverAbas[i], tempo, ativa ? COR_ROSA_NEON : COR_CIANO_NEON);
    }
    DrawRectangle((int)painelEsq.x + 12, (int)painelEsq.y + 52, (int)painelEsq.width - 24, 2, ColorAlpha(COR_ROXO_NEON, 0.45f));

    if (m->abaEsquerda == 0) {
        const char *infoCaso = notaGab
            ? "OCORRÊNCIA: Homicídio de Gabriel Moura (22 anos).\n"
              "LOCAL: Festa universitária / barranco a 200m do galpão (Recife, 2045).\n"
              "JANELA DA MORTE: 02:50 - 03:05 (golpe na têmpora).\n\n"
              "COMO USAR O MURAL:\n"
              "- Arraste evidências do inventário inferior para o mural.\n"
              "- Clique em uma FOTO e depois em uma EVIDÊNCIA para ligá-las.\n"
              "- Evidências ambíguas ligadas a Gabriel precisam que você conecte a outra ponta!\n"
              "- Clique direito desfaz a ligação."
            : "OCORRÊNCIA: Desaparecimento de Gabriel Moura (22 anos).\n"
              "LOCAL: Festa universitária / Campus Recife (2045).\n"
              "STATUS: Buscas em andamento. Última vez visto por volta das 03:00.\n\n"
              "COMO USAR O MURAL:\n"
              "- Arraste evidências do inventário inferior para o mural.\n"
              "- Clique em uma FOTO e depois em uma EVIDÊNCIA para ligá-las.\n"
              "- Evidências ambíguas ligadas a Gabriel precisam que você conecte a outra ponta!\n"
              "- Clique direito desfaz a ligação.";
        DesenharTextoQuebrado(assets->fontes.regular, infoCaso,
            (Rectangle){ painelEsq.x + 16, painelEsq.y + 66, painelEsq.width - 32, 540 }, 19, 1.0f, WHITE);
    } else if (m->abaEsquerda == 1) {
        const char *nomesP[3] = { "Caio Vasconcelos (Suspeito)", "Juliana Costa (Suspeita)", "Gabriel Moura (Vítima)" };
        for (int i = 0; i < 3; i++) {
            Rectangle r = { painelEsq.x + 14, painelEsq.y + 62 + i * 42, painelEsq.width - 28, 36 };
            bool sel = (m->itemEsquerda == i);
            DrawRectangleRec(r, sel ? ColorAlpha(COR_ROSA_NEON, 0.22f) : (Color){ 24, 14, 44, 200 });
            DrawRectangleLinesEx(r, 1.5f, sel ? COR_ROSA_NEON : ColorAlpha(COR_ROXO_NEON, 0.5f));
            DrawTextEx(assets->fontes.bold, nomesP[i], (Vector2){ r.x + 10, r.y + 8 }, 19, 1.0f, sel ? COR_ROSA_NEON : WHITE);
        }
        const char *fotosMug[3] = {
            "AssetsRay/Personagens/Caio/CaioMugshot.png",
            "AssetsRay/Personagens/Julia/juliana retrato.png",
            "AssetsRay/Personagens/Gabriel/GabrielMugshot.png"
        };
        Color coresMug[3] = { COR_ROXO_NEON, COR_ROSA_NEON, COR_CIANO_NEON };
        Rectangle rMug = { painelEsq.x + 16, painelEsq.y + 196, 98, 118 };
        DrawRectangleRounded(rMug, 0.06f, 4, (Color){ 16, 10, 28, 230 });
        DesenharTexturaContain(ObterTexturaCache(&m->cachePessoas[m->itemEsquerda], fotosMug[m->itemEsquerda]), rMug, WHITE);
        DrawRectangleRoundedLinesEx(rMug, 0.06f, 4, 1.5f, coresMug[m->itemEsquerda]);

        const char *cabecalhoP[3] = {
            "CAIO VASCONCELOS\n22 anos\nCiência da Comp.\nPai influente na NEXARA",
            "JULIANA COSTA\n21 anos\nCiência da Comp.\nDisputava vaga na NEXARA",
            "GABRIEL MOURA\n22 anos (Vítima)\nCiência da Comp.\nVisto na festa (~03:00)"
        };
        DesenharTextoQuebrado(assets->fontes.regular, cabecalhoP[m->itemEsquerda],
            (Rectangle){ painelEsq.x + 124, painelEsq.y + 198, painelEsq.width - 140, 118 }, 18, 1.0f, WHITE);

        char ficha[512] = "";
        if (m->itemEsquerda == 0) {
            snprintf(ficha, sizeof(ficha),
                "NOTAS DO CASO:\n%s%s",
                notaCaio ? "- Discutiu com Gabriel na festa. Diz ter saído às 02:40 (relógio sem bateria).\n" : "- Ainda não interrogado.\n",
                (notaConfronto && culpado == 1) ? "- Confrontado sobre vídeo adulterado e repasses; chamou advogado." : "");
        } else if (m->itemEsquerda == 1) {
            snprintf(ficha, sizeof(ficha),
                "NOTAS DO CASO:\n%s%s",
                notaJu ? "- Brigou com Gabriel pela vaga na NEXARA. Diz ter saído às 03:00 de app.\n" : "- Ainda não interrogada.\n",
                (notaConfronto && culpado == 0) ? "- Confrontada sobre corrida Mobi às 03:41; pediu advogado." : "");
        } else {
            snprintf(ficha, sizeof(ficha),
                "REGISTROS DA VÍTIMA:\n%s%s",
                notaGab ? "- Corpo encontrado no barranco (golpe na têmpora entre 02:50 e 03:05).\n" : "- Buscas em andamento.\n",
                notaNuvem ? "- Backup na nuvem recuperado pela perícia." : "");
        }
        DesenharTextoQuebrado(assets->fontes.regular, ficha, (Rectangle){ painelEsq.x + 16, painelEsq.y + 324, painelEsq.width - 32, 285 }, 19, 1.0f, LIGHTGRAY);
    } else {
        for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
            bool ehAmb = (i >= MURAL_NUM_EV_OFIC);
            bool desb = m->evDesbloqueada[i];
            int st = ehAmb ? (desb ? 1 : 0) : statusProva[i + 3];
            Rectangle r = { painelEsq.x + 14, painelEsq.y + 60 + i * 28.0f, painelEsq.width - 28, 25 };
            bool sel = (m->itemEsquerda == i);
            Color cSt = (st == 1) ? (ehAmb ? COR_AMARELO_AVISO : COR_VERDE_OK) : (st == -1) ? COR_VERMELHO_ERR : GRAY;
            DrawRectangleRec(r, sel ? ColorAlpha(COR_CIANO_NEON, 0.22f) : (Color){ 24, 14, 44, 200 });
            DrawRectangleLinesEx(r, 1.2f, sel ? COR_CIANO_NEON : ColorAlpha(cSt, 0.65f));
            const char *nomeLista = (!desb && st == 0) ? "??????" : NomeCurtoEvidenciaMural(i, culpado);
            DrawTextEx(assets->fontes.bold, nomeLista, (Vector2){ r.x + 10, r.y + 4 }, 16, 1.0f, sel ? COR_CIANO_NEON : (st == -1 ? COR_VERMELHO_ERR : (desb ? WHITE : GRAY)));
        }

        int idx = m->itemEsquerda;
        char det[640] = "";
        if (idx < MURAL_NUM_EV_OFIC) {
            int cena = idx + 3;
            if (statusProva[cena] == 0) {
                snprintf(det, sizeof(det), "PROVA DE LÓGICA (??????)\nSTATUS: BLOQUEADA\n\nContinue investigando para descobrir e desbloquear esta evidência.");
            } else if (statusProva[cena] == -1) {
                snprintf(det, sizeof(det), "PROVA DE LÓGICA (NÃO VALIDADA)\nVocê errou o desafio lógico. A pista aparece no Mural em vermelho e pode ser conectada, mas não valida a lógica.\n\nONDE FOI ACHADA:\n%s",
                    OndeAchadaEvidenciaMural(idx, culpado));
            } else {
                const char *classif = (respostaProva[cena] == 1) ? "Marcada: REAL" : (respostaProva[cena] == 2) ? "Marcada: GERADA POR IA" : "Não classificada";
                const char *muralSt = m->evPosicionada[idx]
                    ? (m->evLigadaA[idx] >= 0 ? "No Mural (Ligada)" : "No Mural (Sem linha)")
                    : "No Inventário inferior";
                snprintf(det, sizeof(det), "PROVA DE LÓGICA\nONDE FOI ACHADA:\n%s\n\nANÁLISE: %s\nMURAL: %s",
                    OndeAchadaEvidenciaMural(idx, culpado), classif, muralSt);
            }
        } else {
            if (!m->evDesbloqueada[idx]) {
                snprintf(det, sizeof(det), "PISTA EXTRA (??????)\nSTATUS: BLOQUEADA\n\nContinue a investigação para descobrir esta pista.");
            } else {
                snprintf(det, sizeof(det), "PISTA AMBÍGUA (EXTRA)\nONDE FOI ACHADA:\n%s\n\nCONTEÚDO:\n%s",
                    OndeAchadaEvidenciaMural(idx, culpado), ConteudoAmbiguoMural(idx, culpado));
            }
        }
        // Preview de imagem da evidencia quando disponivel e desbloqueada
        const char *imgEv = NULL;
        bool evVisivel = (idx < MURAL_NUM_EV_OFIC) ? (statusProva[idx + 3] != 0) : m->evDesbloqueada[idx];
        if (evVisivel) {
            if (idx == 1) imgEv = (culpado == 0) ? "AssetsRay/Personagens/Julia/novodeepfake.png" : "AssetsRay/Personagens/Julia/juliafotoreal.png";
            else if (idx == 4 && culpado == 1) imgEv = "AssetsRay/Evidências/caiodeepfake.png";
        }
        float yDet = painelEsq.y + 322;
        if (imgEv != NULL) {
            Rectangle rPrev = { painelEsq.x + 16, painelEsq.y + 318, painelEsq.width - 32, 126 };
            DrawRectangleRounded(rPrev, 0.05f, 4, (Color){ 12, 8, 24, 230 });
            DesenharTexturaContain(ObterTexturaCache(&m->cacheEvImg, imgEv), rPrev, WHITE);
            DrawRectangleRoundedLinesEx(rPrev, 0.05f, 4, 1.4f, COR_CIANO_NEON);
            yDet = painelEsq.y + 452;
        }
        DesenharTextoQuebrado(assets->fontes.regular, det, (Rectangle){ painelEsq.x + 16, yDet, painelEsq.width - 32, (painelEsq.y + 615) - yDet }, 17, 1.0f, WHITE);

        if (idx == 0 && statusProva[3] == 1) {
            Rectangle btnPlay = { painelEsq.x + 18, painelEsq.y + 620 - 46, 130, 34 };
            DesenharBotaoCyber(assets->fontes.bold, btnPlay, m->audioTocando ? "|| PAUSAR" : "> OUVIR", 18,
                CheckCollisionPointRec(mouse, btnPlay) ? 1.0f : 0.0f, tempo, COR_CIANO_NEON);
        }
    }

    // =========================================================================
    // 2. MURAL PRINCIPAL 2.5D COM ZOOM E LINHAS TECNOLOGICAS
    // =========================================================================
    Rectangle areaMural = { 378, 66, 878, 516 };
    DrawRectangleRec(areaMural, (Color){ 10, 7, 22, 240 });

    float parX = ((mouse.x - (areaMural.x + areaMural.width * 0.5f)) / areaMural.width) * 14.0f;
    float parY = ((mouse.y - (areaMural.y + areaMural.height * 0.5f)) / areaMural.height) * 10.0f;

    BeginScissorMode((int)(areaMural.x * camZoom), (int)(areaMural.y * camZoom), (int)(areaMural.width * camZoom), (int)(areaMural.height * camZoom));

    float step = 44.0f * m->zoom;
    float offX = fmodf((m->pan.x * m->zoom - parX * 0.5f), step);
    float offY = fmodf((m->pan.y * m->zoom - parY * 0.5f), step);
    for (float gx = areaMural.x + offX - step; gx < areaMural.x + areaMural.width + step; gx += step) {
        DrawLine((int)gx, (int)areaMural.y, (int)gx, (int)(areaMural.y + areaMural.height), ColorAlpha(COR_CIANO_NEON, 0.08f));
    }
    for (float gy = areaMural.y + offY - step; gy < areaMural.y + areaMural.height + step; gy += step) {
        DrawLine((int)areaMural.x, (int)gy, (int)(areaMural.x + areaMural.width), (int)gy, ColorAlpha(COR_ROXO_NEON, 0.08f));
    }

    if (m->pessoaSelecionada >= 0) {
        Vector2 pOrig = MuralParaTela(m->posPessoa[m->pessoaSelecionada], areaMural, m->zoom, m->pan);
        pOrig.x -= parX * 0.35f - 58.0f * m->zoom;
        pOrig.y -= parY * 0.35f;
        DesenharLinhaTecnologicaMural(pOrig, mouse, COR_ROSA_NEON, tempo, false);
    }

    // Linhas tecnologicas (conectam da borda direita da Foto ate a borda esquerda da Evidencia)
    for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
        if (!m->evPosicionada[i]) continue;
        Vector2 pEv = MuralParaTela(m->posEv[i], areaMural, m->zoom, m->pan);
        pEv.x -= parX * 0.50f + 82.0f * m->zoom;
        pEv.y -= parY * 0.50f;

        if (m->evLigadaA[i] >= 0) {
            Vector2 p1 = MuralParaTela(m->posPessoa[m->evLigadaA[i]], areaMural, m->zoom, m->pan);
            p1.x -= parX * 0.35f - 58.0f * m->zoom;
            p1.y -= parY * 0.35f;
            Color cLinha = m->evAutoLigadaA[i] ? COR_CIANO_NEON : COR_ROSA_NEON;
            DesenharLinhaTecnologicaMural(p1, pEv, cLinha, tempo, m->evAutoLigadaA[i]);
        }
        if (m->evLigadaB[i] >= 0) {
            Vector2 p2 = MuralParaTela(m->posPessoa[m->evLigadaB[i]], areaMural, m->zoom, m->pan);
            p2.x -= parX * 0.35f - 58.0f * m->zoom;
            p2.y -= parY * 0.35f;
            DesenharLinhaTecnologicaMural(p2, pEv, COR_AMARELO_AVISO, tempo, false);
        }
    }

    // 3 Mugshots em formato de COLUNA
    const char *fotosP[3] = {
        "AssetsRay/Personagens/Caio/CaioMugshot.png",
        "AssetsRay/Personagens/Julia/juliana retrato.png",
        "AssetsRay/Personagens/Gabriel/GabrielMugshot.png"
    };
    const char *titP[3] = { "CAIO", "JULIANA", "GABRIEL" };
    Color corP[3] = { COR_ROXO_NEON, COR_ROSA_NEON, COR_CIANO_NEON };

    for (int p = 0; p < MURAL_NUM_PESSOAS; p++) {
        Vector2 sc = MuralParaTela(m->posPessoa[p], areaMural, m->zoom, m->pan);
        sc.x -= parX * 0.35f; sc.y -= parY * 0.35f;
        float w = 116.0f * m->zoom, h = 136.0f * m->zoom;
        Rectangle rCard = { sc.x - w * 0.5f, sc.y - h * 0.5f, w, h };

        DrawRectangleRounded((Rectangle){ rCard.x + 8 * m->zoom, rCard.y + 10 * m->zoom, w, h }, 0.08f, 6, (Color){ 0, 0, 0, 155 });
        DrawRectangleRounded(rCard, 0.08f, 6, (Color){ 20, 12, 38, 245 });

        Texture2D tex = ObterTexturaCache(&m->cachePessoas[p], fotosP[p]);
        Rectangle rImg = { rCard.x + 6 * m->zoom, rCard.y + 6 * m->zoom, w - 12 * m->zoom, h - 34 * m->zoom };
        DesenharTexturaContain(tex, rImg, WHITE);

        bool sel = (m->pessoaSelecionada == p);
        float pulso = sel ? (0.7f + 0.3f * sinf((float)tempo * 6.0f)) : 0.85f;
        DrawRectangleRoundedLinesEx(rCard, 0.08f, 6, sel ? 3.0f : 1.8f, ColorAlpha(sel ? COR_AMARELO_AVISO : corP[p], pulso));
        DrawTextEx(assets->fontes.bold, titP[p], (Vector2){ rCard.x + 10 * m->zoom, rCard.y + h - 26 * m->zoom }, fmaxf(12.0f, 19.0f * m->zoom), 1.0f, corP[p]);
    }

    // Cards das Evidencias (Logicas + Ambiguas)
    for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
        if (!m->evPosicionada[i]) continue;
        Vector2 sc = MuralParaTela(m->posEv[i], areaMural, m->zoom, m->pan);
        sc.x -= parX * 0.50f; sc.y -= parY * 0.50f;
        float w = 164.0f * m->zoom, h = 72.0f * m->zoom;
        Rectangle rEv = { sc.x - w * 0.5f, sc.y - h * 0.5f, w, h };

        bool ehAmb = (i >= MURAL_NUM_EV_OFIC);
        bool naoValidada = (!ehAmb && statusProva[i + 3] == -1);

        DrawRectangleRounded((Rectangle){ rEv.x + 7 * m->zoom, rEv.y + 9 * m->zoom, w, h }, 0.12f, 6, (Color){ 0, 0, 0, 155 });
        DrawRectangleRounded(rEv, 0.12f, 6, naoValidada ? (Color){ 46, 14, 24, 245 } : (ehAmb ? (Color){ 36, 22, 42, 245 } : (Color){ 18, 24, 46, 245 }));

        bool faltaSegunda = (m->evExigeDupla[i] && m->evLigadaB[i] < 0);
        Color cB = naoValidada ? COR_VERMELHO_ERR
                 : faltaSegunda ? COR_AMARELO_AVISO
                 : (m->evLigadaA[i] >= 0 ? COR_VERDE_OK : COR_AMARELO_AVISO);
        float pulseB = faltaSegunda ? (0.55f + 0.45f * sinf((float)tempo * 5.0f)) : 1.0f;

        DrawRectangleRoundedLinesEx(rEv, 0.12f, 6, 2.0f, ColorAlpha(cB, pulseB));
        DrawTextEx(assets->fontes.bold, NomeCurtoEvidenciaMural(i, culpado),
            (Vector2){ rEv.x + 10 * m->zoom, rEv.y + 10 * m->zoom }, fmaxf(11.0f, 16.0f * m->zoom), 1.0f, naoValidada ? COR_VERMELHO_ERR : WHITE);

        const char *nomesS[3] = { "Caio", "Juliana", "Gabriel" };
        char sub[64] = "Sem ligação";
        if (naoValidada && m->evLigadaA[i] < 0) {
            snprintf(sub, sizeof(sub), "NÃO VALIDADA");
        } else if (naoValidada && m->evLigadaA[i] >= 0) {
            snprintf(sub, sizeof(sub), "N/VAL -> %s", nomesS[m->evLigadaA[i]]);
        } else if (faltaSegunda && m->evLigadaA[i] >= 0) {
            snprintf(sub, sizeof(sub), "LIGAR +1 PESSOA!");
        } else if (m->evLigadaA[i] >= 0 && m->evLigadaB[i] >= 0) {
            snprintf(sub, sizeof(sub), "%s <-> %s", nomesS[m->evLigadaA[i]], nomesS[m->evLigadaB[i]]);
        } else if (m->evLigadaA[i] >= 0) {
            snprintf(sub, sizeof(sub), "-> %s", nomesS[m->evLigadaA[i]]);
        }
        DrawTextEx(assets->fontes.regular, sub,
            (Vector2){ rEv.x + 10 * m->zoom, rEv.y + 38 * m->zoom }, fmaxf(10.0f, 15.0f * m->zoom), 1.0f, cB);
    }

    EndScissorMode();
    DrawRectangleLinesEx(areaMural, 2.0f, ColorAlpha(COR_CIANO_NEON, 0.7f));

    DrawTextEx(assets->fontes.bold, TextFormat("ZOOM: %d%%", (int)(m->zoom * 100.0f + 0.5f)),
        (Vector2){ areaMural.x + 14, areaMural.y + 14 }, 18, 1.0f, ColorAlpha(COR_CIANO_NEON, 0.85f));
    Rectangle btnZ1 = { areaMural.x + areaMural.width - 126, areaMural.y + 10, 34, 32 };
    Rectangle btnZ2 = { areaMural.x + areaMural.width - 88,  areaMural.y + 10, 34, 32 };
    Rectangle btnZ3 = { areaMural.x + areaMural.width - 48,  areaMural.y + 10, 38, 32 };
    DesenharBotaoCyber(assets->fontes.bold, btnZ1, "+", 20, CheckCollisionPointRec(mouse, btnZ1) ? 1.0f : 0.0f, tempo, COR_CIANO_NEON);
    DesenharBotaoCyber(assets->fontes.bold, btnZ2, "-", 20, CheckCollisionPointRec(mouse, btnZ2) ? 1.0f : 0.0f, tempo, COR_CIANO_NEON);
    DesenharBotaoCyber(assets->fontes.bold, btnZ3, "1x", 17, CheckCollisionPointRec(mouse, btnZ3) ? 1.0f : 0.0f, tempo, COR_ROXO_NEON);

    if (m->pessoaSelecionada >= 0) {
        DrawTextEx(assets->fontes.bold, "CLIQUE EM UMA EVIDÊNCIA PARA CONECTAR A LINHA >>",
            (Vector2){ areaMural.x + 150, areaMural.y + 14 }, 18, 1.0f, COR_AMARELO_AVISO);
    } else {
        DrawTextEx(assets->fontes.regular, "Clique na foto p/ ligar | Arraste p/ mover | Clique direito desfaz",
            (Vector2){ areaMural.x + 145, areaMural.y + 14 }, 17, 1.0f, ColorAlpha(LIGHTGRAY, 0.78f));
    }

    // =========================================================================
    // 3. INVENTARIO INFERIOR (9 SLOTS: P1..P5 LOGICA + A1..A4 AMBIGUAS)
    // =========================================================================
    Rectangle areaInv = { 378, 592, 878, 104 };
    DesenharPainelCyber(areaInv, COR_ROXO_NEON);
    DrawTextEx(assets->fontes.bold, "INVENTÁRIO (ARRASTE P/ MURAL | P1..P5 LÓGICA, A1..A4 EXTRAS):",
        (Vector2){ areaInv.x + 12, areaInv.y + 8 }, 16, 1.0f, COR_CIANO_NEON);

    for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
        Rectangle slot = { areaInv.x + 10 + i * 96, areaInv.y + 32, 91, 60 };
        bool ehAmb = (i >= MURAL_NUM_EV_OFIC);
        bool desb = m->evDesbloqueada[i];
        int st = ehAmb ? (desb ? 1 : 0) : statusProva[i + 3];
        bool naoVal = (!ehAmb && st == -1);

        if (!desb && st == 0) {
            DrawRectangleRounded(slot, 0.14f, 4, (Color){ 16, 10, 28, 180 });
            DrawRectangleRoundedLinesEx(slot, 0.14f, 4, 1.2f, ColorAlpha(GRAY, 0.4f));
            DrawTextEx(assets->fontes.regular, "??????", (Vector2){ slot.x + 16, slot.y + 21 }, 15, 1.0f, GRAY);
        } else if (m->evPosicionada[i]) {
            DrawRectangleRounded(slot, 0.14f, 4, naoVal ? (Color){ 42, 12, 20, 180 } : (Color){ 14, 34, 32, 180 });
            DrawRectangleRoundedLinesEx(slot, 0.14f, 4, 1.2f, ColorAlpha(naoVal ? COR_VERMELHO_ERR : COR_VERDE_OK, 0.6f));
            DrawTextEx(assets->fontes.bold, SlotCurtoMural(i, culpado), (Vector2){ slot.x + 6, slot.y + 10 }, 14, 1.0f, LIGHTGRAY);
            DrawTextEx(assets->fontes.bold, naoVal ? "N/ VALID." : "NO MURAL", (Vector2){ slot.x + 6, slot.y + 34 }, 13, 1.0f, naoVal ? COR_VERMELHO_ERR : COR_VERDE_OK);
        } else {
            bool hov = CheckCollisionPointRec(mouse, slot);
            DrawRectangleRounded(slot, 0.14f, 4, hov ? (Color){ 46, 22, 74, 240 } : (naoVal ? (Color){ 46, 14, 24, 235 } : (Color){ 28, 16, 52, 235 }));
            DrawRectangleRoundedLinesEx(slot, 0.14f, 4, 1.8f, hov ? COR_ROSA_NEON : (naoVal ? COR_VERMELHO_ERR : (ehAmb ? COR_AMARELO_AVISO : COR_CIANO_NEON)));
            DrawTextEx(assets->fontes.bold, SlotCurtoMural(i, culpado), (Vector2){ slot.x + 6, slot.y + 10 }, 14, 1.0f, naoVal ? COR_VERMELHO_ERR : WHITE);
            DrawTextEx(assets->fontes.regular, naoVal ? "N/ Valid." : "Arrastar", (Vector2){ slot.x + 6, slot.y + 34 }, 13, 1.0f, naoVal ? COR_VERMELHO_ERR : (ehAmb ? COR_AMARELO_AVISO : COR_CIANO_NEON));
        }
    }

    if (m->arrastandoTipo == 3 && m->arrastandoIdx >= 0) {
        Rectangle ghost = { mouse.x - 82, mouse.y - 36, 164, 72 };
        DrawRectangleRounded(ghost, 0.12f, 6, (Color){ 32, 18, 62, 230 });
        DrawRectangleRoundedLinesEx(ghost, 0.12f, 6, 2.0f, COR_ROSA_NEON);
        DrawTextEx(assets->fontes.bold, NomeCurtoEvidenciaMural(m->arrastandoIdx, culpado),
            (Vector2){ ghost.x + 10, ghost.y + 24 }, 16, 1.0f, WHITE);
    }

    if (m->iaAvisoTimer > 0.0f) {
        float alphaAv = fminf(1.0f, m->iaAvisoTimer);
        Rectangle rAv = { 430, 78, 774, 48 };
        DrawRectangleRounded(rAv, 0.18f, 6, (Color){ 12, 8, 24, (unsigned char)(245 * alphaAv) });
        DrawRectangleRoundedLinesEx(rAv, 0.18f, 6, 2.0f, ColorAlpha(COR_AMARELO_AVISO, alphaAv));
        Vector2 szAv = MeasureTextEx(assets->fontes.bold, m->iaAviso, 21, 1.0f);
        DrawTextEx(assets->fontes.bold, m->iaAviso,
            (Vector2){ rAv.x + (rAv.width - szAv.x) * 0.5f, rAv.y + (rAv.height - szAv.y) * 0.5f },
            21, 1.0f, ColorAlpha(COR_AMARELO_AVISO, alphaAv));
    }

    DesenharScanlinesHUD(tempo);
}

#endif
