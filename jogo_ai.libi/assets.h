#ifndef ASSETS_H
#define ASSETS_H

#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define VIRTUAL_W 1280
#define VIRTUAL_H 720
#define RENDER_W  1920
#define RENDER_H  1080

// Paleta Cyber-Investigativa unificada (padrao CaixaDialogo.png)
#define COR_FUNDO_ESCURO  (Color){ 12, 8, 22, 255 }
#define COR_PAINEL        (Color){ 20, 11, 36, 238 }
#define COR_PAINEL_CLARO  (Color){ 34, 20, 62, 235 }
#define COR_ROXO_NEON     (Color){ 185, 45, 255, 255 }
#define COR_ROSA_NEON     (Color){ 255, 45, 145, 255 }
#define COR_CIANO_NEON    (Color){ 45, 225, 255, 255 }
#define COR_VERDE_OK      (Color){ 55, 220, 140, 255 }
#define COR_VERMELHO_ERR  (Color){ 255, 70, 105, 255 }
#define COR_AMARELO_AVISO (Color){ 255, 200, 65, 255 }

typedef struct {
    Font bold;
    Font regular;
    Font titulo;
    Font teko;
    bool carregada;
} FontesUI;

typedef struct {
    char caminho[256];
    Texture2D tex;
} CacheTextura;

typedef struct {
    Texture2D menuBg;
    Texture2D marcaLogo;
    Texture2D caixaDialogo;

    CacheTextura fundo;
    CacheTextura personagem;
    CacheTextura evidencia;

    FontesUI fontes;
    Music musicaMenu;
    Music musicaJogo;
    Sound sfxClick;
    Sound sfxClicks[5];
    Sound sfxBrilho;
    Sound sfxEscritorio;
    Sound sfxLinkOk;
    Sound sfxLinkErr;
    bool temMusicaMenu;
    bool temMusicaJogo;
    bool temSfxClick;
    bool temSfxClicks[5];
    bool temSfxBrilho;
    bool temSfxEscritorio;
    bool temSfxLinkOk;
    bool temSfxLinkErr;
} GerenciadorAssets;

static inline const char *ResolverCaminho(const char *relativo, char *buffer, int tam) {
    if (!relativo || relativo[0] == '\0') return "";
    const char *prefixos[] = { "", "../", "jogo_ai.libi/", "../../" };
    for (int i = 0; i < 4; i++) {
        snprintf(buffer, tam, "%s%s", prefixos[i], relativo);
        if (FileExists(buffer)) return buffer;
    }
    snprintf(buffer, tam, "%s", relativo);
    return buffer;
}

static inline Font CarregarFonteAltaRes(const char *relativo, int tamanhoBase, int *codepoints, int count) {
    char buf[256];
    const char *p = ResolverCaminho(relativo, buf, sizeof(buf));
    if (!FileExists(p)) {
        char alt[256];
        snprintf(alt, sizeof(alt), "jogo_ai.libi/%s", relativo);
        p = ResolverCaminho(alt, buf, sizeof(buf));
    }
    if (FileExists(p)) {
        Font f = LoadFontEx(p, tamanhoBase, codepoints, count);
        SetTextureFilter(f.texture, TEXTURE_FILTER_BILINEAR);
        return f;
    }
    return (Font){0};
}

static inline FontesUI CarregarFontesUI(void) {
    FontesUI f = {0};
    int codepoints[224];
    int count = 0;
    for (int c = 32; c <= 126; c++) codepoints[count++] = c;
    for (int c = 160; c <= 255; c++) codepoints[count++] = c;

    // Carrega em resolucao alta (48-72px) para nitidez maxima
    f.bold    = CarregarFonteAltaRes("src/fonts/Rajdhani-Bold.ttf", 52, codepoints, count);
    f.regular = CarregarFonteAltaRes("src/fonts/Rajdhani-SemiBold.ttf", 44, codepoints, count);
    f.titulo  = CarregarFonteAltaRes("src/fonts/Orbitron-Regular.ttf", 64, codepoints, count);
    f.teko    = CarregarFonteAltaRes("src/fonts/Teko-Regular.ttf", 72, codepoints, count);

    if (f.bold.texture.id == 0) f.bold = GetFontDefault();
    else f.carregada = true;
    if (f.regular.texture.id == 0) f.regular = f.bold;
    if (f.titulo.texture.id == 0) f.titulo = f.bold;
    if (f.teko.texture.id == 0) f.teko = f.bold;
    return f;
}

static inline Texture2D CarregarTexturaSegura(const char *relativo) {
    char buf[256];
    const char *path = ResolverCaminho(relativo, buf, sizeof(buf));
    if (!FileExists(path)) return (Texture2D){0};
    Texture2D t = LoadTexture(path);
    if (t.id != 0) SetTextureFilter(t, TEXTURE_FILTER_BILINEAR);
    return t;
}

