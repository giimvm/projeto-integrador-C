#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define C4_FORCAR_DIALOGOS 0

typedef enum {
    STATE_MENU = 0,
    STATE_REGRAS,
    STATE_CONFIG,
    STATE_CREDITOS,

    // CENA 1
    STATE_C1_1,
    STATE_C1_2,
    STATE_C1_3,
    STATE_C1_4,
    STATE_C1_5,
    STATE_C1_6,

    // CENA 2
    STATE_C2_0,
    STATE_C2_1,
    STATE_C2_2,
    STATE_C2_3,
    STATE_C2_4,
    STATE_C2_5,
    STATE_C2_6,
    STATE_C2_7,

    STATE_C2_EVIDENCIA,
    STATE_C2_DESAFIO,
    STATE_CONFIRMA_CAIO,
    STATE_C2_PERDIDA,

    // RAMIFICAÇÃO DELEGADO E VEREDITO
    STATE_ACUSA_CAIO_1,   // F1.1
    STATE_ACUSA_CAIO_2,   // F1.2
    STATE_ACUSA_CAIO_3,   // F1.3
    STATE_ACUSA_CAIO_4,   // F1.4
    STATE_VEREDITO_INCORRETO,     // P1
    STATE_EVIDENCIA_DESBLOQUEADA, // P1.1
    STATE_DESAFIO_IA,             // P1.2

    // CENA 3
    STATE_C3_1,
    STATE_C3_2,
    STATE_C3_3,
    STATE_C3_4,
    STATE_C3_PERDIDA,
    STATE_C3_DESBLOQUEADA,
    STATE_C3_AUDIO,
    STATE_C3_PERGUNTA,

    // CENA 4
    STATE_C4_1,
    STATE_C4_2,
    STATE_C4_3,
    STATE_C4_4,
    STATE_C4_5,
    STATE_C4_6,
    STATE_C4_7,
    STATE_C4_8,
    STATE_C4_9,
    STATE_C4_10,
    STATE_C4_11,
    STATE_C4_12,
    STATE_C4_13,
    STATE_C4_14,
    STATE_C4_15,

    STATE_C4_EVIDENCIA,    // c4.16
    STATE_C4_DESAFIO,      // c4.17
    STATE_C4_PERDIDA,      // c4.18
    STATE_C4_DESBLOQUEADA, // c4.19
    STATE_C4_ORIGEM,       // p4.1
    STATE_C4_PERGUNTA,

    // CENA 5
    STATE_C5_1,
    STATE_C5_2,
    STATE_C5_3,
    STATE_C5_4,
    STATE_C5_5,
    STATE_C5_6,
    STATE_C5_7,
    STATE_C5_8,
    STATE_C5_9,
    STATE_C5_10,
    STATE_C5_EVIDENCIA,
    STATE_C5_DESAFIO,
    STATE_C5_PERDIDA,
    STATE_C5_DESBLOQUEADA,
    STATE_C5_ORIGEM,
    STATE_C5_PERGUNTA,

    // CENA 6
    STATE_C6_1,
    STATE_C6_2,
    STATE_C6_3,
    STATE_C6_4,
    STATE_C6_5,
    STATE_C6_6,
    STATE_C6_7,
    STATE_C6_8,
    STATE_C6_9,
    STATE_C6_10,
    STATE_C6_11,
    STATE_C6_12,
    STATE_C6_EVIDENCIA,
    STATE_C6_DESAFIO,
    STATE_C6_PERDIDA,
    STATE_C6_DESBLOQUEADA,
    STATE_C6_ORIGEM,
    STATE_C6_PERGUNTA,

    // CENA 7
    STATE_C7_1,            // c7_1  CHEFE: "A perícia conseguiu recuperar..."
    STATE_C7_2,            // c7_2  VOCÊ: "Uma fotografia recebida por Gabriel..."
    STATE_C7_EVIDENCIA,    // c7_3  Nova evidência encontrada
    STATE_C7_DESAFIO,      // c7_4  Desafio de lógica
    STATE_C7_PERDIDA,      // c7_5  Evidência perdida
    STATE_C7_DESBLOQUEADA, // P7_1  Evidência desbloqueada
    STATE_C7_ORIGEM,       // p7_2  Origem da foto (clique para continuar)
    STATE_C7_PERGUNTA,     // p7_3  Verdadeira ou manipulada?

    // CENA 8 (decisão final)
    STATE_C8_1,            // c8_1  A decisão final (Tomar a decisão)
    STATE_C8_2,            // c8_2  DELEGADO: "Você chegou ao fim da investigação..."
    STATE_C8_3,            // c8_3  VOCÊ: "Entendido..."
    STATE_C8_4,            // c8_4  DELEGADO: "Então, investigadora, quem..."
    STATE_C8_5,            // c8_5  Escolha o seu veredito

    // FINAL 2 (acusou o Caio)
    STATE_F2_1,            // F2_1  VOCÊ: "Meu veredito é que Caio Vasconcelos é o culpado."
    STATE_F2_2,            // F2_2  DELEGADO: "Você tem certeza dessa decisão?" (Sim / Não)
    STATE_F2_3,            // F2_3  Veredito incorreto (clique para continuar)
    STATE_F2_4,            // F2_4  Escaneie a explicação (Jogar novamente)

    // FINAL 3 (acusou a Juliana - veredito correto)
    STATE_F3_1,            // F3_1  VOCÊ: "Meu veredito é que Juliana Costa é a culpada."
    STATE_F3_2,            // F3_2  DELEGADO: "Você tem certeza dessa decisão?" (Sim / Não)
    STATE_F3_3,            // F3_3  Veredito correto
    STATE_F3_4,            // F3_4  Escaneie a explicação (Jogar novamente)

    // Provisório (ainda sem imagem)
    STATE_DOSSIE_PROV      // veredito: Ver o dossiê (a fazer)
} GameState;


// ======================================================================
// PROGRESSO DO JOGADOR
// ATENÇÃO: este bloco é um ESBOÇO para o código compilar.
// Se a struct do jogador e a função SalvarProgresso já existirem em outro
// arquivo do projeto (.h/.c), APAGUE este bloco e use um #include no lugar,
// senão vai dar erro de declaração duplicada.
// ======================================================================
typedef struct {
    int acertos;
    int erros;
    char evidenciaPerdida[128];
} Jogador;

void SalvarProgresso(Jogador j) {
    FILE* f = fopen("progresso.txt", "w");
    if (!f) return;
    fprintf(f, "acertos=%d\nerros=%d\nperdida=%s\n", j.acertos, j.erros, j.evidenciaPerdida);
    fclose(f);
}


#define MAX_DEBUG_RECTS 32
static Rectangle g_debugRects[MAX_DEBUG_RECTS];
static int g_numDebugRects = 0;

static void RegistrarDebug(Rectangle btn) {
    if (g_numDebugRects < MAX_DEBUG_RECTS) g_debugRects[g_numDebugRects++] = btn;
}


// ======= SONS GLOBAIS (usados dentro de BotaoClicado / CliqueQualquerLugar / telas de evidência) =======
Sound sfxClick = { 0 };      // ai.libi_3_click
bool temSfxClick = false;

Sound sfxBrilho = { 0 };     // ai.libi_4_brilho (evidência desbloqueada)
bool temSfxBrilho = false;


// Retorna o primeiro caminho existente entre as variações de pasta (menu e todas as cenas)
const char* AcharArquivoAudio(const char* nomes[], int qtd) {
    const char* pastas[] = {
        "src/menu_inicial/", "jogo_ai.libi/src/menu_inicial/", "menu_inicial/",
        "src/cena1/",        "jogo_ai.libi/src/cena1/",        "cena1/",
        "src/cena2/",        "jogo_ai.libi/src/cena2/",        "cena2/",
        "src/cena3/",        "jogo_ai.libi/src/cena3/",        "cena3/",
        "src/cena4/",        "jogo_ai.libi/src/cena4/",        "cena4/",
        "src/cena5/",        "jogo_ai.libi/src/cena5/",        "cena5/",
        "src/cena6/",        "jogo_ai.libi/src/cena6/",        "cena6/",
        "src/cena7/",        "jogo_ai.libi/src/cena7/",        "cena7/",
        "src/cena8/",        "jogo_ai.libi/src/cena8/",        "cena8/",
        "src/cenafinal/",    "jogo_ai.libi/src/cenafinal/",    "cenafinal/",
        ""
    };
    const int qtdPastas = sizeof(pastas) / sizeof(pastas[0]);
    static char caminho[256];
    for (int n = 0; n < qtd; n++) {
        for (int p = 0; p < qtdPastas; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return caminho;
        }
    }
    return NULL;
}


bool BotaoClicado(Rectangle btn, Vector2 posMouse) {
    RegistrarDebug(btn);
    bool clicou = CheckCollisionPointRec(posMouse, btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (clicou && temSfxClick) PlaySound(sfxClick);
    return clicou;
}


// Clique em qualquer lugar da tela (usado para avançar a Cena 1) + som de clique
bool CliqueQualquerLugar(void) {
    bool clicou = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if (clicou && temSfxClick) PlaySound(sfxClick);
    return clicou;
}


// Avançar em telas de diálogo: SÓ avança ao clicar no botão específico da tela
bool AvancarDialogo(Rectangle btnAvancar, Vector2 posMouse) {
    return BotaoClicado(btnAvancar, posMouse);
}


// Desenha uma textura esticada na área "dest".
// Se a textura NÃO carregou e a área é a tela inteira, mostra um aviso roxo em vez de tela preta.
void DesenharTela(Texture2D t, Rectangle dest) {
    if (t.id == 0) {
        if (dest.width >= 700.0f) {
            DrawRectangleRec(dest, (Color){ 40, 20, 70, 255 });
            DrawText("IMAGEM NAO ENCONTRADA", 190, 250, 30, RAYWHITE);
            DrawText("Veja o terminal (WARNING) para o nome do arquivo.", 150, 300, 18, LIGHTGRAY);
            DrawText("Os botoes desta tela continuam funcionando. F1 mostra as areas.", 110, 330, 18, LIGHTGRAY);
        }
        return;
    }
    Rectangle orig = { 0.0f, 0.0f, (float)t.width, (float)t.height };
    Vector2 zero = { 0.0f, 0.0f };
    DrawTexturePro(t, orig, dest, zero, 0.0f, WHITE);
}


// Desenha só um pedaço de uma textura (fx, fy, fw, fh = fração 0..1 da imagem) na mesma posição da tela 800x600.
// Usado na tela de configurações para combinar "volume ATIVADO" + "tela cheia ATIVADO".
void DesenharRecorte(Texture2D t, float fx, float fy, float fw, float fh, float larguraTela, float alturaTela) {
    if (t.id == 0) return;
    Rectangle src = { fx * t.width, fy * t.height, fw * t.width, fh * t.height };
    Rectangle dst = { fx * larguraTela, fy * alturaTela, fw * larguraTela, fh * alturaTela };
    Vector2 zero = { 0.0f, 0.0f };
    DrawTexturePro(t, src, dst, zero, 0.0f, WHITE);
}


// Gera mipmaps e filtro bilinear (se a textura existir)
void PrepararTextura(Texture2D* t) {
    if (t->id != 0) {
        GenTextureMipmaps(t);
        SetTextureFilter(*t, TEXTURE_FILTER_TRILINEAR);
    }
}


// Carrega uma imagem do menu (menu, regras, configurações, créditos) procurando em várias pastas.
// Aceita até 2 nomes alternativos (o segundo pode ser NULL).
Texture2D CarregarMenu(const char* nome1, const char* nome2) {
    const char* pastas[] = { "src/menu_inicial/", "jogo_ai.libi/src/menu_inicial/", "menu_inicial/", "" };
    const char* nomes[2] = { nome1, nome2 };
    char caminho[256];

    for (int n = 0; n < 2; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "MENU: imagem NAO encontrada: %s", nome1);
    return (Texture2D){ 0 };
}


// Tenta carregar uma imagem da Cena 1 em várias pastas e com dois nomes possíveis (C1_N.png ou C1.N.png)
Texture2D CarregarCena1(const char* nome1, const char* nome2) {
    const char* pastas[] = { "src/cena1/", "cena1/", "jogo_ai.libi/src/cena1/", "" };
    const char* nomes[2] = { nome1, nome2 };
    char caminho[256];

    for (int n = 0; n < 2; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "CENA1: imagem NAO encontrada: %s (nem %s)", nome1, nome2 ? nome2 : "-");
    return (Texture2D){ 0 };
}


// Tenta carregar uma imagem da Cena 2 em várias pastas e com dois nomes possíveis
Texture2D CarregarCena2(const char* nome1, const char* nome2) {
    const char* pastas[] = { "src/cena2/", "cena2/", "jogo_ai.libi/src/cena2/", "" };
    const char* nomes[2] = { nome1, nome2 };
    char caminho[256];

    for (int n = 0; n < 2; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "CENA2: imagem NAO encontrada: %s (nem %s)", nome1, nome2 ? nome2 : "-");
    return (Texture2D){ 0 };
}


// Tenta carregar uma imagem da Cena 3 em várias pastas e com dois nomes possíveis
Texture2D CarregarCena3(const char* nome1, const char* nome2) {
    const char* pastas[] = { "src/cena3/", "cena3/", "src/cena2/", "" };
    const char* nomes[2] = { nome1, nome2 };
    char caminho[256];

    for (int n = 0; n < 2; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) return LoadTexture(caminho);
        }
    }
    TraceLog(LOG_WARNING, "CENA3: imagem NAO encontrada: %s (nem %s)", nome1, nome2 ? nome2 : "-");
    return (Texture2D){ 0 };
}


