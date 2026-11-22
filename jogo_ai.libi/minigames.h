#ifndef MINIGAMES_H
#define MINIGAMES_H

#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *proposicao;
    const char *pergunta;
    const char *opcoes[4];
    int numOpcoes;
    int respostaCorreta;
    int opcaoAcusaCaio;
    const char *explicacao;
} DesafioLogica;

typedef struct {
    int opcaoSelecionada;
    int resultado; // 0 = jogando, 1 = acertou, -1 = errou, 2 = acusou Caio
    float hoverOpcoes[4];
    float hoverCont;
    float animFeedback;
} EstadoMiniGame;

static const DesafioLogica desafiosCenas[8] = {
    {0}, {0},
    // CENA 2
    {
        "\"Se eu fui embora as 2h40, entao a camera da saida me registrou.\"",
        "Considere que Caio realmente saiu as 2h40. O que se conclui?",
        {
            "a) A camera deve ter registrado Caio saindo.",
            "b) A camera nao registrou Caio.",
            "c) Nada se conclui.",
            "d) Caio e o culpado."
        },
        4, 0, 3,
        "Modus Ponens (Se P entao Q): se P e verdadeiro, Q tambem deve ocorrer."
    },
    // CENA 3
    {
        "\"O audio e autentico E a voz e de Gabriel.\"",
        "Considere que a frase inteira e falsa. Qual e a sua negacao logica?",
        {
            "a) O audio nao e autentico e a voz nao e de Gabriel.",
            "b) O audio nao e autentico ou a voz nao e de Gabriel.",
            "c) O audio e autentico ou a voz nao e de Gabriel.",
            "d) O audio e autentico e a voz nao e de Gabriel."
        },
        4, 1, -1,
        "Lei de De Morgan: negar (P E Q) resulta em (nao P OU nao Q)."
    },
    // CENA 4
    {
        "\"Se a conversa veio do celular de Caio, entao a pericia confirma o horario.\"",
        "Sabendo que a pericia NAO confirmou o horario, o que podemos concluir?",
        {
            "a) A conversa veio do celular de Caio.",
            "b) A conversa NAO veio do celular de Caio.",
            "c) Caio apagou todas as mensagens.",
            "d) Gabriel nao estava na festa."
        },
        4, 1, -1,
        "Contrapositiva: (P -> Q) equivale a (nao Q -> nao P)."
    },
    // CENA 5
    {
        "\"Juliana estava no carro as 03h07 OU ela continuou na festa.\"",
        "Se descobrirmos que essa afirmacao inteira e FALSA, o que e verdade?",
        {
            "a) Ela nao estava no carro E nao continuou na festa.",
            "b) Ela nao estava no carro OU nao continuou na festa.",
            "c) Ela estava no carro as 03h07.",
            "d) Nenhuma das anteriores."
        },
        4, 0, -1,
        "Lei de De Morgan: negar (P OU Q) resulta em (nao P E nao Q)."
    },
    // CENA 6
    {
        "\"O sinal estava na estrada as 03h20 E o corpo foi encontrado la.\"",
        "Para uma frase com o conectivo E (conjuncao) ser VERDADEIRA, o que e necessario?",
        {
            "a) Basta que apenas uma das partes seja verdadeira.",
            "b) As duas partes precisam ser falsas.",
            "c) Ambas as partes precisam ser verdadeiras.",
            "d) A primeira parte precisa ser falsa."
        },
        4, 2, -1,
        "Na conjuncao (P E Q), ambas as partes precisam ser verdadeiras."
    },
    // CENA 7
    {
        "\"Se a foto de 02h58 for real, entao Juliana estava no estacionamento.\"",
        "Se a foto NAO for real, o que a logica garante apenas por essa frase?",
        {
            "a) Juliana com certeza nao estava la.",
            "b) Juliana com certeza estava la.",
            "c) Nada pode ser garantido apenas por essa implicacao.",
            "d) A camera da festa tambem e falsa."
        },
        4, 2, -1,
        "Em (Se P entao Q), quando P e falso, Q pode ser verdadeiro ou falso."
    }
};

static inline void IniciarMiniGame(EstadoMiniGame *mg) {
    memset(mg, 0, sizeof(*mg));
    mg->opcaoSelecionada = -1;
}