static inline Texture2D ObterTexturaCache(CacheTextura *cache, const char *relativo) {
    if (!relativo || relativo[0] == '\0') {
        if (cache->tex.id != 0) {
            UnloadTexture(cache->tex);
            cache->tex = (Texture2D){0};
        }
        cache->caminho[0] = '\0';
        return (Texture2D){0};
    }
    if (strcmp(cache->caminho, relativo) == 0 && cache->tex.id != 0) {
        return cache->tex;
    }
    if (cache->tex.id != 0) {
        UnloadTexture(cache->tex);
        cache->tex = (Texture2D){0};
    }
    cache->tex = CarregarTexturaSegura(relativo);
    snprintf(cache->caminho, sizeof(cache->caminho), "%s", relativo);
    return cache->tex;
}

static inline void IniciarGerenciadorAssets(GerenciadorAssets *g) {
    memset(g, 0, sizeof(*g));
    g->fontes = CarregarFontesUI();

    g->menuBg       = CarregarTexturaSegura("AssetsRay/MenuPrincipal/menu (1).png");
    g->marcaLogo    = CarregarTexturaSegura("AssetsRay/MenuPrincipal/ailibe marca.png");
    g->caixaDialogo = CarregarTexturaSegura("AssetsRay/UI/CaixaDialogo.png");

    char buf[256];
    const char *m1 = ResolverCaminho("src/audio/ai.libi_1.wav", buf, sizeof(buf));
    if (!FileExists(m1)) m1 = ResolverCaminho("jogo_ai.libi/src/audio/ai.libi_1.wav", buf, sizeof(buf));
    if (FileExists(m1)) { g->musicaMenu = LoadMusicStream(m1); g->musicaMenu.looping = true; g->temMusicaMenu = true; }

    const char *m2 = ResolverCaminho("src/audio/ai.libi_2.wav", buf, sizeof(buf));
    if (!FileExists(m2)) m2 = ResolverCaminho("jogo_ai.libi/src/audio/ai.libi_2.wav", buf, sizeof(buf));
    if (FileExists(m2)) { g->musicaJogo = LoadMusicStream(m2); g->musicaJogo.looping = true; g->temMusicaJogo = true; }

    // 5 presets procedurais de click suave e tecnologico
    {
        const int sr = 22050;
        for (int p = 0; p < 5; p++) {
            float dur = (p == 0) ? 0.022f : (p == 1) ? 0.028f : (p == 2) ? 0.018f : (p == 3) ? 0.032f : 0.024f;
            int frames = (int)(sr * dur);
            short *samples = (short *)MemAlloc((unsigned int)frames * sizeof(short));
            if (samples) {
                unsigned int rng = 7331u + (unsigned int)p * 1307u;
                float lp = 0.0f;
                for (int i = 0; i < frames; i++) {
                    float t = (float)i / (float)sr;
                    float u = t / dur;
                    float env = expf(-u * 6.5f) * (u < 0.08f ? (u / 0.08f) : 1.0f);
                    rng = rng * 1664525u + 1013904223u;
                    float noise = ((float)(rng & 0xFFFF) / 32768.0f) - 1.0f;
                    lp = lp * 0.72f + noise * 0.28f;
                    float val = 0.0f;
                    if (p == 0) {
                        // Soft Tactile Click: suave e seco
                        val = (lp * 0.65f + sinf(2.0f * 3.14159f * 420.0f * t) * 0.35f) * env * 0.35f;
                    } else if (p == 1) {
                        // Wood/Glass Tap: grave suave
                        val = (sinf(2.0f * 3.14159f * (520.0f - 220.0f * u) * t) * 0.75f + lp * 0.25f) * env * 0.34f;
                    } else if (p == 2) {
                        // Micro Switch: clique mecanico curtinho
                        val = (noise * 0.45f * expf(-u * 10.0f) + sinf(2.0f * 3.14159f * 680.0f * t) * 0.35f) * env * 0.32f;
                    } else if (p == 3) {
                        // Soft UI Pop: bolha digital grave
                        val = sinf(2.0f * 3.14159f * (340.0f + 260.0f * u) * t) * env * 0.34f;
                    } else {
                        // Clean Tick: tick neutro discreto
                        val = (lp * 0.4f + sinf(2.0f * 3.14159f * 840.0f * t) * 0.45f) * env * 0.28f;
                    }
                    if (val > 0.9f) val = 0.9f;
                    if (val < -0.9f) val = -0.9f;
                    samples[i] = (short)(val * 32767.0f);
                }
                Wave w = { (unsigned int)frames, (unsigned int)sr, 16, 1, samples };
                g->sfxClicks[p] = LoadSoundFromWave(w);
                g->temSfxClicks[p] = true;
                MemFree(samples);
            }
        }
        if (g->temSfxClicks[0]) {
            g->sfxClick = g->sfxClicks[0];
            g->temSfxClick = true;
        }
    }

    const char *s2 = ResolverCaminho("src/audio/ai.libi_4_brilho.mp3", buf, sizeof(buf));
    if (!FileExists(s2)) s2 = ResolverCaminho("jogo_ai.libi/src/audio/ai.libi_4_brilho.mp3", buf, sizeof(buf));
    if (FileExists(s2)) { g->sfxBrilho = LoadSound(s2); g->temSfxBrilho = true; }

    // Som procedural de cadeira sendo puxada + cliques de teclado (0.75s, 22050Hz)
    {
        const int sr = 22050;
        const int frames = (int)(sr * 0.75f);
        short *samples = (short *)MemAlloc((unsigned int)frames * sizeof(short));
        if (samples) {
            unsigned int rng = 1337u;
            for (int i = 0; i < frames; i++) {
                float t = (float)i / (float)sr;
                rng = rng * 1664525u + 1013904223u;
                float noise = ((float)(rng & 0xFFFF) / 32768.0f) - 1.0f;
                float val = 0.0f;
                // 0.00 - 0.28s: rangido grave suave de cadeira sendo puxada
                if (t < 0.28f) {
                    float env = sinf((t / 0.28f) * 3.14159f);
                    val += (sinf(t * 2.0f * 3.14159f * 115.0f) * 0.45f + noise * 0.35f) * env * 0.30f;
                }
                // 0.32 - 0.72s: rajada rapida de teclas mecanicas + click
                for (int k = 0; k < 5; k++) {
                    float tk = 0.32f + k * 0.075f;
                    if (t >= tk && t < tk + 0.025f) {
                        float env = 1.0f - (t - tk) / 0.025f;
                        val += (noise * 0.7f + sinf((t - tk) * 1800.0f) * 0.3f) * env * 0.38f;
                    }
                }
                if (val > 0.9f) val = 0.9f;
                if (val < -0.9f) val = -0.9f;
                samples[i] = (short)(val * 32767.0f);
            }
            Wave w = { (unsigned int)frames, (unsigned int)sr, 16, 1, samples };
            g->sfxEscritorio = LoadSoundFromWave(w);
            g->temSfxEscritorio = true;
            MemFree(samples);
        }
    }

    // Som positivo curto de ligacao correta no Mural (0.16s, chime ascendente)
    {
        const int sr = 22050;
        const int frames = (int)(sr * 0.16f);
        short *samples = (short *)MemAlloc((unsigned int)frames * sizeof(short));
        if (samples) {
            for (int i = 0; i < frames; i++) {
                float t = (float)i / (float)sr;
                float u = (float)i / (float)frames;
                float f = (u < 0.45f) ? 523.25f : 783.99f;
                float env = expf(-u * 4.5f) * (u < 0.05f ? (u / 0.05f) : 1.0f);
                float val = (sinf(2.0f * 3.14159f * f * t) * 0.7f + sinf(2.0f * 3.14159f * f * 2.0f * t) * 0.2f) * env * 0.30f;
                if (val > 0.9f) val = 0.9f;
                if (val < -0.9f) val = -0.9f;
                samples[i] = (short)(val * 32767.0f);
            }
            Wave w = { (unsigned int)frames, (unsigned int)sr, 16, 1, samples };
            g->sfxLinkOk = LoadSoundFromWave(w);
            g->temSfxLinkOk = true;
            MemFree(samples);
        }
    }

    // Som negativo curto de ligacao errada no Mural (0.20s, tom descendente)
    {
        const int sr = 22050;
        const int frames = (int)(sr * 0.20f);
        short *samples = (short *)MemAlloc((unsigned int)frames * sizeof(short));
        if (samples) {
            for (int i = 0; i < frames; i++) {
                float t = (float)i / (float)sr;
                float u = (float)i / (float)frames;
                float f = (u < 0.50f) ? 240.0f : 165.0f;
                float env = expf(-u * 4.0f) * (u < 0.04f ? (u / 0.04f) : 1.0f);
                float val = (sinf(2.0f * 3.14159f * f * t) * 0.65f + sinf(2.0f * 3.14159f * f * 1.5f * t) * 0.25f) * env * 0.34f;
                if (val > 0.9f) val = 0.9f;
                if (val < -0.9f) val = -0.9f;
                samples[i] = (short)(val * 32767.0f);
            }
            Wave w = { (unsigned int)frames, (unsigned int)sr, 16, 1, samples };
            g->sfxLinkErr = LoadSoundFromWave(w);
            g->temSfxLinkErr = true;
            MemFree(samples);
        }
    }
}

