#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "assets.h"
#include "roteiro.h"
#include "minigames.h"
#include "celular.h"
#include "progresso.h"

typedef enum {
    TELA_MENU = 0,
    TELA_CONFIG,
    TELA_CREDITOS,

    // CENA 1
    TELA_C1_OCORRENCIA,
    TELA_C1_GABRIEL,
    TELA_C1_CAIO,
    TELA_C1_CELULAR,
    TELA_C1_INTERROGATORIO,

    // MOTOR DE DIALOGO
    TELA_DIALOGO,

    // FLUXO DE EVIDENCIAS E DESAFIOS
    TELA_NOVA_EVIDENCIA,
    TELA_MINIGAME,
    TELA_CONFIRMA_CAIO,
    TELA_EVIDENCIA_PERDIDA,
    TELA_EVIDENCIA_DESBLOQUEADA,
    TELA_ANALISE_PROVA,
    TELA_C6_HORAS_DEPOIS,

    // DECISAO FINAL E DOSSIE
    TELA_VEREDITO,
    TELA_CONFIRMA_VEREDITO,
    TELA_FINAL,
    TELA_DOSSIE
} TelaAtual;

typedef enum {
    SEQ_CENA2 = 0,
    SEQ_ACUSA_CAIO,
    SEQ_CENA3,
    SEQ_CENA4,
    SEQ_CENA5,
    SEQ_CENA6A,
    SEQ_CENA6B,
    SEQ_CENA7,
    SEQ_CENA8
} SequenciaDialogo;

typedef struct {
    TelaAtual tela;
    TelaAtual telaAnterior;
    SequenciaDialogo seq;
    int falaIdx;
    int cenaAtual; // 2..7

    // Efeito typing rapido
    float charsVisiveis;
    bool falaCompleta;

    // Progresso das provas (index 2..7): 0=bloqueada, 1=desbloqueada, -1=perdida
    int statusProva[8];
    int respostaProva[8]; // 0=nao respondida, 1=verdadeira, 2=manipulada

    // Notas dinamicas do Dossie conforme as conversas avancam
    bool notaCaioCena2;
    bool notaCaioCena4;
    bool notaJulianaCena5;
    bool notaGabrielCena6;

    // Audio da Cena 3
    bool audioTocando;
    float audioTempo;

    // Veredito
    int acusadoFinal; // 1 = Caio, 2 = Juliana
    bool vitoria;

    // Configuracoes
    float volMusica;
    float volSfx;
    bool telaCheia;
    bool menuPausa;
    bool configVeioDePausa;

    // Dossie
    int dossieAba; // 0 = Evidencias, 1 = Suspeitos
    int dossieItem;

    // Hovers suaves
    float hoverMenu[4];
    float hoverConfig[5];
    float hoverPausa[4];
    float hoverVeredito[3];
    float hoverSimNao[2];
    float hoverAnalise[3];
    float hoverBtnUnico;
    float hoverDossieTop[3];

    // Animacoes de transicao e personagem
    TelaAtual telaUltima;
    float transicaoT;
    char ultimoPersonagem[128];
    float charAnimT;

    // Janela
    int janelaW;
    int janelaH;

    // Modo Dev discreto (senha: RLL)
    bool devDesbloqueado;
    bool devPopupSenha;
    bool devPainelAberto;
    int devCliquesSecretos;
    char devSenhaInput[8];
    int devSenhaLen;
    bool devSenhaErro;

    // Auto-dialogo e Historico de conversa
    bool autoDialogo;
    float autoTimer;
    bool historicoAberto;
    float historicoScroll;
    bool seqVisitada[9];
} EstadoJogo;

static void ObterSequencia(SequenciaDialogo seq, const Fala **outFalas, int *outTotal) {
    switch (seq) {
        case SEQ_CENA2:      *outFalas = falasCena2;     *outTotal = NUM_FALAS_CENA2; break;
        case SEQ_ACUSA_CAIO: *outFalas = falasAcusaCaio; *outTotal = NUM_FALAS_ACUSA_CAIO; break;
        case SEQ_CENA3:      *outFalas = falasCena3;     *outTotal = NUM_FALAS_CENA3; break;
        case SEQ_CENA4:      *outFalas = falasCena4;     *outTotal = NUM_FALAS_CENA4; break;
        case SEQ_CENA5:      *outFalas = falasCena5;     *outTotal = NUM_FALAS_CENA5; break;
        case SEQ_CENA6A:     *outFalas = falasCena6A;    *outTotal = NUM_FALAS_CENA6A; break;
        case SEQ_CENA6B:     *outFalas = falasCena6B;    *outTotal = NUM_FALAS_CENA6B; break;
        case SEQ_CENA7:      *outFalas = falasCena7;     *outTotal = NUM_FALAS_CENA7; break;
        case SEQ_CENA8:      *outFalas = falasCena8;     *outTotal = NUM_FALAS_CENA8; break;
    }
}

static void IniciarSequencia(EstadoJogo *e, SequenciaDialogo seq, int cenaNum) {
    e->tela = TELA_DIALOGO;
    e->seq = seq;
    e->cenaAtual = cenaNum;
    e->falaIdx = 0;
    e->charsVisiveis = 0.0f;
    e->falaCompleta = false;
    e->autoTimer = 0.0f;
    e->historicoAberto = false;
    if ((int)seq >= 0 && (int)seq < 9) e->seqVisitada[(int)seq] = true;
}

static void NovoJogo(EstadoJogo *e, EstadoCelular *cel, PlayerStats *stats) {
    float vm = e->volMusica, vs = e->volSfx;
    bool tc = e->telaCheia;
    bool dev = e->devDesbloqueado;
    int jw = e->janelaW, jh = e->janelaH;
    memset(e, 0, sizeof(*e));
    e->volMusica = vm;
    e->volSfx = vs;
    e->telaCheia = tc;
    e->devDesbloqueado = dev;
    e->janelaW = jw;
    e->janelaH = jh;
    e->transicaoT = 1.0f;
    e->charAnimT = 1.0f;
    e->tela = TELA_C1_OCORRENCIA;
    e->telaUltima = TELA_C1_OCORRENCIA;
    e->cenaAtual = 1;
    IniciarCelular(cel);
    ReiniciarProgresso(stats);
}

static void AlternarTelaCheia(EstadoJogo *e) {
    e->telaCheia = !e->telaCheia;
    if (e->telaCheia) {
        e->janelaW = GetScreenWidth();
        e->janelaH = GetScreenHeight();
    }
    ToggleBorderlessWindowed();
    if (!e->telaCheia && e->janelaW > 0 && e->janelaH > 0) {
        SetWindowSize(e->janelaW, e->janelaH);
    }
}

static void TocarClick(const GerenciadorAssets *g) {
    if (g->temSfxClick) PlaySound(g->sfxClick);
}

static void TocarBrilho(const GerenciadorAssets *g) {
    if (g->temSfxBrilho) PlaySound(g->sfxBrilho);
}

static Color CorDoFalante(const char *falante) {
    if (strcmp(falante, "VOCÊ") == 0 || strcmp(falante, "VOCE") == 0) return COR_CIANO_NEON;
    if (strcmp(falante, "CAIO") == 0) return (Color){ 190, 130, 255, 255 };
    if (strcmp(falante, "JULIANA") == 0) return COR_ROSA_NEON;
    return COR_AMARELO_AVISO;
}

static void DesenharBarraTopo(FontesUI *f, Vector2 mouse, double tempo, float hoverDos, int cenaAtual) {
    // Badge HUD esquerda
    if (cenaAtual >= 1) {
        DesenharBadgeCyber(f->bold, (Vector2){ 22, 14 }, TextFormat("AI.LIBI // CASO 0%d", cenaAtual), 20, COR_ROXO_NEON);
    }
    Rectangle btnDos  = { 1008, 14, 190, 40 };
    Rectangle btnMenu = { 1210, 14, 48, 40 };
    DesenharBotaoCyber(f->bold, btnDos, "DOSSIE [D]", 22, hoverDos, tempo, COR_CIANO_NEON);
    DrawRectangleRec(btnMenu, (Color){ 20, 12, 38, 220 });
    DrawRectangleLinesEx(btnMenu, 1.5f, CheckCollisionPointRec(mouse, btnMenu) ? COR_ROSA_NEON : ColorAlpha(COR_ROXO_NEON, 0.7f));
    DesenharIconeMenu(btnMenu, mouse);
}

static void DesenharCardCentral(FontesUI *f, const char *titulo, const char *subtitulo, const char *textoBtn, float hoverBtn, double tempo, Color corDestaque) {
    DesenharFundoCyberGrid(tempo, corDestaque);
    Rectangle card = { 180, 118, 920, 484 };
    DesenharPainelCyber(card, corDestaque);

    Vector2 szTit = MeasureTextEx(f->titulo, titulo, 42, 1.0f);
    DrawTextEx(f->titulo, titulo, (Vector2){ card.x + (card.width - szTit.x) * 0.5f + 2, card.y + 72 }, 42, 1.0f, ColorAlpha(corDestaque, 0.35f));
    DrawTextEx(f->titulo, titulo, (Vector2){ card.x + (card.width - szTit.x) * 0.5f, card.y + 70 }, 42, 1.0f, WHITE);
    DrawRectangle((int)(card.x + 120), (int)(card.y + 132), (int)(card.width - 240), 2, ColorAlpha(corDestaque, 0.45f));

    if (subtitulo && subtitulo[0] != '\0') {
        DesenharTextoQuebrado(f->regular, subtitulo, (Rectangle){ card.x + 80, card.y + 168, card.width - 160, 170 }, 28, 1.0f, LIGHTGRAY);
    }
    if (textoBtn && textoBtn[0] != '\0') {
        Rectangle btn = { 455, 455, 370, 54 };
        DesenharBotaoCyber(f->bold, btn, textoBtn, 26, hoverBtn, tempo, corDestaque);
    }
}

