#ifndef MINIGAMES_H
#define MINIGAMES_H

#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>

#define TEMPO_LIMITE_DESAFIO 30.0f

typedef struct {
    const char *proposicao;
    const char *pergunta;
    const char *opcoes[3];
    int respostaCorreta; // 0, 1 ou 2
    const char *explicacao;
} DesafioLogica;

typedef struct {
    int selecionada;        // -1 = nenhuma, 0..2
    int tentativasRestantes; // 2 tentativas
    bool mostrandoTutorial;
    bool tutorialVisto;
    bool erroPrimeiraTentativa;
    int resultado;          // 0 = jogando, 1 = acertou, -1 = errou/tempo esgotado
    float hoverOp[3];
    float hoverCont;
    float animFeedback;
    float tempoRestante;
    bool tempoEsgotado;
} EstadoMiniGame;

// Desafios intuitivos de Logica Proposicional (Cenas 3, 4, 5, 6 e 7)
static const DesafioLogica desafiosCenas[8] = {
    {0}, {0}, {0},
    // CENA 3 - Prova 1: Negacao do E
    {
        "Afirmação: \"O áudio é real E a voz é de Caio.\"",
        "Se descobrirmos que essa frase inteira é FALSA, o que precisa ser verdade?",
        {
            "O áudio NÃO é real OU a voz NÃO é de Caio.",
            "O áudio é real E a voz é de Caio ao mesmo tempo.",
            "Nenhum áudio foi gravado naquela noite."
        },
        0,
        "Correto! Para uma frase com 'E' ser falsa, basta UMA das partes ser falsa (troca 'E' por 'OU' e nega)."
    },
    // CENA 4 - Prova 2: Negacao do OU
    {
        "Afirmação: \"Juliana foi de carro OU ficou na festa.\"",
        "Se essa frase inteira for FALSA, o que realmente aconteceu?",
        {
            "Ela foi de carro, mas não ficou na festa.",
            "Ela NÃO foi de carro E NÃO ficou na festa.",
            "Ela ficou na festa OU foi de carro."
        },
        1,
        "Correto! Uma frase com 'OU' só é falsa quando AS DUAS partes são falsas ao mesmo tempo."
    },
    // CENA 5 - Prova 3: Conjuncao (E)
    {
        "Afirmação: \"O celular estava no barranco E o corpo estava lá.\"",
        "O que precisa acontecer para essa frase com 'E' ser VERDADEIRA?",
        {
            "Apenas o celular precisa estar no barranco.",
            "As duas coisas precisam ser verdadeiras ao mesmo tempo.",
            "Basta que uma das duas seja falsa."
        },
        1,
        "Correto! Na lógica, 'P E Q' só é verdadeiro quando P e Q são verdadeiros juntos."
    },
    // CENA 6 - Prova 4: Se... Entao (Contrapositiva)
    {
        "Regra: \"SE Gabriel estava vivo às 02:15, ENTÃO ele salvou o arquivo.\"",
        "Sabemos que ele NÃO salvou nada depois das 03:05. O que concluímos?",
        {
            "Ele NÃO estava vivo depois das 03:05.",
            "Ele com certeza salvou o arquivo às 04:00.",
            "O arquivo foi apagado automaticamente."
        },
        0,
        "Correto! Se 'SE P, ENTÃO Q' vale e Q não aconteceu, então P também não aconteceu."
    },
    // CENA 7 - Prova 5: Se... Entao com condicao falsa
    {
        "Regra: \"SE o registro de saída for válido, ENTÃO o álibi é confirmado.\"",
        "Se o registro NÃO for válido, o que essa regra garante?",
        {
            "O álibi é automaticamente verdadeiro.",
            "O suspeito com certeza fugiu a pé.",
            "Nada garantido: o álibi pode ser verdadeiro ou falso."
        },
        2,
        "Correto! Quando a condição do 'SE' é falsa, a regra não garante nada sobre o resultado."
    }
};