static inline Sound ObterSfxClick(const GerenciadorAssets *g, int preset) {
    if (preset >= 0 && preset < 5 && g->temSfxClicks[preset]) return g->sfxClicks[preset];
    return g->sfxClick;
}

static inline void DescarregarGerenciadorAssets(GerenciadorAssets *g) {
    if (g->menuBg.id != 0) UnloadTexture(g->menuBg);
    if (g->marcaLogo.id != 0) UnloadTexture(g->marcaLogo);
    if (g->caixaDialogo.id != 0) UnloadTexture(g->caixaDialogo);
    if (g->fundo.tex.id != 0) UnloadTexture(g->fundo.tex);
    if (g->personagem.tex.id != 0) UnloadTexture(g->personagem.tex);
    if (g->evidencia.tex.id != 0) UnloadTexture(g->evidencia.tex);

    if (g->fontes.carregada) {
        UnloadFont(g->fontes.bold);
        if (g->fontes.regular.texture.id != g->fontes.bold.texture.id) UnloadFont(g->fontes.regular);
        if (g->fontes.titulo.texture.id != g->fontes.bold.texture.id) UnloadFont(g->fontes.titulo);
        if (g->fontes.teko.texture.id != g->fontes.bold.texture.id) UnloadFont(g->fontes.teko);
    }
    if (g->temMusicaMenu) UnloadMusicStream(g->musicaMenu);
    if (g->temMusicaJogo) UnloadMusicStream(g->musicaJogo);
    if (g->temSfxClick && !g->temSfxClicks[0]) UnloadSound(g->sfxClick);
    for (int i = 0; i < 5; i++) {
        if (g->temSfxClicks[i]) UnloadSound(g->sfxClicks[i]);
    }
    if (g->temSfxBrilho) UnloadSound(g->sfxBrilho);
    if (g->temSfxEscritorio) UnloadSound(g->sfxEscritorio);
    if (g->temSfxLinkOk) UnloadSound(g->sfxLinkOk);
    if (g->temSfxLinkErr) UnloadSound(g->sfxLinkErr);
}