static void DesenharMapaCena6(FontesUI *f, Rectangle area, double tempo) {
    DesenharPainelCyber(area, COR_CIANO_NEON);
    for (int x = (int)area.x + 40; x < (int)(area.x + area.width); x += 45) {
        DrawLine(x, (int)area.y + 16, x, (int)(area.y + area.height - 16), (Color){ 30, 65, 95, 120 });
    }
    for (int y = (int)area.y + 40; y < (int)(area.y + area.height); y += 45) {
        DrawLine((int)area.x + 16, y, (int)(area.x + area.width - 16), y, (Color){ 30, 65, 95, 120 });
    }
    Vector2 p1 = { area.x + 45, area.y + area.height - 45 };
    Vector2 p2 = { area.x + area.width * 0.52f, area.y + area.height * 0.48f };
    Vector2 p3 = { area.x + area.width - 45, area.y + 50 };
    DrawLineEx(p1, p2, 6.0f, (Color){ 75, 95, 130, 255 });
    DrawLineEx(p2, p3, 6.0f, (Color){ 75, 95, 130, 255 });

    Vector2 pinCel = { area.x + area.width * 0.42f, area.y + area.height * 0.42f };
    float radar = fmodf((float)tempo * 28.0f, 38.0f);
    DrawCircleLines((int)pinCel.x, (int)pinCel.y, 10.0f + radar, ColorAlpha(COR_ROSA_NEON, 1.0f - radar / 38.0f));
    DrawCircleLines((int)pinCel.x, (int)pinCel.y, 28, ColorAlpha(COR_ROSA_NEON, 0.65f));
    DrawCircle((int)pinCel.x, (int)pinCel.y, 9, COR_ROSA_NEON);
    DrawRectangleRounded((Rectangle){ pinCel.x + 16, pinCel.y - 42, 238, 60 }, 0.2f, 6, COR_PAINEL);
    DrawRectangleRoundedLinesEx((Rectangle){ pinCel.x + 16, pinCel.y - 42, 238, 60 }, 0.2f, 6, 1.5f, COR_ROSA_NEON);
    DrawTextEx(f->bold, "CELULAR GABRIEL", (Vector2){ pinCel.x + 26, pinCel.y - 36 }, 21, 1.0f, COR_ROSA_NEON);
    DrawTextEx(f->regular, "03:20 (Aproximado)", (Vector2){ pinCel.x + 26, pinCel.y - 12 }, 20, 1.0f, WHITE);

    Vector2 pinCorpo = { area.x + area.width * 0.50f, area.y + area.height * 0.64f };
    DrawCircle((int)pinCorpo.x, (int)pinCorpo.y, 8, COR_AMARELO_AVISO);
    DrawRectangleRounded((Rectangle){ pinCorpo.x + 16, pinCorpo.y - 12, 245, 58 }, 0.2f, 6, COR_PAINEL);
    DrawRectangleRoundedLinesEx((Rectangle){ pinCorpo.x + 16, pinCorpo.y - 12, 245, 58 }, 0.2f, 6, 1.5f, COR_AMARELO_AVISO);
    DrawTextEx(f->bold, "CORPO ENCONTRADO", (Vector2){ pinCorpo.x + 26, pinCorpo.y - 6 }, 21, 1.0f, COR_AMARELO_AVISO);
    DrawTextEx(f->regular, "Margem da estrada", (Vector2){ pinCorpo.x + 26, pinCorpo.y + 18 }, 20, 1.0f, WHITE);
}

static void DesenharPlayerAudio(FontesUI *f, Rectangle area, bool tocando, float tempo, Vector2 mouse) {
    DesenharPainelCyber(area, COR_ROXO_NEON);

    int barras = 44;
    float bw = (area.width - 80) / (float)barras;
    for (int i = 0; i < barras; i++) {
        float phase = tocando ? (tempo * 7.5f + i * 0.55f) : (i * 0.45f);
        float amp = 14.0f + fabsf(sinf(phase)) * 42.0f * (tocando ? 1.0f : 0.45f);
        Rectangle b = { area.x + 40 + i * bw, area.y + 66 - amp * 0.5f, bw - 4, amp };
        DrawRectangleRounded(b, 0.5f, 4, (i % 3 == 0) ? COR_ROSA_NEON : COR_CIANO_NEON);
    }

    const char *transcricao =
        "\"Eu avisei pra voce ficar calado... [inaudivel]\n"
        "Agora vai se arrepender. Acha que pode falar e sair daqui como se nada tivesse acontecido?\n"
        "[inaudivel] Voce nao vai a lugar nenhum.\"";
    DesenharTextoQuebrado(f->regular, transcricao, (Rectangle){ area.x + 30, area.y + 118, area.width - 60, 130 }, 24, 1.0f, WHITE);

    Rectangle btnPlay = { area.x + 30, area.y + area.height - 62, 155, 42 };
    bool hoverPlay = CheckCollisionPointRec(mouse, btnPlay);
    DesenharBotaoCyber(f->bold, btnPlay, tocando ? "|| PAUSAR" : "> OUVIR", 22, hoverPlay ? 1.0f : 0.0f, tempo, tocando ? COR_ROSA_NEON : COR_CIANO_NEON);
    DrawTextEx(f->regular, "Duracao: 00:18 | Entrega Anonima", (Vector2){ area.x + 198, area.y + area.height - 52 }, 22, 1.0f, LIGHTGRAY);
}

static void AtualizarDevOverlay(EstadoJogo *e, EstadoCelular *cel, PlayerStats *stats, const GerenciadorAssets *assets, Vector2 mouse) {
    if (e->devPopupSenha) {
        int key = GetCharPressed();
        while (key > 0) {
            if (((key >= 'a' && key <= 'z') || (key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9')) && e->devSenhaLen < 6) {
                if (key >= 'a' && key <= 'z') key -= 32;
                e->devSenhaInput[e->devSenhaLen++] = (char)key;
                e->devSenhaInput[e->devSenhaLen] = '\0';
                e->devSenhaErro = false;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && e->devSenhaLen > 0) {
            e->devSenhaInput[--e->devSenhaLen] = '\0';
            e->devSenhaErro = false;
        }
        Rectangle btnOk = { 490, 412, 140, 44 };
        Rectangle btnCanc = { 650, 412, 140, 44 };
        bool confirmar = IsKeyPressed(KEY_ENTER) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnOk));
        bool cancelar = IsKeyPressed(KEY_ESCAPE) || (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnCanc));
        if (confirmar) {
            TocarClick(assets);
            if (strcmp(e->devSenhaInput, "RLL") == 0) {
                e->devDesbloqueado = true;
                e->devPopupSenha = false;
                e->devPainelAberto = true;
                e->devSenhaInput[0] = '\0';
                e->devSenhaLen = 0;
            } else {
                e->devSenhaErro = true;
            }
        } else if (cancelar) {
            TocarClick(assets);
            e->devPopupSenha = false;
            e->devSenhaInput[0] = '\0';
            e->devSenhaLen = 0;
            e->devSenhaErro = false;
        }
        return;
    }

    if (e->devPainelAberto) {
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_F9)) {
            e->devPainelAberto = false;
            return;
        }
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

        // Botao fechar
        if (CheckCollisionPointRec(mouse, (Rectangle){ 980, 90, 140, 40 })) {
            TocarClick(assets);
            e->devPainelAberto = false;
            return;
        }

        // Coluna 1: Pular/Voltar Cenas (12 botoes em grid 2x6)
        for (int i = 0; i < 12; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle b = { 160 + col * 225, 195 + row * 56, 210, 44 };
            if (CheckCollisionPointRec(mouse, b)) {
                TocarClick(assets);
                e->menuPausa = false;
                e->devPainelAberto = false;
                e->audioTocando = false;
                switch (i) {
                    case 0: e->tela = TELA_C1_OCORRENCIA; e->cenaAtual = 1; break;
                    case 1: e->tela = TELA_C1_CELULAR; e->cenaAtual = 1; break;
                    case 2: IniciarSequencia(e, SEQ_CENA2, 2); break;
                    case 3: IniciarSequencia(e, SEQ_CENA3, 3); break;
                    case 4: IniciarSequencia(e, SEQ_CENA4, 4); break;
                    case 5: IniciarSequencia(e, SEQ_CENA5, 5); break;
                    case 6: IniciarSequencia(e, SEQ_CENA6A, 6); break;
                    case 7: IniciarSequencia(e, SEQ_CENA6B, 6); break;
                    case 8: IniciarSequencia(e, SEQ_CENA7, 7); break;
                    case 9: IniciarSequencia(e, SEQ_CENA8, 8); break;
                    case 10: e->tela = TELA_VEREDITO; e->cenaAtual = 8; break;
                    case 11: e->telaAnterior = (e->tela == TELA_DOSSIE) ? TELA_MENU : e->tela; e->tela = TELA_DOSSIE; break;
                }
                return;
            }
        }

        // Coluna 2: Evidencias, Run e Efeitos Visuais
        for (int i = 0; i < 8; i++) {
            Rectangle b = { 650, 195 + i * 54, 450, 44 };
            if (CheckCollisionPointRec(mouse, b)) {
                TocarClick(assets);
                switch (i) {
                    case 0:
                        for (int k = 2; k <= 7; k++) e->statusProva[k] = 1;
                        e->notaCaioCena2 = e->notaCaioCena4 = e->notaJulianaCena5 = e->notaGabrielCena6 = true;
                        break;
                    case 1:
                        for (int k = 2; k <= 7; k++) { e->statusProva[k] = 0; e->respostaProva[k] = 0; }
                        e->notaCaioCena2 = e->notaCaioCena4 = e->notaJulianaCena5 = e->notaGabrielCena6 = false;
                        break;
                    case 2:
                        if (e->cenaAtual < 2 || e->cenaAtual > 7) e->cenaAtual = 2;
                        e->statusProva[e->cenaAtual] = 1;
                        e->devPainelAberto = false;
                        e->tela = TELA_ANALISE_PROVA;
                        break;
                    case 3:
                        e->devPainelAberto = false;
                        NovoJogo(e, cel, stats);
                        break;
                    case 4:
                        ReiniciarProgresso(stats);
                        break;
                    case 5: gEfeitoScanlines = !gEfeitoScanlines; break;
                    case 6: gEfeitoGrid = !gEfeitoGrid; break;
                    case 7: gEfeitoGlitch = !gEfeitoGlitch; break;
                }
                return;
            }
        }
    }
}