// Tenta carregar uma imagem da Cena 4 em várias pastas e com até 3 nomes possíveis
Texture2D CarregarCena4(const char* nome1, const char* nome2, const char* nome3) {
    const char* pastas[] = { "src/cena4/", "cena4/", "src/cena3/", "" };
    const char* nomes[3] = { nome1, nome2, nome3 };
    char caminho[256];

    for (int n = 0; n < 3; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) { TraceLog(LOG_INFO, "CENA4: carregou %s", caminho); return LoadTexture(caminho); }
        }
    }
    TraceLog(LOG_WARNING, "CENA4: imagem NAO encontrada: %s", nome1);
    return (Texture2D){ 0 };
}


// Verifica se existe uma imagem da Cena 4 (sem carregar)
bool ExisteCena4(int numero) {
    const char* pastas[] = { "src/cena4/", "cena4/", "src/cena3/", "" };
    const char* formatos[] = { "c4.%d.png", "C4.%d.png", "c4_%d.png" };
    char caminho[256], nome[64];

    for (int f = 0; f < 3; f++) {
        snprintf(nome, sizeof(nome), formatos[f], numero);
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nome);
            if (FileExists(caminho)) return true;
        }
    }
    return false;
}

Texture2D CarregarCena4Num(int numero) {
    char a[64], b[64], c[64];
    snprintf(a, sizeof(a), "c4.%d.png", numero);
    snprintf(b, sizeof(b), "C4.%d.png", numero);
    snprintf(c, sizeof(c), "c4_%d.png", numero);
    return CarregarCena4(a, b, c);
}


// Tenta carregar uma imagem da Cena 5 em várias pastas e com até 4 nomes possíveis
// (procura SÓ nas pastas da cena 5, para não pegar imagem de outra cena por engano)
Texture2D CarregarCena5(const char* n1, const char* n2, const char* n3, const char* n4) {
    const char* pastas[] = { "src/cena5/", "cena5/", "jogo_ai.libi/src/cena5/", "" };
    const char* nomes[4] = { n1, n2, n3, n4 };
    char caminho[256];

    for (int n = 0; n < 4; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) {
                TraceLog(LOG_INFO, "CENA5: carregou %s", caminho);
                return LoadTexture(caminho);
            }
        }
    }
    TraceLog(LOG_WARNING, "CENA5: imagem NAO encontrada: %s", n1);
    return (Texture2D){ 0 };
}

Texture2D CarregarCena5Num(int numero) {
    char a[64], b[64], c[64], d[64];
    snprintf(a, sizeof(a), "c5_%d.png", numero);
    snprintf(b, sizeof(b), "c5.%d.png", numero);
    snprintf(c, sizeof(c), "C5_%d.png", numero);
    snprintf(d, sizeof(d), "C5.%d.png", numero);
    return CarregarCena5(a, b, c, d);
}

// Telas "P5_X" da Cena 5 (P5_1 = desbloqueada, P5_2 = origem, P5_3 = pergunta)
Texture2D CarregarCena5P(int numero) {
    char a[64], b[64], c[64], d[64];
    snprintf(a, sizeof(a), "P5_%d.png", numero);
    snprintf(b, sizeof(b), "p5_%d.png", numero);
    snprintf(c, sizeof(c), "P5.%d.png", numero);
    snprintf(d, sizeof(d), "p5.%d.png", numero);
    return CarregarCena5(a, b, c, d);
}


// Tenta carregar uma imagem da Cena 6 em várias pastas e com até 5 nomes possíveis
// (procura SÓ nas pastas da cena 6, para não pegar imagem de outra cena por engano)
Texture2D CarregarCena6(const char* n1, const char* n2, const char* n3, const char* n4, const char* n5) {
    const char* pastas[] = { "src/cena6/", "cena6/", "jogo_ai.libi/src/cena6/", "" };
    const char* nomes[5] = { n1, n2, n3, n4, n5 };
    char caminho[256];

    for (int n = 0; n < 5; n++) {
        if (nomes[n] == NULL) continue;
        for (int p = 0; p < 4; p++) {
            snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nomes[n]);
            if (FileExists(caminho)) {
                TraceLog(LOG_INFO, "CENA6: carregou %s", caminho);
                return LoadTexture(caminho);
            }
        }
    }
    TraceLog(LOG_WARNING, "CENA6: imagem NAO encontrada: %s", n1);
    return (Texture2D){ 0 };
}

// c6.N.png / c6_N.png / C6.N.png / C6_N.png
Texture2D CarregarCena6Num(int numero) {
    char a[64], b[64], c[64], d[64], e[64];
    snprintf(a, sizeof(a), "c6.%d.png", numero);
    snprintf(b, sizeof(b), "c6_%d.png", numero);
    snprintf(c, sizeof(c), "C6.%d.png", numero);
    snprintf(d, sizeof(d), "C6_%d.png", numero);
    snprintf(e, sizeof(e), "c6_%d_1.png", numero);
    return CarregarCena6(a, b, c, d, e);
}

// Telas "p6_X" da Cena 6 (p6_1 = origem, p6_2 = pergunta)
Texture2D CarregarCena6P(int numero) {
    char a[64], b[64], c[64], d[64], e[64];
    snprintf(a, sizeof(a), "p6.%d.png", numero);
    snprintf(b, sizeof(b), "p6_%d.png", numero);
    snprintf(c, sizeof(c), "P6.%d.png", numero);
    snprintf(d, sizeof(d), "P6_%d.png", numero);
    snprintf(e, sizeof(e), "p6_%d_1.png", numero);
    return CarregarCena6(a, b, c, d, e);
}


// Algumas imagens (c8_4, F2_1, F2_2, F3_1, F3_2) foram exportadas com uma faixa
// TRANSPARENTE no topo, que aparecia como uma barra preta em cima da cena.
// Esta função carrega a imagem e corta as linhas totalmente transparentes
// do topo e da base antes de criar a textura.
static bool LinhaTransparente(Image img, int y) {
    const unsigned char* px = (const unsigned char*)img.data;
    for (int x = 0; x < img.width; x++) {
        if (px[(y * img.width + x) * 4 + 3] > 8) return false;   // canal alpha
    }
    return true;
}

Texture2D CarregarTexturaSemBordaTransparente(const char* caminho) {
    Image img = LoadImage(caminho);
    if (img.data == NULL) return (Texture2D){ 0 };

    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

    int topo = 0;
    int base = img.height - 1;
    while (topo < base && LinhaTransparente(img, topo)) topo++;
    while (base > topo && LinhaTransparente(img, base)) base--;

    if (topo > 0 || base < img.height - 1) {
        TraceLog(LOG_INFO, "Cortando faixa transparente: topo=%d base=%d (%s)", topo, img.height - 1 - base, caminho);
        ImageCrop(&img, (Rectangle){ 0.0f, (float)topo, (float)img.width, (float)(base - topo + 1) });
    }

    Texture2D t = LoadTextureFromImage(img);
    UnloadImage(img);
    return t;
}


// ======================================================================
// CENA 7, CENA 8 e FINAL 2: carregador genérico
// Procura "<etiqueta><sep><numero>.png" onde a etiqueta pode estar como foi passada,
// toda minúscula ou toda maiúscula (c7 / C7, P7 / p7, F2 / f2) e o separador
// pode ser "_" ou "." (c7_1.png ou c7.1.png). Procura em várias pastas possíveis.
// ======================================================================
Texture2D CarregarGenerica(const char* etiqueta, int numero) {
    const char* pastas[] = {
        // Pastas reais do projeto: src/cena7, src/cena8, src/cenafinal
        "src/cena7/",     "cena7/",     "jogo_ai.libi/src/cena7/",
        "src/cena8/",     "cena8/",     "jogo_ai.libi/src/cena8/",
        "src/cenafinal/", "cenafinal/", "jogo_ai.libi/src/cenafinal/",
        ""
    };
    const int qtdPastas = sizeof(pastas) / sizeof(pastas[0]);
    const char seps[2] = { '_', '.' };
    char tag[16], nome[64], caminho[256];

    for (int modo = 0; modo < 3; modo++) {           // 0 = como veio, 1 = minúscula, 2 = maiúscula
        snprintf(tag, sizeof(tag), "%s", etiqueta);
        for (char* c = tag; *c; c++) {
            if (modo == 1) *c = (char)tolower((unsigned char)*c);
            else if (modo == 2) *c = (char)toupper((unsigned char)*c);
        }
        for (int s = 0; s < 2; s++) {
            snprintf(nome, sizeof(nome), "%s%c%d.png", tag, seps[s], numero);
            for (int p = 0; p < qtdPastas; p++) {
                snprintf(caminho, sizeof(caminho), "%s%s", pastas[p], nome);
                if (FileExists(caminho)) {
                    TraceLog(LOG_INFO, "CENA7/8/F2/F3: carregou %s", caminho);
                    return CarregarTexturaSemBordaTransparente(caminho);
                }
            }
        }
    }
    TraceLog(LOG_WARNING, "CENA7/8/F2: imagem NAO encontrada: %s_%d.png", etiqueta, numero);
    return (Texture2D){ 0 };
}


// Liga/desliga a tela cheia mantendo o jogo proporcional (barras pretas nas laterais).
// Truque importante: ao entrar, ajusta a janela para o tamanho do monitor ANTES do ToggleFullscreen;
// assim o raylib não troca a resolução do monitor para 800x600.
void DefinirTelaCheia(bool ativar, int larguraJanela, int alturaJanela) {
    if (ativar == IsWindowFullscreen()) return;

    int monitor = GetCurrentMonitor();

    if (ativar) {
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    } else {
        ToggleFullscreen();
        SetWindowSize(larguraJanela, alturaJanela);

        // Centraliza a janela no monitor atual
        Vector2 posMonitor = GetMonitorPosition(monitor);
        SetWindowPosition((int)posMonitor.x + (GetMonitorWidth(monitor) - larguraJanela) / 2,
                          (int)posMonitor.y + (GetMonitorHeight(monitor) - alturaJanela) / 2);
    }
}