static inline Vector2 ObterMouseVirtual(Rectangle destTela) {
    Vector2 m = GetMousePosition();
    if (destTela.width <= 0.0f || destTela.height <= 0.0f) return m;
    float vx = (m.x - destTela.x) * ((float)VIRTUAL_W / destTela.width);
    float vy = (m.y - destTela.y) * ((float)VIRTUAL_H / destTela.height);
    if (vx < 0) vx = 0;
    if (vx > VIRTUAL_W) vx = VIRTUAL_W;
    if (vy < 0) vy = 0;
    if (vy > VIRTUAL_H) vy = VIRTUAL_H;
    return (Vector2){ vx, vy };
}

static inline Rectangle CalcularDestinoTela(int winW, int winH) {
    float escala = fminf((float)winW / VIRTUAL_W, (float)winH / VIRTUAL_H);
    float w = VIRTUAL_W * escala;
    float h = VIRTUAL_H * escala;
    return (Rectangle){ (winW - w) * 0.5f, (winH - h) * 0.5f, w, h };
}

static inline void DesenharTexturaCover(Texture2D t, Rectangle dest, Color tint) {
    if (t.id == 0) return;
    float ratioSrc = (float)t.width / (float)t.height;
    float ratioDst = dest.width / dest.height;
    Rectangle src = { 0, 0, (float)t.width, (float)t.height };
    if (ratioSrc > ratioDst) {
        float novoW = t.height * ratioDst;
        src.x = (t.width - novoW) * 0.5f;
        src.width = novoW;
    } else {
        float novoH = t.width / ratioDst;
        src.y = (t.height - novoH) * 0.5f;
        src.height = novoH;
    }
    DrawTexturePro(t, src, dest, (Vector2){0, 0}, 0.0f, tint);
}

static inline void DesenharTexturaContain(Texture2D t, Rectangle dest, Color tint) {
    if (t.id == 0) return;
    float escala = fminf(dest.width / (float)t.width, dest.height / (float)t.height);
    float w = t.width * escala;
    float h = t.height * escala;
    Rectangle out = { dest.x + (dest.width - w) * 0.5f, dest.y + (dest.height - h) * 0.5f, w, h };
    DrawTexturePro(t, (Rectangle){0, 0, (float)t.width, (float)t.height}, out, (Vector2){0, 0}, 0.0f, tint);
}

// =========================================================================
// ELEMENTOS VISUAIS CYBERPUNK HUD + ANIMACOES DINAMICAS
// =========================================================================