static inline void IniciarMiniGame(EstadoMiniGame *mg, int cena) {
    bool tutVisto = mg->tutorialVisto;
    memset(mg, 0, sizeof(*mg));
    mg->tutorialVisto = tutVisto;
    mg->selecionada = -1;
    mg->tentativasRestantes = 2;
    mg->tempoRestante = TEMPO_LIMITE_DESAFIO;

    // Tutorial aparece APENAS na primeira evidencia (Cena 3)
    if (cena == 3 && !mg->tutorialVisto) {
        mg->mostrandoTutorial = true;
        mg->tutorialVisto = true;
    }
}

static inline int AtualizarMiniGame(EstadoMiniGame *mg, int cena, Vector2 mouse, float dt, Sound sfxClick, bool temSfx) {
    if (cena < 3 || cena > 7) return 0;
    const DesafioLogica *d = &desafiosCenas[cena];

    if (mg->mostrandoTutorial) {
        Rectangle btnOk = { 440, 536, 400, 52 };
        AtualizarHover(&btnOk, &mg->hoverCont, 1, mouse, dt);
        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnOk)) ||
            IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (temSfx) PlaySound(sfxClick);
            mg->mostrandoTutorial = false;
        }
        return 0;
    }

    if (mg->resultado != 0 && mg->animFeedback < 1.0f) {
        mg->animFeedback += dt * 4.0f;
        if (mg->animFeedback > 1.0f) mg->animFeedback = 1.0f;
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

        Rectangle ops[3] = {
            { 160, 290, 960, 62 },
            { 160, 366, 960, 62 },
            { 160, 442, 960, 62 }
        };
        AtualizarHover(ops, mg->hoverOp, 3, mouse, dt);

        int escolha = -1;
        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) escolha = 0;
        else if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) escolha = 1;
        else if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) escolha = 2;
        else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = 0; i < 3; i++) {
                if (CheckCollisionPointRec(mouse, ops[i])) {
                    escolha = i;
                    break;
                }
            }
        }

        if (escolha >= 0) {
            if (temSfx) PlaySound(sfxClick);
            mg->selecionada = escolha;
            if (escolha == d->respostaCorreta) {
                mg->resultado = 1;
                mg->animFeedback = 0.0f;
            } else {
                mg->tentativasRestantes--;
                if (mg->tentativasRestantes <= 0) {
                    mg->resultado = -1;
                    mg->animFeedback = 0.0f;
                } else {
                    mg->erroPrimeiraTentativa = true;
                }
            }
        }
    } else {
        Rectangle btnCont = { 440, 560, 400, 52 };
        AtualizarHover(&btnCont, &mg->hoverCont, 1, mouse, dt);
        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnCont)) ||
            IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            if (temSfx) PlaySound(sfxClick);
            return mg->resultado;
        }
    }

    return 0;
}