static inline int AtualizarMiniGame(EstadoMiniGame *mg, int cena, Vector2 mouse, float dt, Sound sfxClick, bool temSfx) {
    if (cena < 2 || cena > 7) return 0;
    const DesafioLogica *d = &desafiosCenas[cena];

    if (mg->resultado != 0 && mg->animFeedback < 1.0f) {
        mg->animFeedback += dt * 4.0f;
        if (mg->animFeedback > 1.0f) mg->animFeedback = 1.0f;
    }

    Rectangle opts[4];
    for (int i = 0; i < d->numOpcoes; i++) {
        opts[i] = (Rectangle){ 155, 294 + i * 62, 970, 52 };
    }
    if (mg->resultado == 0) {
        AtualizarHover(opts, mg->hoverOpcoes, d->numOpcoes, mouse, dt);
        for (int i = 0; i < d->numOpcoes; i++) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, opts[i])) {
                if (temSfx) PlaySound(sfxClick);
                mg->opcaoSelecionada = i;
                mg->animFeedback = 0.0f;
                if (d->opcaoAcusaCaio >= 0 && i == d->opcaoAcusaCaio) mg->resultado = 2;
                else if (i == d->respostaCorreta) mg->resultado = 1;
                else mg->resultado = -1;
                break;
            }
        }
    } else {
        Rectangle btnCont = { 465, 600, 350, 50 };
        AtualizarHover(&btnCont, &mg->hoverCont, 1, mouse, dt);
        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnCont)) ||
            IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (temSfx) PlaySound(sfxClick);
            return mg->resultado;
        }
    }
    return 0;
}

static inline void DesenharMiniGame(const EstadoMiniGame *mg, FontesUI *f, int cena, double tempo) {
    if (cena < 2 || cena > 7) return;
    const DesafioLogica *d = &desafiosCenas[cena];

    DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
    float shakeX = 0.0f;
    if (mg->resultado == -1 && mg->animFeedback < 0.6f) {
        shakeX = sinf(mg->animFeedback * 48.0f) * 8.0f * (1.0f - mg->animFeedback / 0.6f);
    }
    DesenharPainelCyber((Rectangle){ 95 + shakeX, 52, 1090, 622 }, COR_ROXO_NEON);

    DesenharBadgeCyber(f->bold, (Vector2){ 155 + shakeX, 74 }, TextFormat("DESAFIO DE LOGICA // CENA 0%d", cena), 22, COR_CIANO_NEON);
    DesenharTextoQuebrado(f->titulo, d->proposicao, (Rectangle){ 155 + shakeX, 124, 970, 88 }, 28, 1.0f, COR_ROSA_NEON);
    DesenharTextoQuebrado(f->regular, d->pergunta, (Rectangle){ 155 + shakeX, 224, 970, 62 }, 26, 1.0f, WHITE);

    for (int i = 0; i < d->numOpcoes; i++) {
        Rectangle opt = { 155 + shakeX, 294 + i * 62, 970, 52 };
        Color corBorda = COR_ROXO_NEON;
        float hov = (mg->resultado == 0) ? mg->hoverOpcoes[i] : 0.0f;
        if (mg->resultado != 0) {
            if (i == d->respostaCorreta) { corBorda = COR_VERDE_OK; hov = 0.85f; }
            else if (i == mg->opcaoSelecionada) { corBorda = COR_VERMELHO_ERR; hov = 0.75f; }
            else corBorda = ColorAlpha(GRAY, 0.45f);
        }
        DesenharBotaoCyber(f->regular, opt, d->opcoes[i], 25, hov, tempo, corBorda);
    }

    if (mg->resultado != 0) {
        const char *titulo = (mg->resultado == 1) ? "DESAFIO RESOLVIDO!" :
                             (mg->resultado == 2) ? "ACUSACAO SELECIONADA" : "RESPOSTA INCORRETA";
        Color corRes = (mg->resultado == 1) ? COR_VERDE_OK :
                       (mg->resultado == 2) ? COR_AMARELO_AVISO : COR_VERMELHO_ERR;

        DrawTextEx(f->bold, titulo, (Vector2){ 155, 550 }, 25, 1.0f, corRes);
        if (mg->resultado != 2) {
            DesenharTextoQuebrado(f->regular, d->explicacao, (Rectangle){ 415, 551, 710, 48 }, 22, 1.0f, LIGHTGRAY);
        }
        Rectangle btnCont = { 465, 600, 350, 50 };
        DesenharBotaoCyber(f->bold, btnCont, "CONTINUAR >>", 25, mg->hoverCont, tempo, corRes);
    }
    DesenharScanlinesHUD(tempo);
}

#endif