static inline void AtualizarHover(const Rectangle *btns, float *hovers, int count, Vector2 posMouse, float dt) {
    float fator = fminf(1.0f, dt * 14.0f);
    for (int i = 0; i < count; i++) {
        float alvo = CheckCollisionPointRec(posMouse, btns[i]) ? 1.0f : 0.0f;
        hovers[i] += (alvo - hovers[i]) * fator;
    }
}

static bool gEfeitoGrid = true;
static bool gEfeitoScanlines = true;
static bool gEfeitoGlitch = true;

// Fundo cybernetico animado: grade holografica + particulas de dados flutuantes
static inline void DesenharFundoCyberGrid(double tempo, Color corAcento) {
    DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, COR_FUNDO_ESCURO);
    DrawRectangleGradientV(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 16, 8, 34, 255 }, (Color){ 8, 4, 18, 255 });
    if (!gEfeitoGrid) return;

    // Grade holografica com movimento sutil
    float offY = fmodf((float)tempo * 14.0f, 48.0f);
    Color gridCol = ColorAlpha(corAcento, 0.07f);
    for (int x = 0; x <= VIRTUAL_W; x += 48) {
        DrawLine(x, 0, x, VIRTUAL_H, gridCol);
    }
    for (int y = -48; y <= VIRTUAL_H + 48; y += 48) {
        DrawLine(0, (int)(y + offY), VIRTUAL_W, (int)(y + offY), gridCol);
    }

    // Particulas/chuva de dados deterministica (sem alocacao)
    for (int i = 0; i < 28; i++) {
        float seed = (float)i * 73.13f;
        float px = fmodf(seed * 19.7f, (float)VIRTUAL_W);
        float vel = 32.0f + fmodf(seed * 7.3f, 55.0f);
        float py = fmodf(seed * 11.1f + (float)tempo * vel, (float)VIRTUAL_H + 40.0f) - 20.0f;
        float len = 10.0f + fmodf(seed * 3.1f, 22.0f);
        Color pc = (i % 3 == 0) ? ColorAlpha(COR_ROSA_NEON, 0.22f)
                 : (i % 3 == 1) ? ColorAlpha(COR_CIANO_NEON, 0.20f)
                                : ColorAlpha(COR_ROXO_NEON, 0.18f);
        DrawLineEx((Vector2){ px, py }, (Vector2){ px, py + len }, 1.6f, pc);
        DrawCircle((int)px, (int)(py + len), 1.5f, ColorAlpha(pc, 0.55f));
    }
}

// Overlay sutil de scanlines CRT + feixe scanner (desativado)
static inline void DesenharScanlinesHUD(double tempo) {
    (void)tempo;
}

// Seta rosa animada a esquerda do texto
static inline void DesenharHoverCyberRosa(Rectangle r, float t, double tempo) {
    if (t <= 0.01f) return;
    Color rosaNeon  = { 255, 45, 140, (unsigned char)(245 * t) };
    Color rosaClaro = { 255, 145, 200, (unsigned char)(220 * t) };
    float cy = r.y + r.height * 0.5f;
    float pulso = sinf((float)tempo * 7.5f) * 2.5f * t;
    float ax = r.x + 6.0f * t + pulso;
    Vector2 p1 = { ax, cy - 8.0f * t };
    Vector2 p2 = { ax + 11.0f * t, cy };
    Vector2 p3 = { ax, cy + 8.0f * t };
    DrawTriangle(p1, p3, p2, rosaNeon);
    DrawLineEx((Vector2){ ax - 5.0f * t, cy - 6.0f * t }, (Vector2){ ax + 1.0f * t, cy }, 2.0f, rosaClaro);
    DrawLineEx((Vector2){ ax - 5.0f * t, cy + 6.0f * t }, (Vector2){ ax + 1.0f * t, cy }, 2.0f, rosaClaro);
}

// Botao solto apenas com seta rosa animada e texto rosa no hover (sem retangulo)
static inline void DesenharBotaoSolto(Font f, Rectangle r, const char *texto, float fontSize, float hoverT, double tempo, Color corBase) {
    DesenharHoverCyberRosa(r, hoverT, tempo);
    float textX = r.x + 16.0f + 18.0f * hoverT;
    float textY = r.y + (r.height - fontSize) * 0.5f;
    Color cor = {
        (unsigned char)(corBase.r + (255 - corBase.r) * hoverT),
        (unsigned char)(corBase.g + (45  - corBase.g) * hoverT),
        (unsigned char)(corBase.b + (145 - corBase.b) * hoverT),
        255
    };
    DrawTextEx(f, texto, (Vector2){ textX + 1.5f, textY + 1.5f }, fontSize, 1.2f, (Color){ 0, 0, 0, 140 });
    DrawTextEx(f, texto, (Vector2){ textX, textY }, fontSize, 1.2f, cor);
}

