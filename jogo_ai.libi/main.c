#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "assets.h"
#include "roteiro.h"
#include "minigames.h"
#include "celular.h"
#include "progresso.h"
#include "cutscene.h"
#include "mural.h"

typedef enum {
    CULPADO_JULIANA = 0,
    CULPADO_CAIO = 1
} CulpadoRota;

typedef enum {
    TELA_MENU = 0,
    TELA_CONFIG,
    TELA_CREDITOS,
    TELA_NOME_JOGADOR,

    // CENA 1
    TELA_C1_COMPUTADOR,
    TELA_C1_GABRIEL,
    TELA_C1_CAIO,
    TELA_C1_INTERROGATORIO,

    // MOTOR DE DIALOGO
    TELA_DIALOGO,

    // FLUXO DE EVIDENCIAS E DESAFIOS
    TELA_NOVA_EVIDENCIA,
    TELA_MINIGAME,
    TELA_CONFIRMA_PRECIPITADO,
    TELA_EVIDENCIA_PERDIDA,
    TELA_EVIDENCIA_DESBLOQUEADA,
    TELA_ANALISE_PROVA,

    // SALA DE SUSPEITOS, VEREDITO, CUTSCENE E MURAL
    TELA_SALA_SUSPEITOS,
    TELA_VEREDITO,
    TELA_CONFIRMA_VEREDITO,
    TELA_CUTSCENE,
    TELA_FINAL,
    TELA_DOSSIE
} TelaAtual;

typedef enum {
    SEQ_CENA1_ABERTURA = 0,
    SEQ_CENA1,
    SEQ_CENA2,
    SEQ_CENA3,
    SEQ_CENA3_POS,
    SEQ_CENA4,
    SEQ_CENA4_POS,
    SEQ_CENA5,
    SEQ_CENA6,
    SEQ_CENA6_POS,
    SEQ_CENA7A,
    SEQ_CENA7B,
    SEQ_CENA8,
    SEQ_FINAL_PERFEITO,
    SEQ_FINAL_VERDADE,
    SEQ_FINAL_DUVIDA,
    SEQ_FINAL_COLAPSO,
    SEQ_FINAL_PRECIPITADO,
    SEQ_FINAL_INJUSTICA
} SequenciaDialogo;
#define TOTAL_SEQS 19

typedef struct {
    TelaAtual tela;
    TelaAtual telaAnterior;
    SequenciaDialogo seq;
    int falaIdx;
    int cenaAtual; // 2..7

    // Efeito typing rapido
    float charsVisiveis;
    bool falaCompleta;

    // Rota sorteada (Juliana ou Caio) - evita repetir em partidas seguidas
    CulpadoRota culpado;
    int ultimoCulpado; // -1 = primeira partida da sessao
    char nomeJogador[24];
    int nomeJogadorLen;
    bool nomeVazioErro;
    float toastMuralTimer;
    char toastMuralTxt[80];

    // Progresso das 5 provas (index 3..7 = Cenas 3..7): 0=bloqueada, 1=desbloqueada, -1=perdida
    int statusProva[8];
    int respostaProva[8]; // 0=nao respondida, 1=Real, 2=Gerada por IA
    float analiseTempoRestante;
    bool analiseTempoEsgotado;

    // Notas dinamicas do Dossie conforme as conversas avancam
    bool notaCaioCena2;
    bool notaJulianaCena4;
    bool notaGabrielCena5;
    bool notaNuvemCena6;
    bool notaConfrontoCena7;
    bool podeAcusar;
    bool setaMuralTutorial;

    // Audio da Cena 3
    bool audioTocando;
    float audioTempo;

    // Veredito e 6 Finais
    int acusadoFinal; // 1 = Caio, 2 = Juliana
    int tipoFinal;    // 1=Verdade Absoluta, 2=A Verdade, 3=A Duvida, 4=Colapso Forense, 5=Precipitacao, 6=A Injustica
    bool vitoria;

    // Configuracoes
    float volMusica;
    float volSfx;
    bool telaCheia;
    bool modoTechNight;
    bool menuPausa;
    bool configVeioDePausa;
    int clickPreset;

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
    bool seqVisitada[TOTAL_SEQS];
} EstadoJogo;

static int RespostaCorretaProva(int cena, CulpadoRota culpado) {
    if (cena == 3) return (culpado == CULPADO_JULIANA) ? 2 : 1; // Audio: IA na rota Juliana, REAL na rota Caio
    if (cena == 4) return (culpado == CULPADO_JULIANA) ? 2 : 1; // Foto:  IA na rota Juliana, REAL na rota Caio
    if (cena == 5) return 1; // GPS: REAL em ambas
    if (cena == 6) return 1; // Nuvem: REAL em ambas
    if (cena == 7) return (culpado == CULPADO_JULIANA) ? 1 : 2; // Saida: Mobi REAL (Juliana) / Video IA (Caio)
    return 0;
}

static int AlvoCorretoLogica(int cena, CulpadoRota culpado) {
    int alvoCulp = (culpado == CULPADO_JULIANA) ? 1 : 0;
    if (cena == 4) return 1;    // P2: Foto da Juliana
    if (cena == 5) return 2;    // P3: Celular do Gabriel
    return alvoCulp;            // P1 (Audio), P4 (Nuvem), P5 (Saida): culpado
}

static int ContarProvasValidadas(const EstadoJogo *e, const EstadoMural *m) {
    int count = 0;
    for (int c = 3; c <= 7; c++) {
        int i = c - 3;
        if (e->statusProva[c] == 1 &&
            e->respostaProva[c] == RespostaCorretaProva(c, e->culpado) &&
            m->evPosicionada[i] && m->evLigadaA[i] == AlvoCorretoLogica(c, e->culpado)) {
            count++;
        }
    }
    return count;
}

static bool AcertouTodasLogicas(const EstadoJogo *e) {
    for (int c = 3; c <= 7; c++) {
        if (e->statusProva[c] != 1) return false;
    }
    return true;
}

static int ContarAmbiguasCorretas(const EstadoJogo *e, const EstadoMural *m) {
    int alvoCulp = (e->culpado == CULPADO_JULIANA) ? 1 : 0;
    int count = 0;
    for (int i = MURAL_NUM_EV_OFIC; i < MURAL_NUM_TOTAL; i++) {
        if (!m->evPosicionada[i]) continue;
        if (i == 5) {
            if (m->evLigadaA[i] == 2 && m->evLigadaB[i] == alvoCulp) count++;
        } else if (i == 8) {
            if (m->evLigadaA[i] == 1 || m->evLigadaA[i] == alvoCulp) count++;
        } else if (i == 9) {
            if (m->evLigadaA[i] == 2 || m->evLigadaB[i] == 2) count++;
        } else {
            if (m->evLigadaA[i] == alvoCulp) count++;
        }
    }
    return count;
}