static void DesenharDevOverlay(const EstadoJogo *e, FontesUI *f, Vector2 mouse, double tempo) {
    if (e->devPopupSenha) {
        DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, 215 });
        DesenharPainelCyber((Rectangle){ 400, 210, 480, 280 }, COR_CIANO_NEON);
        DrawTextEx(f->titulo, "ACESSO RESTRITO // DEV", (Vector2){ 465, 240 }, 26, 1.0f, COR_CIANO_NEON);
        DrawTextEx(f->regular, "Digite a senha de desenvolvedor:", (Vector2){ 465, 286 }, 24, 1.0f, LIGHTGRAY);

        Rectangle box = { 465, 330, 350, 50 };
        DrawRectangleRec(box, (Color){ 14, 10, 28, 255 });
        DrawRectangleLinesEx(box, 2.0f, e->devSenhaErro ? COR_VERMELHO_ERR : COR_ROSA_NEON);

        char mask[8] = "";
        for (int i = 0; i < e->devSenhaLen && i < 6; i++) mask[i] = '*';
        if ((int)(tempo * 4.0) % 2 == 0) {
            mask[e->devSenhaLen] = '_';
            mask[e->devSenhaLen + 1] = '\0';
        }
        DrawTextEx(f->titulo, mask, (Vector2){ box.x + 18, box.y + 11 }, 28, 3.0f, WHITE);

        if (e->devSenhaErro) {
            DrawTextEx(f->bold, "SENHA INVALIDA!", (Vector2){ 465, 384 }, 19, 1.0f, COR_VERMELHO_ERR);
        }

        Rectangle btnOk = { 490, 412, 140, 44 };
        Rectangle btnCanc = { 650, 412, 140, 44 };
        DesenharBotaoCyber(f->bold, btnOk, "ENTRAR", 22, CheckCollisionPointRec(mouse, btnOk) ? 1.0f : 0.0f, tempo, COR_VERDE_OK);
        DesenharBotaoCyber(f->bold, btnCanc, "CANCELAR", 22, CheckCollisionPointRec(mouse, btnCanc) ? 1.0f : 0.0f, tempo, COR_ROSA_NEON);
    }

    if (e->devPainelAberto) {
        DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, 225 });
        DesenharPainelCyber((Rectangle){ 120, 68, 1040, 584 }, COR_VERDE_OK);
        DrawTextEx(f->titulo, "SYS.DEV // CONSOLE DE TESTES", (Vector2){ 160, 94 }, 28, 1.0f, COR_VERDE_OK);
        DesenharBotaoCyber(f->bold, (Rectangle){ 980, 90, 140, 40 }, "FECHAR [F9]", 20, CheckCollisionPointRec(mouse, (Rectangle){ 980, 90, 140, 40 }) ? 1.0f : 0.0f, tempo, COR_ROSA_NEON);
        DrawRectangle(160, 140, 960, 2, ColorAlpha(COR_VERDE_OK, 0.45f));

        DrawTextEx(f->bold, "PULAR / VOLTAR PARA CENA:", (Vector2){ 160, 158 }, 22, 1.0f, COR_CIANO_NEON);
        const char *cenasTxt[12] = {
            "C1: Ocorrencia", "C1: Celular", "C2: Caio", "C3: Audio",
            "C4: Mensagens", "C5: Juliana", "C6A: Mapa/Corpo", "C6B: Horas Depois",
            "C7: Foto Festa", "C8: Juiz", "Veredito", "Abrir Dossie"
        };
        for (int i = 0; i < 12; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle b = { 160 + col * 225, 195 + row * 56, 210, 44 };
            DesenharBotaoCyber(f->bold, b, cenasTxt[i], 20, CheckCollisionPointRec(mouse, b) ? 1.0f : 0.0f, tempo, COR_CIANO_NEON);
        }

        DrawTextEx(f->bold, "EVIDENCIAS, RUN & EFEITOS:", (Vector2){ 650, 158 }, 22, 1.0f, COR_ROSA_NEON);
        const char *acoesTxt[8] = {
            "DESBLOQUEAR TODAS EVIDENCIAS (+NOTAS)",
            "RESETAR EVIDENCIAS & RESPOSTAS",
            "ANALISAR EVIDENCIA DA CENA ATUAL",
            "REINICIAR RUN (NOVO JOGO)",
            "ZERAR PROGRESSO SALVO (ACERTOS/ERROS)",
            gEfeitoScanlines ? "EFEITO SCANLINES HUD: [ON]" : "EFEITO SCANLINES HUD: [OFF]",
            gEfeitoGrid      ? "EFEITO GRID & PARTICULAS: [ON]" : "EFEITO GRID & PARTICULAS: [OFF]",
            gEfeitoGlitch    ? "EFEITO GLITCH NA LOGO: [ON]" : "EFEITO GLITCH NA LOGO: [OFF]"
        };
        for (int i = 0; i < 8; i++) {
            Rectangle b = { 650, 195 + i * 54, 450, 44 };
            Color cor = (i < 3) ? COR_ROXO_NEON : (i < 5) ? COR_AMARELO_AVISO : COR_VERDE_OK;
            if (i == 5 && !gEfeitoScanlines) cor = GRAY;
            if (i == 6 && !gEfeitoGrid) cor = GRAY;
            if (i == 7 && !gEfeitoGlitch) cor = GRAY;
            DesenharBotaoCyber(f->bold, b, acoesTxt[i], 19, CheckCollisionPointRec(mouse, b) ? 1.0f : 0.0f, tempo, cor);
        }
    }
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    int initW = 1440;
    int initH = 810;
    InitWindow(initW, initH, "AI.libi - Investigacao & Logica Proposicional");
    int mon = GetCurrentMonitor();
    int monW = GetMonitorWidth(mon);
    int monH = GetMonitorHeight(mon);
    if (monW > 0 && monH > 0 && (initW > monW - 60 || initH > monH - 80)) {
        float esc = fminf((float)(monW - 60) / (float)VIRTUAL_W, (float)(monH - 80) / (float)VIRTUAL_H);
        initW = (int)(VIRTUAL_W * esc);
        initH = (int)(VIRTUAL_H * esc);
        SetWindowSize(initW, initH);
    }
    SetExitKey(0);
    SetTargetFPS(60);
    InitAudioDevice();

    RenderTexture2D canvas = LoadRenderTexture(RENDER_W, RENDER_H);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);
    Camera2D cam = { 0 };
    cam.zoom = (float)RENDER_W / (float)VIRTUAL_W;

    GerenciadorAssets assets;
    IniciarGerenciadorAssets(&assets);

    EstadoJogo e = {0};
    e.tela = TELA_MENU;
    e.telaUltima = TELA_MENU;
    e.transicaoT = 1.0f;
    e.charAnimT = 1.0f;
    e.volMusica = 0.45f;
    e.volSfx = 0.80f;

    EstadoMiniGame miniGame;
    IniciarMiniGame(&miniGame);

    EstadoCelular celular;
    IniciarCelular(&celular);

    PlayerStats stats = {0};
    strcpy(stats.evidenciaPerdida, "Nenhuma");
    CarregarProgresso(&stats);

    if (assets.temMusicaMenu) PlayMusicStream(assets.musicaMenu);
    if (assets.temMusicaJogo) PlayMusicStream(assets.musicaJogo);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;
        double tempo = GetTime();

        bool naAreaMenu = (e.tela == TELA_MENU || e.tela == TELA_CONFIG || e.tela == TELA_CREDITOS);
        if (assets.temMusicaMenu) {
            SetMusicVolume(assets.musicaMenu, naAreaMenu ? e.volMusica : 0.0f);
            UpdateMusicStream(assets.musicaMenu);
        }
        if (assets.temMusicaJogo) {
            SetMusicVolume(assets.musicaJogo, !naAreaMenu ? e.volMusica : 0.0f);
            UpdateMusicStream(assets.musicaJogo);
        }
        if (assets.temSfxClick) SetSoundVolume(assets.sfxClick, e.volSfx);
        if (assets.temSfxBrilho) SetSoundVolume(assets.sfxBrilho, e.volSfx);

        if (IsKeyPressed(KEY_F11)) AlternarTelaCheia(&e);

        Rectangle destTela = CalcularDestinoTela(GetScreenWidth(), GetScreenHeight());
        Vector2 mouse = ObterMouseVirtual(destTela);

        if (e.audioTocando) e.audioTempo += dt;

        // Animacoes globais de transicao e entrada de personagem
        if (e.tela != e.telaUltima) {
            e.transicaoT = 0.0f;
            e.telaUltima = e.tela;
        }
        if (e.transicaoT < 1.0f) {
            e.transicaoT += dt * 4.5f;
            if (e.transicaoT > 1.0f) e.transicaoT = 1.0f;
        }
        if (e.charAnimT < 1.0f) {
            e.charAnimT += dt * 4.2f;
            if (e.charAnimT > 1.0f) e.charAnimT = 1.0f;
        }

        bool emGameplay = !naAreaMenu && e.tela != TELA_DOSSIE;
        if (emGameplay) {
            if (IsKeyPressed(KEY_ESCAPE)) e.menuPausa = !e.menuPausa;
            if (IsKeyPressed(KEY_D) && !e.menuPausa) {
                e.telaAnterior = e.tela;
                e.tela = TELA_DOSSIE;
            }
        }

        if (e.devDesbloqueado && IsKeyPressed(KEY_F9)) {
            e.devPainelAberto = !e.devPainelAberto;
        }

        // =================== ATUALIZACAO ===================
        if (e.devPopupSenha || e.devPainelAberto) {
            AtualizarDevOverlay(&e, &celular, &stats, &assets, mouse);
        } else if (e.menuPausa) {
            Rectangle pausaBtns[4] = {
                { 465, 252, 350, 48 },
                { 465, 310, 350, 48 },
                { 465, 368, 350, 48 },
                { 465, 426, 350, 48 }
            };
            AtualizarHover(pausaBtns, e.hoverPausa, 4, mouse, dt);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, pausaBtns[0])) { TocarClick(&assets); e.menuPausa = false; }
                else if (CheckCollisionPointRec(mouse, pausaBtns[1])) { TocarClick(&assets); e.menuPausa = false; e.tela = TELA_MENU; }
                else if (CheckCollisionPointRec(mouse, pausaBtns[2])) {
                    TocarClick(&assets);
                    e.menuPausa = false;
                    e.configVeioDePausa = true;
                    e.telaAnterior = e.tela;
                    e.tela = TELA_CONFIG;
                }
                else if (CheckCollisionPointRec(mouse, pausaBtns[3])) { CloseWindow(); }
            }
        } else {
            Rectangle btnDosTop  = { 1008, 14, 190, 40 };
            Rectangle btnMenuTop = { 1210, 14, 48, 40 };
            AtualizarHover(&btnDosTop, &e.hoverDossieTop[0], 1, mouse, dt);
            bool clicouBarra = false;

            if (emGameplay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, btnMenuTop)) {
                    TocarClick(&assets);
                    e.menuPausa = true;
                    clicouBarra = true;
                } else if (CheckCollisionPointRec(mouse, btnDosTop)) {
                    TocarClick(&assets);
                    e.telaAnterior = e.tela;
                    e.tela = TELA_DOSSIE;
                    clicouBarra = true;
                }
            }

            if (!clicouBarra) switch (e.tela) {
                case TELA_MENU: {
                    Rectangle menuBtns[4] = {
                        { 95, 325, 320, 58 },
                        { 95, 392, 320, 58 },
                        { 95, 459, 320, 58 },
                        { 95, 526, 320, 58 }
                    };
                    AtualizarHover(menuBtns, e.hoverMenu, 4, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, menuBtns[0])) { TocarClick(&assets); NovoJogo(&e, &celular, &stats); }
                        else if (CheckCollisionPointRec(mouse, menuBtns[1])) { TocarClick(&assets); e.configVeioDePausa = false; e.telaAnterior = TELA_MENU; e.tela = TELA_CONFIG; }
                        else if (CheckCollisionPointRec(mouse, menuBtns[2])) { TocarClick(&assets); e.tela = TELA_CREDITOS; }
                        else if (CheckCollisionPointRec(mouse, menuBtns[3])) { CloseWindow(); }
                    }
                } break;

                case TELA_CONFIG: {
                    Rectangle rowMusica  = { 140, 230, 740, 54 };
                    Rectangle rowEfeitos = { 140, 305, 740, 54 };
                    Rectangle rowTela    = { 140, 380, 480, 54 };
                    Rectangle btnVoltar  = { 140, 470, 280, 54 };
                    Rectangle cfgRows[4] = { rowMusica, rowEfeitos, rowTela, btnVoltar };
                    AtualizarHover(cfgRows, e.hoverConfig, 4, mouse, dt);

                    Rectangle btnMusMenos = { 495, 238, 36, 38 };
                    Rectangle sliderMus   = { 540, 243, 220, 28 };
                    Rectangle btnMusMais  = { 770, 238, 36, 38 };

                    Rectangle btnSfxMenos = { 495, 313, 36, 38 };
                    Rectangle sliderSfx   = { 540, 318, 220, 28 };
                    Rectangle btnSfxMais  = { 770, 313, 36, 38 };

                    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, (Rectangle){ sliderMus.x - 4, sliderMus.y - 6, sliderMus.width + 8, 40 })) {
                            float raw = (mouse.x - sliderMus.x) / sliderMus.width;
                            e.volMusica = roundf(fmaxf(0.0f, fminf(1.0f, raw)) * 20.0f) / 20.0f;
                        }
                        if (CheckCollisionPointRec(mouse, (Rectangle){ sliderSfx.x - 4, sliderSfx.y - 6, sliderSfx.width + 8, 40 })) {
                            float raw = (mouse.x - sliderSfx.x) / sliderSfx.width;
                            e.volSfx = roundf(fmaxf(0.0f, fminf(1.0f, raw)) * 20.0f) / 20.0f;
                        }
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btnMusMenos)) {
                            e.volMusica = fmaxf(0.0f, roundf((e.volMusica - 0.05f) * 20.0f) / 20.0f);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, btnMusMais)) {
                            e.volMusica = fminf(1.0f, roundf((e.volMusica + 0.05f) * 20.0f) / 20.0f);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, btnSfxMenos)) {
                            e.volSfx = fmaxf(0.0f, roundf((e.volSfx - 0.05f) * 20.0f) / 20.0f);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, btnSfxMais)) {
                            e.volSfx = fminf(1.0f, roundf((e.volSfx + 0.05f) * 20.0f) / 20.0f);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, rowTela)) {
                            TocarClick(&assets);
                            AlternarTelaCheia(&e);
                        } else if (CheckCollisionPointRec(mouse, btnVoltar)) {
                            TocarClick(&assets);
                            e.devCliquesSecretos = 0;
                            e.tela = e.telaAnterior;
                            if (e.configVeioDePausa) e.menuPausa = true;
                        } else if (CheckCollisionPointRec(mouse, (Rectangle){ 832, 526, 68, 28 })) {
                            e.devCliquesSecretos++;
                            if (e.devDesbloqueado) {
                                TocarClick(&assets);
                                e.devPainelAberto = true;
                            } else if (e.devCliquesSecretos >= 3) {
                                TocarClick(&assets);
                                e.devCliquesSecretos = 0;
                                e.devPopupSenha = true;
                                e.devSenhaInput[0] = '\0';
                                e.devSenhaLen = 0;
                                e.devSenhaErro = false;
                            }
                        }
                    }
                    if (IsKeyPressed(KEY_ESCAPE)) {
                        e.tela = e.telaAnterior;
                        if (e.configVeioDePausa) e.menuPausa = true;
                    }
                } break;

                case TELA_CREDITOS: {
                    Rectangle btnVoltar = { 490, 555, 300, 50 };
                    AtualizarHover(&btnVoltar, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnVoltar)) || IsKeyPressed(KEY_ESCAPE)) {
                        TocarClick(&assets);
                        e.tela = TELA_MENU;
                    }
                } break;

                case TELA_C1_OCORRENCIA: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        TocarClick(&assets);
                        e.tela = TELA_C1_GABRIEL;
                    }
                } break;

                case TELA_C1_GABRIEL:
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        TocarClick(&assets);
                        e.tela = TELA_C1_CAIO;
                    }
                    break;

                case TELA_C1_CAIO:
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        TocarClick(&assets);
                        e.tela = TELA_C1_CELULAR;
                    }
                    break;

                case TELA_C1_CELULAR:
                    if (AtualizarCelular(&celular, mouse, dt, assets.sfxClick, assets.temSfxClick) == 1) {
                        e.tela = TELA_C1_INTERROGATORIO;
                    }
                    break;

                case TELA_C1_INTERROGATORIO: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        TocarClick(&assets);
                        IniciarSequencia(&e, SEQ_CENA2, 2);
                    }
                } break;

                case TELA_DIALOGO: {
                    Rectangle btnHist = { 60 + 1160 - 445.0f, 446 + 18.0f, 118.0f, 30.0f };
                    Rectangle btnAuto = { 60 + 1160 - 315.0f, 446 + 18.0f, 130.0f, 30.0f };

                    if (e.historicoAberto) {
                        float wheel = GetMouseWheelMove();
                        if (wheel != 0.0f) e.historicoScroll -= wheel * 42.0f;
                        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) e.historicoScroll += 320.0f * dt;
                        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) e.historicoScroll -= 320.0f * dt;
                        if (e.historicoScroll < 0.0f) e.historicoScroll = 0.0f;
                        Rectangle btnFecharHist = { 980, 88, 150, 40 };
                        if (IsKeyPressed(KEY_H) || IsKeyPressed(KEY_ESCAPE) ||
                            (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnFecharHist))) {
                            TocarClick(&assets);
                            e.historicoAberto = false;
                        }
                        break;
                    }

                    if (IsKeyPressed(KEY_H)) {
                        TocarClick(&assets);
                        e.historicoAberto = true;
                        e.historicoScroll = 99999.0f;
                        break;
                    }

                    bool clicouBotaoCaixa = false;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btnHist)) {
                            TocarClick(&assets);
                            e.historicoAberto = true;
                            e.historicoScroll = 99999.0f;
                            clicouBotaoCaixa = true;
                        } else if (CheckCollisionPointRec(mouse, btnAuto)) {
                            TocarClick(&assets);
                            e.autoDialogo = !e.autoDialogo;
                            e.autoTimer = 0.0f;
                            clicouBotaoCaixa = true;
                        }
                    }

                    const Fala *falas = NULL;
                    int total = 0;
                    ObterSequencia(e.seq, &falas, &total);
                    int lenTexto = (int)strlen(falas[e.falaIdx].texto);

                    // Efeito typing rapido e fluido
                    if (!e.falaCompleta) {
                        e.charsVisiveis += 110.0f * dt;
                        if ((int)e.charsVisiveis >= lenTexto) {
                            e.charsVisiveis = (float)lenTexto;
                            e.falaCompleta = true;
                            e.autoTimer = 0.0f;
                        }
                    } else if (e.autoDialogo) {
                        e.autoTimer += dt;
                    }

                    // Atualiza notas dinamicas do dossie conforme o dialogo avanca
                    if (e.seq == SEQ_CENA2 && e.falaIdx >= 5) e.notaCaioCena2 = true;
                    if (e.seq == SEQ_CENA4 && e.falaIdx >= 4) e.notaCaioCena4 = true;
                    if (e.seq == SEQ_CENA5 && e.falaIdx >= 7) e.notaJulianaCena5 = true;
                    if (e.seq == SEQ_CENA6A && e.falaIdx >= 2) e.notaGabrielCena6 = true;

                    bool autoAvancou = (e.autoDialogo && e.falaCompleta && e.autoTimer >= 3.0f);
                    bool clicouAvancar = (!clicouBotaoCaixa && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse.y > 65) ||
                                         IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
                    if (clicouAvancar || autoAvancou) {
                        if (!e.falaCompleta && !autoAvancou) {
                            e.charsVisiveis = (float)lenTexto;
                            e.falaCompleta = true;
                            e.autoTimer = 0.0f;
                        } else {
                            if (!autoAvancou) TocarClick(&assets);
                            e.autoTimer = 0.0f;
                            if (e.falaIdx + 1 < total) {
                                e.falaIdx++;
                                e.charsVisiveis = 0.0f;
                                e.falaCompleta = false;
                            } else {
                                if (e.seq == SEQ_ACUSA_CAIO) {
                                    e.vitoria = false;
                                    e.acusadoFinal = 1;
                                    e.tela = TELA_FINAL;
                                } else if (e.seq == SEQ_CENA6A) {
                                    e.tela = TELA_C6_HORAS_DEPOIS;
                                } else if (e.seq == SEQ_CENA8) {
                                    e.tela = TELA_VEREDITO;
                                } else {
                                    TocarBrilho(&assets);
                                    e.tela = TELA_NOVA_EVIDENCIA;
                                }
                            }
                        }
                    }
                } break;

                case TELA_NOVA_EVIDENCIA: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) {
                        TocarClick(&assets);
                        IniciarMiniGame(&miniGame);
                        e.tela = TELA_MINIGAME;
                    }
                } break;

                case TELA_MINIGAME: {
                    int res = AtualizarMiniGame(&miniGame, e.cenaAtual, mouse, dt, assets.sfxClick, assets.temSfxClick);
                    if (res == 1) {
                        e.statusProva[e.cenaAtual] = 1;
                        stats.acertos++;
                        SalvarProgresso(stats);
                        TocarBrilho(&assets);
                        e.tela = TELA_EVIDENCIA_DESBLOQUEADA;
                    } else if (res == -1) {
                        e.statusProva[e.cenaAtual] = -1;
                        stats.erros++;
                        snprintf(stats.evidenciaPerdida, sizeof(stats.evidenciaPerdida), "Cena %d", e.cenaAtual);
                        SalvarProgresso(stats);
                        e.tela = TELA_EVIDENCIA_PERDIDA;
                    } else if (res == 2) {
                        e.tela = TELA_CONFIRMA_CAIO;
                    }
                } break;

                case TELA_CONFIRMA_CAIO: {
                    Rectangle btns[2] = { { 300, 420, 310, 52 }, { 670, 420, 310, 52 } };
                    AtualizarHover(btns, e.hoverSimNao, 2, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btns[0])) {
                            TocarClick(&assets);
                            IniciarSequencia(&e, SEQ_ACUSA_CAIO, 2);
                        } else if (CheckCollisionPointRec(mouse, btns[1])) {
                            TocarClick(&assets);
                            IniciarMiniGame(&miniGame);
                            e.tela = TELA_MINIGAME;
                        }
                    }
                } break;

                case TELA_EVIDENCIA_PERDIDA: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) {
                        TocarClick(&assets);
                        if (e.cenaAtual == 2) IniciarSequencia(&e, SEQ_CENA3, 3);
                        else if (e.cenaAtual == 3) IniciarSequencia(&e, SEQ_CENA4, 4);
                        else if (e.cenaAtual == 4) IniciarSequencia(&e, SEQ_CENA5, 5);
                        else if (e.cenaAtual == 5) IniciarSequencia(&e, SEQ_CENA6A, 6);
                        else if (e.cenaAtual == 6) IniciarSequencia(&e, SEQ_CENA7, 7);
                        else if (e.cenaAtual == 7) IniciarSequencia(&e, SEQ_CENA8, 8);
                    }
                } break;

                case TELA_EVIDENCIA_DESBLOQUEADA: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) {
                        TocarClick(&assets);
                        e.audioTocando = false;
                        e.audioTempo = 0.0f;
                        e.tela = TELA_ANALISE_PROVA;
                    }
                } break;

                case TELA_ANALISE_PROVA: {
                    if (e.cenaAtual == 3) {
                        Rectangle btnPlay = { 155, 443, 155, 42 };
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnPlay)) {
                            TocarClick(&assets);
                            e.audioTocando = !e.audioTocando;
                        }
                    }
                    Rectangle btns[3] = {
                        { 655, 446, 230, 54 },
                        { 905, 446, 230, 54 },
                        { 655, 542, 480, 54 }
                    };
                    AtualizarHover(btns, e.hoverAnalise, 3, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (e.respostaProva[e.cenaAtual] == 0) {
                            if (CheckCollisionPointRec(mouse, btns[0])) { TocarClick(&assets); e.respostaProva[e.cenaAtual] = 1; }
                            else if (CheckCollisionPointRec(mouse, btns[1])) { TocarClick(&assets); e.respostaProva[e.cenaAtual] = 2; }
                        } else if (CheckCollisionPointRec(mouse, btns[2])) {
                            TocarClick(&assets);
                            e.audioTocando = false;
                            if (e.cenaAtual == 2) IniciarSequencia(&e, SEQ_CENA3, 3);
                            else if (e.cenaAtual == 3) IniciarSequencia(&e, SEQ_CENA4, 4);
                            else if (e.cenaAtual == 4) IniciarSequencia(&e, SEQ_CENA5, 5);
                            else if (e.cenaAtual == 5) IniciarSequencia(&e, SEQ_CENA6A, 6);
                            else if (e.cenaAtual == 6) IniciarSequencia(&e, SEQ_CENA7, 7);
                            else if (e.cenaAtual == 7) IniciarSequencia(&e, SEQ_CENA8, 8);
                        }
                    }
                } break;

                case TELA_C6_HORAS_DEPOIS: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) {
                        TocarClick(&assets);
                        IniciarSequencia(&e, SEQ_CENA6B, 6);
                    }
                } break;

                case TELA_VEREDITO: {
                    Rectangle ops[3] = {
                        { 310, 322, 660, 54 },
                        { 310, 392, 660, 54 },
                        { 310, 462, 660, 54 }
                    };
                    AtualizarHover(ops, e.hoverVeredito, 3, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, ops[0])) { TocarClick(&assets); e.acusadoFinal = 1; e.tela = TELA_CONFIRMA_VEREDITO; }
                        else if (CheckCollisionPointRec(mouse, ops[1])) { TocarClick(&assets); e.acusadoFinal = 2; e.tela = TELA_CONFIRMA_VEREDITO; }
                        else if (CheckCollisionPointRec(mouse, ops[2])) { TocarClick(&assets); e.telaAnterior = TELA_VEREDITO; e.tela = TELA_DOSSIE; }
                    }
                } break;

                case TELA_CONFIRMA_VEREDITO: {
                    Rectangle btns[2] = { { 340, 420, 260, 54 }, { 680, 420, 260, 54 } };
                    AtualizarHover(btns, e.hoverSimNao, 2, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btns[0])) {
                            TocarClick(&assets);
                            e.vitoria = (e.acusadoFinal == 2);
                            e.tela = TELA_FINAL;
                        } else if (CheckCollisionPointRec(mouse, btns[1])) {
                            TocarClick(&assets);
                            e.tela = TELA_VEREDITO;
                        }
                    }
                } break;

                case TELA_FINAL: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) ||
                        IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        TocarClick(&assets);
                        e.tela = TELA_MENU;
                    }
                } break;

                case TELA_DOSSIE: {
                    Rectangle topBtns[3] = {
                        { 95, 86, 200, 42 },
                        { 315, 86, 200, 42 },
                        { 1020, 86, 170, 42 }
                    };
                    AtualizarHover(topBtns, e.hoverDossieTop, 3, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, topBtns[2])) {
                            TocarClick(&assets);
                            e.audioTocando = false;
                            e.tela = e.telaAnterior;
                        } else if (CheckCollisionPointRec(mouse, topBtns[0])) {
                            TocarClick(&assets); e.dossieAba = 0; e.dossieItem = 0;
                        } else if (CheckCollisionPointRec(mouse, topBtns[1])) {
                            TocarClick(&assets); e.dossieAba = 1; e.dossieItem = 0;
                        } else {
                            int maxItens = (e.dossieAba == 0) ? 6 : 3;
                            for (int i = 0; i < maxItens; i++) {
                                Rectangle item = { 95, 154 + i * 74, 320, 62 };
                                if (CheckCollisionPointRec(mouse, item)) {
                                    TocarClick(&assets);
                                    e.dossieItem = i;
                                    e.audioTocando = false;
                                }
                            }
                            if (e.dossieAba == 0 && e.dossieItem == 1 && e.statusProva[3] == 1) {
                                Rectangle btnPlay = { 510, 528, 155, 42 };
                                if (CheckCollisionPointRec(mouse, btnPlay)) {
                                    TocarClick(&assets);
                                    e.audioTocando = !e.audioTocando;
                                }
                            }
                        }
                    }
                    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_D)) {
                        e.audioTocando = false;
                        e.tela = e.telaAnterior;
                    }
                } break;
            }
        }

        // =================== DESENHO NO CANVAS 1920x1080 ===================
        BeginTextureMode(canvas);
        ClearBackground(COR_FUNDO_ESCURO);
        BeginMode2D(cam);

        switch (e.tela) {
            case TELA_MENU: {
                if (assets.menuBg.id != 0) DesenharTexturaCover(assets.menuBg, (Rectangle){0, 0, VIRTUAL_W, VIRTUAL_H}, WHITE);
                DrawRectangleGradientH(0, 0, 560, VIRTUAL_H, (Color){ 12, 6, 22, 180 }, (Color){ 12, 6, 22, 0 });

                Rectangle rectLogo = { 82, 85, 340, 175 };
                if (assets.marcaLogo.id != 0) {
                    DesenharLogoGlitchTech(assets.marcaLogo, rectLogo, tempo);
                } else {
                    DrawTextEx(assets.fontes.teko, "ai.libi", (Vector2){ 95, 115 }, 72, 2.0f, WHITE);
                }

                const char *menuTextos[4] = { "jogar", "configuracoes", "creditos", "sair" };
                for (int i = 0; i < 4; i++) {
                    Rectangle r = { 95, 325 + i * 67, 320, 58 };
                    DesenharBotaoSolto(assets.fontes.teko, r, menuTextos[i], 54, e.hoverMenu[i], tempo, (Color){ 238, 235, 248, 255 });
                }
                DrawTextEx(assets.fontes.bold, "SYS // CYBER-INVESTIGACAO v2.0", (Vector2){ 935, 682 }, 20, 1.0f, ColorAlpha(COR_CIANO_NEON, 0.70f));
                DesenharScanlinesHUD(tempo);
            } break;

            case TELA_CONFIG: {
                if (assets.menuBg.id != 0) DesenharTexturaCover(assets.menuBg, (Rectangle){0, 0, VIRTUAL_W, VIRTUAL_H}, WHITE);
                DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 10, 6, 20, 195 });

                DesenharPainelCyber((Rectangle){ 100, 110, 820, 460 }, COR_ROSA_NEON);
                DrawTextEx(assets.fontes.teko, "configuracoes", (Vector2){ 145, 134 }, 58, 1.5f, WHITE);
                DrawRectangle(145, 198, 730, 2, ColorAlpha(COR_ROSA_NEON, 0.45f));

                // 1. Ajustar Musica
                DesenharBotaoSolto(assets.fontes.teko, (Rectangle){ 140, 230, 340, 54 }, "ajustar musica", 46, e.hoverConfig[0], tempo, WHITE);
                Rectangle btnMusMenos = { 495, 238, 36, 38 };
                Rectangle sliderMus   = { 540, 243, 220, 28 };
                Rectangle btnMusMais  = { 770, 238, 36, 38 };
                DrawTextEx(assets.fontes.teko, "<", (Vector2){ btnMusMenos.x + 8, btnMusMenos.y - 4 }, 46, 1.0f, CheckCollisionPointRec(mouse, btnMusMenos) ? COR_ROSA_NEON : WHITE);
                DesenharBarraCyberInclinada(sliderMus, e.volMusica);
                DrawTextEx(assets.fontes.teko, ">", (Vector2){ btnMusMais.x + 8, btnMusMais.y - 4 }, 46, 1.0f, CheckCollisionPointRec(mouse, btnMusMais) ? COR_ROSA_NEON : WHITE);
                DrawTextEx(assets.fontes.teko, TextFormat("%d%%", (int)(e.volMusica * 100.0f + 0.5f)), (Vector2){ 822, 235 }, 40, 1.0f, COR_CIANO_NEON);

                // 2. Ajustar Efeitos
                DesenharBotaoSolto(assets.fontes.teko, (Rectangle){ 140, 305, 340, 54 }, "ajustar efeitos", 46, e.hoverConfig[1], tempo, WHITE);
                Rectangle btnSfxMenos = { 495, 313, 36, 38 };
                Rectangle sliderSfx   = { 540, 318, 220, 28 };
                Rectangle btnSfxMais  = { 770, 313, 36, 38 };
                DrawTextEx(assets.fontes.teko, "<", (Vector2){ btnSfxMenos.x + 8, btnSfxMenos.y - 4 }, 46, 1.0f, CheckCollisionPointRec(mouse, btnSfxMenos) ? COR_ROSA_NEON : WHITE);
                DesenharBarraCyberInclinada(sliderSfx, e.volSfx);
                DrawTextEx(assets.fontes.teko, ">", (Vector2){ btnSfxMais.x + 8, btnSfxMais.y - 4 }, 46, 1.0f, CheckCollisionPointRec(mouse, btnSfxMais) ? COR_ROSA_NEON : WHITE);
                DrawTextEx(assets.fontes.teko, TextFormat("%d%%", (int)(e.volSfx * 100.0f + 0.5f)), (Vector2){ 822, 310 }, 40, 1.0f, COR_CIANO_NEON);

                // 3. Tela cheia
                DesenharBotaoSolto(assets.fontes.teko, (Rectangle){ 140, 380, 340, 54 }, "tela cheia", 46, e.hoverConfig[2], tempo, WHITE);
                DrawTextEx(assets.fontes.teko, e.telaCheia ? "ligada" : "desligada", (Vector2){ 540, 384 }, 44, 1.0f, e.telaCheia ? COR_ROSA_NEON : LIGHTGRAY);

                // 4. Voltar
                DesenharBotaoSolto(assets.fontes.teko, (Rectangle){ 140, 470, 280, 54 }, "voltar", 48, e.hoverConfig[3], tempo, COR_CIANO_NEON);
                // Decoracao discreta (3 cliques abre o modo dev)
                DrawTextEx(assets.fontes.bold, "SYS//2.0", (Vector2){ 836, 530 }, 16, 1.0f, ColorAlpha(COR_ROXO_NEON, 0.40f));
                DesenharScanlinesHUD(tempo);
            } break;

            case TELA_CREDITOS: {
                if (assets.menuBg.id != 0) DesenharTexturaCover(assets.menuBg, (Rectangle){0, 0, VIRTUAL_W, VIRTUAL_H}, WHITE);
                DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 10, 6, 20, 195 });
                DesenharPainelCyber((Rectangle){ 200, 95, 880, 535 }, COR_ROXO_NEON);
                DrawTextEx(assets.fontes.titulo, "CREDITOS - PROJETO INTEGRADOR", (Vector2){ 290, 138 }, 34, 1.0f, COR_ROSA_NEON);
                const char *cred =
                    "AI.libi e um jogo investigativo desenvolvido em C com Raylib,\n"
                    "combinando Logica Proposicional, Pensamento Critico e Alfabetizacao Digital.\n\n"
                    "Motor otimizado com carregamento sob demanda (cache de texturas),\n"
                    "roteiro dirigido por dados e resolucao virtual escalavel.";
                DesenharTextoQuebrado(assets.fontes.regular, cred, (Rectangle){ 265, 218, 750, 260 }, 28, 1.0f, WHITE);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 490, 555, 300, 50 }, "VOLTAR", 25, e.hoverBtnUnico, tempo, COR_CIANO_NEON);
                DesenharScanlinesHUD(tempo);
            } break;

            case TELA_C1_OCORRENCIA:
                DesenharCardCentral(&assets.fontes, "NOVA OCORRENCIA", "Novo caso de desaparecimento registrado no sistema.\nAcesse os arquivos iniciais para iniciar a investigacao.", "ACESSAR CASO >>", e.hoverBtnUnico, tempo, COR_ROSA_NEON);
                break;

            case TELA_C1_GABRIEL: {
                DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
                DesenharPainelCyber((Rectangle){ 135, 85, 1010, 550 }, COR_ROXO_NEON);
                Rectangle moldura = { 775, 135, 310, 410 };
                DrawRectangleRec(moldura, (Color){ 15, 8, 28, 220 });
                DrawRectangleLinesEx(moldura, 2.0f, ColorAlpha(COR_CIANO_NEON, 0.7f));
                Texture2D gab = ObterTexturaCache(&assets.evidencia, "AssetsRay/Personagens/Gabriel/GabrielMugshot.png");
                DesenharTexturaContain(gab, (Rectangle){ 785, 145, 290, 390 }, WHITE);
                float scanG = moldura.y + fmodf((float)tempo * 120.0f, moldura.height);
                DrawLineEx((Vector2){ moldura.x, scanG }, (Vector2){ moldura.x + moldura.width, scanG }, 2.0f, ColorAlpha(COR_CIANO_NEON, 0.65f));

                DesenharBadgeCyber(assets.fontes.titulo, (Vector2){ 195, 135 }, "GABRIEL MOURA - 22 ANOS", 28, COR_ROSA_NEON);
                DrawTextEx(assets.fontes.bold, "ESTUDANTE UNIVERSITARIO | RECIFE - PE", (Vector2){ 195, 215 }, 27, 1.0f, WHITE);
                DrawTextEx(assets.fontes.bold, "ULTIMA LOCALIZACAO:", (Vector2){ 195, 282 }, 26, 1.0f, COR_AMARELO_AVISO);
                DrawTextEx(assets.fontes.regular, "Festa Universitaria", (Vector2){ 195, 318 }, 28, 1.0f, WHITE);
                DrawTextEx(assets.fontes.bold, "HORARIO ESTIMADO:", (Vector2){ 195, 385 }, 26, 1.0f, COR_AMARELO_AVISO);
                DrawTextEx(assets.fontes.regular, "03:00", (Vector2){ 195, 421 }, 28, 1.0f, WHITE);
                float p = 0.55f + 0.45f * sinf((float)tempo * 4.5f);
                DrawTextEx(assets.fontes.bold, "CLIQUE PARA CONTINUAR >>", (Vector2){ 195, 558 }, 24, 1.0f, ColorAlpha(COR_CIANO_NEON, p));
            } break;

            case TELA_C1_CAIO: {
                DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
                DesenharPainelCyber((Rectangle){ 135, 85, 1010, 550 }, COR_ROXO_NEON);
                Rectangle moldura = { 775, 135, 310, 410 };
                DrawRectangleRec(moldura, (Color){ 15, 8, 28, 220 });
                DrawRectangleLinesEx(moldura, 2.0f, ColorAlpha(COR_ROXO_NEON, 0.7f));
                Texture2D caio = ObterTexturaCache(&assets.evidencia, "AssetsRay/Personagens/Caio/CaioMugshot.png");
                DesenharTexturaContain(caio, (Rectangle){ 785, 145, 290, 390 }, WHITE);
                float scanC = moldura.y + fmodf((float)tempo * 120.0f, moldura.height);
                DrawLineEx((Vector2){ moldura.x, scanC }, (Vector2){ moldura.x + moldura.width, scanC }, 2.0f, ColorAlpha(COR_ROSA_NEON, 0.65f));

                DesenharBadgeCyber(assets.fontes.titulo, (Vector2){ 195, 135 }, "ULTIMA INFORMACAO", 28, COR_CIANO_NEON);
                DesenharTextoQuebrado(assets.fontes.bold, "Ele foi visto tendo uma discussao com seu colega, CAIO VASCONCELOS.", (Rectangle){ 195, 232, 535, 170 }, 30, 1.0f, WHITE);
                float p = 0.55f + 0.45f * sinf((float)tempo * 4.5f);
                DrawTextEx(assets.fontes.bold, "CLIQUE PARA ACESSAR O NEXUS-MSG >>", (Vector2){ 195, 552 }, 24, 1.0f, ColorAlpha(COR_CIANO_NEON, p));
            } break;

            case TELA_C1_CELULAR:
                DesenharCelular(&celular, &assets.fontes, tempo);
                break;

            case TELA_C1_INTERROGATORIO:
                DesenharCardCentral(&assets.fontes, "INTERROGATORIO", "E hora de ouvir os envolvidos e buscar respostas.", "IR INTERROGAR CAIO >>", e.hoverBtnUnico, tempo, COR_ROXO_NEON);
                break;

            case TELA_DIALOGO: {
                const Fala *falas = NULL;
                int total = 0;
                ObterSequencia(e.seq, &falas, &total);
                const Fala *atual = &falas[e.falaIdx];

                if (strcmp(e.ultimoPersonagem, atual->personagem) != 0) {
                    snprintf(e.ultimoPersonagem, sizeof(e.ultimoPersonagem), "%s", atual->personagem);
                    e.charAnimT = 0.0f;
                }

                Texture2D bg = ObterTexturaCache(&assets.fundo, atual->fundo);
                Texture2D ch = ObterTexturaCache(&assets.personagem, atual->personagem);
                DesenharTexturaCover(bg, (Rectangle){ 0, 0, VIRTUAL_W, VIRTUAL_H }, WHITE);

                // Entrada dinamica do personagem + respiracao sutil
                float slideX = (1.0f - e.charAnimT) * 28.0f;
                float floatY = sinf((float)tempo * 2.1f) * 2.5f;
                Color tintChar = (Color){ 255, 255, 255, (unsigned char)(255 * e.charAnimT) };
                DesenharTexturaContain(ch, (Rectangle){ 190 + slideX, 12 + floatY, 900, 665 }, tintChar);

                // Caixa de dialogo proporcional (recorte exato da area nao-transparente de CaixaDialogo.png)
                Rectangle boxDest = { 60, 446, 1160, 260 };
                if (assets.caixaDialogo.id != 0) {
                    Rectangle srcBox = { 0, 68, (float)assets.caixaDialogo.width, (float)assets.caixaDialogo.height - 77.0f };
                    DrawTexturePro(assets.caixaDialogo, srcBox, boxDest, (Vector2){0, 0}, 0.0f, WHITE);
                } else {
                    DesenharPainelCyber(boxDest, COR_ROXO_NEON);
                }

                // Nome do falante centralizado exatamente dentro da aba superior da caixa
                Color corNome = CorDoFalante(atual->falante);
                Vector2 szNome = MeasureTextEx(assets.fontes.bold, atual->falante, 28, 1.5f);
                float abaCenterX = boxDest.x + 282.0f * (boxDest.width / 1261.0f);
                DrawTextEx(assets.fontes.bold, atual->falante, (Vector2){ abaCenterX - szNome.x * 0.5f, boxDest.y + 23.0f }, 28, 1.5f, corNome);

                // Onda de voz animada enquanto digita + botoes HIST/AUTO + indicador REC
                DesenharOndaVoz((Rectangle){ boxDest.x + 485.0f, boxDest.y + 20.0f, 115.0f, 28.0f }, tempo, !e.falaCompleta, corNome);

                Rectangle btnHist = { boxDest.x + boxDest.width - 445.0f, boxDest.y + 18.0f, 118.0f, 30.0f };
                Rectangle btnAuto = { boxDest.x + boxDest.width - 315.0f, boxDest.y + 18.0f, 130.0f, 30.0f };
                bool hovH = CheckCollisionPointRec(mouse, btnHist);
                bool hovA = CheckCollisionPointRec(mouse, btnAuto);
                DrawRectangleRounded(btnHist, 0.25f, 6, (Color){ 20, 11, 36, 235 });
                DrawRectangleRoundedLinesEx(btnHist, 0.25f, 6, 1.5f, hovH ? COR_ROSA_NEON : ColorAlpha(COR_ROXO_NEON, 0.85f));
                DrawTextEx(assets.fontes.bold, "HIST [H]", (Vector2){ btnHist.x + 18.0f, btnHist.y + 5.0f }, 20, 1.0f, hovH ? COR_ROSA_NEON : WHITE);

                Color corAuto = e.autoDialogo ? COR_CIANO_NEON : (hovA ? COR_ROSA_NEON : ColorAlpha(COR_ROXO_NEON, 0.85f));
                DrawRectangleRounded(btnAuto, 0.25f, 6, e.autoDialogo ? (Color){ 16, 40, 58, 240 } : (Color){ 20, 11, 36, 235 });
                DrawRectangleRoundedLinesEx(btnAuto, 0.25f, 6, 1.5f, corAuto);
                DrawTextEx(assets.fontes.bold, e.autoDialogo ? "AUTO: ON" : "AUTO: OFF", (Vector2){ btnAuto.x + 14.0f, btnAuto.y + 5.0f }, 20, 1.0f, e.autoDialogo ? COR_CIANO_NEON : WHITE);

                float recPulso = 0.45f + 0.55f * sinf((float)tempo * 5.0f);
                DrawCircle((int)(boxDest.x + boxDest.width - 135.0f), (int)(boxDest.y + 34.0f), 5.0f, ColorAlpha(COR_ROSA_NEON, recPulso));
                DrawTextEx(assets.fontes.bold, "REC", (Vector2){ boxDest.x + boxDest.width - 122.0f, boxDest.y + 22.0f }, 22, 1.0f, ColorAlpha(WHITE, 0.85f));

                // Texto grande (30px) perfeitamente alinhado dentro da caixa + cursor piscando
                char trecho[512];
                int n = (int)e.charsVisiveis;
                if (n >= (int)sizeof(trecho) - 2) n = (int)sizeof(trecho) - 2;
                strncpy(trecho, atual->texto, n);
                if (!e.falaCompleta && ((int)(tempo * 8.0) % 2 == 0)) {
                    trecho[n] = '_';
                    trecho[n + 1] = '\0';
                } else {
                    trecho[n] = '\0';
                }
                DesenharTextoQuebrado(assets.fontes.regular, trecho, (Rectangle){ boxDest.x + 68.0f, boxDest.y + 82.0f, boxDest.width - 136.0f, 114.0f }, 30, 1.0f, WHITE);

                if (e.historicoAberto) {
                    DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, 215 });
                    DesenharPainelCyber((Rectangle){ 110, 72, 1060, 576 }, COR_ROXO_NEON);
                    DrawTextEx(assets.fontes.titulo, "HISTORICO DE CONVERSAS", (Vector2){ 150, 92 }, 28, 1.0f, COR_CIANO_NEON);
                    Rectangle btnFecharHist = { 980, 88, 150, 40 };
                    DesenharBotaoCyber(assets.fontes.bold, btnFecharHist, "FECHAR [H]", 20, CheckCollisionPointRec(mouse, btnFecharHist) ? 1.0f : 0.0f, tempo, COR_ROSA_NEON);
                    DrawRectangle(150, 138, 980, 2, ColorAlpha(COR_ROXO_NEON, 0.45f));

                    // Calcula altura total e desenha com scissor
                    float totalH = 0.0f;
                    for (int s = 0; s < 9; s++) {
                        if (!e.seqVisitada[s] && (SequenciaDialogo)s != e.seq) continue;
                        const Fala *sf = NULL;
                        int stot = 0;
                        ObterSequencia((SequenciaDialogo)s, &sf, &stot);
                        int lim = ((SequenciaDialogo)s == e.seq) ? (e.falaIdx + 1) : stot;
                        for (int k = 0; k < lim; k++) totalH += 72.0f;
                    }
                    float maxScroll = fmaxf(0.0f, totalH - 460.0f);
                    if (e.historicoScroll > maxScroll) e.historicoScroll = maxScroll;

                    BeginScissorMode((int)(145 * cam.zoom), (int)(150 * cam.zoom), (int)(990 * cam.zoom), (int)(475 * cam.zoom));
                    float cy = 154.0f - e.historicoScroll;
                    for (int s = 0; s < 9; s++) {
                        if (!e.seqVisitada[s] && (SequenciaDialogo)s != e.seq) continue;
                        const Fala *sf = NULL;
                        int stot = 0;
                        ObterSequencia((SequenciaDialogo)s, &sf, &stot);
                        int lim = ((SequenciaDialogo)s == e.seq) ? (e.falaIdx + 1) : stot;
                        for (int k = 0; k < lim; k++) {
                            if (cy + 70.0f >= 145.0f && cy <= 630.0f) {
                                Color cf = CorDoFalante(sf[k].falante);
                                DrawTextEx(assets.fontes.bold, sf[k].falante, (Vector2){ 155, cy }, 22, 1.0f, cf);
                                DesenharTextoQuebrado(assets.fontes.regular, sf[k].texto, (Rectangle){ 305, cy, 825, 66 }, 22, 1.0f, WHITE);
                                DrawRectangle(155, (int)(cy + 66), 975, 1, ColorAlpha(COR_ROXO_NEON, 0.22f));
                            }
                            cy += 72.0f;
                        }
                    }
                    EndScissorMode();
                }
            } break;

            case TELA_NOVA_EVIDENCIA:
                DesenharCardCentral(&assets.fontes, "NOVA EVIDENCIA ENCONTRADA", "Uma nova pista pode ajudar na investigacao.\nResolva o desafio de logica para ter acesso a ela.", "RESOLVER DESAFIO >>", e.hoverBtnUnico, tempo, COR_ROXO_NEON);
                break;

            case TELA_MINIGAME:
                DesenharMiniGame(&miniGame, &assets.fontes, e.cenaAtual, tempo);
                break;

            case TELA_CONFIRMA_CAIO:
                DesenharCardCentral(&assets.fontes, "ACUSAR CAIO AGORA?", "Voce selecionou: Caio e o culpado.\nTem certeza dessa decisao?", NULL, 0, tempo, COR_AMARELO_AVISO);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 300, 420, 310, 54 }, "a) SIM, ACUSAR CAIO", 24, e.hoverSimNao[0], tempo, COR_VERMELHO_ERR);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 670, 420, 310, 54 }, "b) NAO, INVESTIGAR", 24, e.hoverSimNao[1], tempo, COR_CIANO_NEON);
                break;

            case TELA_EVIDENCIA_PERDIDA:
                DesenharCardCentral(&assets.fontes, "EVIDENCIA PERDIDA", "Voce nao resolveu o desafio logico.\nA investigacao deve continuar sem esta evidencia.", "CONTINUAR >>", e.hoverBtnUnico, tempo, COR_VERMELHO_ERR);
                break;

            case TELA_EVIDENCIA_DESBLOQUEADA:
                DesenharCardCentral(&assets.fontes, "EVIDENCIA DESBLOQUEADA", "Nova pista registrada no seu Dossie.\nAnalise os detalhes com atencao antes de tirar conclusoes.", "ACESSAR EVIDENCIA >>", e.hoverBtnUnico, tempo, COR_VERDE_OK);
                break;

            case TELA_ANALISE_PROVA: {
                DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
                DesenharPainelCyber((Rectangle){ 75, 68, 1130, 604 }, COR_ROXO_NEON);

                const char *titulo = "ANALISE DE EVIDENCIA";
                const char *desc = "";
                bool ehManipulada = false;
                const char *motivo = "";
                Rectangle imgRect = { 115, 145, 500, 385 };
                bool temScannerImg = false;

                if (e.cenaAtual == 2) {
                    titulo = "CAMERA 03 - SAIDA DA FESTA (02:48)";
                    desc = "Registro da camera de seguranca da saida da festa mostrando Caio indo embora as 02:48.";
                    DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/CaioTrueEvidence.png"), imgRect, WHITE);
                    temScannerImg = true;
                    ehManipulada = false;
                    motivo = "Metadados e iluminacao consistentes com a cena real da saida.";
                } else if (e.cenaAtual == 3) {
                    titulo = "AUDIO ANONIMO DA FESTA";
                    desc = "Audio enviado anonimamente com trechos distorcidos e voz semelhante a de Caio.";
                    DesenharPlayerAudio(&assets.fontes, (Rectangle){ 115, 145, 500, 360 }, e.audioTocando, e.audioTempo, mouse);
                    ehManipulada = true;
                    motivo = "Glitches e trechos robotizados tipicos de sintese de voz por IA.";
                } else if (e.cenaAtual == 4) {
                    titulo = "MENSAGENS EXTRAIDAS DO CELULAR";
                    desc = "Origem: Celular de Caio, extraido pela pericia com autorizacao judicial.";
                    DesenharPainelCyber((Rectangle){ 115, 145, 500, 440 }, COR_CIANO_NEON);
                    const char *chatTxt =
                        "GABRIEL (02:11): cara, eu vou falar. precisam saber o que ela fez\n"
                        "CAIO (02:12): fala com ela primeiro, na frente de todo mundo nao\n"
                        "GABRIEL (02:12): ela nao falou com ninguem quando pegou o premio\n"
                        "CAIO (02:14): to pedindo so isso\n"
                        "GABRIEL (02:15): desce aqui que a gente resolve\n"
                        "CAIO (02:31): faz o que quiser entao. eu vou embora\n"
                        "GABRIEL (02:33): vai nessa";
                    DesenharTextoQuebrado(assets.fontes.regular, chatTxt, (Rectangle){ 138, 168, 454, 395 }, 22, 1.0f, WHITE);
                    ehManipulada = false;
                    motivo = "Extracao oficial da pericia judicial, compativel com o horario 02:48.";
                } else if (e.cenaAtual == 5) {
                    titulo = "FOTO DO ALIBI DE JULIANA (03:07)";
                    desc = "Selfie dentro do carro apresentada por Juliana. Observe o reflexo na janela.";
                    DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/JuliaDeepfake.png"), imgRect, WHITE);
                    temScannerImg = true;
                    ehManipulada = true;
                    motivo = "Inconsistencia visual: o reflexo na janela nao corresponde a pose real.";
                } else if (e.cenaAtual == 6) {
                    titulo = "LOCALIZACAO DO CELULAR DE GABRIEL";
                    desc = "Registro de antena as 03:20 proximo ao barranco. Sem todos os metadados tecnicos.";
                    DesenharMapaCena6(&assets.fontes, imgRect, tempo);
                    ehManipulada = false;
                    motivo = "Registro compativel com o local onde o corpo foi encontrado.";
                } else if (e.cenaAtual == 7) {
                    titulo = "FOTO RECUPERADA DO CELULAR DE GABRIEL (02:58)";
                    desc = "Foto recebida por Gabriel naquela noite. Observe a roda do carro e a calcada.";
                    DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/JulianaDeepfake.png"), imgRect, WHITE);
                    temScannerImg = true;
                    ehManipulada = true;
                    motivo = "Distorcoes geometricas na roda/meio-fio indicam manipulacao por IA.";
                }

                if (temScannerImg) {
                    DrawRectangleLinesEx(imgRect, 1.5f, ColorAlpha(COR_CIANO_NEON, 0.55f));
                    float sy = imgRect.y + fmodf((float)tempo * 95.0f, imgRect.height);
                    DrawLineEx((Vector2){ imgRect.x, sy }, (Vector2){ imgRect.x + imgRect.width, sy }, 2.0f, ColorAlpha(COR_CIANO_NEON, 0.7f));
                }

                DrawTextEx(assets.fontes.titulo, titulo, (Vector2){ 115, 90 }, 28, 1.0f, COR_CIANO_NEON);
                DesenharTextoQuebrado(assets.fontes.regular, desc, (Rectangle){ 655, 155, 505, 145 }, 26, 1.0f, WHITE);
                DrawTextEx(assets.fontes.bold, "Essa prova e verdadeira ou manipulada por IA?", (Vector2){ 655, 368 }, 26, 1.0f, COR_ROSA_NEON);

                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 655, 446, 230, 54 }, "VERDADEIRA", 25, e.hoverAnalise[0], tempo,
                    (e.respostaProva[e.cenaAtual] == 1) ? COR_CIANO_NEON : COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 905, 446, 230, 54 }, "MANIPULADA", 25, e.hoverAnalise[1], tempo,
                    (e.respostaProva[e.cenaAtual] == 2) ? COR_ROSA_NEON : COR_ROXO_NEON);

                if (e.respostaProva[e.cenaAtual] != 0) {
                    bool acertou = (e.respostaProva[e.cenaAtual] == (ehManipulada ? 2 : 1));
                    DrawTextEx(assets.fontes.bold, acertou ? "ANALISE CORRETA!" : "ATENCAO AS PISTAS!", (Vector2){ 655, 305 }, 25, 1.0f, acertou ? COR_VERDE_OK : COR_AMARELO_AVISO);
                    DesenharTextoQuebrado(assets.fontes.regular, motivo, (Rectangle){ 655, 336, 505, 65 }, 22, 1.0f, LIGHTGRAY);
                    DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 655, 542, 480, 54 }, "CONTINUAR INVESTIGACAO >>", 25, e.hoverAnalise[2], tempo, COR_VERDE_OK);
                }
            } break;

            case TELA_C6_HORAS_DEPOIS:
                DesenharCardCentral(&assets.fontes, "ALGUMAS HORAS DEPOIS", "Enquanto a pericia analisa os arquivos, voce revisa os depoimentos em busca de conexoes.", "CONTINUAR >>", e.hoverBtnUnico, tempo, COR_CIANO_NEON);
                break;

            case TELA_VEREDITO:
                DesenharCardCentral(&assets.fontes, "ESCOLHA O SEU VEREDITO", "Quem e responsavel pelo desaparecimento e morte de Gabriel Moura?", NULL, 0, tempo, COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 310, 322, 660, 54 }, "a) Caio Vasconcelos e o culpado.", 25, e.hoverVeredito[0], tempo, COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 310, 392, 660, 54 }, "b) Juliana Costa e a culpada.", 25, e.hoverVeredito[1], tempo, COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 310, 462, 660, 54 }, "c) Revisar o Dossie de Evidencias [D]", 25, e.hoverVeredito[2], tempo, COR_CIANO_NEON);
                break;

            case TELA_CONFIRMA_VEREDITO: {
                const char *alvo = (e.acusadoFinal == 1) ? "Caio Vasconcelos" : "Juliana Costa";
                DesenharCardCentral(&assets.fontes, "CONFIRMAR VEREDITO", TextFormat("Voce esta acusando %s.\nTem certeza dessa decisao?", alvo), NULL, 0, tempo, COR_AMARELO_AVISO);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 340, 420, 260, 54 }, "SIM, CONFIRMAR", 25, e.hoverSimNao[0], tempo, COR_VERDE_OK);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 680, 420, 260, 54 }, "NAO, VOLTAR", 25, e.hoverSimNao[1], tempo, COR_CIANO_NEON);
            } break;

            case TELA_FINAL: {
                const char *tit = e.vitoria ? "VEREDITO CORRETO!" : "VEREDITO INCORRETO";
                const char *txt = e.vitoria
                    ? "Voce revelou a verdade! Caio tinha alibi comprovado (02:48) e as fotos apresentadas por Juliana tinham sinais claros de manipulacao por IA. Caso encerrado com sucesso!"
                    : "Voce acusou a pessoa errada. Reavalie as contradicoes nos depoimentos e nas evidencias digitais e tente novamente!";
                DesenharCardCentral(&assets.fontes, tit, txt, "VOLTAR AO MENU", e.hoverBtnUnico, tempo, e.vitoria ? COR_VERDE_OK : COR_VERMELHO_ERR);
                DrawTextEx(assets.fontes.bold, TextFormat("Desafios de Logica: %d acertos | %d erros", stats.acertos, stats.erros), (Vector2){ 400, 520 }, 24, 1.0f, COR_CIANO_NEON);
            } break;

            case TELA_DOSSIE: {
                DesenharFundoCyberGrid(tempo, COR_CIANO_NEON);
                DesenharPainelCyber((Rectangle){ 55, 62, 1170, 616 }, COR_CIANO_NEON);

                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 95, 82, 200, 44 }, "EVIDENCIAS", 24, e.dossieAba == 0 ? 0.85f : e.hoverDossieTop[0], tempo, e.dossieAba == 0 ? COR_ROSA_NEON : COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 315, 82, 200, 44 }, "SUSPEITOS", 24, e.dossieAba == 1 ? 0.85f : e.hoverDossieTop[1], tempo, e.dossieAba == 1 ? COR_ROSA_NEON : COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 1010, 82, 180, 44 }, "FECHAR [D]", 24, e.hoverDossieTop[2], tempo, COR_CIANO_NEON);
                DrawRectangle(95, 136, 1090, 2, ColorAlpha(COR_ROXO_NEON, 0.45f));

                if (e.dossieAba == 0) {
                    const char *nomesEv[6] = {
                        "1. Camera Saida (02:48)", "2. Audio Anonimo", "3. Mensagens Celular",
                        "4. Foto Carro (03:07)", "5. Localizacao (03:20)", "6. Foto Festa (02:58)"
                    };
                    for (int i = 0; i < 6; i++) {
                        int st = e.statusProva[i + 2];
                        Rectangle item = { 95, 154 + i * 74, 320, 62 };
                        bool sel = (e.dossieItem == i);
                        bool hov = CheckCollisionPointRec(mouse, item);
                        if (sel || hov) DesenharHoverCyberRosa(item, sel ? 1.0f : 0.6f, tempo);
                        DrawTextEx(assets.fontes.bold, nomesEv[i], (Vector2){ item.x + 26, item.y + 6 }, 23, 1.0f, sel ? COR_ROSA_NEON : WHITE);
                        const char *sub = (st == 1) ? "COLETADA" : (st == -1) ? "PERDIDA" : "BLOQUEADA";
                        Color csub = (st == 1) ? COR_VERDE_OK : (st == -1) ? COR_VERMELHO_ERR : GRAY;
                        DrawTextEx(assets.fontes.regular, sub, (Vector2){ item.x + 26, item.y + 34 }, 20, 1.0f, csub);
                    }

                    int cenaEv = e.dossieItem + 2;
                    Rectangle det = { 445, 150, 745, 496 };
                    DesenharPainelCyber(det, ColorAlpha(COR_ROXO_NEON, 0.55f));

                    if (e.statusProva[cenaEv] != 1) {
                        DrawTextEx(assets.fontes.bold, e.statusProva[cenaEv] == -1 ? "EVIDENCIA PERDIDA NO DESAFIO" : "EVIDENCIA AINDA NAO COLETADA", (Vector2){ 595, 375 }, 26, 1.0f, GRAY);
                    } else if (cenaEv == 2) {
                        DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/CaioTrueEvidence.png"), (Rectangle){ 480, 172, 675, 360 }, WHITE);
                        DrawTextEx(assets.fontes.regular, "Registro da camera de saida da festa as 02:48.", (Vector2){ 485, 558 }, 24, 1.0f, LIGHTGRAY);
                    } else if (cenaEv == 3) {
                        DesenharPlayerAudio(&assets.fontes, (Rectangle){ 480, 172, 675, 418 }, e.audioTocando, e.audioTempo, mouse);
                    } else if (cenaEv == 4) {
                        DesenharTextoQuebrado(assets.fontes.regular,
                            "ORIGEM: Celular de Caio (Pericia Judicial)\n\n"
                            "GABRIEL (02:11): cara, eu vou falar. precisam saber o que ela fez\n"
                            "CAIO (02:12): fala com ela primeiro, na frente de todo mundo nao\n"
                            "CAIO (02:31): faz o que quiser entao. eu vou embora\n"
                            "GABRIEL (02:33): vai nessa",
                            (Rectangle){ 485, 182, 665, 420 }, 25, 1.0f, WHITE);
                    } else if (cenaEv == 5) {
                        DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/JuliaDeepfake.png"), (Rectangle){ 480, 170, 675, 360 }, WHITE);
                        DrawTextEx(assets.fontes.regular, "Foto enviada por Juliana a mae, marcada com horario 03:07.", (Vector2){ 485, 558 }, 24, 1.0f, LIGHTGRAY);
                    } else if (cenaEv == 6) {
                        DesenharMapaCena6(&assets.fontes, (Rectangle){ 480, 172, 675, 360 }, tempo);
                        DrawTextEx(assets.fontes.regular, "Sinal de antena do celular de Gabriel as 03:20.", (Vector2){ 485, 558 }, 24, 1.0f, LIGHTGRAY);
                    } else if (cenaEv == 7) {
                        DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/JulianaDeepfake.png"), (Rectangle){ 480, 170, 675, 360 }, WHITE);
                        DrawTextEx(assets.fontes.regular, "Foto recuperada do aparelho de Gabriel, marcada as 02:58.", (Vector2){ 485, 558 }, 24, 1.0f, LIGHTGRAY);
                    }
                } else {
                    const char *sus[3] = { "Caio Vasconcelos", "Juliana Costa", "Gabriel Moura (Vitima)" };
                    const char *fotos[3] = {
                        "AssetsRay/Personagens/Caio/CaioMugshot.png",
                        "AssetsRay/Personagens/Julia/juliana retrato.png",
                        "AssetsRay/Personagens/Gabriel/GabrielMugshot.png"
                    };

                    for (int i = 0; i < 3; i++) {
                        Rectangle item = { 95, 154 + i * 74, 320, 62 };
                        bool sel = (e.dossieItem == i);
                        bool hov = CheckCollisionPointRec(mouse, item);
                        if (sel || hov) DesenharHoverCyberRosa(item, sel ? 1.0f : 0.6f, tempo);
                        DrawTextEx(assets.fontes.bold, sus[i], (Vector2){ item.x + 26, item.y + 18 }, 25, 1.0f, sel ? COR_ROSA_NEON : WHITE);
                    }

                    int idx = (e.dossieItem >= 0 && e.dossieItem < 3) ? e.dossieItem : 0;
                    Rectangle ficha = { 445, 150, 745, 496 };
                    DesenharPainelCyber(ficha, ColorAlpha(COR_ROXO_NEON, 0.55f));
                    DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, fotos[idx]), (Rectangle){ 465, 175, 220, 290 }, WHITE);

                    char infoFicha[1024] = "";
                    if (idx == 0) {
                        snprintf(infoFicha, sizeof(infoFicha),
                            "Nome: Caio Vasconcelos\n"
                            "Idade: 22 anos | Cidade: Recife - PE | Altura: 1,78m\n"
                            "Perfil: Estudante de Computacao, reservado e direto.\n"
                            "Personalidade: Defensivo sob pressao, leal aos amigos.\n"
                            "Relacao: Colega de faculdade de Gabriel.\n\n"
                            "ANOTACOES DO DEPOIMENTO:\n%s%s",
                            e.notaCaioCena2 ? "- Afirmou ter discutido com Gabriel e ido embora por volta das 2h40.\n" : "- Ainda nao interrogado.\n",
                            e.notaCaioCena4 ? "- Alegou que a discussao foi sobre um suposto trabalho de Juliana.\n" : ""
                        );
                    } else if (idx == 1) {
                        snprintf(infoFicha, sizeof(infoFicha),
                            "Nome: Juliana Costa\n"
                            "Idade: 21 anos | Cidade: Recife - PE | Altura: 1,68m\n"
                            "Perfil: Estudante destaque de Engenharia, muito articulada.\n"
                            "Personalidade: Confiante, fria e calculista em conversas.\n"
                            "Relacao: Colega de curso e conhecida de Gabriel e Caio.\n\n"
                            "ANOTACOES DO DEPOIMENTO:\n%s",
                            e.notaJulianaCena5 ? "- Afirmou ter conversado normalmente com Gabriel, ficado ate ~3h e enviado foto a mae.\n" : "- Ainda nao interrogada.\n"
                        );
                    } else {
                        snprintf(infoFicha, sizeof(infoFicha),
                            "Nome: Gabriel Moura\n"
                            "Idade: 22 anos | Cidade: Recife - PE | Altura: 1,81m\n"
                            "Perfil: Estudante universitario, envolvido em projetos academicos.\n"
                            "Personalidade: Idealista, transparente e firme nas proprias conviccoes.\n"
                            "Relacao: Vitima do caso (visto pela ultima vez na festa).\n\n"
                            "REGISTROS DA INVESTIGACAO:\n"
                            "- Ultima localizacao conhecida: Festa universitaria (~03:00).\n%s",
                            e.notaGabrielCena6 ? "- Corpo encontrado em um barranco as margens da estrada (caso tratado como homicidio).\n" : ""
                        );
                    }
                    DrawTextEx(assets.fontes.titulo, sus[idx], (Vector2){ 705, 172 }, 28, 1.0f, COR_ROSA_NEON);
                    DesenharTextoQuebrado(assets.fontes.regular, infoFicha, (Rectangle){ 705, 214, 465, 420 }, 24, 1.0f, WHITE);
                }
                DesenharScanlinesHUD(tempo);
            } break;
        }

        if (emGameplay) {
            DesenharBarraTopo(&assets.fontes, mouse, tempo, e.hoverDossieTop[0], e.cenaAtual);
            if (e.tela != TELA_DIALOGO && e.tela != TELA_C1_CELULAR && e.tela != TELA_MINIGAME) {
                DesenharScanlinesHUD(tempo);
            }
        }

        if (e.menuPausa) {
            DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 8, 5, 18, 215 });
            DesenharPainelCyber((Rectangle){ 415, 160, 450, 355 }, COR_ROSA_NEON);
            DrawTextEx(assets.fontes.teko, "PAUSA", (Vector2){ 592, 180 }, 54, 1.5f, WHITE);
            DrawRectangle(465, 240, 350, 2, ColorAlpha(COR_ROSA_NEON, 0.45f));

            const char *pausaTxt[4] = { "RESUMIR JOGO", "MENU PRINCIPAL", "CONFIGURACOES", "SAIR DO JOGO" };
            Color pausaCor[4] = { COR_VERDE_OK, COR_CIANO_NEON, COR_ROXO_NEON, COR_ROSA_NEON };
            for (int i = 0; i < 4; i++) {
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 465, 252 + i * 58, 350, 48 }, pausaTxt[i], 24, e.hoverPausa[i], tempo, pausaCor[i]);
            }
        }

        DesenharDevOverlay(&e, &assets.fontes, mouse, tempo);

        EndMode2D();
        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexturePro(canvas.texture,
            (Rectangle){ 0, 0, (float)RENDER_W, -(float)RENDER_H },
            destTela, (Vector2){ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(canvas);
    DescarregarGerenciadorAssets(&assets);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