// Botao chanfrado cyberpunk completo (para acoes, opcoes e confirmacoes)
static inline void DesenharBotaoCyber(Font f, Rectangle r, const char *texto, float fontSize, float hoverT, double tempo, Color corDestaque) {
    float c = 12.0f;
    Vector2 pts[8] = {
        { r.x + c, r.y }, { r.x + r.width - c, r.y },
        { r.x + r.width, r.y + c }, { r.x + r.width, r.y + r.height - c },
        { r.x + r.width - c, r.y + r.height }, { r.x + c, r.y + r.height },
        { r.x, r.y + r.height - c }, { r.x, r.y + c }
    };
    Vector2 centro = { r.x + r.width * 0.5f, r.y + r.height * 0.5f };
    Color bg = {
        (unsigned char)(24 + 28 * hoverT),
        (unsigned char)(14 + 14 * hoverT),
        (unsigned char)(46 + 34 * hoverT),
        235
    };
    for (int i = 0; i < 8; i++) {
        DrawTriangle(centro, pts[(i + 1) % 8], pts[i], bg);
    }

    float pulso = 0.72f + 0.28f * sinf((float)tempo * 4.5f);
    Color borda = ColorAlpha(corDestaque, (0.55f + 0.45f * hoverT) * (hoverT > 0.1f ? pulso : 1.0f));
    if (hoverT > 0.05f) {
        for (int i = 0; i < 8; i++) {
            DrawLineEx(pts[i], pts[(i + 1) % 8], 5.5f, ColorAlpha(corDestaque, 0.18f * hoverT));
        }
    }
    for (int i = 0; i < 8; i++) {
        DrawLineEx(pts[i], pts[(i + 1) % 8], 2.0f + hoverT * 0.8f, borda);
    }

    // Barra lateral esquerda neon
    DrawLineEx((Vector2){ r.x + 4, r.y + c }, (Vector2){ r.x + 4, r.y + r.height - c }, 3.0f, ColorAlpha(corDestaque, 0.45f + 0.55f * hoverT));

    Vector2 sz = MeasureTextEx(f, texto, fontSize, 1.2f);
    float tx = r.x + (r.width - sz.x) * 0.5f;
    float ty = r.y + (r.height - fontSize) * 0.5f;
    Color txtCol = (hoverT > 0.2f) ? WHITE : ColorAlpha(WHITE, 0.92f);
    DrawTextEx(f, texto, (Vector2){ tx + 1.5f, ty + 1.5f }, fontSize, 1.2f, (Color){ 0, 0, 0, 160 });
    DrawTextEx(f, texto, (Vector2){ tx, ty }, fontSize, 1.2f, txtCol);
}

// Painel chanfrado cyberpunk com glow, cantoneiras e detalhes HUD
static inline void DesenharPainelCyber(Rectangle r, Color corBorda) {
    float c = 18.0f;
    Vector2 pts[8] = {
        { r.x + c, r.y },
        { r.x + r.width - c, r.y },
        { r.x + r.width, r.y + c },
        { r.x + r.width, r.y + r.height - c },
        { r.x + r.width - c, r.y + r.height },
        { r.x + c, r.y + r.height },
        { r.x, r.y + r.height - c },
        { r.x, r.y + c }
    };

    // Sombra/glow externo sutil
    for (int i = 0; i < 8; i++) {
        DrawLineEx(pts[i], pts[(i + 1) % 8], 7.0f, ColorAlpha(corBorda, 0.14f));
    }

    Vector2 centro = { r.x + r.width * 0.5f, r.y + r.height * 0.5f };
    for (int i = 0; i < 8; i++) {
        DrawTriangle(centro, pts[(i + 1) % 8], pts[i], COR_PAINEL);
    }

    // Faixa superior sutil estilo HUD
    DrawRectangleGradientV((int)(r.x + c), (int)r.y + 2, (int)(r.width - c * 2), 36,
        ColorAlpha(corBorda, 0.14f), ColorAlpha(corBorda, 0.0f));

    for (int i = 0; i < 8; i++) {
        DrawLineEx(pts[i], pts[(i + 1) % 8], 2.4f, corBorda);
    }
    DrawRectangleLinesEx((Rectangle){ r.x + 8, r.y + 8, r.width - 16, r.height - 16 }, 1.0f, ColorAlpha(COR_ROSA_NEON, 0.25f));

    // Cantoneiras tecnologicas
    DrawLineEx((Vector2){ r.x + c + 12, r.y - 2 }, (Vector2){ r.x + c + 110, r.y - 2 }, 3.5f, COR_ROSA_NEON);
    DrawLineEx((Vector2){ r.x + r.width - c - 110, r.y + r.height + 2 }, (Vector2){ r.x + r.width - c - 12, r.y + r.height + 2 }, 3.5f, COR_CIANO_NEON);
    // Micro-tracos decorativos no canto superior direito
    for (int k = 0; k < 4; k++) {
        float tx = r.x + r.width - c - 18 - k * 10;
        DrawLineEx((Vector2){ tx, r.y + 6 }, (Vector2){ tx - 5, r.y + 13 }, 2.0f, ColorAlpha(COR_CIANO_NEON, 0.55f));
    }
}