static void ObterSequencia(SequenciaDialogo seq, CulpadoRota culpado, const Fala **outFalas, int *outTotal) {
    bool ehJu = (culpado == CULPADO_JULIANA);
    switch (seq) {
        case SEQ_CENA1_ABERTURA: *outFalas = falasCena1Abertura; *outTotal = NUM_FALAS_CENA1_ABERTURA; break;
        case SEQ_CENA1:     *outFalas = falasCena1; *outTotal = NUM_FALAS_CENA1; break;
        case SEQ_CENA2:     *outFalas = falasCena2; *outTotal = NUM_FALAS_CENA2; break;
        case SEQ_CENA3:     *outFalas = falasCena3; *outTotal = NUM_FALAS_CENA3; break;
        case SEQ_CENA3_POS:
            *outFalas = ehJu ? falasCena3PosJuliana : falasCena3PosCaio;
            *outTotal = ehJu ? NUM_FALAS_CENA3_POS_JULIANA : NUM_FALAS_CENA3_POS_CAIO;
            break;
        case SEQ_CENA4:     *outFalas = falasCena4; *outTotal = NUM_FALAS_CENA4; break;
        case SEQ_CENA4_POS:
            *outFalas = ehJu ? falasCena4PosJuliana : falasCena4PosCaio;
            *outTotal = ehJu ? NUM_FALAS_CENA4_POS_JULIANA : NUM_FALAS_CENA4_POS_CAIO;
            break;
        case SEQ_CENA5:     *outFalas = falasCena5; *outTotal = NUM_FALAS_CENA5; break;
        case SEQ_CENA6:     *outFalas = falasCena6; *outTotal = NUM_FALAS_CENA6; break;
        case SEQ_CENA6_POS:
            *outFalas = ehJu ? falasCena6PosJuliana : falasCena6PosCaio;
            *outTotal = ehJu ? NUM_FALAS_CENA6_POS_JULIANA : NUM_FALAS_CENA6_POS_CAIO;
            break;
        case SEQ_CENA7A:
            *outFalas = ehJu ? falasCena7A_Juliana : falasCena7A_Caio;
            *outTotal = ehJu ? NUM_FALAS_CENA7A_Juliana : NUM_FALAS_CENA7A_Caio;
            break;
        case SEQ_CENA7B:
            *outFalas = ehJu ? falasCena7B_Juliana : falasCena7B_Caio;
            *outTotal = ehJu ? NUM_FALAS_CENA7B_Juliana : NUM_FALAS_CENA7B_Caio;
            break;
        case SEQ_CENA8:     *outFalas = falasCena8; *outTotal = NUM_FALAS_CENA8; break;
        case SEQ_FINAL_PERFEITO:
            *outFalas = ehJu ? falasFinalPerfeitoJuliana : falasFinalPerfeitoCaio;
            *outTotal = ehJu ? NUM_FALAS_FINAL_PERF_JULIANA : NUM_FALAS_FINAL_PERF_CAIO;
            break;
        case SEQ_FINAL_VERDADE:
            *outFalas = ehJu ? falasFinalVerdadeJuliana : falasFinalVerdadeCaio;
            *outTotal = ehJu ? NUM_FALAS_FINAL_VERD_JULIANA : NUM_FALAS_FINAL_VERD_CAIO;
            break;
        case SEQ_FINAL_DUVIDA:
            *outFalas = falasFinalDuvida; *outTotal = NUM_FALAS_FINAL_DUVIDA; break;
        case SEQ_FINAL_COLAPSO:
            *outFalas = falasFinalColapso; *outTotal = NUM_FALAS_FINAL_COLAPSO; break;
        case SEQ_FINAL_PRECIPITADO:
            *outFalas = falasFinalPrecipitado; *outTotal = NUM_FALAS_FINAL_PRECIPITADO; break;
        case SEQ_FINAL_INJUSTICA:
            // Se o culpado era Juliana, o inocente acusado foi Caio (e vice-versa)
            *outFalas = ehJu ? falasFinalInjusticaCaio : falasFinalInjusticaJuliana;
            *outTotal = ehJu ? NUM_FALAS_FINAL_INJ_CAIO : NUM_FALAS_FINAL_INJ_JULIANA;
            break;
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
    if ((int)seq >= 0 && (int)seq < TOTAL_SEQS) e->seqVisitada[(int)seq] = true;
}

static void SalvarConfigEstado(const EstadoJogo *e) {
    ConfigSalva cfg = { e->volMusica, e->volSfx, e->telaCheia, e->modoTechNight };
    SalvarConfig(cfg);
}

static void NovoJogo(EstadoJogo *e, EstadoCelular *cel, EstadoMural *mural, PlayerStats *stats) {
    float vm = e->volMusica, vs = e->volSfx;
    bool tc = e->telaCheia;
    bool tn = e->modoTechNight;
    bool dev = e->devDesbloqueado;
    int jw = e->janelaW, jh = e->janelaH;
    int ultCulp = e->ultimoCulpado;
    char nomeSalvo[24] = "";
    snprintf(nomeSalvo, sizeof(nomeSalvo), "%s", e->nomeJogador);
    memset(e, 0, sizeof(*e));
    snprintf(e->nomeJogador, sizeof(e->nomeJogador), "%s", (nomeSalvo[0] != '\0') ? nomeSalvo : "Helena");
    e->nomeJogadorLen = (int)strlen(e->nomeJogador);
    e->volMusica = vm;
    e->volSfx = vs;
    e->telaCheia = tc;
    e->modoTechNight = tn;
    e->devDesbloqueado = dev;
    e->janelaW = jw;
    e->janelaH = jh;
    // Re-semeia com entropia do instante do clique para garantir runs realmente randomizadas
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(GetTime() * 1000000.0) ^ (unsigned int)clock();
    SetRandomSeed(seed);
    for (int r = 0; r < 8; r++) GetRandomValue(0, 100);

    if (ultCulp < 0) {
        e->culpado = (GetRandomValue(0, 99) < 50) ? CULPADO_JULIANA : CULPADO_CAIO;
    } else {
        e->culpado = (ultCulp == (int)CULPADO_JULIANA) ? CULPADO_CAIO : CULPADO_JULIANA;
    }
    e->ultimoCulpado = (int)e->culpado;
    e->transicaoT = 1.0f;
    e->charAnimT = 1.0f;
    IniciarSequencia(e, SEQ_CENA1_ABERTURA, 1);
    e->telaUltima = TELA_DIALOGO;
    IniciarCelular(cel);
    IniciarMural(mural);
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
    SalvarConfigEstado(e);
}

static void ResetarConfigsTechNight(EstadoJogo *e, PlayerStats *stats) {
    if (!e->modoTechNight) return;
    e->volMusica = 0.45f;
    e->volSfx = 0.80f;
    e->nomeJogador[0] = '\0';
    e->nomeJogadorLen = 0;
    gEfeitoGrid = true;
    gEfeitoGlitch = true;
    ReiniciarProgresso(stats);
    SalvarConfigEstado(e);
}

static void TocarClick(const GerenciadorAssets *g) {
    if (g->temSfxClick) PlaySound(g->sfxClick);
}

static void TocarBrilho(const GerenciadorAssets *g) {
    if (g->temSfxBrilho) PlaySound(g->sfxBrilho);
}

static Color CorDoFalante(const char *falante) {
    if (strcmp(falante, "JOGADOR") == 0 || strcmp(falante, "PENSAMENTO") == 0 ||
        strcmp(falante, "HELENA") == 0 || strcmp(falante, "VOCÊ") == 0 || strcmp(falante, "VOCE") == 0) return COR_CIANO_NEON;
    if (strcmp(falante, "CAIO") == 0) return (Color){ 190, 130, 255, 255 };
    if (strcmp(falante, "JULIANA") == 0) return COR_ROSA_NEON;
    if (strcmp(falante, "ADVOGADA") == 0 || strcmp(falante, "ADVOGADO") == 0) return COR_VERDE_OK;
    return COR_AMARELO_AVISO;
}

static const char *NomeFalanteExibido(const char *falante, const EstadoJogo *e, char *buf, int tam) {
    const char *nome = (e->nomeJogador[0] != '\0') ? e->nomeJogador : "Helena";
    if (strcmp(falante, "JOGADOR") == 0) {
        snprintf(buf, tam, "%s", nome);
        return buf;
    }
    if (strcmp(falante, "PENSAMENTO") == 0) {
        snprintf(buf, tam, "%s (Pensamento)", nome);
        return buf;
    }
    return falante;
}

static void DesenharBarraTopo(FontesUI *f, Vector2 mouse, double tempo, float hoverDos, bool podeAcusar, bool setaMuralTutorial, float toastTimer, const char *toastTxt) {
    if (toastTimer > 0.0f && toastTxt && toastTxt[0] != '\0') {
        float alpha = fminf(1.0f, toastTimer);
        Rectangle rT = { 22, 14, 495, 38 };
        DrawRectangleRounded(rT, 0.2f, 6, (Color){ 16, 36, 48, (unsigned char)(230 * alpha) });
        DrawRectangleRoundedLinesEx(rT, 0.2f, 6, 1.5f, ColorAlpha(COR_VERDE_OK, alpha));
        DrawTextEx(f->bold, toastTxt, (Vector2){ rT.x + 14, rT.y + 9 }, 18, 1.0f, ColorAlpha(COR_VERDE_OK, alpha));
    }
    if (podeAcusar) {
        Rectangle btnAcusar = { 804, 14, 186, 40 };
        float hovAc = CheckCollisionPointRec(mouse, btnAcusar) ? 1.0f : 0.0f;
        DesenharBotaoCyber(f->bold, btnAcusar, "ACUSAR", 21, hovAc, tempo, COR_VERMELHO_ERR);
    }
    Rectangle btnDos  = { 1000, 14, 186, 40 };
    Rectangle btnMenu = { 1196, 14, 62, 40 };
    DesenharBotaoCyber(f->bold, btnDos, "MURAL [D]", 22, hoverDos, tempo, setaMuralTutorial ? COR_AMARELO_AVISO : COR_CIANO_NEON);
    if (setaMuralTutorial) {
        DesenharSetaTutorialMural((Vector2){ 1093, 122 }, (Vector2){ 1093, 60 }, COR_AMARELO_AVISO, tempo);
        DrawTextEx(f->bold, "ABRA O MURAL!", (Vector2){ 1028, 128 }, 17, 1.0f, COR_AMARELO_AVISO);
    }
    bool hovMenu = CheckCollisionPointRec(mouse, btnMenu);
    DrawRectangleRounded(btnMenu, 0.16f, 6, hovMenu ? (Color){ 28, 22, 54, 220 } : (Color){ 14, 10, 28, 175 });
    DrawRectangleRoundedLinesEx(btnMenu, 0.16f, 6, 1.3f, hovMenu ? COR_CIANO_NEON : ColorAlpha(COR_ROXO_NEON, 0.55f));
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

static void DesenharMapaLocalizacao(FontesUI *f, Rectangle area, double tempo) {
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

    Vector2 pinCel = { area.x + area.width * 0.36f, area.y + area.height * 0.40f };
    float radar = fmodf((float)tempo * 28.0f, 38.0f);
    DrawCircleLines((int)pinCel.x, (int)pinCel.y, 10.0f + radar, ColorAlpha(COR_ROSA_NEON, 1.0f - radar / 38.0f));
    DrawCircleLines((int)pinCel.x, (int)pinCel.y, 28, ColorAlpha(COR_ROSA_NEON, 0.65f));
    DrawCircle((int)pinCel.x, (int)pinCel.y, 9, COR_ROSA_NEON);
    DrawRectangleRounded((Rectangle){ pinCel.x + 16, pinCel.y - 42, 248, 60 }, 0.2f, 6, COR_PAINEL);
    DrawRectangleRoundedLinesEx((Rectangle){ pinCel.x + 16, pinCel.y - 42, 248, 60 }, 0.2f, 6, 1.5f, COR_ROSA_NEON);
    DrawTextEx(f->bold, "CELULAR GABRIEL", (Vector2){ pinCel.x + 26, pinCel.y - 36 }, 21, 1.0f, COR_ROSA_NEON);
    DrawTextEx(f->regular, "03:20 | Ultimo: 03:22", (Vector2){ pinCel.x + 26, pinCel.y - 12 }, 20, 1.0f, WHITE);

    Vector2 pinCorpo = { area.x + area.width * 0.42f, area.y + area.height * 0.64f };
    DrawCircle((int)pinCorpo.x, (int)pinCorpo.y, 8, COR_AMARELO_AVISO);
    DrawRectangleRounded((Rectangle){ pinCorpo.x + 16, pinCorpo.y - 12, 258, 58 }, 0.2f, 6, COR_PAINEL);
    DrawRectangleRoundedLinesEx((Rectangle){ pinCorpo.x + 16, pinCorpo.y - 12, 258, 58 }, 0.2f, 6, 1.5f, COR_AMARELO_AVISO);
    DrawTextEx(f->bold, "BARRANCO (200m GALPAO)", (Vector2){ pinCorpo.x + 26, pinCorpo.y - 6 }, 20, 1.0f, COR_AMARELO_AVISO);
    DrawTextEx(f->regular, "3 torres / chip integro", (Vector2){ pinCorpo.x + 26, pinCorpo.y + 18 }, 20, 1.0f, WHITE);
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
        "GABRIEL: \"Você não vai fazer nada, Caio. Você nunca precisou fazer nada na vida.\"\n\n"
        "\"CAIO\": \"Se você abrir a boca, eu juro que acabo com você.\"";
    DesenharTextoQuebrado(f->regular, transcricao, (Rectangle){ area.x + 30, area.y + 118, area.width - 60, 145 }, 24, 1.0f, WHITE);

    Rectangle btnPlay = { area.x + 30, area.y + area.height - 62, 155, 42 };
    bool hoverPlay = CheckCollisionPointRec(mouse, btnPlay);
    DesenharBotaoCyber(f->bold, btnPlay, tocando ? "|| PAUSAR" : "> OUVIR", 22, hoverPlay ? 1.0f : 0.0f, tempo, tocando ? COR_ROSA_NEON : COR_CIANO_NEON);
    DrawTextEx(f->regular, "Duração: 00:12 | Entrega Anônima", (Vector2){ area.x + 198, area.y + area.height - 52 }, 22, 1.0f, LIGHTGRAY);
}

static void AtualizarDevOverlay(EstadoJogo *e, EstadoCelular *cel, EstadoMural *mural, PlayerStats *stats, const GerenciadorAssets *assets, Vector2 mouse) {
    (void)cel;
    (void)stats;
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
        if (CheckCollisionPointRec(mouse, (Rectangle){ 1040, 66, 130, 38 })) {
            TocarClick(assets);
            e->devPainelAberto = false;
            return;
        }

        // Coluna 1: Pular/Voltar Cenas (12 botoes em grid 2x6)
        for (int i = 0; i < 12; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle b = { 112 + col * 235, 168 + row * 56, 220, 44 };
            if (CheckCollisionPointRec(mouse, b)) {
                TocarClick(assets);
                e->menuPausa = false;
                e->devPainelAberto = false;
                e->audioTocando = false;
                switch (i) {
                    case 0: e->tela = TELA_C1_COMPUTADOR; e->cenaAtual = 1; e->podeAcusar = false; break;
                    case 1: IniciarSequencia(e, SEQ_CENA1, 1); e->podeAcusar = false; break;
                    case 2: IniciarSequencia(e, SEQ_CENA2, 2); e->podeAcusar = false; break;
                    case 3: IniciarSequencia(e, SEQ_CENA3, 3); e->podeAcusar = false; break;
                    case 4: IniciarSequencia(e, SEQ_CENA4, 4); e->podeAcusar = false; break;
                    case 5: e->podeAcusar = true; IniciarSequencia(e, SEQ_CENA5, 5); break;
                    case 6: e->podeAcusar = true; IniciarSequencia(e, SEQ_CENA6, 6); break;
                    case 7: e->podeAcusar = true; IniciarSequencia(e, SEQ_CENA7A, 7); break;
                    case 8: e->podeAcusar = true; IniciarSequencia(e, SEQ_CENA7B, 7); break;
                    case 9: e->podeAcusar = false; IniciarSequencia(e, SEQ_CENA8, 8); break;
                    case 10: e->podeAcusar = false; e->tela = TELA_SALA_SUSPEITOS; e->cenaAtual = 8; break;
                    case 11:
                        SincronizarMuralComProvas(mural, (int)e->culpado, e->cenaAtual, e->statusProva);
                        e->telaAnterior = (e->tela == TELA_DOSSIE) ? TELA_MENU : e->tela;
                        e->tela = TELA_DOSSIE;
                        break;
                }
                return;
            }
        }

        // Botoes inferiores da Coluna 1: Reiniciar Run + Modo Tech Night
        if (CheckCollisionPointRec(mouse, (Rectangle){ 112, 520, 220, 44 })) {
            TocarClick(assets);
            e->devPainelAberto = false;
            NovoJogo(e, cel, mural, stats);
            return;
        }
        if (CheckCollisionPointRec(mouse, (Rectangle){ 347, 520, 220, 44 })) {
            TocarClick(assets);
            e->modoTechNight = !e->modoTechNight;
            SalvarConfigEstado(e);
            return;
        }
        // 5 Presets de Click Tecnologico
        for (int c = 0; c < 5; c++) {
            Rectangle bc = { 112 + c * 92, 608, 84, 36 };
            if (CheckCollisionPointRec(mouse, bc)) {
                e->clickPreset = c;
                if (assets->temSfxClicks[c]) PlaySound(assets->sfxClicks[c]);
                else TocarClick(assets);
                return;
            }
        }

        // Coluna 2 Superior: Mural & Rota (4 botoes)
        for (int i = 0; i < 4; i++) {
            Rectangle b = { 625, 168 + i * 50, 540, 42 };
            if (CheckCollisionPointRec(mouse, b)) {
                TocarClick(assets);
                switch (i) {
                    case 0: // Desbloqueia todas SEM ligar automaticamente
                        IniciarMural(mural);
                        for (int k = 3; k <= 7; k++) {
                            int idx = k - 3;
                            e->statusProva[k] = 1;
                            e->respostaProva[k] = RespostaCorretaProva(k, e->culpado);
                            mural->evDesbloqueada[idx] = true;
                            mural->evProcessada[idx] = true;
                        }
                        for (int idx = MURAL_NUM_EV_OFIC; idx < MURAL_NUM_TOTAL; idx++) {
                            mural->evDesbloqueada[idx] = true;
                            mural->evProcessada[idx] = true;
                        }
                        e->notaCaioCena2 = e->notaJulianaCena4 = e->notaGabrielCena5 = e->notaNuvemCena6 = e->notaConfrontoCena7 = true;
                        break;
                    case 1: // Valida e liga todas no mural
                        for (int k = 3; k <= 7; k++) { e->statusProva[k] = 1; e->respostaProva[k] = RespostaCorretaProva(k, e->culpado); }
                        SincronizarMuralComProvas(mural, (int)e->culpado, 8, e->statusProva);
                        for (int idx = 0; idx < MURAL_NUM_EV_OFIC; idx++) {
                            if (!mural->evPosicionada[idx]) {
                                mural->evPosicionada[idx] = true;
                                mural->evLigadaA[idx] = (e->culpado == CULPADO_JULIANA) ? 1 : 0;
                            }
                        }
                        e->notaCaioCena2 = e->notaJulianaCena4 = e->notaGabrielCena5 = e->notaNuvemCena6 = e->notaConfrontoCena7 = true;
                        break;
                    case 2:
                        for (int k = 3; k <= 7; k++) { e->statusProva[k] = 0; e->respostaProva[k] = 0; }
                        IniciarMural(mural);
                        e->notaCaioCena2 = e->notaJulianaCena4 = e->notaGabrielCena5 = e->notaNuvemCena6 = e->notaConfrontoCena7 = false;
                        break;
                    case 3:
                        e->culpado = (e->culpado == CULPADO_JULIANA) ? CULPADO_CAIO : CULPADO_JULIANA;
                        e->ultimoCulpado = (int)e->culpado;
                        break;
                }
                return;
            }
        }

        // Coluna 2 Inferior: Grid 2x3 para rodar os 6 Finais direto
        for (int i = 0; i < 6; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle b = { 625 + col * 276, 412 + row * 50, 264, 42 };
            if (CheckCollisionPointRec(mouse, b)) {
                TocarClick(assets);
                e->devPainelAberto = false;
                e->podeAcusar = false;
                e->tipoFinal = i + 1;
                e->vitoria = (i < 2);
                SequenciaDialogo seqsF[6] = {
                    SEQ_FINAL_PERFEITO, SEQ_FINAL_VERDADE, SEQ_FINAL_DUVIDA,
                    SEQ_FINAL_COLAPSO, SEQ_FINAL_PRECIPITADO, SEQ_FINAL_INJUSTICA
                };
                IniciarSequencia(e, seqsF[i], 8);
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
        DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, 228 });
        DesenharPainelCyber((Rectangle){ 80, 48, 1120, 624 }, COR_VERDE_OK);
        DrawTextEx(f->titulo, "SYS.DEV // CONSOLE DE TESTES", (Vector2){ 112, 70 }, 28, 1.0f, COR_VERDE_OK);
        DesenharBotaoCyber(f->bold, (Rectangle){ 1040, 66, 130, 38 }, "FECHAR [F9]", 19, CheckCollisionPointRec(mouse, (Rectangle){ 1040, 66, 130, 38 }) ? 1.0f : 0.0f, tempo, COR_ROSA_NEON);
        DrawRectangle(112, 116, 1058, 2, ColorAlpha(COR_VERDE_OK, 0.45f));

        DrawTextEx(f->bold, "1. SALTAR PARA CENA:", (Vector2){ 112, 134 }, 21, 1.0f, COR_CIANO_NEON);
        const char *cenasTxt[12] = {
            "C1: Escritório", "C1: Delegado", "C2: Caio", "C3: Pacote",
            "C4: Juliana", "C5: O Corpo", "C6: Nuvem", "C7A: Confronto",
            "C7B: Pós-Saída", "C8: Delegado", "Suspeitos (Algemas)", "Abrir Mural"
        };
        for (int i = 0; i < 12; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle b = { 112 + col * 235, 168 + row * 56, 220, 44 };
            DesenharBotaoCyber(f->bold, b, cenasTxt[i], 20, CheckCollisionPointRec(mouse, b) ? 1.0f : 0.0f, tempo, COR_CIANO_NEON);
        }

        // Botoes inferiores da Coluna 1: Reiniciar Run + Modo Tech Night
        DesenharBotaoCyber(f->bold, (Rectangle){ 112, 520, 220, 44 }, "REINICIAR RUN", 20,
            CheckCollisionPointRec(mouse, (Rectangle){ 112, 520, 220, 44 }) ? 1.0f : 0.0f, tempo, COR_AMARELO_AVISO);
        DesenharBotaoCyber(f->bold, (Rectangle){ 347, 520, 220, 44 },
            e->modoTechNight ? "TECH NIGHT: [ON]" : "TECH NIGHT: [OFF]", 19,
            CheckCollisionPointRec(mouse, (Rectangle){ 347, 520, 220, 44 }) ? 1.0f : 0.0f, tempo,
            e->modoTechNight ? COR_VERDE_OK : GRAY);
        DrawTextEx(f->regular,
            e->modoTechNight ? "Tech Night ON: ao terminar uma run, reseta audio/tela/nome e persiste." : "Tech Night OFF: configuracoes permanecem normais apos a run.",
            (Vector2){ 112, 572 }, 16, 1.0f, LIGHTGRAY);
        DrawTextEx(f->bold, "SOM DE CLICK (5 PRESETS TECH):", (Vector2){ 112, 590 }, 16, 1.0f, COR_CIANO_NEON);
        for (int c = 0; c < 5; c++) {
            Rectangle bc = { 112 + c * 92, 608, 84, 36 };
            bool sel = (e->clickPreset == c);
            DesenharBotaoCyber(f->bold, bc, TextFormat("CLICK %d", c + 1), 16,
                sel ? 0.9f : (CheckCollisionPointRec(mouse, bc) ? 1.0f : 0.0f), tempo,
                sel ? COR_ROSA_NEON : COR_CIANO_NEON);
        }

        DrawTextEx(f->bold, "2. MURAL & ROTA:", (Vector2){ 625, 134 }, 21, 1.0f, COR_ROSA_NEON);
        const char *acoesTxt[4] = {
            "DESBLOQUEAR PROVAS (SEM LIGAR NO MURAL)",
            "VALIDAR + LIGAR TODAS AS 5 PROVAS",
            "RESETAR PROVAS & MURAL",
            (e->culpado == CULPADO_JULIANA) ? "ROTA ATIVA: JULIANA (CLIQUE P/ CAIO)" : "ROTA ATIVA: CAIO (CLIQUE P/ JULIANA)"
        };
        for (int i = 0; i < 4; i++) {
            Rectangle b = { 625, 168 + i * 50, 540, 42 };
            Color cor = (i < 3) ? COR_ROXO_NEON : COR_ROSA_NEON;
            DesenharBotaoCyber(f->bold, b, acoesTxt[i], 19, CheckCollisionPointRec(mouse, b) ? 1.0f : 0.0f, tempo, cor);
        }

        DrawTextEx(f->bold, "3. VER CENAS FINAIS DIRETO:", (Vector2){ 625, 380 }, 21, 1.0f, COR_VERDE_OK);
        const char *finaisTxt[6] = {
            "F1: VERDADE ABSOLUTA", "F2: A VERDADE (4/5)",
            "F3: A DÚVIDA (2-3/5)", "F4: COLAPSO FORENSE",
            "F5: PRECIPITAÇÃO",     "F6: A INJUSTIÇA"
        };
        for (int i = 0; i < 6; i++) {
            int col = i % 2;
            int row = i / 2;
            Rectangle b = { 625 + col * 276, 412 + row * 50, 264, 42 };
            Color cor = (i < 2) ? COR_VERDE_OK : (i < 5) ? COR_AMARELO_AVISO : COR_VERMELHO_ERR;
            DesenharBotaoCyber(f->bold, b, finaisTxt[i], 18, CheckCollisionPointRec(mouse, b) ? 1.0f : 0.0f, tempo, cor);
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
    SetRandomSeed((unsigned int)time(NULL));
    InitAudioDevice();

    RenderTexture2D canvas = LoadRenderTexture(RENDER_W, RENDER_H);
    SetTextureFilter(canvas.texture, TEXTURE_FILTER_BILINEAR);
    Camera2D cam = { 0 };
    cam.zoom = (float)RENDER_W / (float)VIRTUAL_W;

    GerenciadorAssets assets;
    IniciarGerenciadorAssets(&assets);

    EstadoJogo e = {0};
    e.ultimoCulpado = -1;
    e.tela = TELA_MENU;
    e.telaUltima = TELA_MENU;
    e.transicaoT = 1.0f;
    e.charAnimT = 1.0f;
    e.volMusica = 0.45f;
    e.volSfx = 0.80f;

    ConfigSalva cfg = { e.volMusica, e.volSfx, true, false };
    CarregarConfig(&cfg);
    e.volMusica = cfg.volMusica;
    e.volSfx = cfg.volSfx;
    e.modoTechNight = cfg.modoTechNight;
    // Sempre comeca com tela cheia
    AlternarTelaCheia(&e);

    EstadoMiniGame miniGame;
    IniciarMiniGame(&miniGame, 0);

    EstadoCelular celular;
    IniciarCelular(&celular);

    EstadoCutscene cutscene = {0};

    EstadoMural mural = {0};
    IniciarMural(&mural);

    PlayerStats stats = {0};
    strcpy(stats.evidenciaPerdida, "Nenhuma");
    CarregarProgresso(&stats);

    if (assets.temMusicaMenu) PlayMusicStream(assets.musicaMenu);
    if (assets.temMusicaJogo) PlayMusicStream(assets.musicaJogo);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        if (dt > 0.05f) dt = 0.05f;
        double tempo = GetTime();

        bool naAreaMenu = (e.tela == TELA_MENU || e.tela == TELA_CONFIG || e.tela == TELA_CREDITOS || e.tela == TELA_NOME_JOGADOR);
        if (assets.temMusicaMenu) {
            SetMusicVolume(assets.musicaMenu, naAreaMenu ? e.volMusica : 0.0f);
            UpdateMusicStream(assets.musicaMenu);
        }
        if (assets.temMusicaJogo) {
            SetMusicVolume(assets.musicaJogo, !naAreaMenu ? e.volMusica : 0.0f);
            UpdateMusicStream(assets.musicaJogo);
        }
        assets.sfxClick = ObterSfxClick(&assets, e.clickPreset);
        for (int c = 0; c < 5; c++) {
            if (assets.temSfxClicks[c]) SetSoundVolume(assets.sfxClicks[c], e.volSfx);
        }
        if (assets.temSfxClick) SetSoundVolume(assets.sfxClick, e.volSfx);
        if (assets.temSfxBrilho) SetSoundVolume(assets.sfxBrilho, e.volSfx);
        if (assets.temSfxEscritorio) SetSoundVolume(assets.sfxEscritorio, e.volSfx);
        if (assets.temSfxLinkOk) SetSoundVolume(assets.sfxLinkOk, e.volSfx);
        if (assets.temSfxLinkErr) SetSoundVolume(assets.sfxLinkErr, e.volSfx);
        if (e.toastMuralTimer > 0.0f) e.toastMuralTimer -= dt;

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
        bool abriuMuralNesteFrame = false;
        if (emGameplay) {
            if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_M)) e.menuPausa = !e.menuPausa;
            if (IsKeyPressed(KEY_D) && !e.menuPausa && e.tela != TELA_CUTSCENE && e.tela != TELA_FINAL) {
                SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                if (!mural.muralAbertoUmaVez) {
                    mural.abaEsquerda = 0;
                    mural.muralAbertoUmaVez = true;
                }
                e.setaMuralTutorial = false;
                e.telaAnterior = e.tela;
                e.tela = TELA_DOSSIE;
                abriuMuralNesteFrame = true;
            }
        }
        bool usarCursorAlgemas = (e.tela == TELA_SALA_SUSPEITOS && !e.menuPausa && !e.devPainelAberto && !e.devPopupSenha);
        if (usarCursorAlgemas) {
            if (!IsCursorHidden()) HideCursor();
        } else {
            if (IsCursorHidden()) ShowCursor();
        }

        if (IsKeyPressed(KEY_F9)) {
            if (e.devDesbloqueado) {
                e.devPainelAberto = !e.devPainelAberto;
            } else {
                e.devPopupSenha = true;
                e.devSenhaInput[0] = '\0';
                e.devSenhaLen = 0;
                e.devSenhaErro = false;
            }
        }

        // =================== ATUALIZACAO ===================
        if (e.devPopupSenha || e.devPainelAberto) {
            AtualizarDevOverlay(&e, &celular, &mural, &stats, &assets, mouse);
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
            Rectangle btnAcuTop  = { 804, 14, 186, 40 };
            Rectangle btnDosTop  = { 1000, 14, 186, 40 };
            Rectangle btnMenuTop = { 1196, 14, 62, 40 };
            AtualizarHover(&btnDosTop, &e.hoverDossieTop[0], 1, mouse, dt);
            bool clicouBarra = false;

            if (emGameplay && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (e.podeAcusar && CheckCollisionPointRec(mouse, btnAcuTop)) {
                    TocarClick(&assets);
                    e.telaAnterior = e.tela;
                    if (e.cenaAtual < 7) {
                        e.tela = TELA_CONFIRMA_PRECIPITADO;
                    } else {
                        e.podeAcusar = false;
                        e.tela = TELA_VEREDITO;
                    }
                    clicouBarra = true;
                } else if (CheckCollisionPointRec(mouse, btnMenuTop)) {
                    TocarClick(&assets);
                    e.menuPausa = true;
                    clicouBarra = true;
                } else if (CheckCollisionPointRec(mouse, btnDosTop)) {
                    TocarClick(&assets);
                    SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                    mural.abaEsquerda = 0;
                    e.setaMuralTutorial = false;
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
                        if (CheckCollisionPointRec(mouse, menuBtns[0])) {
                            TocarClick(&assets);
                            e.nomeJogador[0] = '\0';
                            e.nomeJogadorLen = 0;
                            e.tela = TELA_NOME_JOGADOR;
                        }
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
                            SalvarConfigEstado(&e);
                        }
                        if (CheckCollisionPointRec(mouse, (Rectangle){ sliderSfx.x - 4, sliderSfx.y - 6, sliderSfx.width + 8, 40 })) {
                            float raw = (mouse.x - sliderSfx.x) / sliderSfx.width;
                            e.volSfx = roundf(fmaxf(0.0f, fminf(1.0f, raw)) * 20.0f) / 20.0f;
                            SalvarConfigEstado(&e);
                        }
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btnMusMenos)) {
                            e.volMusica = fmaxf(0.0f, roundf((e.volMusica - 0.05f) * 20.0f) / 20.0f);
                            SalvarConfigEstado(&e);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, btnMusMais)) {
                            e.volMusica = fminf(1.0f, roundf((e.volMusica + 0.05f) * 20.0f) / 20.0f);
                            SalvarConfigEstado(&e);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, btnSfxMenos)) {
                            e.volSfx = fmaxf(0.0f, roundf((e.volSfx - 0.05f) * 20.0f) / 20.0f);
                            SalvarConfigEstado(&e);
                            TocarClick(&assets);
                        } else if (CheckCollisionPointRec(mouse, btnSfxMais)) {
                            e.volSfx = fminf(1.0f, roundf((e.volSfx + 0.05f) * 20.0f) / 20.0f);
                            SalvarConfigEstado(&e);
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

                case TELA_NOME_JOGADOR: {
                    int key = GetCharPressed();
                    while (key > 0) {
                        if (key >= 32 && key <= 125 && e.nomeJogadorLen < 20) {
                            e.nomeJogador[e.nomeJogadorLen++] = (char)key;
                            e.nomeJogador[e.nomeJogadorLen] = '\0';
                            e.nomeVazioErro = false;
                        }
                        key = GetCharPressed();
                    }
                    if (IsKeyPressed(KEY_BACKSPACE) && e.nomeJogadorLen > 0) {
                        e.nomeJogador[--e.nomeJogadorLen] = '\0';
                        e.nomeVazioErro = false;
                    }
                    Rectangle btnIniciar = { 455, 455, 370, 54 };
                    AtualizarHover(&btnIniciar, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnIniciar)) || IsKeyPressed(KEY_ENTER)) {
                        TocarClick(&assets);
                        if (e.nomeJogadorLen == 0) {
                            e.nomeVazioErro = true;
                        } else {
                            NovoJogo(&e, &celular, &mural, &stats);
                        }
                    } else if (IsKeyPressed(KEY_ESCAPE)) {
                        e.tela = TELA_MENU;
                    }
                } break;

                case TELA_C1_COMPUTADOR: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        if (assets.temSfxEscritorio) PlaySound(assets.sfxEscritorio);
                        else TocarClick(&assets);
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
                        IniciarSequencia(&e, SEQ_CENA1, 1);
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
                    if (IsKeyPressed(KEY_A)) {
                        TocarClick(&assets);
                        e.autoDialogo = !e.autoDialogo;
                        e.autoTimer = 0.0f;
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
                    ObterSequencia(e.seq, e.culpado, &falas, &total);
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

                    // Atualiza notas dinamicas do mural + toast discreto de atualizacao
                    if (e.seq == SEQ_CENA1_ABERTURA && e.falaIdx >= 1 && !mural.evProcessada[9]) {
                        mural.evProcessada[9] = true;
                        mural.evDesbloqueada[9] = true;
                        SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                        e.setaMuralTutorial = true;
                        e.toastMuralTimer = 4.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] NOVA PISTA: Testemunho dos pais de Gabriel [D]");
                    }
                    if (e.seq == SEQ_CENA2 && e.falaIdx >= 5 && !e.notaCaioCena2) {
                        e.notaCaioCena2 = true;
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Depoimento de Caio [D]");
                    }
                    if (e.seq == SEQ_CENA4 && e.falaIdx >= 6 && !e.notaJulianaCena4) {
                        e.notaJulianaCena4 = true;
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Depoimento de Juliana [D]");
                    }
                    if (e.seq == SEQ_CENA5 && e.falaIdx >= 2 && !e.notaGabrielCena5) {
                        e.notaGabrielCena5 = true;
                        e.podeAcusar = true;
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Laudo do corpo [D]");
                    }
                    if (e.seq == SEQ_CENA6_POS && !e.notaNuvemCena6) {
                        e.notaNuvemCena6 = true;
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Backup da nuvem [D]");
                    }
                    if (e.seq == SEQ_CENA7B && e.falaIdx >= 5 && !e.notaConfrontoCena7) {
                        e.notaConfrontoCena7 = true;
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Anotações finais [D]");
                    }

                    bool autoAvancou = (e.autoDialogo && e.falaCompleta && e.autoTimer >= 3.0f);
                    bool devSkipDlg = (e.devDesbloqueado && IsKeyPressed(KEY_S));
                    bool clicouAvancar = (!clicouBotaoCaixa && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse.y > 65) ||
                                         IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || devSkipDlg;
                    if (clicouAvancar || autoAvancou) {
                        if (devSkipDlg) e.falaIdx = total - 1;
                        if (!e.falaCompleta && !autoAvancou && !devSkipDlg) {
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
                                if (e.seq == SEQ_CENA1_ABERTURA) {
                                    e.tela = TELA_C1_COMPUTADOR;
                                } else if (e.seq == SEQ_CENA1) {
                                    e.tela = TELA_C1_INTERROGATORIO;
                                } else if (e.seq == SEQ_CENA2) {
                                    e.toastMuralTimer = 3.5f;
                                    snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Nova pista disponível [D]");
                                    IniciarSequencia(&e, SEQ_CENA3, 3);
                                } else if (e.seq == SEQ_CENA3_POS) {
                                    IniciarSequencia(&e, SEQ_CENA4, 4);
                                } else if (e.seq == SEQ_CENA4_POS) {
                                    e.podeAcusar = true;
                                    IniciarSequencia(&e, SEQ_CENA5, 5);
                                } else if (e.seq == SEQ_CENA6_POS) {
                                    IniciarSequencia(&e, SEQ_CENA7A, 7);
                                } else if (e.seq == SEQ_CENA7B) {
                                    e.podeAcusar = false;
                                    IniciarSequencia(&e, SEQ_CENA8, 8);
                                } else if (e.seq == SEQ_CENA8) {
                                    e.podeAcusar = false;
                                    e.tela = TELA_SALA_SUSPEITOS;
                                } else if (e.seq == SEQ_FINAL_PERFEITO) {
                                    e.vitoria = true; e.tipoFinal = 1;
                                    MontarCutsceneFinal(&cutscene, e.tipoFinal, (int)e.culpado, e.statusProva, e.respostaProva);
                                    e.tela = TELA_CUTSCENE;
                                } else if (e.seq == SEQ_FINAL_VERDADE) {
                                    e.vitoria = true; e.tipoFinal = 2;
                                    MontarCutsceneFinal(&cutscene, e.tipoFinal, (int)e.culpado, e.statusProva, e.respostaProva);
                                    e.tela = TELA_CUTSCENE;
                                } else if (e.seq == SEQ_FINAL_DUVIDA) {
                                    e.vitoria = false; e.tipoFinal = 3;
                                    MontarCutsceneFinal(&cutscene, e.tipoFinal, (int)e.culpado, e.statusProva, e.respostaProva);
                                    e.tela = TELA_CUTSCENE;
                                } else if (e.seq == SEQ_FINAL_COLAPSO) {
                                    e.vitoria = false; e.tipoFinal = 4;
                                    MontarCutsceneFinal(&cutscene, e.tipoFinal, (int)e.culpado, e.statusProva, e.respostaProva);
                                    e.tela = TELA_CUTSCENE;
                                } else if (e.seq == SEQ_FINAL_PRECIPITADO) {
                                    e.vitoria = false; e.tipoFinal = 5;
                                    MontarCutsceneFinal(&cutscene, e.tipoFinal, (int)e.culpado, e.statusProva, e.respostaProva);
                                    e.tela = TELA_CUTSCENE;
                                } else if (e.seq == SEQ_FINAL_INJUSTICA) {
                                    e.vitoria = false; e.tipoFinal = 6;
                                    MontarCutsceneFinal(&cutscene, e.tipoFinal, (int)e.culpado, e.statusProva, e.respostaProva);
                                    e.tela = TELA_CUTSCENE;
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
                        IniciarMiniGame(&miniGame, e.cenaAtual);
                        e.tela = TELA_MINIGAME;
                    }
                } break;

                case TELA_MINIGAME: {
                    int res = AtualizarMiniGame(&miniGame, e.cenaAtual, mouse, dt, assets.sfxClick, assets.temSfxClick);
                    if (res == 1) {
                        e.statusProva[e.cenaAtual] = 1;
                        SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Prova desbloqueada [D]");
                        stats.acertos++;
                        SalvarProgresso(stats);
                        TocarBrilho(&assets);
                        e.tela = TELA_EVIDENCIA_DESBLOQUEADA;
                    } else if (res == -1) {
                        e.statusProva[e.cenaAtual] = -1;
                        SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                        e.toastMuralTimer = 3.5f;
                        snprintf(e.toastMuralTxt, sizeof(e.toastMuralTxt), "[!] MURAL ATUALIZADO: Prova não validada [D]");
                        stats.erros++;
                        snprintf(stats.evidenciaPerdida, sizeof(stats.evidenciaPerdida), "Prova %d (Cena %d)", e.cenaAtual - 2, e.cenaAtual);
                        SalvarProgresso(stats);
                        e.tela = TELA_EVIDENCIA_PERDIDA;
                    }
                } break;

                case TELA_CONFIRMA_PRECIPITADO: {
                    Rectangle btns[2] = { { 290, 420, 325, 52 }, { 665, 420, 325, 52 } };
                    AtualizarHover(btns, e.hoverSimNao, 2, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btns[0])) {
                            TocarClick(&assets);
                            e.podeAcusar = false;
                            e.tipoFinal = 5;
                            IniciarSequencia(&e, SEQ_FINAL_PRECIPITADO, 8);
                        } else if (CheckCollisionPointRec(mouse, btns[1])) {
                            TocarClick(&assets);
                            e.tela = e.telaAnterior;
                        }
                    }
                } break;

                case TELA_EVIDENCIA_PERDIDA: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) {
                        TocarClick(&assets);
                        if (e.cenaAtual == 3) IniciarSequencia(&e, SEQ_CENA4, 4);
                        else if (e.cenaAtual == 4) { e.podeAcusar = true; IniciarSequencia(&e, SEQ_CENA5, 5); }
                        else if (e.cenaAtual == 5) IniciarSequencia(&e, SEQ_CENA6, 6);
                        else if (e.cenaAtual == 6) IniciarSequencia(&e, SEQ_CENA7A, 7);
                        else if (e.cenaAtual == 7) IniciarSequencia(&e, SEQ_CENA7B, 7);
                    }
                } break;

                case TELA_EVIDENCIA_DESBLOQUEADA: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) {
                        TocarClick(&assets);
                        e.audioTocando = false;
                        e.audioTempo = 0.0f;
                        e.analiseTempoRestante = 30.0f;
                        e.analiseTempoEsgotado = false;
                        e.tela = TELA_ANALISE_PROVA;
                    }
                } break;

                case TELA_ANALISE_PROVA: {
                    if (e.cenaAtual == 3) {
                        Rectangle btnPlay = { 145, 443, 155, 42 };
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnPlay)) {
                            TocarClick(&assets);
                            e.audioTocando = !e.audioTocando;
                        }
                    }
                    if (e.respostaProva[e.cenaAtual] == 0 && !e.analiseTempoEsgotado) {
                        e.analiseTempoRestante -= dt;
                        if (e.analiseTempoRestante <= 0.0f) {
                            e.analiseTempoRestante = 0.0f;
                            e.analiseTempoEsgotado = true;
                        }
                    }
                    Rectangle btns[3] = {
                        { 655, 446, 230, 54 },
                        { 905, 446, 230, 54 },
                        { 655, 542, 480, 54 }
                    };
                    AtualizarHover(btns, e.hoverAnalise, 3, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (e.respostaProva[e.cenaAtual] == 0 && !e.analiseTempoEsgotado) {
                            if (CheckCollisionPointRec(mouse, btns[0])) { TocarClick(&assets); e.respostaProva[e.cenaAtual] = 1; }
                            else if (CheckCollisionPointRec(mouse, btns[1])) { TocarClick(&assets); e.respostaProva[e.cenaAtual] = 2; }
                        } else if (CheckCollisionPointRec(mouse, btns[2])) {
                            TocarClick(&assets);
                            e.audioTocando = false;
                            bool acertou = (e.respostaProva[e.cenaAtual] == RespostaCorretaProva(e.cenaAtual, e.culpado));
                            if (e.cenaAtual == 3) {
                                if (acertou) IniciarSequencia(&e, SEQ_CENA3_POS, 3);
                                else IniciarSequencia(&e, SEQ_CENA4, 4);
                            } else if (e.cenaAtual == 4) {
                                if (acertou) IniciarSequencia(&e, SEQ_CENA4_POS, 4);
                                else { e.podeAcusar = true; IniciarSequencia(&e, SEQ_CENA5, 5); }
                            } else if (e.cenaAtual == 5) {
                                IniciarSequencia(&e, SEQ_CENA6, 6);
                            } else if (e.cenaAtual == 6) {
                                IniciarSequencia(&e, SEQ_CENA6_POS, 6);
                            } else if (e.cenaAtual == 7) {
                                IniciarSequencia(&e, SEQ_CENA7B, 7);
                            }
                        }
                    }
                } break;

                case TELA_SALA_SUSPEITOS: {
                    Rectangle caioHit = { 120, 140, 340, 560 };
                    Rectangle juHit   = { 820, 140, 340, 560 };
                    Rectangle btnMural = { 495, 636, 290, 46 };
                    Rectangle ops[3] = { caioHit, juHit, btnMural };
                    AtualizarHover(ops, e.hoverVeredito, 3, mouse, dt);
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        if (CheckCollisionPointRec(mouse, btnMural)) {
                            TocarClick(&assets);
                            SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                            if (!mural.muralAbertoUmaVez) {
                                mural.abaEsquerda = 0;
                                mural.muralAbertoUmaVez = true;
                            }
                            e.telaAnterior = TELA_SALA_SUSPEITOS;
                            e.tela = TELA_DOSSIE;
                        } else if (CheckCollisionPointRec(mouse, caioHit)) {
                            TocarClick(&assets);
                            e.acusadoFinal = 1;
                            e.tela = TELA_CONFIRMA_VEREDITO;
                        } else if (CheckCollisionPointRec(mouse, juHit)) {
                            TocarClick(&assets);
                            e.acusadoFinal = 2;
                            e.tela = TELA_CONFIRMA_VEREDITO;
                        }
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
                        else if (CheckCollisionPointRec(mouse, ops[2])) {
                            TocarClick(&assets);
                            SincronizarMuralComProvas(&mural, (int)e.culpado, e.cenaAtual, e.statusProva);
                            e.telaAnterior = TELA_VEREDITO;
                            e.tela = TELA_DOSSIE;
                        }
                    }
                } break;

                case TELA_CONFIRMA_VEREDITO: {
                    Rectangle btns[2] = { { 290, 566, 300, 52 }, { 690, 566, 300, 52 } };
                    AtualizarHover(btns, e.hoverSimNao, 2, mouse, dt);
                    bool sim = IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1) ||
                               (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btns[0]));
                    bool nao = IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_ESCAPE) ||
                               (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btns[1]));
                    if (sim) {
                        TocarClick(&assets);
                        e.podeAcusar = false;
                        bool acusouCorreto = (e.acusadoFinal == 2 && e.culpado == CULPADO_JULIANA) ||
                                             (e.acusadoFinal == 1 && e.culpado == CULPADO_CAIO);
                        if (!acusouCorreto) {
                            e.vitoria = false;
                            e.tipoFinal = 6;
                            IniciarSequencia(&e, SEQ_FINAL_INJUSTICA, 8);
                        } else {
                            int provasLogVal = ContarProvasValidadas(&e, &mural);
                            int ambCorretas = ContarAmbiguasCorretas(&e, &mural);
                            bool todasLogicas = AcertouTodasLogicas(&e);
                            if (todasLogicas && provasLogVal >= 3 && ambCorretas == MURAL_NUM_AMB) {
                                e.vitoria = true;
                                e.tipoFinal = 1;
                                IniciarSequencia(&e, SEQ_FINAL_PERFEITO, 8);
                            } else if (provasLogVal >= 2 && ambCorretas >= 1) {
                                e.vitoria = true;
                                e.tipoFinal = 2;
                                IniciarSequencia(&e, SEQ_FINAL_VERDADE, 8);
                            } else if (provasLogVal >= 1 || ambCorretas >= 1) {
                                e.vitoria = false;
                                e.tipoFinal = 3;
                                IniciarSequencia(&e, SEQ_FINAL_DUVIDA, 8);
                            } else {
                                e.vitoria = false;
                                e.tipoFinal = 4;
                                IniciarSequencia(&e, SEQ_FINAL_COLAPSO, 8);
                            }
                        }
                    } else if (nao) {
                        TocarClick(&assets);
                        e.tela = TELA_SALA_SUSPEITOS;
                    }
                } break;

                case TELA_CUTSCENE: {
                    if (AtualizarCutscene(&cutscene, mouse, dt, e.devDesbloqueado, assets.sfxClick, assets.temSfxClick) == 1) {
                        e.tela = TELA_FINAL;
                    }
                } break;

                case TELA_FINAL: {
                    Rectangle btn = { 455, 455, 370, 54 };
                    AtualizarHover(&btn, &e.hoverBtnUnico, 1, mouse, dt);
                    if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btn)) ||
                        IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                        TocarClick(&assets);
                        ResetarConfigsTechNight(&e, &stats);
                        e.tela = TELA_MENU;
                    }
                } break;

                case TELA_DOSSIE: {
                    if (!abriuMuralNesteFrame &&
                        AtualizarMural(&mural, (int)e.culpado, e.cenaAtual, e.statusProva, mouse, dt, assets.sfxClick, assets.temSfxClick, assets.sfxLinkOk, assets.temSfxLinkOk, assets.sfxLinkErr, assets.temSfxLinkErr) == 1) {
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

            case TELA_NOME_JOGADOR: {
                DesenharFundoCyberGrid(tempo, COR_CIANO_NEON);
                DesenharPainelCyber((Rectangle){ 260, 140, 760, 420 }, COR_CIANO_NEON);
                DrawTextEx(assets.fontes.titulo, "IDENTIFICAÇÃO DO(A) INVESTIGADOR(A)", (Vector2){ 315, 185 }, 28, 1.0f, COR_CIANO_NEON);
                DrawTextEx(assets.fontes.regular, "Digite seu nome para os registros do caso (obrigatório):", (Vector2){ 315, 240 }, 22, 1.0f, LIGHTGRAY);

                Rectangle box = { 340, 300, 600, 58 };
                DrawRectangleRec(box, (Color){ 14, 10, 28, 255 });
                DrawRectangleLinesEx(box, 2.0f, e.nomeVazioErro ? COR_VERMELHO_ERR : COR_ROSA_NEON);

                char disp[32] = "";
                snprintf(disp, sizeof(disp), "%s%s", e.nomeJogador, ((int)(tempo * 3.0) % 2 == 0) ? "_" : "");
                DrawTextEx(assets.fontes.titulo, disp, (Vector2){ box.x + 18, box.y + 14 }, 28, 1.5f, WHITE);

                if (e.nomeVazioErro) {
                    DrawTextEx(assets.fontes.bold, "Digite um nome para iniciar a investigação!", (Vector2){ 340, 372 }, 21, 1.0f, COR_VERMELHO_ERR);
                }

                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 455, 455, 370, 54 }, "INICIAR INVESTIGAÇÃO >>", 24, e.hoverBtnUnico, tempo, COR_VERDE_OK);
            } break;

            case TELA_C1_COMPUTADOR: {
                Texture2D bgEsc = ObterTexturaCache(&assets.fundo, BG_ESCRITORIO);
                DesenharFundoCyberGrid(tempo, COR_CIANO_NEON);
                if (bgEsc.id != 0) {
                    DesenharTexturaCover(bgEsc, (Rectangle){ 0, 0, VIRTUAL_W, VIRTUAL_H }, WHITE);
                    DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 8, 5, 18, 110 });
                }
                Rectangle card = { 210, 140, 860, 440 };
                DesenharPainelCyber(card, COR_CIANO_NEON);
                Vector2 szTit = MeasureTextEx(assets.fontes.titulo, "TERMINAL DA INVESTIGAÇÃO", 38, 1.0f);
                DrawTextEx(assets.fontes.titulo, "TERMINAL DA INVESTIGAÇÃO", (Vector2){ card.x + (card.width - szTit.x) * 0.5f, card.y + 62 }, 38, 1.0f, WHITE);
                DrawRectangle((int)(card.x + 110), (int)(card.y + 118), (int)(card.width - 220), 2, ColorAlpha(COR_CIANO_NEON, 0.45f));
                DesenharTextoQuebrado(assets.fontes.regular,
                    "O Delegado Meireles saiu da sala.\nSente-se à mesa e acesse o computador para consultar os dados iniciais da denúncia.",
                    (Rectangle){ card.x + 70, card.y + 154, card.width - 140, 140 }, 26, 1.0f, LIGHTGRAY);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 455, 455, 370, 54 }, "ACESSAR COMPUTADOR >>", 24, e.hoverBtnUnico, tempo, COR_VERDE_OK);
            } break;

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
                DrawTextEx(assets.fontes.bold, "CIÊNCIA DA COMPUTAÇÃO | RECIFE - PE", (Vector2){ 195, 215 }, 27, 1.0f, WHITE);
                DrawTextEx(assets.fontes.bold, "ÚLTIMA LOCALIZAÇÃO:", (Vector2){ 195, 282 }, 26, 1.0f, COR_AMARELO_AVISO);
                DrawTextEx(assets.fontes.regular, "Festa Universitária", (Vector2){ 195, 318 }, 28, 1.0f, WHITE);
                DrawTextEx(assets.fontes.bold, "HORÁRIO ESTIMADO:", (Vector2){ 195, 385 }, 26, 1.0f, COR_AMARELO_AVISO);
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

                DesenharBadgeCyber(assets.fontes.titulo, (Vector2){ 195, 135 }, "ÚLTIMA INFORMAÇÃO", 28, COR_CIANO_NEON);
                DesenharTextoQuebrado(assets.fontes.bold, "Foi visto discutindo com Caio Vasconcelos.\n\nCaio está sendo levado para a delegacia para questionamento.", (Rectangle){ 195, 224, 535, 220 }, 28, 1.0f, WHITE);
                float p = 0.55f + 0.45f * sinf((float)tempo * 4.5f);
                DrawTextEx(assets.fontes.bold, "CLIQUE PARA CONTINUAR >>", (Vector2){ 195, 552 }, 24, 1.0f, ColorAlpha(COR_CIANO_NEON, p));
            } break;

            case TELA_C1_INTERROGATORIO:
                DesenharCardCentral(&assets.fontes, "INTERROGATÓRIO", "Hora de ouvir os envolvidos e confrontar cada versão dos fatos.", "IR INTERROGAR CAIO >>", e.hoverBtnUnico, tempo, COR_ROXO_NEON);
                break;

            case TELA_DIALOGO: {
                const Fala *falas = NULL;
                int total = 0;
                ObterSequencia(e.seq, e.culpado, &falas, &total);
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
                bool ehCorpoInteiro = (atual->personagem[0] != '\0') &&
                    (strstr(atual->personagem, "inteiro") != NULL ||
                     strstr(atual->personagem, "corpo") != NULL ||
                     strstr(atual->personagem, "Advogada") != NULL);
                if (ehCorpoInteiro) {
                    // Estilo Doki Doki: zoom maior, corta as pernas para baixo da tela (sem flutuar)
                    DesenharTexturaContain(ch, (Rectangle){ 220 + slideX, 22 + floatY, 840, 1080 }, tintChar);
                } else {
                    DesenharTexturaContain(ch, (Rectangle){ 190 + slideX, 12 + floatY, 900, 665 }, tintChar);
                }

                // Caixa de dialogo proporcional (recorte exato da area nao-transparente de CaixaDialogo.png)
                Rectangle boxDest = { 60, 446, 1160, 260 };
                if (assets.caixaDialogo.id != 0) {
                    Rectangle srcBox = { 0, 68, (float)assets.caixaDialogo.width, (float)assets.caixaDialogo.height - 77.0f };
                    DrawTexturePro(assets.caixaDialogo, srcBox, boxDest, (Vector2){0, 0}, 0.0f, WHITE);
                } else {
                    DesenharPainelCyber(boxDest, COR_ROXO_NEON);
                }

                // Nome do falante com efeito leve de tecnologia
                char nomeBuf[48];
                const char *nomeExibido = NomeFalanteExibido(atual->falante, &e, nomeBuf, sizeof(nomeBuf));
                Color corNome = CorDoFalante(atual->falante);
                Vector2 szNome = MeasureTextEx(assets.fontes.bold, nomeExibido, 25, 1.2f);
                float abaCenterX = boxDest.x + 282.0f * (boxDest.width / 1261.0f);
                float jitterTech = (fmodf((float)tempo, 2.8f) < 0.14f) ? sinf((float)tempo * 90.0f) * 1.8f : 0.0f;
                DrawTextEx(assets.fontes.bold, nomeExibido, (Vector2){ abaCenterX - szNome.x * 0.5f + 2.0f + jitterTech, boxDest.y + 25.0f }, 25, 1.2f, ColorAlpha(COR_ROSA_NEON, 0.35f));
                DrawTextEx(assets.fontes.bold, nomeExibido, (Vector2){ abaCenterX - szNome.x * 0.5f, boxDest.y + 24.0f }, 25, 1.2f, corNome);

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
                DrawTextEx(assets.fontes.bold, e.autoDialogo ? "AUTO [A]: ON" : "AUTO [A]: OFF", (Vector2){ btnAuto.x + 9.0f, btnAuto.y + 5.0f }, 18, 1.0f, e.autoDialogo ? COR_CIANO_NEON : WHITE);

                float recPulso = 0.45f + 0.55f * sinf((float)tempo * 5.0f);
                DrawCircle((int)(boxDest.x + boxDest.width - 135.0f), (int)(boxDest.y + 34.0f), 5.0f, ColorAlpha(COR_ROSA_NEON, recPulso));
                DrawTextEx(assets.fontes.bold, "REC", (Vector2){ boxDest.x + boxDest.width - 122.0f, boxDest.y + 22.0f }, 22, 1.0f, ColorAlpha(WHITE, 0.85f));

                // Texto mais abaixo e melhor centralizado dentro da caixa
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
                DesenharTextoQuebrado(assets.fontes.regular, trecho, (Rectangle){ boxDest.x + 86.0f, boxDest.y + 118.0f, boxDest.width - 172.0f, 112.0f }, 29, 1.0f, WHITE);

                if (e.historicoAberto) {
                    DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, 215 });
                    DesenharPainelCyber((Rectangle){ 110, 72, 1060, 576 }, COR_ROXO_NEON);
                    DrawTextEx(assets.fontes.titulo, "HISTORICO DE CONVERSAS", (Vector2){ 150, 92 }, 28, 1.0f, COR_CIANO_NEON);
                    Rectangle btnFecharHist = { 980, 88, 150, 40 };
                    DesenharBotaoCyber(assets.fontes.bold, btnFecharHist, "FECHAR [H]", 20, CheckCollisionPointRec(mouse, btnFecharHist) ? 1.0f : 0.0f, tempo, COR_ROSA_NEON);
                    DrawRectangle(150, 138, 980, 2, ColorAlpha(COR_ROXO_NEON, 0.45f));

                    // Calcula altura total e desenha com scissor
                    float totalH = 0.0f;
                    for (int s = 0; s < TOTAL_SEQS; s++) {
                        if (!e.seqVisitada[s] && (SequenciaDialogo)s != e.seq) continue;
                        const Fala *sf = NULL;
                        int stot = 0;
                        ObterSequencia((SequenciaDialogo)s, e.culpado, &sf, &stot);
                        int lim = ((SequenciaDialogo)s == e.seq) ? (e.falaIdx + 1) : stot;
                        for (int k = 0; k < lim; k++) totalH += 72.0f;
                    }
                    float maxScroll = fmaxf(0.0f, totalH - 460.0f);
                    if (e.historicoScroll > maxScroll) e.historicoScroll = maxScroll;

                    BeginScissorMode((int)(145 * cam.zoom), (int)(150 * cam.zoom), (int)(990 * cam.zoom), (int)(475 * cam.zoom));
                    float cy = 154.0f - e.historicoScroll;
                    for (int s = 0; s < TOTAL_SEQS; s++) {
                        if (!e.seqVisitada[s] && (SequenciaDialogo)s != e.seq) continue;
                        const Fala *sf = NULL;
                        int stot = 0;
                        ObterSequencia((SequenciaDialogo)s, e.culpado, &sf, &stot);
                        int lim = ((SequenciaDialogo)s == e.seq) ? (e.falaIdx + 1) : stot;
                        for (int k = 0; k < lim; k++) {
                            if (cy + 70.0f >= 145.0f && cy <= 630.0f) {
                                char hNomeBuf[48];
                                const char *hNome = NomeFalanteExibido(sf[k].falante, &e, hNomeBuf, sizeof(hNomeBuf));
                                Color cf = CorDoFalante(sf[k].falante);
                                DrawTextEx(assets.fontes.bold, hNome, (Vector2){ 155, cy }, 20, 1.0f, cf);
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
                DesenharCardCentral(&assets.fontes, "NOVA EVIDÊNCIA DISPONÍVEL", "Uma nova pista pode mudar o rumo da investigação.\nResolva o desafio de lógica em até 20 segundos para desbloqueá-la.", "DESBLOQUEAR EVIDÊNCIA >>", e.hoverBtnUnico, tempo, COR_ROXO_NEON);
                break;

            case TELA_MINIGAME:
                DesenharMiniGame(&miniGame, &assets.fontes, e.cenaAtual, tempo);
                break;

            case TELA_CONFIRMA_PRECIPITADO:
                DesenharCardCentral(&assets.fontes, "ACUSAR ANTES DO FIM?", "Você ainda não concluiu todas as perícias nem o confronto final.\nTem certeza que quer formalizar a acusação agora?", NULL, 0, tempo, COR_AMARELO_AVISO);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 290, 420, 325, 54 }, "[1] SIM, ACUSAR AGORA", 23, e.hoverSimNao[0], tempo, COR_VERMELHO_ERR);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 665, 420, 325, 54 }, "[2] NÃO, CONTINUAR", 23, e.hoverSimNao[1], tempo, COR_CIANO_NEON);
                break;

            case TELA_EVIDENCIA_PERDIDA:
                DesenharCardCentral(&assets.fontes, "EVIDÊNCIA NÃO VALIDADA", "Você errou o desafio lógico.\nA pista foi para o Mural em vermelho (Não Validada) e pode ser ligada, mas não valida a lógica.", "CONTINUAR >>", e.hoverBtnUnico, tempo, COR_VERMELHO_ERR);
                break;

            case TELA_EVIDENCIA_DESBLOQUEADA:
                DesenharCardCentral(&assets.fontes, "EVIDÊNCIA DESBLOQUEADA", "Pista liberada para análise na Lupa Forense.\nVocê terá 30 segundos para classificá-la como Real ou Gerada por IA.", "ABRIR LUPA FORENSE >>", e.hoverBtnUnico, tempo, COR_VERDE_OK);
                break;

            case TELA_ANALISE_PROVA: {
                DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);
                DesenharPainelCyber((Rectangle){ 75, 68, 1130, 604 }, COR_ROXO_NEON);

                bool ehJu = (e.culpado == CULPADO_JULIANA);
                const char *titulo = "ANÁLISE DE EVIDÊNCIA";
                const char *desc = "";
                bool ehManipulada = (RespostaCorretaProva(e.cenaAtual, e.culpado) == 2);
                const char *motivo = "";
                Rectangle imgRect = { 115, 145, 500, 385 };
                bool temScannerImg = false;

                if (e.cenaAtual == 3) {
                    titulo = "PROVA 1 - ÁUDIO ANÔNIMO";
                    DesenharPlayerAudio(&assets.fontes, (Rectangle){ 115, 145, 500, 360 }, e.audioTocando, e.audioTempo, mouse);
                    if (ehJu) {
                        desc = "LUPA FORENSE: Nenhuma respiração na voz de \"Caio\"; música de fundo em loop a cada 4s; corte seco em frequências altas; sotaque oscila no meio da frase.";
                        motivo = "Gerada por IA: ausência de respiração e corte seco nas frequências.";
                    } else {
                        desc = "LUPA FORENSE: Ruído de vento natural contínuo; respiração ofegante compatível com esforço; assinatura acústica do ambiente sem cortes; frequência vocal estável.";
                        motivo = "Prova Real: gravação limpa, sem sinais de síntese ou montagem.";
                    }
                } else if (e.cenaAtual == 4) {
                    titulo = "PROVA 2 - FOTO DE JULIANA NO CARRO (03:05)";
                    const char *fotoP2 = ehJu ? "AssetsRay/Personagens/Julia/novodeepfake.png" : "AssetsRay/Personagens/Julia/juliafotoreal.png";
                    DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, fotoP2), imgRect, WHITE);
                    temScannerImg = true;
                    if (ehJu) {
                        desc = "LUPA FORENSE: O carro tem volante (os autônomos da região não têm); o brinco esquerdo some no reflexo da janela; texto do painel embaralhado; horário editado.";
                        motivo = "Gerada por IA: volante inexistente na frota, reflexo falho e horário adulterado.";
                    } else {
                        desc = "LUPA FORENSE: Veículo autônomo sem volante (padrão da frota local); reflexo da janela coerente com a iluminação externa; metadados originais da câmera intactos.";
                        motivo = "Prova Real: iluminação, reflexo e metadados confirmam o horário 03:05.";
                    }
                } else if (e.cenaAtual == 5) {
                    titulo = "PROVA 3 - LOCALIZAÇÃO DO CELULAR DE GABRIEL";
                    desc = "LUPA FORENSE: Celular parado no barranco a 200m do galpão às 03:20 (último sinal 03:22). Dados batem com três torres; bateria coerente; chip íntegro; metadados sem edição.";
                    DesenharMapaLocalizacao(&assets.fontes, imgRect, tempo);
                    motivo = "Prova Real: Gabriel nunca saiu da festa. Morreu no local antes das 03:05.";
                } else if (e.cenaAtual == 6) {
                    titulo = "PROVA 4 - ARQUIVO OCULTO NA NUVEM";
                    DesenharPainelCyber((Rectangle){ 115, 145, 500, 385 }, COR_CIANO_NEON);
                    if (ehJu) {
                        const char *nuvemTxt =
                            "RASCUNHO DE E-MAIL // 02:15\n"
                            "Para: Comitê de Ética da Faculdade\n\n"
                            "\"Não consigo mais segurar isso. Entreguei o código-fonte do projeto da Juliana pro prof. João Paulo em troca da vaga na NEXARA. Vou contar tudo pra ela hoje.\"";
                        DesenharTextoQuebrado(assets.fontes.regular, nuvemTxt, (Rectangle){ 142, 172, 446, 335 }, 23, 1.0f, WHITE);
                        desc = "LUPA FORENSE: Timestamp do servidor validado; IP de origem compatível com o Wi-Fi da festa; histórico de digitação orgânico.";
                        motivo = "Prova Real: Gabriel confessou ter roubado o projeto de três anos de Juliana.";
                    } else {
                        const char *nuvemTxt =
                            "PLANILHA CRIPTOGRAFADA // EXTRATOS\n\n"
                            "Origem: Bolsas falsas (Vasconcelos Holdings)\n"
                            "Conta intermediária: Gabriel Moura\n"
                            "Destino final: Empresa de fachada (Caio V.)\n"
                            "Status: Comprovantes de transferência intactos";
                        DesenharTextoQuebrado(assets.fontes.regular, nuvemTxt, (Rectangle){ 142, 172, 446, 335 }, 23, 1.0f, WHITE);
                        desc = "LUPA FORENSE: Assinatura criptográfica do Banco Central válida; chaves de autenticação íntegras; documentos sem alteração.";
                        motivo = "Prova Real: o pai de Caio usava Gabriel como laranja para lavar dinheiro.";
                    }
                } else if (e.cenaAtual == 7) {
                    if (ehJu) {
                        titulo = "PROVA 5 - CORRIDA DE JULIANA (MOBI)";
                        desc = "LUPA FORENSE: Assinatura da Mobi válida; trilha de GPS contínua; nenhuma outra corrida registrada na conta naquela noite.";
                        DesenharPainelCyber((Rectangle){ 115, 145, 500, 385 }, COR_CIANO_NEON);
                        const char *mobiTxt =
                            "MOBI // REGISTRO OFICIAL DE CORRIDA\n\n"
                            "Conta: Juliana Costa\n"
                            "Solicitada às: 03:41\n"
                            "Embarque: Estrada de serviço (ao lado do galpão)\n"
                            "Veículo: Autônomo, SEM volante\n"
                            "Passageiros: 1";
                        DesenharTextoQuebrado(assets.fontes.regular, mobiTxt, (Rectangle){ 142, 175, 446, 330 }, 24, 1.0f, WHITE);
                        motivo = "Prova Real: derruba o álibi das 03:00 e o carro com volante da foto.";
                    } else {
                        titulo = "PROVA 5 - CÂMERA DO ESTACIONAMENTO (02:42)";
                        DesenharTexturaContain(ObterTexturaCache(&assets.evidencia, "AssetsRay/Evidências/caiodeepfake.png"), imgRect, WHITE);
                        temScannerImg = true;
                        desc = "LUPA FORENSE: Carro de Caio na cancela às 02:42. Placa com caracteres distorcidos; sombra dos postes no ângulo errado para o horário; ruído digital injetado sobre o frame original.";
                        motivo = "Gerada por IA: Caio forjou o vídeo de saída para encobrir o horário real.";
                    }
                }

                if (temScannerImg) {
                    DrawRectangleLinesEx(imgRect, 1.5f, ColorAlpha(COR_CIANO_NEON, 0.55f));
                    float sy = imgRect.y + fmodf((float)tempo * 95.0f, imgRect.height);
                    DrawLineEx((Vector2){ imgRect.x, sy }, (Vector2){ imgRect.x + imgRect.width, sy }, 2.0f, ColorAlpha(COR_CIANO_NEON, 0.7f));
                }

                DrawTextEx(assets.fontes.titulo, titulo, (Vector2){ 115, 90 }, 26, 1.0f, COR_CIANO_NEON);
                if (e.respostaProva[e.cenaAtual] == 0) {
                    Color corT = (e.analiseTempoRestante <= 7.0f) ? COR_VERMELHO_ERR : COR_AMARELO_AVISO;
                    DesenharBadgeCyber(assets.fontes.bold, (Vector2){ 995, 86 }, TextFormat("TEMPO: %02ds", (int)ceilf(e.analiseTempoRestante)), 21, corT);
                }

                DesenharTextoQuebrado(assets.fontes.regular, desc, (Rectangle){ 655, 145, 505, 155 }, 24, 1.0f, WHITE);
                DrawTextEx(assets.fontes.bold, "Classifique esta evidência:", (Vector2){ 655, 396 }, 26, 1.0f, COR_ROSA_NEON);

                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 655, 446, 230, 54 }, "REAL", 25, e.hoverAnalise[0], tempo,
                    (e.respostaProva[e.cenaAtual] == 1) ? COR_CIANO_NEON : COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 905, 446, 230, 54 }, "GERADA POR IA", 23, e.hoverAnalise[1], tempo,
                    (e.respostaProva[e.cenaAtual] == 2) ? COR_ROSA_NEON : COR_ROXO_NEON);

                if (e.respostaProva[e.cenaAtual] != 0 || e.analiseTempoEsgotado) {
                    bool acertou = (e.respostaProva[e.cenaAtual] == (ehManipulada ? 2 : 1));
                    const char *statusTit = e.analiseTempoEsgotado && e.respostaProva[e.cenaAtual] == 0
                        ? "TEMPO ESGOTADO! (NÃO VALIDADA)"
                        : (acertou ? "PROVA VALIDADA!" : "CLASSIFICAÇÃO INCORRETA!");
                    Color corRes = (e.respostaProva[e.cenaAtual] != 0 && acertou) ? COR_VERDE_OK : COR_AMARELO_AVISO;
                    DrawTextEx(assets.fontes.bold, statusTit, (Vector2){ 655, 305 }, 24, 1.0f, corRes);
                    DesenharTextoQuebrado(assets.fontes.regular, motivo, (Rectangle){ 655, 336, 505, 65 }, 21, 1.0f, LIGHTGRAY);
                    DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 655, 542, 480, 54 }, "CONTINUAR INVESTIGAÇÃO >>", 25, e.hoverAnalise[2], tempo, COR_VERDE_OK);
                }
            } break;

            case TELA_SALA_SUSPEITOS: {
                Texture2D bgSusp = ObterTexturaCache(&assets.fundo, BG_SUSPEITOS);
                DesenharTexturaCover(bgSusp, (Rectangle){ 0, 0, VIRTUAL_W, VIRTUAL_H }, WHITE);
                DrawRectangleGradientV(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 8, 5, 18, 60 }, (Color){ 8, 5, 18, 175 });

                Rectangle caioHit = { 120, 140, 340, 560 };
                Rectangle juHit   = { 820, 140, 340, 560 };
                bool hovCaio = CheckCollisionPointRec(mouse, caioHit);
                bool hovJu   = CheckCollisionPointRec(mouse, juHit);

                // Posicionados como na parede de reconhecimento (pernas descendo alem da borda inferior)
                Texture2D tCaio = ObterTexturaCache(&assets.personagem, CAIO_CORPO);
                Rectangle rCaio = hovCaio ? (Rectangle){ 66, 108, 448, 718 } : (Rectangle){ 75, 118, 430, 690 };
                if (hovCaio) {
                    DrawRectangleRounded(caioHit, 0.04f, 6, ColorAlpha(COR_ROXO_NEON, 0.14f));
                    DrawRectangleRoundedLinesEx(caioHit, 0.04f, 6, 2.5f, ColorAlpha(COR_ROXO_NEON, 0.85f));
                }
                DesenharTexturaContain(tCaio, rCaio, hovCaio ? WHITE : (hovJu ? (Color){ 155, 155, 175, 255 } : (Color){ 215, 215, 230, 255 }));

                Texture2D tJuliana = ObterTexturaCache(&assets.evidencia, JULIA_CORPO);
                Rectangle rJu = hovJu ? (Rectangle){ 766, 108, 448, 718 } : (Rectangle){ 775, 118, 430, 690 };
                if (hovJu) {
                    DrawRectangleRounded(juHit, 0.04f, 6, ColorAlpha(COR_ROSA_NEON, 0.14f));
                    DrawRectangleRoundedLinesEx(juHit, 0.04f, 6, 2.5f, ColorAlpha(COR_ROSA_NEON, 0.85f));
                }
                DesenharTexturaContain(tJuliana, rJu, hovJu ? WHITE : (hovCaio ? (Color){ 155, 155, 175, 255 } : (Color){ 215, 215, 230, 255 }));

                if (hovCaio) {
                    DesenharBadgeCyber(assets.fontes.bold, (Vector2){ 145, 576 }, "ALVO: CAIO [CLIQUE P/ ALGEMAR]", 20, COR_ROXO_NEON);
                } else if (hovJu) {
                    DesenharBadgeCyber(assets.fontes.bold, (Vector2){ 820, 576 }, "ALVO: JULIANA [CLIQUE P/ ALGEMAR]", 20, COR_ROSA_NEON);
                }

                DesenharBadgeCyber(assets.fontes.titulo, (Vector2){ 425, 76 }, "SALA DE RECONHECIMENTO", 26, COR_CIANO_NEON);
                DesenharTextoQuebrado(assets.fontes.regular,
                    "Mire com as algemas e clique no suspeito que deseja prender (ou pressione [D] para revisar o Mural).",
                    (Rectangle){ 415, 128, 450, 75 }, 22, 1.0f, WHITE);

                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 495, 636, 290, 46 }, "ABRIR MURAL [D]", 21, e.hoverVeredito[2], tempo, COR_CIANO_NEON);

                // POV das algemas como cursor + Mira tatica
                if (!e.menuPausa && !e.devPainelAberto && !e.devPopupSenha) {
                    static Texture2D maosCache = {0};
                    if (maosCache.id == 0) maosCache = CarregarTexturaSegura("AssetsRay/Personagens/Player/Maos.png");
                    if (maosCache.id != 0) {
                        float drawW = 760.0f;
                        float drawH = drawW * ((float)maosCache.height / (float)maosCache.width);
                        float px = mouse.x - drawW * 0.38f;
                        float py = mouse.y - drawH * 0.18f;
                        if (py + drawH < VIRTUAL_H + 8.0f) py = VIRTUAL_H + 8.0f - drawH;
                        if (px < -120.0f) px = -120.0f;
                        if (px + drawW > VIRTUAL_W + 220.0f) px = VIRTUAL_W + 220.0f - drawW;
                        Rectangle srcM = { 0, 0, (float)maosCache.width, (float)maosCache.height };
                        Rectangle dstM = { px, py, drawW, drawH };
                        DrawTexturePro(maosCache, srcM, dstM, (Vector2){0, 0}, 0.0f, WHITE);
                    }
                    // Mira tatica sobre o ponteiro
                    Color corMira = hovCaio ? COR_ROXO_NEON : (hovJu ? COR_ROSA_NEON : COR_CIANO_NEON);
                    float rMira = (hovCaio || hovJu) ? 20.0f + 2.5f * sinf((float)tempo * 8.0f) : 16.0f;
                    DrawCircleLines((int)mouse.x, (int)mouse.y, rMira, corMira);
                    DrawLineEx((Vector2){ mouse.x - rMira - 7, mouse.y }, (Vector2){ mouse.x - 6, mouse.y }, 2.0f, corMira);
                    DrawLineEx((Vector2){ mouse.x + 6, mouse.y }, (Vector2){ mouse.x + rMira + 7, mouse.y }, 2.0f, corMira);
                    DrawLineEx((Vector2){ mouse.x, mouse.y - rMira - 7 }, (Vector2){ mouse.x, mouse.y - 6 }, 2.0f, corMira);
                    DrawLineEx((Vector2){ mouse.x, mouse.y + 6 }, (Vector2){ mouse.x, mouse.y + rMira + 7 }, 2.0f, corMira);
                    DrawCircleV(mouse, 3.0f, WHITE);
                }
            } break;

            case TELA_VEREDITO:
                DesenharCardCentral(&assets.fontes, "ESCOLHA SEU VEREDITO", "Quem é o responsável pela morte de Gabriel Moura?", NULL, 0, tempo, COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 310, 322, 660, 54 }, "[A] Caio Vasconcelos é o culpado.", 25, e.hoverVeredito[0], tempo, COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 310, 392, 660, 54 }, "[B] Juliana Costa é a culpada.", 25, e.hoverVeredito[1], tempo, COR_ROXO_NEON);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 310, 462, 660, 54 }, "[C] Ver o dossiê [D]", 25, e.hoverVeredito[2], tempo, COR_CIANO_NEON);
                break;

            case TELA_CONFIRMA_VEREDITO: {
                DesenharFundoCyberGrid(tempo, COR_AMARELO_AVISO);
                DesenharPainelCyber((Rectangle){ 90, 62, 1100, 594 }, COR_AMARELO_AVISO);

                bool acusouCaio = (e.acusadoFinal == 1);
                int idxPessoa = acusouCaio ? 0 : 1; // 0=Caio, 1=Juliana
                const char *alvo = acusouCaio ? "Caio Vasconcelos" : "Juliana Costa";
                Color corAlvo = acusouCaio ? COR_ROXO_NEON : COR_ROSA_NEON;

                DesenharBadgeCyber(assets.fontes.titulo, (Vector2){ 125, 84 },
                    TextFormat("CONFIRMAR PRISÃO // %s", alvo), 25, COR_AMARELO_AVISO);
                DrawRectangle(125, 134, 1030, 2, ColorAlpha(COR_AMARELO_AVISO, 0.45f));

                // Coluna Esquerda: Preview do Dossie do Suspeito
                Rectangle boxPerfil = { 125, 152, 430, 385 };
                DrawRectangleRounded(boxPerfil, 0.04f, 6, (Color){ 18, 12, 34, 220 });
                DrawRectangleRoundedLinesEx(boxPerfil, 0.04f, 6, 1.8f, corAlvo);

                Texture2D mug = ObterTexturaCache(&assets.personagem, acusouCaio ? "AssetsRay/Personagens/Caio/CaioMugshot.png" : "AssetsRay/Personagens/Julia/juliana retrato.png");
                DesenharTexturaContain(mug, (Rectangle){ 145, 168, 140, 160 }, WHITE);
                DrawRectangleLinesEx((Rectangle){ 145, 168, 140, 160 }, 1.5f, corAlvo);

                DrawTextEx(assets.fontes.bold, alvo, (Vector2){ 300, 174 }, 23, 1.0f, corAlvo);
                DrawTextEx(assets.fontes.regular, acusouCaio ? "22 anos | Ciência da Comp.\nFilho de sócio da NEXARA" : "21 anos | Ciência da Comp.\nDisputava vaga na NEXARA",
                    (Vector2){ 300, 208 }, 18, 1.0f, LIGHTGRAY);

                char notasAlvo[360] = "";
                if (acusouCaio) {
                    snprintf(notasAlvo, sizeof(notasAlvo), "NOTAS DO DOSSIÊ:\n%s%s",
                        e.notaCaioCena2 ? "- Discutiu com Gabriel na festa; alega saída às 02:40.\n" : "- Poucos registros iniciais.\n",
                        (e.notaConfrontoCena7 && e.culpado == CULPADO_CAIO) ? "- Confrontado sobre vídeo adulterado e repasses." : "");
                } else {
                    snprintf(notasAlvo, sizeof(notasAlvo), "NOTAS DO DOSSIÊ:\n%s%s",
                        e.notaJulianaCena4 ? "- Brigou com Gabriel pela vaga na NEXARA; alega app às 03:00.\n" : "- Poucos registros iniciais.\n",
                        (e.notaConfrontoCena7 && e.culpado == CULPADO_JULIANA) ? "- Confrontada sobre corrida Mobi às 03:41." : "");
                }
                DesenharTextoQuebrado(assets.fontes.regular, notasAlvo, (Rectangle){ 145, 344, 390, 180 }, 19, 1.0f, WHITE);

                // Coluna Direita: Evidencias linkadas ou nao no Mural
                Rectangle boxEv = { 575, 152, 580, 385 };
                DrawRectangleRounded(boxEv, 0.04f, 6, (Color){ 18, 12, 34, 220 });
                DrawRectangleRoundedLinesEx(boxEv, 0.04f, 6, 1.8f, COR_CIANO_NEON);
                DrawTextEx(assets.fontes.bold, "RESUMO DO MURAL (EVIDÊNCIAS LINKADAS):", (Vector2){ 595, 166 }, 20, 1.0f, COR_CIANO_NEON);

                for (int i = 0; i < MURAL_NUM_TOTAL; i++) {
                    bool ehAmb = (i >= MURAL_NUM_EV_OFIC);
                    bool desb = mural.evDesbloqueada[i];
                    int st = ehAmb ? (desb ? 1 : 0) : e.statusProva[i + 3];
                    float yRow = 202.0f + i * 35.0f;
                    const char *nomeEv = (!desb && st == 0) ? "??????" : NomeCurtoEvidenciaMural(i, (int)e.culpado);
                    const char *statusLink = "Bloqueada";
                    Color cRow = GRAY;
                    if (desb || st != 0) {
                        bool ligadaAlvo = (mural.evLigadaA[i] == idxPessoa || mural.evLigadaB[i] == idxPessoa);
                        bool ligadaOutro = (mural.evLigadaA[i] >= 0 || mural.evLigadaB[i] >= 0);
                        if (!ehAmb && st == -1) {
                            statusLink = ligadaAlvo ? "Não validada (Ligada a este)" : "Não validada (Sem linha)";
                            cRow = COR_VERMELHO_ERR;
                        } else if (ligadaAlvo) {
                            statusLink = "LINKADA A ESTE SUSPEITO";
                            cRow = COR_VERDE_OK;
                        } else if (ligadaOutro) {
                            statusLink = "Linkada a outro";
                            cRow = COR_AMARELO_AVISO;
                        } else {
                            statusLink = "NÃO LINKADA NO MURAL";
                            cRow = COR_VERMELHO_ERR;
                        }
                    }
                    DrawTextEx(assets.fontes.bold, nomeEv, (Vector2){ 595, yRow }, 17, 1.0f, WHITE);
                    DrawTextEx(assets.fontes.regular, statusLink, (Vector2){ 855, yRow }, 16, 1.0f, cRow);
                }

                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 290, 566, 300, 52 }, "[1] SIM, PRENDER", 23, e.hoverSimNao[0], tempo, COR_VERDE_OK);
                DesenharBotaoCyber(assets.fontes.bold, (Rectangle){ 690, 566, 300, 52 }, "[2] NÃO, VOLTAR", 23, e.hoverSimNao[1], tempo, COR_CIANO_NEON);
            } break;

            case TELA_CUTSCENE:
                DesenharCutscene(&cutscene, &assets, mouse, tempo, e.devDesbloqueado);
                break;

            case TELA_FINAL: {
                const char *nomeCulpado = (e.culpado == CULPADO_JULIANA) ? "Juliana Costa" : "Caio Vasconcelos";
                const char *tit = "CASO ENCERRADO";
                char txt[512] = "";
                Color corFinal = COR_VERMELHO_ERR;
                if (e.tipoFinal == 1) {
                    tit = "FINAL 1 - VERDADE ABSOLUTA";
                    snprintf(txt, sizeof(txt),
                        "CASO PERFEITO (5/5 PROVAS)! Você prendeu %s e ainda expôs o esquema inteiro nos bastidores da NEXARA.\nCASO ENCERRADO - EXCELENTE TRABALHO, INVESTIGADORA!",
                        nomeCulpado);
                    corFinal = COR_VERDE_OK;
                } else if (e.tipoFinal == 2) {
                    tit = "FINAL 2 - A VERDADE";
                    snprintf(txt, sizeof(txt),
                        "VEREDITO CORRETO! %s confessou o crime diante das provas técnicas validadas.\nCASO ENCERRADO - PARABÉNS, INVESTIGADORA!",
                        nomeCulpado);
                    corFinal = COR_VERDE_OK;
                } else if (e.tipoFinal == 3) {
                    tit = "FINAL 3 - A DÚVIDA";
                    snprintf(txt, sizeof(txt),
                        "VEREDITO CORRETO, MAS SEM PROVAS SUFICIENTES. %s foi absolvido(a) pela Lei da Dúvida Digital.\nCASO ARQUIVADO - A VERDADE FICOU PELO CAMINHO.",
                        nomeCulpado);
                    corFinal = COR_AMARELO_AVISO;
                } else if (e.tipoFinal == 4) {
                    tit = "FINAL 4 - COLAPSO FORENSE";
                    snprintf(txt, sizeof(txt),
                        "INQUÉRITO ARQUIVADO ANTES DO TRIBUNAL. Com quase nenhuma prova validada, o Ministério Público rejeitou o caso.\nDICA: Valide pelo menos 4 provas para sustentar a prisão.");
                    corFinal = COR_VERMELHO_ERR;
                } else if (e.tipoFinal == 5) {
                    tit = "FINAL 5 - A PRECIPITAÇÃO";
                    snprintf(txt, sizeof(txt),
                        "ACUSAÇÃO PRECIPITADA! Você tentou encerrar o caso antes de concluir a perícia da nuvem e o confronto final.\nO Delegado Meireles te afastou da investigação.");
                    corFinal = COR_AMARELO_AVISO;
                } else {
                    tit = "FINAL 6 - A INJUSTIÇA";
                    snprintf(txt, sizeof(txt),
                        "VEREDITO INCORRETO. Um inocente pagou pelo crime. O verdadeiro culpado era %s.\nCASO ENCERRADO - VOCÊ PERDEU.",
                        nomeCulpado);
                    corFinal = COR_VERMELHO_ERR;
                }
                DesenharCardCentral(&assets.fontes, tit, txt, "VOLTAR AO MENU", e.hoverBtnUnico, tempo, corFinal);
                DrawTextEx(assets.fontes.bold, TextFormat("Lógica Validada: %d/5 | Extras Corretas: %d/%d | Desafios: %d acertos, %d erros",
                    ContarProvasValidadas(&e, &mural), ContarAmbiguasCorretas(&e, &mural), MURAL_NUM_AMB, stats.acertos, stats.erros),
                    (Vector2){ 210, 525 }, 21, 1.0f, COR_CIANO_NEON);
            } break;

            case TELA_DOSSIE: {
                DesenharMural(&mural, &assets, (int)e.culpado, e.statusProva, e.respostaProva,
                    e.notaCaioCena2, e.notaJulianaCena4, e.notaGabrielCena5, e.notaNuvemCena6, e.notaConfrontoCena7,
                    ContarProvasValidadas(&e, &mural), mouse, tempo, cam.zoom);
            } break;
        }

        if (emGameplay && e.tela != TELA_CUTSCENE) {
            DesenharBarraTopo(&assets.fontes, mouse, tempo, e.hoverDossieTop[0], e.podeAcusar, e.setaMuralTutorial, e.toastMuralTimer, e.toastMuralTxt);
            if (e.tela != TELA_DIALOGO && e.tela != TELA_MINIGAME) {
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

    DescarregarMuralCaches(&mural);
    DescarregarCutsceneCaches(&cutscene);
    UnloadRenderTexture(canvas);
    DescarregarGerenciadorAssets(&assets);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
