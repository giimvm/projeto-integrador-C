#ifndef MINIGAMES_H
#define MINIGAMES_H

#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>

#define TEMPO_LIMITE_DESAFIO 20.0f

typedef struct {
    const char *proposicao;
    const char *pergunta;
    const char *opcoes[4];
    int numOpcoes;
    int respostaCorreta;
    const char *explicacao;
} DesafioLogica;

typedef struct {
    int opcaoSelecionada;
    int resultado; // 0 = jogando, 1 = acertou, -1 = errou/tempo esgotado
    float hoverOpcoes[4];
    float hoverCont;
    float animFeedback;
    float tempoRestante;
    bool tempoEsgotado;
} EstadoMiniGame;

// Desafios de logica das 5 provas (Cenas 3, 4, 5, 6 e 7)
static const DesafioLogica desafiosCenas[8] = {
    {0}, {0}, {0},
    // CENA 3 - Prova 1: Audio anonimo
    {
        "\"O áudio é autêntico E a voz é de Caio.\"",
        "Considere que a frase inteira é falsa. Qual é a sua negação lógica?",
        {
            "a) O áudio não é autêntico e a voz não é de Caio.",
            "b) O áudio não é autêntico ou a voz não é de Caio.",
            "c) O áudio é autêntico ou a voz não é de Caio.",
            "d) O áudio é autêntico e a voz não é de Caio."
        },
        4, 1,
        "Lei de De Morgan: negar (P E Q) resulta em (não P OU não Q)."
    },
    // CENA 4 - Prova 2: Foto de Juliana no carro
    {
        "\"Juliana estava no carro às 03:05 OU ela continuou na festa.\"",
        "Se descobrirmos que essa afirmação inteira é FALSA, o que é verdade?",
        {
            "a) Ela não estava no carro E não continuou na festa.",
            "b) Ela não estava no carro OU não continuou na festa.",
            "c) Ela estava no carro às 03:05.",
            "d) Nenhuma das anteriores."
        },
        4, 0,
        "Lei de De Morgan: negar (P OU Q) resulta em (não P E não Q)."
    },
    // CENA 5 - Prova 3: Localizacao do celular de Gabriel
    {
        "\"O sinal estava no barranco às 03:20 E o corpo foi encontrado lá.\"",
        "Para uma proposição com o conectivo E (conjunção) ser VERDADEIRA, o que é necessário?",
        {
            "a) Basta que apenas uma das partes seja verdadeira.",
            "b) As duas partes precisam ser falsas.",
            "c) Ambas as partes precisam ser verdadeiras.",
            "d) A primeira parte precisa ser falsa."
        },
        4, 2,
        "Na conjunção (P E Q), ambas as partes precisam ser verdadeiras."
    },
    // CENA 6 - Prova 4: Backup na nuvem de Gabriel
    {
        "\"Se o arquivo foi salvo na nuvem antes das 03:05, então Gabriel estava vivo.\"",
        "Sabendo que Gabriel NÃO estava vivo após 03:05, o que podemos concluir?",
        {
            "a) O arquivo foi salvo após as 03:05.",
            "b) O arquivo NÃO foi salvo antes das 03:05.",
            "c) O servidor da nuvem foi invadido.",
            "d) Nada pode ser concluído."
        },
        4, 1,
        "Contrapositiva (Modus Tollens): se (P -> Q) e (não Q), então (não P)."
    },
    // CENA 7 - Prova 5: Registro de saida (Mobi ou Camera do estacionamento)
    {
        "\"Se o registro de saída é válido, então o horário do álibi é confirmado.\"",
        "Se o registro de saída NÃO for válido, o que a lógica garante apenas por essa frase?",
        {
            "a) O horário do álibi com certeza é falso.",
            "b) O horário do álibi com certeza é verdadeiro.",
            "c) Nada pode ser garantido apenas por essa implicação.",
            "d) O suspeito necessariamente fugiu a pé."
        },
        4, 2,
        "Em uma implicação (Se P então Q), quando P é falso, Q pode ser verdadeiro ou falso."
    }
};

static inline void IniciarMiniGame(EstadoMiniGame *mg) {
    memset(mg, 0, sizeof(*mg));
    mg->opcaoSelecionada = -1;
    mg->tempoRestante = TEMPO_LIMITE_DESAFIO;
}

static inline int AtualizarMiniGame(EstadoMiniGame *mg, int cena, Vector2 mouse, float dt, Sound sfxClick, bool temSfx) {
    if (cena < 3 || cena > 7) return 0;
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
        mg->tempoRestante -= dt;
        if (mg->tempoRestante <= 0.0f) {
            mg->tempoRestante = 0.0f;
            mg->tempoEsgotado = true;
            mg->resultado = -1;
            mg->animFeedback = 0.0f;
            return 0;
        }
        AtualizarHover(opts, mg->hoverOpcoes, d->numOpcoes, mouse, dt);
        for (int i = 0; i < d->numOpcoes; i++) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, opts[i])) {
                if (temSfx) PlaySound(sfxClick);
                mg->opcaoSelecionada = i;
                mg->animFeedback = 0.0f;
                mg->resultado = (i == d->respostaCorreta) ? 1 : -1;
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
    if (cena < 3 || cena > 7) return;
    const DesafioLogica *d = &desafiosCenas[cena];

    DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
    float shakeX = 0.0f;
    if (mg->resultado == -1 && mg->animFeedback < 0.6f) {
        shakeX = sinf(mg->animFeedback * 48.0f) * 8.0f * (1.0f - mg->animFeedback / 0.6f);
    }
    DesenharPainelCyber((Rectangle){ 95 + shakeX, 52, 1090, 622 }, COR_ROXO_NEON);

    DesenharBadgeCyber(f->bold, (Vector2){ 155 + shakeX, 74 }, TextFormat("DESAFIO DE LÓGICA // PROVA 0%d", cena - 2), 22, COR_CIANO_NEON);

    Color corTimer = (mg->tempoRestante <= 5.0f) ? COR_VERMELHO_ERR : COR_AMARELO_AVISO;
    DesenharBadgeCyber(f->bold, (Vector2){ 965 + shakeX, 74 }, TextFormat("TEMPO: %02ds", (int)ceilf(mg->tempoRestante)), 22, corTimer);

    DesenharTextoQuebrado(f->titulo, d->proposicao, (Rectangle){ 155 + shakeX, 124, 970, 88 }, 27, 1.0f, COR_ROSA_NEON);
    DesenharTextoQuebrado(f->regular, d->pergunta, (Rectangle){ 155 + shakeX, 224, 970, 62 }, 25, 1.0f, WHITE);

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
                             mg->tempoEsgotado ? "TEMPO ESGOTADO!" : "RESPOSTA INCORRETA";
        Color corRes = (mg->resultado == 1) ? COR_VERDE_OK : COR_VERMELHO_ERR;

        DrawTextEx(f->bold, titulo, (Vector2){ 155, 550 }, 25, 1.0f, corRes);
        DesenharTextoQuebrado(f->regular, d->explicacao, (Rectangle){ 415, 551, 710, 48 }, 22, 1.0f, LIGHTGRAY);
        Rectangle btnCont = { 465, 600, 350, 50 };
        DesenharBotaoCyber(f->bold, btnCont, "CONTINUAR >>", 25, mg->hoverCont, tempo, corRes);
    }
    DesenharScanlinesHUD(tempo);
}

#endif