// Badge / etiqueta chanfrada para status e tags
static inline void DesenharBadgeCyber(Font f, Vector2 pos, const char *texto, float fontSize, Color cor) {
    Vector2 sz = MeasureTextEx(f, texto, fontSize, 1.0f);
    Rectangle r = { pos.x, pos.y, sz.x + 22.0f, fontSize + 10.0f };
    DrawRectangleRec(r, ColorAlpha(cor, 0.16f));
    DrawRectangleLinesEx(r, 1.5f, ColorAlpha(cor, 0.85f));
    DrawTextEx(f, texto, (Vector2){ pos.x + 11.0f, pos.y + 5.0f }, fontSize, 1.0f, cor);
}

// Espectro de onda de voz/audio animado
static inline void DesenharOndaVoz(Rectangle area, double tempo, bool ativo, Color cor) {
    int barras = (int)(area.width / 7.0f);
    if (barras < 4) barras = 4;
    float bw = area.width / (float)barras;
    for (int i = 0; i < barras; i++) {
        float phase = ativo ? ((float)tempo * 13.0f + i * 0.8f) : (i * 0.6f);
        float frac = ativo ? (0.25f + 0.75f * fabsf(sinf(phase) * cosf(phase * 0.45f))) : 0.15f;
        float h = fmaxf(3.0f, area.height * frac);
        float y = area.y + (area.height - h) * 0.5f;
        DrawRectangleRounded((Rectangle){ area.x + i * bw, y, bw - 2.5f, h }, 0.5f, 3, ColorAlpha(cor, ativo ? 0.90f : 0.35f));
    }
}

// Logo com efeito glitch periodico (aberacao cromatica + slice tear)
static inline void DesenharLogoGlitchTech(Texture2D logo, Rectangle baseRect, double tempo) {
    if (logo.id == 0) return;
    Rectangle r = baseRect;
    Rectangle orig = { 0.0f, 0.0f, (float)logo.width, (float)logo.height };
    Vector2 zero = { 0.0f, 0.0f };

    float ciclo = fmodf((float)tempo, 3.2f);
    bool emGlitch = gEfeitoGlitch && ((ciclo < 0.20f) || (ciclo > 1.75f && ciclo < 1.82f));

    if (!emGlitch) {
        DrawTexturePro(logo, orig, r, zero, 0.0f, WHITE);
    } else {
        float jitter = sinf((float)tempo * 95.0f) * 5.0f;
        float jitterY = cosf((float)tempo * 73.0f) * 1.5f;

        Rectangle rRosa  = { r.x - fabsf(jitter) - 2.0f, r.y + jitterY, r.width, r.height };
        Rectangle rCiano = { r.x + fabsf(jitter) + 2.0f, r.y - jitterY, r.width, r.height };
        DrawTexturePro(logo, orig, rRosa, zero, 0.0f, (Color){ 255, 45, 145, 145 });
        DrawTexturePro(logo, orig, rCiano, zero, 0.0f, (Color){ 0, 235, 255, 135 });

        const int fatias = 3;
        for (int s = 0; s < fatias; s++) {
            float fy0 = (float)s / fatias;
            float fy1 = (float)(s + 1) / fatias;
            float offSlice = sinf((float)tempo * 120.0f + s * 2.7f) * 6.0f;
            Rectangle srcS = { 0.0f, fy0 * logo.height, (float)logo.width, (fy1 - fy0) * logo.height };
            Rectangle dstS = { r.x + offSlice, r.y + fy0 * r.height, r.width, (fy1 - fy0) * r.height };
            DrawTexturePro(logo, srcS, dstS, zero, 0.0f, WHITE);
        }

        float scanY = r.y + fmodf((float)tempo * 9.0f, 1.0f) * r.height;
        DrawRectangle((int)r.x, (int)scanY, (int)r.width, 2, (Color){ 255, 65, 165, 180 });
    }
}