int main(void) {
    const int larguraTela = 800;
    const int alturaTela = 600;


    InitWindow(larguraTela, alturaTela, "ai.libe - Projeto Investigativo");
    InitAudioDevice();
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);   // ESC não fecha mais o jogo (ESC é usado para voltar ao menu)

    GameState estadoAtual = STATE_MENU;

    // Progresso do jogador (acertos, erros, evidência perdida)
    Jogador jogador = {0};

    // ======= CARREGAMENTO DE TEXTURAS DO MENU (multi-caminho) =======
    Texture2D imgMenu = CarregarMenu("menu.png", NULL);

    // Configurações: agora são 2 botões (VOLUME GERAL e TELA CHEIA), cada um ATIVADO/DESATIVADO
    //   configuracoes3 = volume DESATIVADO + tela cheia DESATIVADO
    //   configuracoes2 = volume ATIVADO    + tela cheia DESATIVADO
    //   configuracoes1 = volume DESATIVADO + tela cheia ATIVADO
    //   (volume ATIVADO + tela cheia ATIVADO = configuracoes2 com o recorte da linha "tela cheia" da configuracoes1)
    Texture2D texConfigTudoOff = CarregarMenu("configuracoes3.png", "configuracoes.png");
    Texture2D texConfigVolOn   = CarregarMenu("configuracoes2.png", NULL);
    Texture2D texConfigTelaOn  = CarregarMenu("configuracoes1.png", NULL);

    Texture2D texCreditos = CarregarMenu("creditos.png", NULL);

    Texture2D texRegras[5];
    texRegras[0] = CarregarMenu("REGRAS1.png", "regras1.png");   // Bem vindo, detetive
    texRegras[1] = CarregarMenu("REGRAS2.png", "regras2.png");   // Interrogatórios
    texRegras[2] = CarregarMenu("REGRAS3.png", "regras3.png");   // Desbloqueando evidências
    texRegras[3] = CarregarMenu("REGRAS4.png", "regras4.png");   // Real ou IA?
    texRegras[4] = CarregarMenu("REGRAS5.png", "regras5.png");   // A decisão final (botão JOGAR)

    int paginaRegras = 0;

    // ======= TEXTURAS DA CENA 1 (6 telas) =======
    // C1_1 Nova ocorrência (Acessar caso)      | C1_2 Gabriel Moura (clique para continuar)
    // C1_3 Última informação (clique na foto)  | C1_4 Foto do Caio + "Acessar novas mensagens"
    // C1_5 Mensagens do Chefe (clique p/ cont.) | C1_6 Interrogatório (Ir interrogar Caio)
    // As fotos do Gabriel e do Caio agora já vêm dentro das imagens (não precisa mais de fotogabriel/fotocaio).
    Texture2D imgC1[6] = { 0 };
    for (int k = 0; k < 6; k++) {
        char a[64], b[64];
        snprintf(a, sizeof(a), "C1_%d.png", k + 1);   // nome novo tem prioridade
        snprintf(b, sizeof(b), "C1.%d.png", k + 1);
        imgC1[k] = CarregarCena1(a, b);
    }


    // ======= TEXTURAS DA CENA 2 =======
    Texture2D imgC2[12];
    for (int k = 0; k < 12; k++) {
        char a[64], b[64];
        snprintf(a, sizeof(a), "C2.%d.png", k);
        snprintf(b, sizeof(b), "C2_%d.png", k);
        imgC2[k] = CarregarCena2(b, a);   // C2_N.png (novo) tem prioridade sobre C2.N.png
    }

    // RAMIFICAÇÃO DELEGADO
    Texture2D imgF1[4];
    for (int k = 0; k < 4; k++) {
        char a[64], b[64];
        snprintf(a, sizeof(a), "F1.%d.png", k + 1);
        snprintf(b, sizeof(b), "F1_%d.png", k + 1);
        imgF1[k] = CarregarCena2(b, a);
    }

    Texture2D imgP1   = CarregarCena2("P1.png",   "P1_0.png");
    Texture2D imgP1_1 = CarregarCena2("P1_1.png", "P1.1.png");
    Texture2D imgP1_2 = CarregarCena2("P1_2.png", "P1.2.png");


    // ======= TEXTURAS DA CENA 3 =======
    Texture2D imgC3_1 = CarregarCena3("C3.1.png", "C3_1.png");
    Texture2D imgC3_2 = CarregarCena3("C3.2.png", "C3_2.png");
    Texture2D imgC3_3 = CarregarCena3("C3.3.png", "C3_3.png");
    Texture2D imgC3_4 = CarregarCena3("C3.4.png", "C3_4.png");
    Texture2D imgC3_5 = CarregarCena3("C3.5.png", "C3_5.png");
    Texture2D imgP2   = CarregarCena3("P2.png", "P2.1.png");

    // Player de áudio (4 combinações de ícones)
    Texture2D imgAudioPausaSom  = CarregarCena3("p2.(2.1).png", "p2_2_1_.png");  // pausado + com som
    Texture2D imgAudioPausaMudo = CarregarCena3("p2.(2.2).png", "p2__2_2_.png"); // pausado + mudo
    Texture2D imgAudioPlaySom   = CarregarCena3("p2.(2.3).png", "p2_2__3_.png"); // tocando + com som
    Texture2D imgAudioPlayMudo  = CarregarCena3("p2.(2.4).png", "p2_2_4_.png");  // tocando + mudo
    Texture2D imgP2Pergunta     = CarregarCena3("p2.3.png",     "p2_3.png");     // Verdadeira ou manipulada?

    // Áudio opcional da prova (se o arquivo não existir, o jogo só simula o tempo)
    Music audioProva = { 0 };
    bool temAudio = false;
    if (FileExists("src/cena3/audio_prova.mp3")) audioProva = LoadMusicStream("src/cena3/audio_prova.mp3");
    else if (FileExists("src/cena3/audio_prova.wav")) audioProva = LoadMusicStream("src/cena3/audio_prova.wav");
    else if (FileExists("src/cena3/audio_prova.ogg")) audioProva = LoadMusicStream("src/cena3/audio_prova.ogg");
    if (audioProva.stream.buffer != NULL) {
        temAudio = true;
        audioProva.looping = false;
    }

    // ======= ÁUDIO DO MENU E DO JOGO =======
    // ai.libi_1 = trilha do menu (loop) | ai.libi_2 = trilha do jogo (loop)
    // ai.libi_3_click = clique de botão  | ai.libi_4_brilho = evidência desbloqueada
    Music musicaMenu = { 0 };
    bool temMusicaMenu = false;

    const char* nomesTrilha[] = {
        "ai.libi_1.wav", "ai.libi_1.wav.wav", "ai.libi_1.mp3", "ai.libi_1.mp3.mp3", "ai.libi_1.ogg"
    };
    const char* caminhoTrilha = AcharArquivoAudio(nomesTrilha, sizeof(nomesTrilha) / sizeof(nomesTrilha[0]));
    if (caminhoTrilha != NULL) {
        musicaMenu = LoadMusicStream(caminhoTrilha);
        if (musicaMenu.stream.buffer != NULL) {
            temMusicaMenu = true;
            musicaMenu.looping = true;   // loop infinito
        }
    }
    if (!temMusicaMenu) TraceLog(LOG_WARNING, "MENU: trilha ai.libi_1 NAO encontrada");

    Music musicaJogo = { 0 };
    bool temMusicaJogo = false;
    bool musicaJogoPausada = false;
    float duckMusica = 1.0f;   // 1.0 = volume normal; abaixa enquanto o "brilho" toca

    const char* nomesJogo[] = {
        "ai.libi_2.wav", "ai.libi_2.wav.wav", "ai.libi_2.mp3", "ai.libi_2.mp3.mp3", "ai.libi_2.ogg"
    };
    const char* caminhoJogo = AcharArquivoAudio(nomesJogo, sizeof(nomesJogo) / sizeof(nomesJogo[0]));
    if (caminhoJogo != NULL) {
        musicaJogo = LoadMusicStream(caminhoJogo);
        if (musicaJogo.stream.buffer != NULL) {
            temMusicaJogo = true;
            musicaJogo.looping = true;   // loop infinito
            SetMusicVolume(musicaJogo, 0.0f);
        }
    }
    if (!temMusicaJogo) TraceLog(LOG_WARNING, "JOGO: trilha ai.libi_2 NAO encontrada");

    const char* nomesClick[] = {
        "ai.libi_3_click.mp3", "ai.libi_3_click.mp3.mp3", "ai.libi_3_click.wav", "ai.libi_3_click.ogg"
    };
    const char* caminhoClick = AcharArquivoAudio(nomesClick, sizeof(nomesClick) / sizeof(nomesClick[0]));
    if (caminhoClick != NULL) {
        sfxClick = LoadSound(caminhoClick);
        temSfxClick = (sfxClick.stream.buffer != NULL);
    }
    if (!temSfxClick) TraceLog(LOG_WARNING, "MENU: som ai.libi_3_click NAO encontrado");

    const char* nomesBrilho[] = {
        "ai.libi_4_brilho.mp3", "ai.libi_4_brilho.mp3.mp3", "ai.libi_4_brilho.wav", "ai.libi_4_brilho.ogg"
    };
    const char* caminhoBrilho = AcharArquivoAudio(nomesBrilho, sizeof(nomesBrilho) / sizeof(nomesBrilho[0]));
    if (caminhoBrilho != NULL) {
        sfxBrilho = LoadSound(caminhoBrilho);
        temSfxBrilho = (sfxBrilho.stream.buffer != NULL);
    }
    if (!temSfxBrilho) TraceLog(LOG_WARNING, "JOGO: som ai.libi_4_brilho NAO encontrado");

    // Crossfade entre a trilha do menu e a do jogo
    const float duracaoFade = 1.5f;   // duração da transição, em segundos
    float gananciaMenu = 1.0f;        // 0.0 a 1.0 (começa no máximo para o menu tocar normal ao abrir o jogo)
    float gananciaJogo = 0.0f;        // 0.0 a 1.0

    // ======= TEXTURAS DA CENA 4 =======
    int numDialogosC4 = C4_FORCAR_DIALOGOS ? C4_FORCAR_DIALOGOS : (ExisteCena4(19) ? 15 : 14);
    TraceLog(LOG_INFO, "CENA4: %d dialogos detectados", numDialogosC4);

    Texture2D imgC4[15] = { 0 };
    for (int k = 0; k < numDialogosC4; k++) imgC4[k] = CarregarCena4Num(k + 1);

    // Telas especiais da Cena 4
    Texture2D imgC4Evidencia    = CarregarCena4("c4.15.png", "C4.15.png", "c4_15.png"); // Evidência encontrada
    Texture2D imgC4Desafio      = CarregarCena4("c4.16.png", "C4.16.png", "c4_16.png"); // Questão de lógica
    Texture2D imgC4Perdida      = CarregarCena4("c4.17.png", "C4.17.png", "c4_17.png"); // Evidência perdida
    Texture2D imgC4Desbloqueada = CarregarCena4("c4.18.png", "C4.18.png", "c4_18.png"); // Evidência desbloqueada
    Texture2D imgP4_1           = CarregarCena4("p4.1.png",  "p4_1.png",  "P4.1.png");   // Prova 1
    Texture2D imgP4_2           = CarregarCena4("p4.2.png",  "p4_2.png",  "P4.2.png");   // Prova 2

    // ======= TEXTURAS DA CENA 5 =======
    Texture2D imgC5[10] = { 0 };
    for (int k = 0; k < 10; k++) imgC5[k] = CarregarCena5Num(k + 1);

    Texture2D imgC5Evidencia    = CarregarCena5Num(11);   // c5_11: Nova evidência encontrada
    Texture2D imgC5Desafio      = CarregarCena5Num(12);   // c5_12: Desafio de lógica
    Texture2D imgC5Perdida      = CarregarCena5Num(13);   // c5_13: Evidência perdida
    Texture2D imgC5Desbloqueada = CarregarCena5P(1);      // P5_1: Evidência desbloqueada
    Texture2D imgC5Origem       = CarregarCena5P(2);      // P5_2: Origem (clique para continuar)
    Texture2D imgC5Pergunta     = CarregarCena5P(3);      // P5_3: Verdadeira ou manipulada?

    // ======= TEXTURAS DA CENA 6 =======
    // c6_1..c6_12 = diálogos (o c6_8 é o "Algumas horas depois")
    Texture2D imgC6[12] = { 0 };
    for (int k = 0; k < 12; k++) imgC6[k] = CarregarCena6Num(k + 1);

    Texture2D imgC6Evidencia = CarregarCena6Num(13);   // c6_13: Nova evidência encontrada
    Texture2D imgC6Desafio   = CarregarCena6Num(14);   // c6_14: Desafio de lógica
    Texture2D imgC6Perdida   = CarregarCena6Num(15);   // c6_15: Evidência perdida
    Texture2D imgC6Origem    = CarregarCena6P(1);      // p6_1: Origem (clique para continuar)
    Texture2D imgC6Pergunta  = CarregarCena6P(2);      // p6_2: Verdadeira ou manipulada?
    // Evidência desbloqueada da Cena 6 (opcional: se não existir, usa a da Cena 5)
    Texture2D imgC6Desbloqueada = CarregarCena6("P6.png", "p6.png", "P6_0.png", "p6_0.png", "P6.0.png");

    // ======= TEXTURAS DA CENA 7 =======
    // c7_1 Chefe | c7_2 Você | c7_3 Nova evidência | c7_4 Desafio | c7_5 Perdida
    // P7_1 Desbloqueada | p7_2 Origem | p7_3 Verdadeira ou manipulada?
    Texture2D imgC7_1           = CarregarGenerica("c7", 1);
    Texture2D imgC7_2           = CarregarGenerica("c7", 2);
    Texture2D imgC7Evidencia    = CarregarGenerica("c7", 3);
    Texture2D imgC7Desafio      = CarregarGenerica("c7", 4);
    Texture2D imgC7Perdida      = CarregarGenerica("c7", 5);
    Texture2D imgC7Desbloqueada = CarregarGenerica("P7", 1);
    Texture2D imgC7Origem       = CarregarGenerica("P7", 2);
    Texture2D imgC7Pergunta     = CarregarGenerica("P7", 3);

    // ======= TEXTURAS DA CENA 8 (decisão final) =======
    // c8_1 A decisão final | c8_2 Delegado | c8_3 Você | c8_4 Delegado | c8_5 Escolha o veredito
    Texture2D imgC8[5] = { 0 };
    for (int k = 0; k < 5; k++) imgC8[k] = CarregarGenerica("c8", k + 1);

    // ======= TEXTURAS DO FINAL 2 (acusou o Caio) =======
    // F2_1 Você | F2_2 Tem certeza? | F2_3 Veredito incorreto | F2_4 Escaneie a explicação
    Texture2D imgF2[4] = { 0 };
    for (int k = 0; k < 4; k++) imgF2[k] = CarregarGenerica("F2", k + 1);

    // ======= TEXTURAS DO FINAL 3 (acusou a Juliana) =======
    // F3_1 Você | F3_2 Tem certeza? | F3_3 Veredito correto | F3_4 Escaneie a explicação
    Texture2D imgF3[4] = { 0 };
    for (int k = 0; k < 4; k++) imgF3[k] = CarregarGenerica("F3", k + 1);

    // ======= MIPMAPS + FILTRO =======
    Texture2D* todasTexturas[] = {
        &imgMenu, &texConfigTudoOff, &texConfigVolOn, &texConfigTelaOn, &texCreditos,
        &texRegras[0], &texRegras[1], &texRegras[2], &texRegras[3], &texRegras[4],
        &imgP1, &imgP1_1, &imgP1_2,
        &imgC3_1, &imgC3_2, &imgC3_3, &imgC3_4, &imgC3_5, &imgP2,
        &imgAudioPlayMudo, &imgAudioPlaySom, &imgAudioPausaMudo, &imgAudioPausaSom, &imgP2Pergunta,
        &imgC4Evidencia, &imgC4Desafio, &imgC4Perdida, &imgC4Desbloqueada, &imgP4_1, &imgP4_2,
        &imgC7_1, &imgC7_2, &imgC7Evidencia, &imgC7Desafio, &imgC7Perdida,
        &imgC7Desbloqueada, &imgC7Origem, &imgC7Pergunta
    };
    int totalTexturas = sizeof(todasTexturas) / sizeof(todasTexturas[0]);

    for (int k = 0; k < totalTexturas; k++) PrepararTextura(todasTexturas[k]);
    for (int k = 0; k < 6;  k++) PrepararTextura(&imgC1[k]);
    for (int k = 0; k < 12; k++) PrepararTextura(&imgC2[k]);
    for (int k = 0; k < 4;  k++) PrepararTextura(&imgF1[k]);
    for (int k = 0; k < 15; k++) PrepararTextura(&imgC4[k]);

    for (int k = 0; k < 10; k++) PrepararTextura(&imgC5[k]);
    PrepararTextura(&imgC5Evidencia);
    PrepararTextura(&imgC5Desafio);
    PrepararTextura(&imgC5Perdida);
    PrepararTextura(&imgC5Desbloqueada);
    PrepararTextura(&imgC5Origem);
    PrepararTextura(&imgC5Pergunta);

    for (int k = 0; k < 12; k++) PrepararTextura(&imgC6[k]);
    PrepararTextura(&imgC6Evidencia);
    PrepararTextura(&imgC6Desafio);
    PrepararTextura(&imgC6Perdida);
    PrepararTextura(&imgC6Origem);
    PrepararTextura(&imgC6Pergunta);
    PrepararTextura(&imgC6Desbloqueada);

    for (int k = 0; k < 5; k++) PrepararTextura(&imgC8[k]);
    for (int k = 0; k < 4; k++) PrepararTextura(&imgF2[k]);
    for (int k = 0; k < 4; k++) PrepararTextura(&imgF3[k]);


    Rectangle dest = { 0.0f, 0.0f, (float)larguraTela, (float)alturaTela };

    // ======= BOTÕES E ÁREAS DE CLIQUE (coordenadas na tela virtual 800x600) =======
    // (calculados a partir das imagens 1440x1024: x * 0.5556, y * 0.5859)

    // MENU: JOGAR / REGRAS / CONFIGURAÇÕES / CRÉDITOS
    Rectangle btnJogar    = { 115, 270, 110, 36 };
    Rectangle btnRegras   = { 138, 324,  90, 36 };
    Rectangle btnConfig   = { 108, 374, 176, 36 };
    Rectangle btnCreditos = { 133, 428, 108, 36 };

    // CONFIGURAÇÕES: dois botões de liga/desliga + VOLTAR
    Rectangle btnVolumeGeral = { 574, 236, 110, 26 };   // VOLUME GERAL (ATIVADO/DESATIVADO)
    Rectangle btnTelaCheia   = { 574, 328, 110, 26 };   // TELA CHEIA   (ATIVADO/DESATIVADO)
    Rectangle btnVoltarMenu  = { 100, 503,  90, 32 };   // VOLTAR (configurações)

    // CRÉDITOS: VOLTAR (sem ícone, mais à direita que o das configurações)
    Rectangle btnVoltarCreditos = { 130, 521, 62, 30 };

    // REGRAS (5 páginas): VOLTAR / CONTINUAR (na última página o botão vira "JOGAR", mesma posição)
    Rectangle btnVoltarRegras    = { 100, 520,  95, 32 };
    Rectangle btnContinuarRegras = { 615, 520, 110, 32 };


    // VOLTAR (seta + texto no canto superior esquerdo, igual em todas as telas novas)
    Rectangle btnVoltar = { 28, 36, 92, 32 };

    // Essas imagens (c8_4, F2_1/F2_2, F3_1/F3_2) tinham uma faixa transparente no topo.
    // Como ela agora é cortada ao carregar, o VOLTAR e o AVANÇAR ficam nas posições padrão.
    Rectangle btnVoltarAlto = { 28, 36, 92, 32 };
    Rectangle btnAvancarAlto = { 615, 553, 130, 40 };


    // ---- CENA 1 ---- (calculados a partir das imagens 1440x1024: x * 0.5556, y * 0.5859)
    Rectangle btnVoltarC1_1       = { 28, 36, 92, 32 };      // C1_1 "VOLTAR" (seta + texto, topo esquerdo) -> menu
    Rectangle btnAcessarCaso      = { 287, 397, 225, 51 };   // C1_1 "Acessar caso"
    Rectangle btnFotoCaio         = { 533, 167, 194, 284 };  // C1_3 caixa "Clique para acessar uma foto do Caio"
    Rectangle btnNovasMensagens   = { 114, 340, 242, 40 };   // C1_4 "Acessar novas mensagens"
    Rectangle btnInterrogarCaio   = { 272, 361, 256, 58 };   // C1_6 "Ir interrogar Caio"

    // ======= BOTÃO "CLIQUE PARA CONTINUAR" (CENA 1.4) =======
    Rectangle btnContinuarC1_4 = { 560, 497, 180, 30 }; // Cobre o ícone + "CLIQUE PARA CONTINUAR" (canto inferior direito)
    Rectangle btnVoltarTopo    = { 35,  35,  120, 40 }; // Cobre o botão "VOLTAR" no topo esquerdo


    // ---- CENA 2 ----
    Rectangle btnAvancarC2  = { 615, 553, 130, 40 };
    Rectangle btnResolverC2 = { 304, 382, 182, 41 };   // C2_8 "Resolver desafio"

    // Opções do desafio (C2.9)
    Rectangle btnC2OpcaoA = { 140, 296, 497, 40 };
    Rectangle btnC2OpcaoB = { 140, 350, 497, 40 };
    Rectangle btnC2OpcaoC = { 140, 403, 497, 40 };
    Rectangle btnC2OpcaoD = { 140, 454, 497, 40 };

    // C2.9: VOLTAR invisível (a imagem não desenha o botão, mas o clique funciona no topo esquerdo)
    Rectangle btnVoltarC2_9 = { 28, 36, 92, 32 };

    // Confirmação (C2.10)
    Rectangle btnAcusaCaioSim = { 142, 319, 496, 40 };
    Rectangle btnAcusaCaioNao = { 142, 373, 496, 40 };

    // Evidência perdida (C2.11)
    Rectangle btnContinuarPerdidaC2 = { 285, 403, 222, 50 };   // C2_11 "Continuar investigação"

    // Veredito incorreto (P1)
    Rectangle btnTentarNovamente = { 300, 432, 200, 46 };   // P1 "Tentar novamente"

    // Evidência desbloqueada (P1.1)
    Rectangle btnAcessarEvidencia = { 304, 388, 192, 44 };   // P1_1 "Acessar evidência"

    // Prova verdadeira ou manipulada (P1.2)
    Rectangle btnProvaVerdadeira = { 268, 482, 117, 40 };
    Rectangle btnProvaManipulada = { 423, 482, 120, 40 };


    // ---- CENA 3 ----
    Rectangle btnAvancarC3 = { 615, 553, 130, 40 };

    Rectangle btnResolverC3 = { 309, 384, 182, 38 };

    Rectangle btnC3OpcaoA = { 139, 298, 497, 40 };
    Rectangle btnC3OpcaoB = { 139, 353, 497, 40 };
    Rectangle btnC3OpcaoC = { 139, 407, 497, 40 };
    Rectangle btnC3OpcaoD = { 139, 456, 497, 40 };

    Rectangle btnContinuarPerdida = { 290, 408, 220, 44 };
    Rectangle btnAcessarEvidC3    = { 309, 390, 182, 38 };

    Rectangle btnAudioPlay = { 150, 398, 46, 46 };
    Rectangle btnAudioMudo = { 587, 398, 46, 46 };

    Rectangle btnC3Verdadeira = { 269, 398, 116, 39 };
    Rectangle btnC3Manipulada = { 423, 398, 121, 39 };

    // "Toque para continuar" invisível (p2.3): canto inferior direito do cartão, abaixo dos botões
    // Verdadeira/Manipulada. Área um pouco maior para facilitar o toque.
    Rectangle btnContinuarC3  = { 480, 455, 200, 45 };

    // Estado do player de áudio
    bool  audioTocando = true;
    bool  audioMudo = false;
    float audioTempo = 0.0f;
    const float audioDuracao = 18.0f;   // duração simulada quando não há arquivo audio_prova

    GameState origemC3 = STATE_MENU;

    // ---- CENA 4 ----
    Rectangle btnAvancarC4 = { 615, 553, 130, 40 };

    Rectangle btnResolverC4 = { 310, 392, 180, 40 };

    Rectangle btnC4OpcaoA = { 142, 326, 496, 40 };
    Rectangle btnC4OpcaoB = { 142, 381, 496, 40 };
    Rectangle btnC4OpcaoC = { 142, 434, 496, 40 };
    Rectangle btnC4OpcaoD = { 142, 484, 496, 40 };

    Rectangle btnAcessarEvidC4      = { 309, 390, 182, 38 };
    Rectangle btnContinuarPerdidaC4 = { 280, 415, 240, 40 };
    Rectangle btnContinuarOrigemC4  = { 565, 538, 185, 36 };

    Rectangle btnC4Verdadeira = { 380, 311, 128, 40 };
    Rectangle btnC4Manipulada = { 534, 311, 134, 40 };

    // De onde o jogador veio ao entrar na Cena 4 (para o VOLTAR da primeira tela)
    GameState origemC4 = STATE_MENU;

    // De onde o jogador veio ao entrar na Cena 5
    GameState origemC5 = STATE_MENU;

    // De onde o jogador veio ao entrar na Cena 6
    GameState origemC6 = STATE_MENU;

    // De onde o jogador veio ao entrar na Cena 7
    GameState origemC7 = STATE_MENU;

    // De onde o jogador veio ao entrar na Cena 8
    GameState origemC8 = STATE_MENU;

    // ---- CENA 5 ---- (coordenadas calculadas a partir das imagens 1440x1024)
    Rectangle btnAvancarC5   = { 615, 553, 130, 40 };
    Rectangle btnResolverC5  = { 309, 384, 182, 38 };

    // Opções do desafio (c5_12): 0=a, 1=b, 2=c, 3=d
    Rectangle btnC5Opcao[4] = {
        { 142, 326, 496, 40 },   // a)
        { 142, 381, 496, 40 },   // b)
        { 142, 434, 496, 40 },   // c)
        { 142, 484, 496, 40 }    // d)
    };
    // Resposta CERTA do desafio da Cena 5 (0=a, 1=b, 2=c, 3=d). Troque aqui se for outra.
    const int respostaCertaC5 = 2;

    // c5_13 Evidência perdida -> Continuar investigação
    Rectangle btnContinuarPerdidaC5 = { 298, 420, 204, 42 };

    // P5_1 Evidência desbloqueada -> Acessar evidência
    Rectangle btnAcessarEvidC5 = { 304, 388, 191, 44 };

    // P5_3 Verdadeira ou manipulada?
    Rectangle btnC5Verdadeira = { 419, 297, 129, 40 };
    Rectangle btnC5Manipulada = { 574, 297, 134, 40 };

    // ---- CENA 6 ---- (coordenadas calculadas a partir das imagens 1440x1024)
    Rectangle btnAvancarC6 = { 615, 553, 130, 40 };

    // c6_8 "Algumas horas depois" -> botão Continuar
    Rectangle btnContinuarHorasC6 = { 304, 316, 190, 42 };

    // c6_13 Nova evidência encontrada -> Resolver desafio
    Rectangle btnResolverC6 = { 309, 387, 182, 41 };

    // c6_14 Desafio de lógica: 0=a, 1=b, 2=c, 3=d
    Rectangle btnC6Opcao[4] = {
        { 142, 326, 496, 40 },   // a)
        { 142, 381, 496, 40 },   // b)
        { 142, 434, 496, 40 },   // c)
        { 142, 484, 496, 40 }    // d)
    };
    // Resposta CERTA do desafio da Cena 6 (0=a, 1=b, 2=c, 3=d).
    // b) "O celular não estava perto da estrada às 3h20" (contrapositiva). Troque aqui se for outra.
    const int respostaCertaC6 = 1;

    // c6_15 Evidência perdida -> Continuar investigação
    Rectangle btnContinuarPerdidaC6 = { 286, 403, 220, 49 };

    // Evidência desbloqueada -> Acessar evidência
    Rectangle btnAcessarEvidC6 = { 304, 388, 191, 44 };

    // p6_2 Verdadeira ou manipulada?
    Rectangle btnC6Verdadeira = { 271, 500, 115, 39 };
    Rectangle btnC6Manipulada = { 425, 500, 120, 39 };


    // ---- CENA 7 ---- (coordenadas calculadas a partir das imagens 1440x1024)
    Rectangle btnAvancarC7 = { 615, 553, 130, 40 };

    // c7_3 Nova evidência encontrada -> Resolver desafio
    Rectangle btnResolverC7 = { 309, 387, 182, 41 };

    // c7_4 Desafio de lógica: 0=a, 1=b, 2=c, 3=d
    Rectangle btnC7Opcao[4] = {
        { 141, 290, 503, 54 },   // a) Se o horário bate com o registro, então a foto é autêntica.  (recíproca)
        { 142, 352, 503, 53 },   // b) Se a foto não é autêntica, então o horário não bate.         (inversa)
        { 142, 413, 503, 54 },   // c) Se o horário não bate com o registro, então a foto não é autêntica. (contrapositiva)
        { 142, 475, 503, 54 }    // d) Se a foto é autêntica, então o horário não bate.             (negação)
    };
    // Resposta CERTA do desafio da Cena 7 (0=a, 1=b, 2=c, 3=d).
    // c) é a contrapositiva de "Se P então Q" -> "Se não Q então não P". Troque aqui se for outra.
    const int respostaCertaC7 = 2;

    // c7_5 Evidência perdida -> Continuar investigação
    Rectangle btnContinuarPerdidaC7 = { 286, 403, 222, 49 };

    // P7_1 Evidência desbloqueada -> Acessar evidência
    Rectangle btnAcessarEvidC7 = { 305, 388, 190, 44 };

    // p7_3 Verdadeira ou manipulada?
    Rectangle btnC7Verdadeira = { 271, 492, 116, 39 };
    Rectangle btnC7Manipulada = { 425, 492, 121, 39 };


    // ---- CENA 8 ---- (decisão final)
    Rectangle btnAvancarC8 = { 615, 553, 130, 40 };

    // c8_1 "Tomar a decisão"
    Rectangle btnTomarDecisao = { 306, 356, 181, 42 };

    // c8_5 Escolha o seu veredito
    Rectangle btnVeredCaio     = { 122, 256, 464, 43 };   // a) Caio Vasconcelos é o culpado.
    Rectangle btnVeredJuliana  = { 122, 320, 464, 43 };   // b) Juliana Costa é a culpada.
    Rectangle btnVeredDossie   = { 122, 384, 464, 43 };   // c) Ver o dossiê de evidências.


    // ---- FINAL 2 ---- (acusou o Caio)
    // F2_2 "Você tem certeza dessa decisão?" -> Sim / Não
    Rectangle btnF2Sim = { 322, 524, 75, 36 };
    Rectangle btnF2Nao = { 424, 524, 75, 36 };

    // F2_4 e F3_4 "Jogar novamente" (volta ao menu)
    Rectangle btnJogarNovamente = { 309, 432, 181, 41 };

    // ---- FINAL 3 ---- (acusou a Juliana)
    // F3_2 "Você tem certeza dessa decisão?" tem Sim / Não na mesma posição do F2_2
    Rectangle btnF3Sim = { 322, 524, 75, 36 };
    Rectangle btnF3Nao = { 424, 524, 75, 36 };

    // F3_3 "CLIQUE PARA CONTINUAR" (canto inferior direito, ícone + texto)
    Rectangle btnContinuarF3 = { 486, 464, 178, 34 };


    bool telaCheia = false;
    bool debugBotoes = false;

    // Configurações: VOLUME GERAL agora é um botão ATIVADO/DESATIVADO.
    // (Para o jogo abrir com o som desligado, troque para false.)
    bool volumeAtivo = true;
    const float volMusica  = 0.55f;   // volume fixo da música
    const float volEfeitos = 0.65f;   // volume fixo dos efeitos
    SetMasterVolume(volumeAtivo ? 1.0f : 0.0f);

    Vector2 posMouse = { 0.0f, 0.0f };


    while (!WindowShouldClose()) {

        float telaW = (float)GetScreenWidth();
        float telaH = (float)GetScreenHeight();
        float escala = telaW / (float)larguraTela;
        if (telaH / (float)alturaTela < escala) escala = telaH / (float)alturaTela;
        float offX = (telaW - larguraTela * escala) * 0.5f;
        float offY = (telaH - alturaTela * escala) * 0.5f;

        SetMouseOffset((int)(-offX), (int)(-offY));
        SetMouseScale(1.0f / escala, 1.0f / escala);

        // Câmera: o jogo continua sendo desenhado em coordenadas 800x600, mas cada imagem é
        // escalada direto para a resolução real da tela (sem passar por uma imagem 800x600).
        Camera2D camera = { 0 };
        camera.offset = (Vector2){ offX, offY };
        camera.target = (Vector2){ 0.0f, 0.0f };
        camera.rotation = 0.0f;
        camera.zoom = escala;

        posMouse = GetMousePosition();

        g_numDebugRects = 0;
        if (IsKeyPressed(KEY_F1)) debugBotoes = !debugBotoes;

        telaCheia = IsWindowFullscreen();   // define qual imagem de configurações aparece (ATIVADO / DESATIVADO)


        // ======================= ÁUDIO =======================
        // Volume dos efeitos (fixo)
        if (temSfxClick)  SetSoundVolume(sfxClick, volEfeitos);
        if (temSfxBrilho) SetSoundVolume(sfxBrilho, volEfeitos);

        bool naAreaMenu = (estadoAtual == STATE_MENU   ||
                           estadoAtual == STATE_REGRAS ||
                           estadoAtual == STATE_CONFIG ||
                           estadoAtual == STATE_CREDITOS);

        // O audio_prova está realmente tocando? (player aberto + em play + faixa ainda não terminou)
        bool provaTocando = (estadoAtual == STATE_C3_AUDIO) && audioTocando &&
                            (temAudio ? IsMusicStreamPlaying(audioProva) : (audioTempo < audioDuracao));

        // Quanto o fade avança por frame
        float passoFade = GetFrameTime() / duracaoFade;

        // Trilha do jogo (ai.libi_2): fade-in ao sair do menu, fade-out ao voltar a ele.
        // Pausa só enquanto o audio_prova toca (volta quando ele pausa/termina/sai).
        if (temMusicaJogo) {
            if (naAreaMenu) {
                // Voltou ao menu: faz fade-out e só então para a faixa
                if (musicaJogoPausada) {
                    StopMusicStream(musicaJogo);
                    musicaJogoPausada = false;
                    gananciaJogo = 0.0f;
                }
                gananciaJogo -= passoFade;
                if (gananciaJogo <= 0.0f) {
                    gananciaJogo = 0.0f;
                    if (IsMusicStreamPlaying(musicaJogo)) StopMusicStream(musicaJogo);
                }
                duckMusica = 1.0f;
            } else if (provaTocando) {
                if (IsMusicStreamPlaying(musicaJogo)) {
                    PauseMusicStream(musicaJogo);
                    musicaJogoPausada = true;
                }
            } else {
                if (musicaJogoPausada) {
                    ResumeMusicStream(musicaJogo);
                    musicaJogoPausada = false;
                } else if (!IsMusicStreamPlaying(musicaJogo)) {
                    PlayMusicStream(musicaJogo);
                }

                // Fade-in da trilha do jogo
                gananciaJogo += passoFade;
                if (gananciaJogo > 1.0f) gananciaJogo = 1.0f;

                // Abaixa o volume (com transição suave) enquanto o "brilho" está tocando
                float alvoDuck = (temSfxBrilho && IsSoundPlaying(sfxBrilho)) ? 0.25f : 1.0f;
                float passo = 8.0f * GetFrameTime();
                if (passo > 1.0f) passo = 1.0f;
                duckMusica += (alvoDuck - duckMusica) * passo;
            }

            // Atualiza o stream sempre que estiver tocando (inclusive durante o fade-out)
            if (IsMusicStreamPlaying(musicaJogo)) {
                SetMusicVolume(musicaJogo, volMusica * gananciaJogo * duckMusica);
                UpdateMusicStream(musicaJogo);
            }
        }

        // Trilha do menu (ai.libi_1): fade-out ao começar o jogo, fade-in ao voltar ao menu
        if (temMusicaMenu) {
            if (naAreaMenu) {
                if (!IsMusicStreamPlaying(musicaMenu)) PlayMusicStream(musicaMenu);
                gananciaMenu += passoFade;
                if (gananciaMenu > 1.0f) gananciaMenu = 1.0f;
            } else {
                gananciaMenu -= passoFade;
                if (gananciaMenu <= 0.0f) {
                    gananciaMenu = 0.0f;
                    if (IsMusicStreamPlaying(musicaMenu)) StopMusicStream(musicaMenu);
                }
            }

            if (IsMusicStreamPlaying(musicaMenu)) {
                SetMusicVolume(musicaMenu, volMusica * gananciaMenu);
                UpdateMusicStream(musicaMenu);
            }
        }


        // ======================= LÓGICA =======================
        switch (estadoAtual) {
            case STATE_MENU:
                if (BotaoClicado(btnJogar, posMouse)) estadoAtual = STATE_C1_1;
                else if (BotaoClicado(btnRegras, posMouse)) {
                    paginaRegras = 0;
                    estadoAtual = STATE_REGRAS;
                }
                else if (BotaoClicado(btnConfig, posMouse)) estadoAtual = STATE_CONFIG;
                else if (BotaoClicado(btnCreditos, posMouse)) estadoAtual = STATE_CREDITOS;
                break;


            case STATE_CONFIG:
                if (BotaoClicado(btnVolumeGeral, posMouse)) {
                    // Liga/desliga todo o áudio do jogo
                    volumeAtivo = !volumeAtivo;
                    SetMasterVolume(volumeAtivo ? 1.0f : 0.0f);
                }
                else if (BotaoClicado(btnTelaCheia, posMouse)) {
                    DefinirTelaCheia(!IsWindowFullscreen(), larguraTela, alturaTela);
                }
                else if (BotaoClicado(btnVoltarMenu, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_CREDITOS:
                if (BotaoClicado(btnVoltarCreditos, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                    estadoAtual = STATE_MENU;
                }
                break;


            case STATE_REGRAS:
                if (BotaoClicado(btnContinuarRegras, posMouse)) {
                    if (paginaRegras < 4) {
                        paginaRegras++;
                    } else {
                        // Última página (A DECISÃO FINAL) -> botão JOGAR
                        paginaRegras = 0;
                        estadoAtual = STATE_C1_1;
                    }
                }
                else if (BotaoClicado(btnVoltarRegras, posMouse)) {
                    if (paginaRegras > 0) {
                        paginaRegras--;
                    } else {
                        estadoAtual = STATE_MENU;
                    }
                }
                else if (IsKeyPressed(KEY_ESCAPE)) {
                    paginaRegras = 0;
                    estadoAtual = STATE_MENU;
                }
                break;


            // --- CENA 1 ---
            case STATE_C1_1:
                // Nova ocorrência: botão "VOLTAR" (topo esquerdo) -> menu | "Acessar caso" -> C1_2 | ESC também volta ao menu
                if (IsKeyPressed(KEY_ESCAPE)) estadoAtual = STATE_MENU;
                else if (BotaoClicado(btnVoltarC1_1, posMouse)) estadoAtual = STATE_MENU;
                else if (BotaoClicado(btnAcessarCaso, posMouse)) estadoAtual = STATE_C1_2;
                break;

            case STATE_C1_2:
                // Gabriel Moura: clique para continuar
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_1;
                else if (CliqueQualquerLugar()) estadoAtual = STATE_C1_3;
                break;

            case STATE_C1_3:
                // Última informação: clicar na caixa da foto do Caio
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_2;
                else if (BotaoClicado(btnFotoCaio, posMouse)) estadoAtual = STATE_C1_4;
                break;

            case STATE_C1_4:
                if (BotaoClicado(btnContinuarC1_4, posMouse)) {
                    estadoAtual = STATE_C1_5;
                }
                // Volta para a tela anterior ao clicar no VOLTAR do topo
                else if (BotaoClicado(btnVoltarTopo, posMouse)) {
                    estadoAtual = STATE_C1_3;
                }
                break;

            case STATE_C1_5:
                // Mensagens do Chefe: clique para continuar
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_4;
                else if (CliqueQualquerLugar()) estadoAtual = STATE_C1_6;
                break;

            case STATE_C1_6:
                // Interrogatório: botão "Ir interrogar Caio" -> Cena 2
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C1_5;
                else if (BotaoClicado(btnInterrogarCaio, posMouse)) estadoAtual = STATE_C2_0;
                break;


            // --- CENA 2 ---
            case STATE_C2_EVIDENCIA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C2_7;
                }
                else if (BotaoClicado(btnResolverC2, posMouse)) {
                    estadoAtual = STATE_C2_DESAFIO;
                }
                break;


            case STATE_C2_DESAFIO:
                // C2.9: VOLTAR invisível (topo esquerdo) -> volta para a evidência
                if (BotaoClicado(btnVoltarC2_9, posMouse)) {
                    estadoAtual = STATE_C2_EVIDENCIA;
                }
                else if (BotaoClicado(btnC2OpcaoA, posMouse)) {
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // evidência desbloqueada
                    estadoAtual = STATE_EVIDENCIA_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC2OpcaoB, posMouse) || BotaoClicado(btnC2OpcaoC, posMouse)) {
                    estadoAtual = STATE_C2_PERDIDA;
                }
                else if (BotaoClicado(btnC2OpcaoD, posMouse)) {
                    estadoAtual = STATE_CONFIRMA_CAIO;
                }
                break;


            case STATE_CONFIRMA_CAIO:
                if (BotaoClicado(btnAcusaCaioNao, posMouse)) {
                    estadoAtual = STATE_C2_DESAFIO;
                }
                else if (BotaoClicado(btnAcusaCaioSim, posMouse)) {
                    estadoAtual = STATE_ACUSA_CAIO_1;
                }
                break;


            case STATE_C2_PERDIDA:
                if (BotaoClicado(btnContinuarPerdidaC2, posMouse)) {
                    // Segue para a CENA 3
                    origemC3 = STATE_C2_PERDIDA;
                    estadoAtual = STATE_C3_1;
                }
                break;


            case STATE_ACUSA_CAIO_1:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_CONFIRMA_CAIO;
                else if (AvancarDialogo(btnAvancarC2, posMouse)) estadoAtual = STATE_ACUSA_CAIO_2;
                break;

            case STATE_ACUSA_CAIO_2:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_ACUSA_CAIO_1;
                else if (AvancarDialogo(btnAvancarC2, posMouse)) estadoAtual = STATE_ACUSA_CAIO_3;
                break;

            case STATE_ACUSA_CAIO_3:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_ACUSA_CAIO_2;
                else if (AvancarDialogo(btnAvancarC2, posMouse)) estadoAtual = STATE_ACUSA_CAIO_4;
                break;

            case STATE_ACUSA_CAIO_4:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_ACUSA_CAIO_3;
                else if (AvancarDialogo(btnAvancarC2, posMouse)) estadoAtual = STATE_VEREDITO_INCORRETO;
                break;


            case STATE_VEREDITO_INCORRETO:
                if (BotaoClicado(btnTentarNovamente, posMouse)) {
                    jogador = (Jogador){ 0 };
                    estadoAtual = STATE_MENU;   // volta para o menu inicial
                }
                break;


            case STATE_EVIDENCIA_DESBLOQUEADA:
                if (BotaoClicado(btnAcessarEvidencia, posMouse)) {
                    estadoAtual = STATE_DESAFIO_IA;
                }
                break;


            case STATE_DESAFIO_IA:
                if (BotaoClicado(btnProvaVerdadeira, posMouse) || BotaoClicado(btnProvaManipulada, posMouse)) {
                    origemC3 = STATE_DESAFIO_IA;
                    estadoAtual = STATE_C3_1;
                }
                break;


            // --- CENA 3 ---
            case STATE_C3_1:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = origemC3;
                else if (AvancarDialogo(btnAvancarC3, posMouse)) estadoAtual = STATE_C3_2;
                break;

            case STATE_C3_2:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C3_1;
                else if (AvancarDialogo(btnAvancarC3, posMouse)) estadoAtual = STATE_C3_3;
                break;

            case STATE_C3_3:
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C3_2;
                else if (BotaoClicado(btnResolverC3, posMouse)) estadoAtual = STATE_C3_4;
                break;

            case STATE_C3_4:
                if (BotaoClicado(btnC3OpcaoB, posMouse)) {
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // evidência desbloqueada
                    jogador.acertos++;
                    SalvarProgresso(jogador);
                    estadoAtual = STATE_C3_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC3OpcaoA, posMouse) ||
                         BotaoClicado(btnC3OpcaoC, posMouse) ||
                         BotaoClicado(btnC3OpcaoD, posMouse)) {
                    jogador.erros++;
                    strcpy(jogador.evidenciaPerdida, "Gravação de audio ");
                    SalvarProgresso(jogador);
                    estadoAtual = STATE_C3_PERDIDA;
                }
                break;

            case STATE_C3_PERDIDA:
                if (BotaoClicado(btnContinuarPerdida, posMouse)) {
                    // Segue para a CENA 4
                    origemC4 = STATE_C3_PERDIDA;
                    estadoAtual = STATE_C4_1;
                }
                break;

            case STATE_C3_DESBLOQUEADA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C3_4;
                }
                else if (BotaoClicado(btnAcessarEvidC3, posMouse)) {
                    // Entra no player de áudio
                    audioTocando = true;
                    audioMudo = false;
                    audioTempo = 0.0f;
                    if (temAudio) {
                        StopMusicStream(audioProva);
                        SetMusicVolume(audioProva, 1.0f);
                        PlayMusicStream(audioProva);
                    }
                    estadoAtual = STATE_C3_AUDIO;
                }
                break;

            case STATE_C3_AUDIO:
                if (temAudio) UpdateMusicStream(audioProva);

                // Tempo da faixa (só avança enquanto está tocando)
                if (audioTocando) audioTempo += GetFrameTime();

                if (BotaoClicado(btnVoltar, posMouse)) {
                    if (temAudio) StopMusicStream(audioProva);
                    estadoAtual = STATE_C3_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnAudioPlay, posMouse)) {
                    audioTocando = !audioTocando;
                    if (temAudio) {
                        if (audioTocando) ResumeMusicStream(audioProva);
                        else PauseMusicStream(audioProva);
                    }
                }
                else if (BotaoClicado(btnAudioMudo, posMouse)) {
                    audioMudo = !audioMudo;
                    if (temAudio) SetMusicVolume(audioProva, audioMudo ? 0.0f : 1.0f);
                }
                else if (BotaoClicado(btnContinuarC3, posMouse) || IsKeyPressed(KEY_ENTER)) {
                    if (temAudio) StopMusicStream(audioProva);
                    estadoAtual = STATE_C3_PERGUNTA;
                }
                break;

            case STATE_C3_PERGUNTA:
                // p2.3: VOLTAR invisível (topo esquerdo) -> player de áudio
                if (BotaoClicado(btnVoltar, posMouse)) {
                    audioTempo = 0.0f;
                    audioTocando = true;
                    audioMudo = false;
                    if (temAudio) {
                        SetMusicVolume(audioProva, 1.0f);
                        PlayMusicStream(audioProva);
                    }
                    estadoAtual = STATE_C3_AUDIO;
                }
                // Verdadeira / Manipulada / "toque para continuar" invisível -> Cena 4
                else if (BotaoClicado(btnC3Verdadeira, posMouse) || BotaoClicado(btnC3Manipulada, posMouse) ||
                         BotaoClicado(btnContinuarC3, posMouse)) {
                    origemC4 = STATE_C3_PERGUNTA;
                    estadoAtual = STATE_C4_1;
                }
                break;


            // --- CENA 4 ---
            case STATE_C4_DESAFIO:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_EVIDENCIA;
                }
                else if (BotaoClicado(btnC4OpcaoB, posMouse)) {
                    // Única resposta correta -> tela "evidência desbloqueada" (c4.18)
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // evidência desbloqueada
                    estadoAtual = STATE_C4_DESBLOQUEADA;
                }
                else if (BotaoClicado(btnC4OpcaoA, posMouse) ||
                         BotaoClicado(btnC4OpcaoC, posMouse) ||
                         BotaoClicado(btnC4OpcaoD, posMouse)) {
                    // Qualquer erro -> evidência perdida (c4.17)
                    estadoAtual = STATE_C4_PERDIDA;
                }
                break;

            case STATE_C4_DESBLOQUEADA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_DESAFIO;
                }
                else if (BotaoClicado(btnAcessarEvidC4, posMouse)) {
                    estadoAtual = STATE_C4_ORIGEM;
                }
                break;

            case STATE_C4_PERDIDA:
                if (BotaoClicado(btnContinuarPerdidaC4, posMouse)) {
                    origemC5 = STATE_C4_PERDIDA;
                    estadoAtual = STATE_C5_1;
                }
                break;

            case STATE_C4_EVIDENCIA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_14;
                }
                else if (BotaoClicado(btnResolverC4, posMouse)) {
                    estadoAtual = STATE_C4_DESAFIO;
                }
                break;

            case STATE_C4_ORIGEM:
                if (AvancarDialogo(btnContinuarOrigemC4, posMouse)) {
                    estadoAtual = STATE_C4_PERGUNTA;
                }
                break;

            case STATE_C4_PERGUNTA:
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C4_ORIGEM;
                }
                else if (BotaoClicado(btnC4Verdadeira, posMouse) || BotaoClicado(btnC4Manipulada, posMouse)) {
                    origemC5 = STATE_C4_PERGUNTA;
                    estadoAtual = STATE_C5_1;
                }
                break;


            // --- CENA 5 ---
            case STATE_C5_EVIDENCIA:
                // c5_11 Nova evidência encontrada
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C5_10;
                }
                else if (BotaoClicado(btnResolverC5, posMouse)) {
                    estadoAtual = STATE_C5_DESAFIO;
                }
                break;

            case STATE_C5_DESAFIO: {
                // c5_12 Desafio de lógica: acertou -> P5_1 | errou (qualquer outra) -> c5_13
                int escolhida = -1;
                for (int k = 0; k < 4; k++) {
                    if (BotaoClicado(btnC5Opcao[k], posMouse)) { escolhida = k; break; }
                }
                if (escolhida == respostaCertaC5) {
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // evidência desbloqueada
                    estadoAtual = STATE_C5_DESBLOQUEADA;
                }
                else if (escolhida >= 0) {
                    estadoAtual = STATE_C5_PERDIDA;
                }
                break;
            }

            case STATE_C5_PERDIDA:
                // c5_13 Evidência perdida -> segue para a Cena 6
                if (BotaoClicado(btnContinuarPerdidaC5, posMouse)) {
                    origemC6 = STATE_C5_PERDIDA;
                    estadoAtual = STATE_C6_1;
                }
                break;

            case STATE_C5_DESBLOQUEADA:
                // P5_1 Evidência desbloqueada -> botão Acessar evidência -> P5_2
                if (BotaoClicado(btnAcessarEvidC5, posMouse)) {
                    estadoAtual = STATE_C5_ORIGEM;
                }
                break;

            case STATE_C5_ORIGEM:
                // P5_2 "CLIQUE PARA CONTINUAR" -> P5_3
                if (CliqueQualquerLugar()) {
                    estadoAtual = STATE_C5_PERGUNTA;
                }
                break;

            case STATE_C5_PERGUNTA:
                // P5_3 Verdadeira ou manipulada? -> Cena 6
                if (BotaoClicado(btnC5Verdadeira, posMouse) || BotaoClicado(btnC5Manipulada, posMouse)) {
                    origemC6 = STATE_C5_PERGUNTA;
                    estadoAtual = STATE_C6_1;
                }
                break;


            // --- CENA 6 ---
            case STATE_C6_EVIDENCIA:
                // c6_13 Nova evidência encontrada
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C6_12;
                }
                else if (BotaoClicado(btnResolverC6, posMouse)) {
                    estadoAtual = STATE_C6_DESAFIO;
                }
                break;

            case STATE_C6_DESAFIO: {
                // c6_14 Desafio de lógica: acertou -> evidência desbloqueada | errou -> c6_15
                int escolhida = -1;
                for (int k = 0; k < 4; k++) {
                    if (BotaoClicado(btnC6Opcao[k], posMouse)) { escolhida = k; break; }
                }
                if (escolhida == respostaCertaC6) {
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // evidência desbloqueada
                    estadoAtual = STATE_C6_DESBLOQUEADA;
                }
                else if (escolhida >= 0) {
                    estadoAtual = STATE_C6_PERDIDA;
                }
                break;
            }

            case STATE_C6_PERDIDA:
                // c6_15 Evidência perdida -> segue para a Cena 7
                if (BotaoClicado(btnContinuarPerdidaC6, posMouse)) {
                    origemC7 = STATE_C6_PERDIDA;
                    estadoAtual = STATE_C7_1;
                }
                break;

            case STATE_C6_DESBLOQUEADA:
                // Evidência desbloqueada -> botão Acessar evidência -> p6_1
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C6_DESAFIO;
                }
                else if (BotaoClicado(btnAcessarEvidC6, posMouse)) {
                    estadoAtual = STATE_C6_ORIGEM;
                }
                break;

            case STATE_C6_ORIGEM:
                // p6_1 "CLIQUE PARA CONTINUAR" -> p6_2
                if (CliqueQualquerLugar()) {
                    estadoAtual = STATE_C6_PERGUNTA;
                }
                break;

            case STATE_C6_PERGUNTA:
                // p6_2 Verdadeira ou manipulada? -> Cena 7
                if (BotaoClicado(btnC6Verdadeira, posMouse) || BotaoClicado(btnC6Manipulada, posMouse)) {
                    origemC7 = STATE_C6_PERGUNTA;
                    estadoAtual = STATE_C7_1;
                }
                break;


            // --- CENA 7 ---
            case STATE_C7_1:
                // c7_1 CHEFE: "A perícia conseguiu recuperar uma fotografia..."
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = origemC7;
                else if (AvancarDialogo(btnAvancarC7, posMouse)) estadoAtual = STATE_C7_2;
                break;

            case STATE_C7_2:
                // c7_2 VOCÊ: "Uma fotografia recebida por Gabriel pouco antes de desaparecer?"
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C7_1;
                else if (AvancarDialogo(btnAvancarC7, posMouse)) estadoAtual = STATE_C7_EVIDENCIA;
                break;

            case STATE_C7_EVIDENCIA:
                // c7_3 Nova evidência encontrada
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C7_2;
                }
                else if (BotaoClicado(btnResolverC7, posMouse)) {
                    estadoAtual = STATE_C7_DESAFIO;
                }
                break;

            case STATE_C7_DESAFIO: {
                // c7_4 Desafio de lógica: acertou -> P7_1 | errou -> c7_5
                int escolhida = -1;
                for (int k = 0; k < 4; k++) {
                    if (BotaoClicado(btnC7Opcao[k], posMouse)) { escolhida = k; break; }
                }
                if (escolhida == respostaCertaC7) {
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // evidência desbloqueada
                    estadoAtual = STATE_C7_DESBLOQUEADA;
                }
                else if (escolhida >= 0) {
                    estadoAtual = STATE_C7_PERDIDA;
                }
                break;
            }

            case STATE_C7_PERDIDA:
                // c7_5 Evidência perdida -> segue para a Cena 8
                if (BotaoClicado(btnContinuarPerdidaC7, posMouse)) {
                    origemC8 = STATE_C7_PERDIDA;
                    estadoAtual = STATE_C8_1;
                }
                break;

            case STATE_C7_DESBLOQUEADA:
                // P7_1 Evidência desbloqueada -> Acessar evidência -> p7_2
                if (BotaoClicado(btnAcessarEvidC7, posMouse)) {
                    estadoAtual = STATE_C7_ORIGEM;
                }
                break;

            case STATE_C7_ORIGEM:
                // p7_2 "CLIQUE PARA CONTINUAR" -> p7_3
                if (CliqueQualquerLugar()) {
                    estadoAtual = STATE_C7_PERGUNTA;
                }
                break;

            case STATE_C7_PERGUNTA:
                // p7_3 Verdadeira ou manipulada? -> Cena 8
                if (BotaoClicado(btnC7Verdadeira, posMouse) || BotaoClicado(btnC7Manipulada, posMouse)) {
                    origemC8 = STATE_C7_PERGUNTA;
                    estadoAtual = STATE_C8_1;
                }
                break;


            // --- CENA 8 (decisão final) ---
            case STATE_C8_1:
                // c8_1 A DECISÃO FINAL -> Tomar a decisão
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = origemC8;
                else if (BotaoClicado(btnTomarDecisao, posMouse)) estadoAtual = STATE_C8_2;
                break;

            case STATE_C8_2:
                // c8_2 DELEGADO: "Você chegou ao fim da investigação..."
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C8_1;
                else if (AvancarDialogo(btnAvancarC8, posMouse)) estadoAtual = STATE_C8_3;
                break;

            case STATE_C8_3:
                // c8_3 VOCÊ: "Entendido. Vou considerar tudo o que consegui descobrir..."
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_C8_2;
                else if (AvancarDialogo(btnAvancarC8, posMouse)) estadoAtual = STATE_C8_4;
                break;

            case STATE_C8_4:
                // c8_4 DELEGADO: "Então, investigadora, quem você acredita ser responsável...?"
                // (imagem um pouco mais alta: usa as áreas "Alto")
                if (BotaoClicado(btnVoltarAlto, posMouse)) estadoAtual = STATE_C8_3;
                else if (AvancarDialogo(btnAvancarAlto, posMouse)) estadoAtual = STATE_C8_5;
                break;

            case STATE_C8_5:
                // c8_5 Escolha o seu veredito: a) Caio -> FINAL 2 | b) Juliana -> FINAL 3 | c) Dossiê (a fazer)
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_C8_4;
                }
                else if (BotaoClicado(btnVeredCaio, posMouse)) {
                    estadoAtual = STATE_F2_1;
                }
                else if (BotaoClicado(btnVeredJuliana, posMouse)) {
                    estadoAtual = STATE_F3_1;
                }
                else if (BotaoClicado(btnVeredDossie, posMouse)) {
                    estadoAtual = STATE_DOSSIE_PROV;
                }
                break;


            // --- FINAL 2 (acusou o Caio) ---
            case STATE_F2_1:
                // F2_1 VOCÊ: "Meu veredito é que Caio Vasconcelos é o culpado."
                if (BotaoClicado(btnVoltarAlto, posMouse)) estadoAtual = STATE_C8_5;
                else if (AvancarDialogo(btnAvancarAlto, posMouse)) estadoAtual = STATE_F2_2;
                break;

            case STATE_F2_2:
                // F2_2 DELEGADO: "Você tem certeza dessa decisão?"  Sim -> F2_3 | Não -> volta à escolha do veredito
                if (BotaoClicado(btnVoltarAlto, posMouse)) {
                    estadoAtual = STATE_F2_1;
                }
                else if (BotaoClicado(btnF2Sim, posMouse)) {
                    estadoAtual = STATE_F2_3;
                }
                else if (BotaoClicado(btnF2Nao, posMouse)) {
                    estadoAtual = STATE_C8_5;
                }
                break;

            case STATE_F2_3:
                // F2_3 VEREDITO INCORRETO: clique para continuar
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_F2_2;
                else if (CliqueQualquerLugar()) estadoAtual = STATE_F2_4;
                break;

            case STATE_F2_4:
                // F2_4 Escaneie a explicação -> Jogar novamente (volta ao menu)
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_F2_3;
                }
                else if (BotaoClicado(btnJogarNovamente, posMouse)) {
                    jogador = (Jogador){ 0 };
                    estadoAtual = STATE_MENU;   // volta para o menu inicial
                }
                break;


            // --- FINAL 3 (acusou a Juliana - veredito correto) ---
            case STATE_F3_1:
                // F3_1 VOCÊ: "Meu veredito é que Juliana Costa é a culpada."
                if (BotaoClicado(btnVoltarAlto, posMouse)) estadoAtual = STATE_C8_5;
                else if (AvancarDialogo(btnAvancarAlto, posMouse)) estadoAtual = STATE_F3_2;
                break;

            case STATE_F3_2:
                // F3_2 DELEGADO: "Você tem certeza dessa decisão?"  Sim -> F3_3 | Não -> volta à escolha do veredito
                if (BotaoClicado(btnVoltarAlto, posMouse)) {
                    estadoAtual = STATE_F3_1;
                }
                else if (BotaoClicado(btnF3Sim, posMouse)) {
                    if (temSfxBrilho) PlaySound(sfxBrilho);   // veredito correto
                    estadoAtual = STATE_F3_3;
                }
                else if (BotaoClicado(btnF3Nao, posMouse)) {
                    estadoAtual = STATE_C8_5;
                }
                break;

            case STATE_F3_3:
                // F3_3 VEREDITO CORRETO: botão "CLIQUE PARA CONTINUAR" -> F3_4
                if (BotaoClicado(btnVoltar, posMouse)) estadoAtual = STATE_F3_2;
                else if (BotaoClicado(btnContinuarF3, posMouse)) estadoAtual = STATE_F3_4;
                break;

            case STATE_F3_4:
                // F3_4 Escaneie a explicação -> Jogar novamente (volta ao menu)
                if (BotaoClicado(btnVoltar, posMouse)) {
                    estadoAtual = STATE_F3_3;
                }
                else if (BotaoClicado(btnJogarNovamente, posMouse)) {
                    jogador = (Jogador){ 0 };
                    estadoAtual = STATE_MENU;   // volta para o menu inicial
                }
                break;


            // --- Tela provisória (ainda sem imagem) ---
            case STATE_DOSSIE_PROV:
                if (IsKeyPressed(KEY_ESCAPE) || CliqueQualquerLugar()) {
                    estadoAtual = STATE_C8_5;
                }
                break;


            default:
                if (estadoAtual >= STATE_C2_0 && estadoAtual <= STATE_C2_7) {
                    if (BotaoClicado(btnVoltar, posMouse)) {
                        if (estadoAtual == STATE_C2_0) estadoAtual = STATE_C1_6;
                        else estadoAtual = (GameState)(estadoAtual - 1);
                    }
                    else if (AvancarDialogo(btnAvancarC2, posMouse)) {
                        estadoAtual = (GameState)(estadoAtual + 1);
                    }
                }
                else if (estadoAtual >= STATE_C4_1 && estadoAtual < (GameState)(STATE_C4_1 + numDialogosC4)) {
                    if (BotaoClicado(btnVoltar, posMouse)) {
                        if (estadoAtual == STATE_C4_1) estadoAtual = origemC4;
                        else estadoAtual = (GameState)(estadoAtual - 1);
                    }
                    else if (AvancarDialogo(btnAvancarC4, posMouse)) {
                        if (estadoAtual == (GameState)(STATE_C4_1 + numDialogosC4 - 1)) estadoAtual = STATE_C4_EVIDENCIA;
                        else estadoAtual = (GameState)(estadoAtual + 1);
                    }
                }
                else if (estadoAtual >= STATE_C5_1 && estadoAtual <= STATE_C5_10) {
                    if (BotaoClicado(btnVoltar, posMouse)) {
                        if (estadoAtual == STATE_C5_1) estadoAtual = origemC5;
                        else estadoAtual = (GameState)(estadoAtual - 1);
                    }
                    else if (AvancarDialogo(btnAvancarC5, posMouse)) {
                        if (estadoAtual == STATE_C5_10) estadoAtual = STATE_C5_EVIDENCIA;
                        else estadoAtual = (GameState)(estadoAtual + 1);
                    }
                }
                else if (estadoAtual >= STATE_C6_1 && estadoAtual <= STATE_C6_12) {
                    if (BotaoClicado(btnVoltar, posMouse)) {
                        if (estadoAtual == STATE_C6_1) estadoAtual = origemC6;
                        else estadoAtual = (GameState)(estadoAtual - 1);
                    }
                    else if (estadoAtual == STATE_C6_8) {
                        // Tela "Algumas horas depois": botão Continuar no centro
                        if (BotaoClicado(btnContinuarHorasC6, posMouse)) estadoAtual = STATE_C6_9;
                    }
                    else if (AvancarDialogo(btnAvancarC6, posMouse)) {
                        if (estadoAtual == STATE_C6_12) estadoAtual = STATE_C6_EVIDENCIA;
                        else estadoAtual = (GameState)(estadoAtual + 1);
                    }
                }
                break;
        }

        // ======================= RENDERIZAÇÃO =======================
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        switch (estadoAtual) {
            case STATE_MENU:
                if (imgMenu.id != 0) {
                    DesenharTela(imgMenu, dest);
                } else {
                    DrawText("AI.LIBE - MENU", 310, 150, 24, RAYWHITE);
                    DrawText("JOGAR", (int)btnJogar.x, (int)btnJogar.y, 22, RAYWHITE);
                    DrawText("REGRAS", (int)btnRegras.x, (int)btnRegras.y, 22, RAYWHITE);
                    DrawText("CONFIGURACOES", (int)btnConfig.x, (int)btnConfig.y, 22, RAYWHITE);
                    DrawText("CREDITOS", (int)btnCreditos.x, (int)btnCreditos.y, 22, RAYWHITE);
                }
                break;


            case STATE_CONFIG: {
                bool volOn = volumeAtivo;
                bool telaOn = telaCheia;

                if (!volOn && !telaOn) {
                    DesenharTela(texConfigTudoOff, dest);            // configuracoes3
                } else if (volOn && !telaOn) {
                    DesenharTela(texConfigVolOn, dest);              // configuracoes2
                } else if (!volOn && telaOn) {
                    DesenharTela(texConfigTelaOn, dest);             // configuracoes1
                } else {
                    // Os dois ATIVADOS: base com volume ativado + recorte da linha "tela cheia" ativada
                    DesenharTela(texConfigVolOn, dest);
                    DesenharRecorte(texConfigTelaOn, 0.715f, 0.545f, 0.14f, 0.055f,
                                    (float)larguraTela, (float)alturaTela);
                }

                if (texConfigTudoOff.id == 0 && texConfigVolOn.id == 0 && texConfigTelaOn.id == 0) {
                    DrawText("CONFIGURACOES", 300, 200, 24, RAYWHITE);
                    DrawText(volOn ? "VOLUME GERAL: ATIVADO" : "VOLUME GERAL: DESATIVADO", 250, 245, 18, RAYWHITE);
                    DrawText(telaOn ? "TELA CHEIA: ATIVADO" : "TELA CHEIA: DESATIVADO", 250, 335, 18, RAYWHITE);
                    DrawText("VOLTAR", (int)btnVoltarMenu.x, (int)btnVoltarMenu.y, 20, RAYWHITE);
                }
                break;
            }


            case STATE_CREDITOS:
                if (texCreditos.id != 0) {
                    DesenharTela(texCreditos, dest);
                } else {
                    DrawText("CREDITOS", 340, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", (int)btnVoltarCreditos.x, (int)btnVoltarCreditos.y, 20, RAYWHITE);
                }
                break;


            case STATE_REGRAS:
                if (texRegras[paginaRegras].id != 0) {
                    DesenharTela(texRegras[paginaRegras], dest);
                } else {
                    DrawText("REGRAS DO JOGO", 300, 200, 24, RAYWHITE);
                    DrawText("VOLTAR", (int)btnVoltarRegras.x, (int)btnVoltarRegras.y, 20, RAYWHITE);
                    DrawText(paginaRegras == 4 ? "JOGAR" : "CONTINUAR",
                             (int)btnContinuarRegras.x, (int)btnContinuarRegras.y, 20, RAYWHITE);
                }
                break;


            // --- CENA 1 ---
            case STATE_C1_1: DesenharTela(imgC1[0], dest); break;
            case STATE_C1_2: DesenharTela(imgC1[1], dest); break;
            case STATE_C1_3: DesenharTela(imgC1[2], dest); break;
            case STATE_C1_4: DesenharTela(imgC1[3], dest); break;
            case STATE_C1_5: DesenharTela(imgC1[4], dest); break;
            case STATE_C1_6: DesenharTela(imgC1[5], dest); break;


            // --- CENA 2 ---
            case STATE_C2_EVIDENCIA:  DesenharTela(imgC2[8],  dest); break;
            case STATE_C2_DESAFIO:    DesenharTela(imgC2[9],  dest); break;
            case STATE_CONFIRMA_CAIO: DesenharTela(imgC2[10], dest); break;
            case STATE_C2_PERDIDA:    DesenharTela(imgC2[11], dest); break;

            case STATE_ACUSA_CAIO_1: DesenharTela(imgF1[0], dest); break;
            case STATE_ACUSA_CAIO_2: DesenharTela(imgF1[1], dest); break;
            case STATE_ACUSA_CAIO_3: DesenharTela(imgF1[2], dest); break;
            case STATE_ACUSA_CAIO_4: DesenharTela(imgF1[3], dest); break;

            case STATE_VEREDITO_INCORRETO:     DesenharTela(imgP1,   dest); break;
            case STATE_EVIDENCIA_DESBLOQUEADA: DesenharTela(imgP1_1, dest); break;
            case STATE_DESAFIO_IA:             DesenharTela(imgP1_2, dest); break;


            // --- CENA 3 ---
            case STATE_C3_1: DesenharTela(imgC3_1, dest); break;
            case STATE_C3_2: DesenharTela(imgC3_2, dest); break;
            case STATE_C3_3: DesenharTela(imgC3_3, dest); break;
            case STATE_C3_4: DesenharTela(imgC3_4, dest); break;
            case STATE_C3_PERDIDA: DesenharTela(imgC3_5, dest); break;
            case STATE_C3_DESBLOQUEADA: DesenharTela(imgP2, dest); break;

            case STATE_C3_AUDIO: {
                Texture2D texAudio;
                if (audioTocando) texAudio = audioMudo ? imgAudioPlayMudo  : imgAudioPlaySom;
                else              texAudio = audioMudo ? imgAudioPausaMudo : imgAudioPausaSom;
                DesenharTela(texAudio, dest);
                break;
            }

            case STATE_C3_PERGUNTA: DesenharTela(imgP2Pergunta, dest); break;


            // --- CENA 4 ---
            case STATE_C4_EVIDENCIA:    DesenharTela(imgC4Evidencia, dest); break;
            case STATE_C4_DESAFIO:      DesenharTela(imgC4Desafio, dest); break;
            case STATE_C4_PERDIDA:      DesenharTela(imgC4Perdida, dest); break;
            case STATE_C4_DESBLOQUEADA: DesenharTela(imgC4Desbloqueada, dest); break;
            case STATE_C4_ORIGEM:       DesenharTela(imgP4_1, dest); break;
            case STATE_C4_PERGUNTA:     DesenharTela(imgP4_2, dest); break;


            // --- CENA 5 ---
            case STATE_C5_EVIDENCIA:    DesenharTela(imgC5Evidencia,    dest); break;   // c5_11
            case STATE_C5_DESAFIO:      DesenharTela(imgC5Desafio,      dest); break;   // c5_12
            case STATE_C5_PERDIDA:      DesenharTela(imgC5Perdida,      dest); break;   // c5_13
            case STATE_C5_DESBLOQUEADA: DesenharTela(imgC5Desbloqueada, dest); break;   // P5_1
            case STATE_C5_ORIGEM:       DesenharTela(imgC5Origem,       dest); break;   // P5_2
            case STATE_C5_PERGUNTA:     DesenharTela(imgC5Pergunta,     dest); break;   // P5_3


            // --- CENA 6 ---
            case STATE_C6_EVIDENCIA: DesenharTela(imgC6Evidencia, dest); break;   // c6_13
            case STATE_C6_DESAFIO:   DesenharTela(imgC6Desafio,   dest); break;   // c6_14
            case STATE_C6_PERDIDA:   DesenharTela(imgC6Perdida,   dest); break;   // c6_15
            case STATE_C6_DESBLOQUEADA:
                // Usa a tela própria da Cena 6 (P6.png) se existir; senão reaproveita a da Cena 5 (P5_1)
                DesenharTela(imgC6Desbloqueada.id != 0 ? imgC6Desbloqueada : imgC5Desbloqueada, dest);
                break;
            case STATE_C6_ORIGEM:    DesenharTela(imgC6Origem,    dest); break;   // p6_1
            case STATE_C6_PERGUNTA:  DesenharTela(imgC6Pergunta,  dest); break;   // p6_2


            // --- CENA 7 ---
            case STATE_C7_1:            DesenharTela(imgC7_1,           dest); break;   // c7_1
            case STATE_C7_2:            DesenharTela(imgC7_2,           dest); break;   // c7_2
            case STATE_C7_EVIDENCIA:    DesenharTela(imgC7Evidencia,    dest); break;   // c7_3
            case STATE_C7_DESAFIO:      DesenharTela(imgC7Desafio,      dest); break;   // c7_4
            case STATE_C7_PERDIDA:      DesenharTela(imgC7Perdida,      dest); break;   // c7_5
            case STATE_C7_DESBLOQUEADA: DesenharTela(imgC7Desbloqueada, dest); break;   // P7_1
            case STATE_C7_ORIGEM:       DesenharTela(imgC7Origem,       dest); break;   // p7_2
            case STATE_C7_PERGUNTA:     DesenharTela(imgC7Pergunta,     dest); break;   // p7_3


            // --- CENA 8 ---
            case STATE_C8_1: DesenharTela(imgC8[0], dest); break;
            case STATE_C8_2: DesenharTela(imgC8[1], dest); break;
            case STATE_C8_3: DesenharTela(imgC8[2], dest); break;
            case STATE_C8_4: DesenharTela(imgC8[3], dest); break;
            case STATE_C8_5: DesenharTela(imgC8[4], dest); break;


            // --- FINAL 2 ---
            case STATE_F2_1: DesenharTela(imgF2[0], dest); break;
            case STATE_F2_2: DesenharTela(imgF2[1], dest); break;
            case STATE_F2_3: DesenharTela(imgF2[2], dest); break;
            case STATE_F2_4: DesenharTela(imgF2[3], dest); break;


            // --- FINAL 3 ---
            case STATE_F3_1: DesenharTela(imgF3[0], dest); break;
            case STATE_F3_2: DesenharTela(imgF3[1], dest); break;
            case STATE_F3_3: DesenharTela(imgF3[2], dest); break;
            case STATE_F3_4: DesenharTela(imgF3[3], dest); break;


            // --- Tela provisória ---
            case STATE_DOSSIE_PROV:
                DrawRectangleRec(dest, (Color){ 20, 10, 40, 255 });
                DrawText("DOSSIE DE EVIDENCIAS - EM CONSTRUCAO", 120, 280, 28, RAYWHITE);
                DrawText("Clique ou ESC para voltar ao veredito", 220, 330, 20, GRAY);
                break;


            default:
                if (estadoAtual >= STATE_C2_0 && estadoAtual <= STATE_C2_7) {
                    DesenharTela(imgC2[estadoAtual - STATE_C2_0], dest);
                }
                else if (estadoAtual >= STATE_C4_1 && estadoAtual < (GameState)(STATE_C4_1 + numDialogosC4)) {
                    DesenharTela(imgC4[estadoAtual - STATE_C4_1], dest);
                }
                else if (estadoAtual >= STATE_C5_1 && estadoAtual <= STATE_C5_10) {
                    DesenharTela(imgC5[estadoAtual - STATE_C5_1], dest);
                }
                else if (estadoAtual >= STATE_C6_1 && estadoAtual <= STATE_C6_12) {
                    DesenharTela(imgC6[estadoAtual - STATE_C6_1], dest);
                }
                break;
        }

        if (debugBotoes) {
            for (int k = 0; k < g_numDebugRects; k++) {
                DrawRectangleRec(g_debugRects[k], (Color){ 0, 255, 0, 50 });
                DrawRectangleLinesEx(g_debugRects[k], 2.0f, LIME);
            }
            DrawText("DEBUG (F1)", 10, 580, 16, LIME);
            DrawText(TextFormat("estado=%d  (C4_1=%d  C5_1=%d  C6_1=%d  C7_1=%d  C8_1=%d  F2_1=%d  F3_1=%d)  mouse=%d,%d",
                     (int)estadoAtual, (int)STATE_C4_1, (int)STATE_C5_1, (int)STATE_C6_1,
                     (int)STATE_C7_1, (int)STATE_C8_1, (int)STATE_F2_1, (int)STATE_F3_1,
                     (int)posMouse.x, (int)posMouse.y), 10, 560, 14, LIME);
        }

        EndMode2D();
        EndDrawing();
    }


    // ======= UNLOAD =======
    for (int k = 0; k < 5; k++) UnloadTexture(texRegras[k]);
    UnloadTexture(texConfigTudoOff);
    UnloadTexture(texConfigVolOn);
    UnloadTexture(texConfigTelaOn);
    UnloadTexture(texCreditos);
    UnloadTexture(imgMenu);

    for (int k = 0; k < 6; k++) UnloadTexture(imgC1[k]);

    for (int k = 0; k < 12; k++) UnloadTexture(imgC2[k]);
    for (int k = 0; k < 4;  k++) UnloadTexture(imgF1[k]);
    UnloadTexture(imgP1);
    UnloadTexture(imgP1_1);
    UnloadTexture(imgP1_2);

    UnloadTexture(imgC3_1);
    UnloadTexture(imgC3_2);
    UnloadTexture(imgC3_3);
    UnloadTexture(imgC3_4);
    UnloadTexture(imgC3_5);
    UnloadTexture(imgP2);
    UnloadTexture(imgAudioPlayMudo);
    UnloadTexture(imgAudioPlaySom);
    UnloadTexture(imgAudioPausaMudo);
    UnloadTexture(imgAudioPausaSom);
    UnloadTexture(imgP2Pergunta);

    for (int k = 0; k < 15; k++) UnloadTexture(imgC4[k]);
    UnloadTexture(imgC4Evidencia);
    UnloadTexture(imgC4Desafio);
    UnloadTexture(imgC4Perdida);
    UnloadTexture(imgC4Desbloqueada);
    UnloadTexture(imgP4_1);
    UnloadTexture(imgP4_2);

    for (int k = 0; k < 10; k++) UnloadTexture(imgC5[k]);
    UnloadTexture(imgC5Evidencia);
    UnloadTexture(imgC5Desafio);
    UnloadTexture(imgC5Perdida);
    UnloadTexture(imgC5Desbloqueada);
    UnloadTexture(imgC5Origem);
    UnloadTexture(imgC5Pergunta);

    for (int k = 0; k < 12; k++) UnloadTexture(imgC6[k]);
    UnloadTexture(imgC6Evidencia);
    UnloadTexture(imgC6Desafio);
    UnloadTexture(imgC6Perdida);
    UnloadTexture(imgC6Origem);
    UnloadTexture(imgC6Pergunta);
    UnloadTexture(imgC6Desbloqueada);

    UnloadTexture(imgC7_1);
    UnloadTexture(imgC7_2);
    UnloadTexture(imgC7Evidencia);
    UnloadTexture(imgC7Desafio);
    UnloadTexture(imgC7Perdida);
    UnloadTexture(imgC7Desbloqueada);
    UnloadTexture(imgC7Origem);
    UnloadTexture(imgC7Pergunta);

    for (int k = 0; k < 5; k++) UnloadTexture(imgC8[k]);
    for (int k = 0; k < 4; k++) UnloadTexture(imgF2[k]);
    for (int k = 0; k < 4; k++) UnloadTexture(imgF3[k]);

    if (temAudio) UnloadMusicStream(audioProva);
    if (temMusicaMenu) UnloadMusicStream(musicaMenu);
    if (temMusicaJogo) UnloadMusicStream(musicaJogo);
    if (temSfxClick) UnloadSound(sfxClick);
    if (temSfxBrilho) UnloadSound(sfxBrilho);
    CloseAudioDevice();

    CloseWindow();
    return 0;
}