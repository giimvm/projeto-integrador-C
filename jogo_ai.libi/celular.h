#ifndef CELULAR_H
#define CELULAR_H

#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    int chatAtivo; // 0 = Chefe Marcos, 1 = Sistema AI.LIBI
    bool lido[2];
    float hoverChats[2];
    float hoverAvancar;
    float chatAnimT;
} EstadoCelular;

static inline void IniciarCelular(EstadoCelular *c) {
    memset(c, 0, sizeof(*c));
    c->lido[0] = true;
    c->chatAnimT = 1.0f;
}

static inline int AtualizarCelular(EstadoCelular *c, Vector2 mouse, float dt, Sound sfxClick, bool temSfx) {
    if (c->chatAnimT < 1.0f) {
        c->chatAnimT += dt * 4.5f;
        if (c->chatAnimT > 1.0f) c->chatAnimT = 1.0f;
    }

    Rectangle itens[2] = {
        { 158, 196, 295, 74 },
        { 158, 286, 295, 74 }
    };
    AtualizarHover(itens, c->hoverChats, 2, mouse, dt);

    for (int i = 0; i < 2; i++) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, itens[i])) {
            if (temSfx) PlaySound(sfxClick);
            if (c->chatAtivo != i) c->chatAnimT = 0.0f;
            c->chatAtivo = i;
            c->lido[i] = true;
        }
    }

    Rectangle btnAvancar = { 735, 584, 385, 52 };
    AtualizarHover(&btnAvancar, &c->hoverAvancar, 1, mouse, dt);
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnAvancar)) {
        if (temSfx) PlaySound(sfxClick);
        return 1;
    }
    return 0;
}

static inline void DesenharCelular(const EstadoCelular *c, FontesUI *f, double tempo) {
    DesenharFundoCyberGrid(tempo, COR_CIANO_NEON);
    DesenharPainelCyber((Rectangle){ 115, 68, 1050, 594 }, COR_ROXO_NEON);

    DrawTextEx(f->titulo, "NEXUS-MSG // COMUNICACAO SEGURA", (Vector2){ 158, 92 }, 27, 1.0f, COR_CIANO_NEON);

    // Indicador ONLINE pulsante
    float pulso = 0.55f + 0.45f * sinf((float)tempo * 4.0f);
    DrawCircle(1008, 108, 5.5f, ColorAlpha(COR_VERDE_OK, pulso));
    DrawTextEx(f->bold, "ONLINE", (Vector2){ 1022, 95 }, 24, 1.0f, COR_VERDE_OK);
    DrawRectangle(158, 134, 964, 2, ColorAlpha(COR_ROSA_NEON, 0.45f));

    DrawTextEx(f->bold, "CONVERSAS", (Vector2){ 165, 154 }, 24, 1.0f, LIGHTGRAY);
    const char *nomes[2] = { "Chefe Marcos", "Sistema AI.LIBI" };

    for (int i = 0; i < 2; i++) {
        Rectangle item = { 158, 196 + i * 90, 295, 74 };
        bool ativo = (c->chatAtivo == i);
        Color corItem = ativo ? COR_CIANO_NEON : COR_ROXO_NEON;
        DesenharBotaoCyber(f->bold, item, nomes[i], 26, ativo ? fmaxf(0.55f, c->hoverChats[i]) : c->hoverChats[i], tempo, corItem);
        if (!c->lido[i]) {
            DrawCircle((int)(item.x + item.width - 20), (int)(item.y + 37), 6.0f, ColorAlpha(COR_ROSA_NEON, pulso));
        }
    }

    Rectangle chatArea = { 478, 152, 646, 418 };
    DesenharPainelCyber(chatArea, ColorAlpha(COR_CIANO_NEON, 0.6f));

    float slide = (1.0f - c->chatAnimT) * 28.0f;
    unsigned char alphaMsg = (unsigned char)(255 * c->chatAnimT);

    if (c->chatAtivo == 0) {
        const char *msgs[3] = {
            "A equipe ja comecou a recolher os depoimentos das pessoas que estavam na festa.",
            "Precisamos descobrir o que aconteceu naquela noite. Analise os alibis, as imagens, as mensagens e tudo que conseguirmos encontrar.",
            "E nao confie em uma evidencia so porque ela parece verdadeira."
        };
        float y = chatArea.y + 22;
        for (int i = 0; i < 3; i++) {
            float h = (i == 1) ? 124.0f : 92.0f;
            Rectangle bolha = { chatArea.x + 22 + slide, y, 602, h };
            DrawRectangleRounded(bolha, 0.12f, 8, (Color){ 36, 24, 68, (unsigned char)(240 * c->chatAnimT) });
            DrawRectangle((int)bolha.x, (int)bolha.y + 10, 4, (int)bolha.height - 20, ColorAlpha(COR_CIANO_NEON, c->chatAnimT));
            DesenharTextoQuebrado(f->regular, msgs[i], (Rectangle){ bolha.x + 18, bolha.y + 12, 566, h - 18 }, 25, 1.0f, (Color){ 255, 255, 255, alphaMsg });
            y += h + 16;
        }
    } else {
        const char *msgs[2] = {
            "ALERTA AI.LIBI: Conteudos digitais podem ser gerados ou alterados por Inteligencia Artificial.",
            "Sempre verifique horarios, reflexos, sombras, distorcoes em maos/objetos e metadados antes de concluir seu veredito."
        };
        float y = chatArea.y + 24;
        for (int i = 0; i < 2; i++) {
            Rectangle bolha = { chatArea.x + 22 + slide, y, 602, 118.0f };
            DrawRectangleRounded(bolha, 0.12f, 8, (Color){ 20, 44, 66, (unsigned char)(240 * c->chatAnimT) });
            DrawRectangle((int)bolha.x, (int)bolha.y + 10, 4, (int)bolha.height - 20, ColorAlpha(COR_ROSA_NEON, c->chatAnimT));
            DesenharTextoQuebrado(f->regular, msgs[i], (Rectangle){ bolha.x + 18, bolha.y + 14, 566, 94 }, 25, 1.0f, (Color){ 255, 255, 255, alphaMsg });
            y += 136;
        }
    }

    Rectangle btnAvancar = { 735, 584, 385, 52 };
    DesenharBotaoCyber(f->bold, btnAvancar, "IR PARA O INTERROGATORIO >>", 25, c->hoverAvancar, tempo, COR_VERDE_OK);
    DesenharScanlinesHUD(tempo);
}

#endif