// Barra inclinada cyberpunk com 10 blocos (//////)
static inline void DesenharBarraCyberInclinada(Rectangle area, float valor) {
    const int totalBlocos = 10;
    const float gap = 4.0f;
    const float skew = 6.0f;
    float blocoW = (area.width - gap * (totalBlocos - 1)) / totalBlocos;
    int ativos = (int)(valor * totalBlocos + 0.5f);

    for (int i = 0; i < totalBlocos; i++) {
        float bx = area.x + i * (blocoW + gap);
        float by = area.y;
        float bh = area.height;

        Vector2 tl = { bx + skew, by };
        Vector2 tr = { bx + blocoW + skew, by };
        Vector2 bl = { bx, by + bh };
        Vector2 br = { bx + blocoW, by + bh };

        bool ativo = (i < ativos);
        Color cor = ativo ? (Color){ 255, 45, 140, 240 } : (Color){ 58, 34, 88, 210 };
        DrawTriangle(tl, bl, br, cor);
        DrawTriangle(tl, br, tr, cor);

        Color borda = ativo ? (Color){ 255, 160, 210, 220 } : (Color){ 120, 85, 165, 130 };
        DrawLineEx(tl, tr, 1.0f, borda);
        DrawLineEx(bl, br, 1.0f, borda);
    }
}

// Icone de menu tecnologico discreto (barras assimetricas + nos de dados + [M])
static inline void DesenharIconeMenu(Rectangle r, Vector2 mouse) {
    bool hover = CheckCollisionPointRec(mouse, r);
    Color c = hover ? COR_CIANO_NEON : ColorAlpha(COR_ROXO_NEON, 0.9f);
    Color cTxt = hover ? WHITE : ColorAlpha(LIGHTGRAY, 0.85f);
    // Cantoneiras tecnologicas sutis
    float cs = 6.0f;
    DrawLineEx((Vector2){ r.x, r.y + cs }, (Vector2){ r.x, r.y }, 1.5f, c);
    DrawLineEx((Vector2){ r.x, r.y }, (Vector2){ r.x + cs, r.y }, 1.5f, c);
    DrawLineEx((Vector2){ r.x + r.width - cs, r.y + r.height }, (Vector2){ r.x + r.width, r.y + r.height }, 1.5f, c);
    DrawLineEx((Vector2){ r.x + r.width, r.y + r.height - cs }, (Vector2){ r.x + r.width, r.y + r.height }, 1.5f, c);

    float x = r.x + 11.0f;
    float y = r.y + 11.0f;
    float largs[3] = { 20.0f, 14.0f, 18.0f };
    for (int i = 0; i < 3; i++) {
        DrawCircle((int)x, (int)(y + i * 7.0f + 1.0f), 1.8f, c);
        DrawRectangleRounded((Rectangle){ x + 5.0f, y + i * 7.0f, largs[i], 2.2f }, 0.5f, 4, cTxt);
    }
    DrawText("[M]", (int)(r.x + r.width - 20), (int)(r.y + r.height - 13), 10, ColorAlpha(c, 0.9f));
}

static inline void DesenharTextoQuebrado(Font font, const char *texto, Rectangle area, float fontSize, float spacing, Color cor) {
    if (!texto || texto[0] == '\0') return;
    char linha[512] = "";
    char palavra[128] = "";
    float y = area.y;
    int len = (int)strlen(texto);
    int i = 0;

    while (i <= len) {
        if (texto[i] == '\n' || texto[i] == ' ' || texto[i] == '\0') {
            char teste[650];
            if (linha[0] == '\0') snprintf(teste, sizeof(teste), "%s", palavra);
            else snprintf(teste, sizeof(teste), "%s %s", linha, palavra);

            Vector2 m = MeasureTextEx(font, teste, fontSize, spacing);
            if (m.x > area.width && linha[0] != '\0') {
                DrawTextEx(font, linha, (Vector2){ area.x, y }, fontSize, spacing, cor);
                y += fontSize + 5.0f;
                snprintf(linha, sizeof(linha), "%.*s", (int)sizeof(linha) - 1, palavra);
            } else {
                snprintf(linha, sizeof(linha), "%.*s", (int)sizeof(linha) - 1, teste);
            }
            palavra[0] = '\0';

            if (texto[i] == '\n') {
                DrawTextEx(font, linha, (Vector2){ area.x, y }, fontSize, spacing, cor);
                y += fontSize + 5.0f;
                linha[0] = '\0';
            }
        } else {
            int plen = (int)strlen(palavra);
            if (plen < (int)sizeof(palavra) - 2) {
                palavra[plen] = texto[i];
                palavra[plen + 1] = '\0';
            }
        }
        i++;
    }
    if (linha[0] != '\0' && y + fontSize <= area.y + area.height + 24.0f) {
        DrawTextEx(font, linha, (Vector2){ area.x, y }, fontSize, spacing, cor);
    }
}

#endif