static inline void DesenharMiniGame(const EstadoMiniGame *mg, const FontesUI *f, int cena, double tempo) {
    if (cena < 3 || cena > 7) return;
    const DesafioLogica *d = &desafiosCenas[cena];

    DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
    DesenharPainelCyber((Rectangle){ 95, 76, 1090, 568 }, COR_ROXO_NEON);

    if (mg->mostrandoTutorial) {
        DesenharBadgeCyber(f->titulo, (Vector2){ 140, 104 }, "TUTORIAL // LÓGICA PROPOSICIONAL", 25, COR_CIANO_NEON);
        DrawRectangle((int)140, (int)156, 1000, 2, ColorAlpha(COR_CIANO_NEON, 0.45f));

        const char *tut =
            "Na perícia da AI.libi, você usa Lógica Proposicional para testar se uma afirmação faz sentido:\n\n"
            "• E (Conjunção): Só é verdade se AS DUAS partes forem verdadeiras.\n"
            "   Se for FALSA, basta uma parte falhar -> negue e troque 'E' por 'OU'.\n\n"
            "• OU (Disjunção): É verdade se pelo menos UMA parte for verdadeira.\n"
            "   Se for FALSA, AS DUAS falharam -> negue e troque 'OU' por 'E'.\n\n"
            "• SE... ENTÃO: Se o resultado final NÃO aconteceu, a condição inicial também NÃO aconteceu.\n\n"
            "Você terá 20 segundos e 2 tentativas por desafio. Boa investigação!";
        DesenharTextoQuebrado(f->regular, tut, (Rectangle){ 145, 178, 990, 330 }, 24, 1.0f, WHITE);

        DesenharBotaoCyber(f->bold, (Rectangle){ 440, 536, 400, 52 }, "ENTENDI, COMEÇAR >>", 23, mg->hoverCont, tempo, COR_VERDE_OK);
        return;
    }

    DesenharBadgeCyber(f->bold, (Vector2){ 140, 102 }, TextFormat("DESAFIO LÓGICO // CENA %d", cena), 22, COR_CIANO_NEON);
    DesenharBadgeCyber(f->bold, (Vector2){ 490, 102 }, TextFormat("TENTATIVAS: %d/2", mg->tentativasRestantes), 20,
        (mg->tentativasRestantes == 2) ? COR_VERDE_OK : COR_AMARELO_AVISO);

    if (mg->resultado == 0) {
        Color corT = (mg->tempoRestante <= 7.0f) ? COR_VERMELHO_ERR : COR_AMARELO_AVISO;
        DesenharBadgeCyber(f->bold, (Vector2){ 965, 102 }, TextFormat("TEMPO: %02ds", (int)ceilf(mg->tempoRestante)), 22, corT);
    }

    DesenharTextoQuebrado(f->bold, d->proposicao, (Rectangle){ 145, 162, 990, 60 }, 27, 1.0f, WHITE);
    DesenharTextoQuebrado(f->regular, d->pergunta, (Rectangle){ 145, 224, 990, 52 }, 23, 1.0f, COR_ROSA_NEON);

    for (int i = 0; i < 3; i++) {
        Rectangle op = { 160, 290 + i * 76, 960, 62 };
        Color corOp = COR_ROXO_NEON;
        if (mg->resultado != 0) {
            if (i == d->respostaCorreta) corOp = COR_VERDE_OK;
            else if (i == mg->selecionada) corOp = COR_VERMELHO_ERR;
        } else if (mg->erroPrimeiraTentativa && i == mg->selecionada) {
            corOp = COR_VERMELHO_ERR;
        }
        DesenharBotaoCyber(f->regular, op, TextFormat("[%d] %s", i + 1, d->opcoes[i]), 23, mg->hoverOp[i], tempo, corOp);
    }

    if (mg->erroPrimeiraTentativa && mg->resultado == 0) {
        DesenharBadgeCyber(f->bold, (Vector2){ 160, 525 },
            "Incorreto! Você ainda tem 1 tentativa — releia os conectivos (E / OU / SE...ENTÃO).", 20, COR_AMARELO_AVISO);
    }

    if (mg->resultado != 0) {
        bool ok = (mg->resultado == 1);
        Color corRes = ok ? COR_VERDE_OK : COR_VERMELHO_ERR;
        const char *titRes = ok ? "VALIDAÇÃO LÓGICA CONFIRMADA!"
            : (mg->tempoEsgotado ? "TEMPO ESGOTADO! (NÃO VALIDADA)" : "NÃO VALIDADA! (2 ERROS)");
        DrawTextEx(f->bold, titRes, (Vector2){ 160, 514 }, 23, 1.0f, corRes);
        if (ok) {
            DesenharTextoQuebrado(f->regular, d->explicacao, (Rectangle){ 160, 540, 270, 80 }, 17, 1.0f, LIGHTGRAY);
        }
        DesenharBotaoCyber(f->bold, (Rectangle){ 440, 560, 400, 52 }, "CONTINUAR >>", 24, mg->hoverCont, tempo, corRes);
    }
}

#endif
