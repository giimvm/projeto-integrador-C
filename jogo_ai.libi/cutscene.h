#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "raylib.h"
#include "rlgl.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUM_PAGINAS_HQ 5
#define MAX_QUADROS_PAG 5

typedef struct {
    Vector2 v[4];         // 4 vertices em sentido horario: 0=TL, 1=TR, 2=BR, 3=BL
    const char *tag;      // Etiqueta curta do quadro
    char texto[320];      // Legenda narrativa exibida ao revelar
    Color corBorda;       // Cor neon da moldura (rosa, roxo, ciano, etc.)
    char imgPath[128];    // Caminho relativo da arte do quadro
    Texture2D tex;        // Textura carregada para este quadro
    bool temTex;          // True se a textura foi carregada com sucesso
} QuadroHQ;

typedef struct {
    const char *tituloPagina;
    int numQuadros;
    QuadroHQ quadros[MAX_QUADROS_PAG];
} PaginaHQ;

typedef struct {
    int paginaIdx;          // 0..4 (5 paginas)
    int quadrosVisiveis;    // 1..numQuadros (aparecem um por vez com fade-in)
    float fadeQuadro;       // 0.0 -> 1.0
    float fadePagina;       // 1.0 -> 0.0
    int estadoTransicao;    // 0=revelando quadro, 1=pagina parada, 2=fade out prox pagina, 3=fade out fim
    float hoverPular;
    PaginaHQ paginas[NUM_PAGINAS_HQ];
} EstadoCutscene;

static inline int _RespCorretaHQ(int cena, int culpado) {
    bool ehJu = (culpado == 0);
    if (cena == 3 || cena == 4) return ehJu ? 2 : 1;
    if (cena == 5 || cena == 6) return 1;
    if (cena == 7) return ehJu ? 1 : 2;
    return 0;
}

static inline bool _ProvaValidadaHQ(int cena, int culpado, const int statusProva[8], const int respostaProva[8]) {
    return (statusProva[cena] == 1 && respostaProva[cena] == _RespCorretaHQ(cena, culpado));
}

static inline void DescarregarCutsceneCaches(EstadoCutscene *cs) {
    if (!cs) return;
    for (int p = 0; p < NUM_PAGINAS_HQ; p++) {
        for (int q = 0; q < MAX_QUADROS_PAG; q++) {
            if (cs->paginas[p].quadros[q].temTex && cs->paginas[p].quadros[q].tex.id > 0) {
                UnloadTexture(cs->paginas[p].quadros[q].tex);
            }
            cs->paginas[p].quadros[q].tex = (Texture2D){0};
            cs->paginas[p].quadros[q].temTex = false;
        }
    }
}

static inline void _SetQuadroImg(QuadroHQ *q, const char *path) {
    snprintf(q->imgPath, sizeof(q->imgPath), "%s", path ? path : "");
}

static inline void _CarregarTexturasCutscene(EstadoCutscene *cs) {
    char buf[256];
    for (int p = 0; p < NUM_PAGINAS_HQ; p++) {
        for (int i = 0; i < cs->paginas[p].numQuadros && i < MAX_QUADROS_PAG; i++) {
            QuadroHQ *q = &cs->paginas[p].quadros[i];
            q->temTex = false;
            q->tex = (Texture2D){0};
            if (q->imgPath[0] == '\0') continue;
            const char *resolvido = ResolverCaminho(q->imgPath, buf, sizeof(buf));
            if (FileExists(resolvido)) {
                q->tex = LoadTexture(resolvido);
                if (q->tex.id > 0) {
                    SetTextureFilter(q->tex, TEXTURE_FILTER_BILINEAR);
                    q->temTex = true;
                }
            }
        }
    }
}

static inline void MontarCutsceneFinal(EstadoCutscene *cs, int tipoFinal, int culpado, const int statusProva[8], const int respostaProva[8]) {
    DescarregarCutsceneCaches(cs);
    memset(cs, 0, sizeof(*cs));
    cs->quadrosVisiveis = 1;
    cs->fadeQuadro = 0.0f;
    cs->fadePagina = 1.0f;
    cs->estadoTransicao = 0;

    bool ehJu = (culpado == 0);
    bool p1 = _ProvaValidadaHQ(3, culpado, statusProva, respostaProva);
    bool p2 = _ProvaValidadaHQ(4, culpado, statusProva, respostaProva);
    bool p3 = _ProvaValidadaHQ(5, culpado, statusProva, respostaProva);
    bool p4 = _ProvaValidadaHQ(6, culpado, statusProva, respostaProva);
    bool p5 = _ProvaValidadaHQ(7, culpado, statusProva, respostaProva);
    int totalVal = (p1?1:0) + (p2?1:0) + (p3?1:0) + (p4?1:0) + (p5?1:0);

    // =========================================================================
    // PAGINA 1/5: BACKSTORY (Layout fiel ao TemplateGibi.png: 2 Esq + 3 Dir)
    // Area util: x=95..1185, y=66..546, divisoria central inclinada x=665->545
    // =========================================================================
    cs->paginas[0].tituloPagina = "PÁGINA 1/5 // BACKSTORY: OS DIAS DE FACULDADE";
    cs->paginas[0].numQuadros = 5;

    // Q1: Topo Esquerda
    cs->paginas[0].quadros[0] = (QuadroHQ){
        { {95, 66}, {658, 66}, {602, 294}, {95, 326} },
        "01 // GABRIEL", "", COR_CIANO_NEON
    };
    snprintf(cs->paginas[0].quadros[0].texto, 320, "Gabriel tinha um caso com o Prof. João Paulo e usava a relação para pedir favores na faculdade.");
    _SetQuadroImg(&cs->paginas[0].quadros[0], "AssetsRay/CutsceneRay/hq/p1_q1_gabriel.png");

    // Q2: Topo Direita
    cs->paginas[0].quadros[1] = (QuadroHQ){
        { {672, 66}, {1185, 66}, {1185, 198}, {622, 250} },
        "02 // JULIANA", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[0].quadros[1].texto, 320, "Juliana passou três anos estudando e programando o próprio projeto de IA pela vaga na NEXARA.");
    _SetQuadroImg(&cs->paginas[0].quadros[1], "AssetsRay/CutsceneRay/hq/p1_q2_juliana.png");

    // Q3: Meio Direita
    cs->paginas[0].quadros[2] = (QuadroHQ){
        { {618, 264}, {1185, 212}, {1185, 404}, {580, 366} },
        "03 // CAIO", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[0].quadros[2].texto, 320, "Caio não fazia nada nos estudos e garantiu a vaga na NEXARA chantageando Gabriel.");
    _SetQuadroImg(&cs->paginas[0].quadros[2], "AssetsRay/CutsceneRay/hq/p1_q3_caio.png");

    // Q4: Baixo Esquerda
    cs->paginas[0].quadros[3] = (QuadroHQ){
        { {95, 340}, {598, 308}, {540, 546}, {95, 546} },
        "04 // A AMIZADE", "", COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[0].quadros[3].texto, 320, "Antes de tudo ruir, os três eram colegas inseparáveis no campus de Recife em 2045.");
    _SetQuadroImg(&cs->paginas[0].quadros[3], "AssetsRay/CutsceneRay/hq/p1_q4_amizade.png");

    // Q5: Baixo Direita
    cs->paginas[0].quadros[4] = (QuadroHQ){
        { {576, 380}, {1185, 418}, {1185, 546}, {554, 546} },
        "05 // A RUPTURA SILENCIOSA", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[0].quadros[4].texto, 320, "Juliana perdeu a vaga decepcionada; entre favores, culpa e chantagem, a amizade rachou antes da festa.");
    _SetQuadroImg(&cs->paginas[0].quadros[4], "AssetsRay/CutsceneRay/hq/p1_q5_ruptura.png");

    // =========================================================================
    // PAGINA 2/5: A FESTA E AS PROVAS DIGITAIS (4 quadros inclinados 2x2)
    // =========================================================================
    cs->paginas[1].tituloPagina = "PÁGINA 2/5 // A FESTA E AS PISTAS";
    cs->paginas[1].numQuadros = 4;

    cs->paginas[1].quadros[0] = (QuadroHQ){
        { {95, 66}, {655, 66}, {605, 308}, {95, 282} },
        "01 // BRIGA NA SAÍDA", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[1].quadros[0].texto, 320, "Do lado de fora da festa, Caio e Gabriel discutiram diante de testemunhas.");
    _SetQuadroImg(&cs->paginas[1].quadros[0], "AssetsRay/CutsceneRay/hq/p2_q1_briga.png");

    cs->paginas[1].quadros[1] = (QuadroHQ){
        { {669, 66}, {1185, 66}, {1185, 336}, {619, 308} },
        p4 ? "02 // NUVEM VALIDADA" : "02 // NUVEM PERDIDA", "", p4 ? COR_VERDE_OK : COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[1].quadros[1].texto, 320, ehJu
        ? (p4 ? "Nuvem validada: Gabriel entregou o código de Juliana ao Prof. João Paulo." : "Nuvem não validada: o roubo do projeto ficou sem prova documental.")
        : (p4 ? "Nuvem validada: esquema de laranja e lavagem de dinheiro do pai de Caio." : "Nuvem não validada: o esquema financeiro ficou sem lastro técnico."));
    _SetQuadroImg(&cs->paginas[1].quadros[1], !p4 ? "AssetsRay/CutsceneRay/hq/p2_q2_nuvem_perdida.png" : (ehJu ? "AssetsRay/CutsceneRay/hq/p2_q2_nuvem_ju.png" : "AssetsRay/CutsceneRay/hq/p2_q2_nuvem_caio.png"));

    cs->paginas[1].quadros[2] = (QuadroHQ){
        { {95, 296}, {595, 322}, {655, 546}, {95, 546} },
        "03 // PROVA 1 (ÁUDIO)", "", p1 ? COR_CIANO_NEON : COR_VERMELHO_ERR
    };
    snprintf(cs->paginas[1].quadros[2].texto, 320, ehJu
        ? (p1 ? "Áudio anônimo validado: voz do Caio foi sintetizada por IA." : "Áudio anônimo não validado no mural.")
        : (p1 ? "Áudio anônimo validado: Caio realmente ameaçou Gabriel." : "Áudio anônimo não validado no mural."));
    _SetQuadroImg(&cs->paginas[1].quadros[2], !p1 ? "AssetsRay/CutsceneRay/hq/p2_q3_audio_perdido.png" : (ehJu ? "AssetsRay/CutsceneRay/hq/p2_q3_audio_ju.png" : "AssetsRay/CutsceneRay/hq/p2_q3_audio_caio.png"));

    cs->paginas[1].quadros[3] = (QuadroHQ){
        { {609, 322}, {1185, 350}, {1185, 546}, {669, 546} },
        "04 // PROVA 2 (FOTO NO CARRO)", "", p2 ? COR_CIANO_NEON : COR_VERMELHO_ERR
    };
    snprintf(cs->paginas[1].quadros[3].texto, 320, ehJu
        ? (p2 ? "Foto no carro validada como IA: volante inexistente e horário editado." : "Foto no carro não validada no mural.")
        : (p2 ? "Foto no carro validada como real: Juliana saiu às 03:05." : "Foto no carro não validada no mural."));
    _SetQuadroImg(&cs->paginas[1].quadros[3], !p2 ? "AssetsRay/CutsceneRay/hq/p2_q4_foto_perdida.png" : (ehJu ? "AssetsRay/CutsceneRay/hq/p2_q4_foto_ju.png" : "AssetsRay/CutsceneRay/hq/p2_q4_foto_caio.png"));

    // =========================================================================
    // PAGINA 3/5: O BARRANCO (3 quadros inclinados em cima + faixa panoramica)
    // =========================================================================
    cs->paginas[2].tituloPagina = "PÁGINA 3/5 // O BARRANCO (02:50 - 03:41)";
    cs->paginas[2].numQuadros = 4;

    cs->paginas[2].quadros[0] = (QuadroHQ){
        { {95, 66}, {465, 66}, {415, 382}, {95, 382} },
        "01 // ATRÁS DO GALPÃO", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[2].quadros[0].texto, 320, ehJu
        ? "Nos fundos do galpão, Juliana golpeou Gabriel após a confissão."
        : "Na estrada atrás do galpão, Caio empurrou Gabriel para calar a denúncia.");
    _SetQuadroImg(&cs->paginas[2].quadros[0], ehJu ? "AssetsRay/CutsceneRay/hq/p3_q1_galpao_ju.png" : "AssetsRay/CutsceneRay/hq/p3_q1_galpao_caio.png");

    cs->paginas[2].quadros[1] = (QuadroHQ){
        { {479, 66}, {845, 66}, {795, 382}, {429, 382} },
        p3 ? "02 // GPS 03:20 OK" : "02 // GPS PERDIDO", "", p3 ? COR_VERDE_OK : COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[2].quadros[1].texto, 320, p3
        ? "GPS validado: celular de Gabriel parou no fundo do barranco às 03:20."
        : "GPS não validado: faltou confirmar tecnicamente o último sinal.");
    _SetQuadroImg(&cs->paginas[2].quadros[1], p3 ? "AssetsRay/CutsceneRay/hq/p3_q2_gps_ok.png" : "AssetsRay/CutsceneRay/hq/p3_q2_gps_perdido.png");

    cs->paginas[2].quadros[2] = (QuadroHQ){
        { {859, 66}, {1185, 66}, {1185, 382}, {809, 382} },
        p5 ? "03 // SAÍDA VALIDADA" : "03 // SAÍDA PERDIDA", "", p5 ? COR_CIANO_NEON : COR_VERMELHO_ERR
    };
    snprintf(cs->paginas[2].quadros[2].texto, 320, ehJu
        ? (p5 ? "Mobi 03:41 validada: derrubou o álibi de Juliana das 03:00." : "Corrida da Mobi não validada no mural.")
        : (p5 ? "Câmera 02:42 desmascarada: vídeo forjado por IA." : "Fraude na câmera 02:42 não validada no mural."));
    _SetQuadroImg(&cs->paginas[2].quadros[2], !p5 ? "AssetsRay/CutsceneRay/hq/p3_q3_saida_perdida.png" : (ehJu ? "AssetsRay/CutsceneRay/hq/p3_q3_saida_ju.png" : "AssetsRay/CutsceneRay/hq/p3_q3_saida_caio.png"));

    cs->paginas[2].quadros[3] = (QuadroHQ){
        { {95, 396}, {1185, 396}, {1185, 546}, {95, 546} },
        "04 // LINHA DO TEMPO", "", COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[2].quadros[3].texto, 320, "Entre 02h50 e 03h41, os rastros digitais cruzaram a verdade e a tentativa de encobrimento.");
    _SetQuadroImg(&cs->paginas[2].quadros[3], "AssetsRay/CutsceneRay/hq/p3_q4_linha_tempo.png");

    // =========================================================================
    // PAGINA 4/5: TRIBUNAL E SENTENCA (1 vertical esq + 2 empilhados dir)
    // =========================================================================
    cs->paginas[3].tituloPagina = "PÁGINA 4/5 // TRIBUNAL E O PESO DO MURAL";
    cs->paginas[3].numQuadros = 3;

    cs->paginas[3].quadros[0] = (QuadroHQ){
        { {95, 66}, {615, 66}, {555, 546}, {95, 546} },
        "01 // DOSSIÊ FINAL", "", COR_CIANO_NEON
    };
    snprintf(cs->paginas[3].quadros[0].texto, 320, "Provas validadas e ligadas no Mural: %d/5.\nToda a acusação se apoiou nas conexões feitas por você.", totalVal);
    _SetQuadroImg(&cs->paginas[3].quadros[0], "AssetsRay/CutsceneRay/hq/p4_q1_dossie.png");

    cs->paginas[3].quadros[1] = (QuadroHQ){
        { {629, 66}, {1185, 66}, {1185, 290}, {598, 315} },
        "02 // AUDIÊNCIA NO TRIBUNAL", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[3].quadros[1].texto, 320, (tipoFinal <= 2)
        ? "A defesa invocou a Lei da Dúvida Digital, mas suas provas sustentaram a acusação."
        : "No tribunal, a falta de cadeia de custódia completa pesou contra a acusação.");
    _SetQuadroImg(&cs->paginas[3].quadros[1], (tipoFinal <= 2) ? "AssetsRay/CutsceneRay/hq/p4_q2_audiencia_vitoria.png" : "AssetsRay/CutsceneRay/hq/p4_q2_audiencia_derrota.png");

    cs->paginas[3].quadros[2] = (QuadroHQ){
        { {596, 329}, {1185, 304}, {1185, 546}, {569, 546} },
        "03 // SENTENÇA", "", (tipoFinal <= 2) ? COR_VERDE_OK : COR_VERMELHO_ERR
    };
    snprintf(cs->paginas[3].quadros[2].texto, 320,
        (tipoFinal == 1) ? "FINAL 1 - PERFEITO: Condenação total e inquérito na NEXARA." :
        (tipoFinal == 2) ? "FINAL 2 - BOM: Culpado condenado após confissão em audiência." :
        (tipoFinal == 3) ? "FINAL 3 - INCONCLUSIVO: Réu absolvido por falta de provas suficientes." :
        (tipoFinal == 4) ? "FINAL 4 - INJUSTIÇA E AFASTAMENTO: Acusou o inocente sem provas e foi afastado." :
        (tipoFinal == 5) ? "FINAL 5 - PRECIPITADO: Afastamento do caso antes de reunir todas as evidências." :
                           "FINAL 6 - A INJUSTIÇA: Inocente condenado com base nas provas reunidas.");
    _SetQuadroImg(&cs->paginas[3].quadros[2],
        (tipoFinal <= 2) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_ju_condenada.png" : "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_caio_condenado.png") :
        (tipoFinal == 3) ? "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_absolvido.png" :
        (tipoFinal == 4 || tipoFinal == 5) ? "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_afastamento.png" :
                           (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_injustica_caio.png" : "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_injustica_ju.png"));

    // =========================================================================
    // PAGINA 5/5: EPILOGO E CONSEQUENCIAS (2 grandes divididos na diagonal)
    // =========================================================================
    cs->paginas[4].tituloPagina = "PÁGINA 5/5 // EPÍLOGO E CONSEQUÊNCIAS";
    cs->paginas[4].numQuadros = 2;

    cs->paginas[4].quadros[0] = (QuadroHQ){
        { {95, 66}, {680, 66}, {585, 546}, {95, 546} },
        "01 // DESTINO DO ACUSADO", "", (tipoFinal <= 2) ? COR_VERDE_OK : COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[4].quadros[0].texto, 320,
        (tipoFinal == 1) ? "Escândalo público, bloqueio judicial e queda da NEXARA junto à prisão do culpado." :
        (tipoFinal == 2) ? "Culpado condenado; no cemitério, o inocente se despede de Gabriel em paz." :
        (tipoFinal == 3) ? "Quatro meses depois, o culpado assumiu cargo de destaque na NEXARA." :
        (tipoFinal == 6) ? "O inocente atrás das grades enquanto o verdadeiro culpado segue livre." :
        (tipoFinal == 4) ? "Inocente liberado por falta de provas; investigador afastado e culpado livre." :
                           "O culpado escapou do tribunal enquanto o caso esfriava.");
    _SetQuadroImg(&cs->paginas[4].quadros[0],
        (tipoFinal == 1) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f1_perfeito_ju.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f1_perfeito_caio.png") :
        (tipoFinal == 2) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f2_bom_inocente_caio.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f2_bom_inocente_ju.png") :
        (tipoFinal == 3) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f3_inconclusivo_ju.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f3_inconclusivo_caio.png") :
        (tipoFinal == 4) ? "AssetsRay/CutsceneRay/hq/p5_q1_f4_afastamento.png" :
        (tipoFinal == 5) ? "AssetsRay/CutsceneRay/hq/p5_q1_f5_precipitado.png" :
                           (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f6_injustica_caio_preso.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f6_injustica_ju_presa.png"));

    cs->paginas[4].quadros[1] = (QuadroHQ){
        { {694, 66}, {1185, 66}, {1185, 546}, {599, 546} },
        "02 // REFLEXÃO FINAL", "", COR_CIANO_NEON
    };
    snprintf(cs->paginas[4].quadros[1].texto, 320,
        (tipoFinal <= 2) ? "Em Recife, 2045, nenhuma mentira sintética resistiu às provas bem amarradas." :
                           "Em 2045, a verdade sem cadeia de custódia completa não basta.");
    _SetQuadroImg(&cs->paginas[4].quadros[1], (tipoFinal <= 2) ? "AssetsRay/CutsceneRay/hq/p5_q2_reflexao_vitoria.png" : "AssetsRay/CutsceneRay/hq/p5_q2_reflexao_derrota.png");

    _CarregarTexturasCutscene(cs);
}

static inline Vector2 _LerparV2(Vector2 a, Vector2 b, float t) {
    return (Vector2){ a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t };
}

static inline void DesenharQuadroCyberHQ(const QuadroHQ *q, FontesUI *f, float alpha, double tempo) {
    (void)tempo;
    const Vector2 *v = q->v;
    Vector2 centro = {
        (v[0].x + v[1].x + v[2].x + v[3].x) * 0.25f,
        (v[0].y + v[1].y + v[2].y + v[3].y) * 0.25f
    };

    // Fundo escuro estilo paineis murais ailibi
    Color bg = (Color){ 20, 14, 34, (unsigned char)(242 * alpha) };
    DrawTriangle(v[0], v[2], v[1], bg);
    DrawTriangle(v[0], v[3], v[2], bg);

    // Desenho da arte com encaixe "cover" centralizado e recorte exato no poligono
    if (q->temTex && q->tex.id > 0 && q->tex.width > 0 && q->tex.height > 0) {
        float minX = fminf(fminf(v[0].x, v[1].x), fminf(v[2].x, v[3].x));
        float maxX = fmaxf(fmaxf(v[0].x, v[1].x), fmaxf(v[2].x, v[3].x));
        float minY = fminf(fminf(v[0].y, v[1].y), fminf(v[2].y, v[3].y));
        float maxY = fmaxf(fmaxf(v[0].y, v[1].y), fmaxf(v[2].y, v[3].y));
        float boxW = fmaxf(1.0f, maxX - minX);
        float boxH = fmaxf(1.0f, maxY - minY);
        float texAspect = (float)q->tex.width / (float)q->tex.height;
        float boxAspect = boxW / boxH;

        float uOff = 0.0f, vOff = 0.0f, uSpan = 1.0f, vSpan = 1.0f;
        if (texAspect > boxAspect) {
            uSpan = boxAspect / texAspect;
            uOff = (1.0f - uSpan) * 0.5f;
        } else {
            vSpan = texAspect / boxAspect;
            vOff = (1.0f - vSpan) * 0.5f;
        }

        Vector2 uv[4];
        for (int i = 0; i < 4; i++) {
            float bx = (v[i].x - minX) / boxW;
            float by = (v[i].y - minY) / boxH;
            uv[i] = (Vector2){ uOff + bx * uSpan, vOff + by * vSpan };
        }

        unsigned char aByte = (unsigned char)(255.0f * alpha);
        rlSetTexture(q->tex.id);
        rlBegin(RL_TRIANGLES);
            rlColor4ub(255, 255, 255, aByte);
            rlTexCoord2f(uv[0].x, uv[0].y); rlVertex2f(v[0].x, v[0].y);
            rlTexCoord2f(uv[2].x, uv[2].y); rlVertex2f(v[2].x, v[2].y);
            rlTexCoord2f(uv[1].x, uv[1].y); rlVertex2f(v[1].x, v[1].y);

            rlTexCoord2f(uv[0].x, uv[0].y); rlVertex2f(v[0].x, v[0].y);
            rlTexCoord2f(uv[3].x, uv[3].y); rlVertex2f(v[3].x, v[3].y);
            rlTexCoord2f(uv[2].x, uv[2].y); rlVertex2f(v[2].x, v[2].y);
        rlEnd();
        rlSetTexture(0);
    } else {
        DrawTextEx(f->regular, "TEMPLATE HQ", (Vector2){ centro.x - 52, centro.y - 10 }, 18, 1.0f, ColorAlpha(q->corBorda, 0.22f * alpha));
    }

    // Glow externo + borda principal neon
    Color cGlow = ColorAlpha(q->corBorda, 0.20f * alpha);
    Color cBorda = ColorAlpha(q->corBorda, alpha);
    Color cInterna = ColorAlpha(COR_ROXO_NEON, 0.55f * alpha);
    for (int i = 0; i < 4; i++) {
        Vector2 a = v[i], b = v[(i + 1) % 4];
        DrawLineEx(a, b, 7.0f, cGlow);
        DrawLineEx(a, b, 2.8f, cBorda);

        // Borda interna paralela recuada em direcao ao centro
        Vector2 ia = _LerparV2(a, centro, 0.045f);
        Vector2 ib = _LerparV2(b, centro, 0.045f);
        DrawLineEx(ia, ib, 1.3f, cInterna);
    }

    // Barra de acento neon no topo esquerdo (inspirada nos PNGs murais ailibi)
    Vector2 barFim = _LerparV2(v[0], v[1], 0.28f);
    DrawLineEx(v[0], barFim, 5.5f, cBorda);
    Vector2 barInfIni = _LerparV2(v[3], v[2], 0.72f);
    DrawLineEx(barInfIni, v[2], 4.5f, ColorAlpha(COR_ROSA_NEON, alpha));

    // Etiqueta do quadro dentro do poligono com fundo escuro para legibilidade sobre a arte
    float padX = fmaxf(v[0].x, v[3].x) + 16.0f;
    if (padX > v[0].x + 28.0f) padX = v[0].x + 16.0f;
    float padY = v[0].y + 14.0f;
    Vector2 tamTag = MeasureTextEx(f->bold, q->tag, 18, 1.0f);
    DrawRectangleRounded((Rectangle){ padX - 8.0f, padY - 4.0f, tamTag.x + 16.0f, tamTag.y + 8.0f },
                         0.25f, 4, ColorAlpha((Color){ 10, 6, 20, 255 }, 0.78f * alpha));
    DrawTextEx(f->bold, q->tag, (Vector2){ padX, padY }, 18, 1.0f, cBorda);
}

// Retorna 1 quando terminar todas as 5 paginas ou pular no Modo Dev
static inline int AtualizarCutscene(EstadoCutscene *cs, Vector2 mouse, float dt, bool devDesbloqueado, Sound sfxClick, bool temSfx) {
    float velFade = 2.6f;

    if (cs->estadoTransicao == 0) {
        cs->fadeQuadro += dt * velFade;
        if (cs->fadeQuadro >= 1.0f) {
            cs->fadeQuadro = 1.0f;
            cs->estadoTransicao = 1;
        }
    } else if (cs->estadoTransicao == 2) {
        cs->fadePagina -= dt * velFade;
        if (cs->fadePagina <= 0.0f) {
            cs->fadePagina = 1.0f;
            cs->paginaIdx++;
            cs->quadrosVisiveis = 1;
            cs->fadeQuadro = 0.0f;
            cs->estadoTransicao = 0;
        }
        return 0;
    } else if (cs->estadoTransicao == 3) {
        cs->fadePagina -= dt * velFade;
        if (cs->fadePagina <= 0.0f) {
            cs->fadePagina = 0.0f;
            return 1;
        }
        return 0;
    }

    if (devDesbloqueado) {
        Rectangle btnPular = { 985, 18, 265, 40 };
        AtualizarHover(&btnPular, &cs->hoverPular, 1, mouse, dt);
        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, btnPular)) || IsKeyPressed(KEY_S)) {
            if (temSfx) PlaySound(sfxClick);
            cs->estadoTransicao = 3;
            return 0;
        }
    }

    bool avancar = (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouse.y > 65) ||
                   IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER);
    if (avancar) {
        if (temSfx) PlaySound(sfxClick);
        if (cs->estadoTransicao == 0) {
            cs->fadeQuadro = 1.0f;
            cs->estadoTransicao = 1;
        } else {
            const PaginaHQ *pag = &cs->paginas[cs->paginaIdx];
            if (cs->quadrosVisiveis < pag->numQuadros) {
                cs->quadrosVisiveis++;
                cs->fadeQuadro = 0.0f;
                cs->estadoTransicao = 0;
            } else if (cs->paginaIdx + 1 < NUM_PAGINAS_HQ) {
                cs->estadoTransicao = 2;
            } else {
                cs->estadoTransicao = 3;
            }
        }
    }
    return 0;
}

static inline void DesenharCutscene(EstadoCutscene *cs, GerenciadorAssets *assets, Vector2 mouse, double tempo, bool devDesbloqueado) {
    (void)mouse;
    const PaginaHQ *pag = &cs->paginas[cs->paginaIdx];
    DesenharFundoCyberGrid(tempo, COR_ROXO_NEON);

    for (int i = 0; i < NUM_PAGINAS_HQ; i++) {
        DrawRectangleRec((Rectangle){ 640 + i * 28, 28, 20, 8 }, (i == cs->paginaIdx) ? COR_ROSA_NEON : ColorAlpha(GRAY, 0.4f));
    }

    if (devDesbloqueado) {
        Rectangle btnPular = { 985, 18, 265, 40 };
        DesenharBotaoCyber(assets->fontes.bold, btnPular, "PULAR HQ [S]", 19, cs->hoverPular, tempo, COR_AMARELO_AVISO);
    }

    // Desenha os quadros poligonais da pagina um por vez com fade-in
    for (int i = 0; i < cs->quadrosVisiveis && i < pag->numQuadros; i++) {
        float alpha = (i == cs->quadrosVisiveis - 1) ? cs->fadeQuadro : 1.0f;
        DesenharQuadroCyberHQ(&pag->quadros[i], &assets->fontes, alpha, tempo);
    }

    // Legenda narrativa limpa na parte inferior (para o quadro atual revelado)
    int atualIdx = cs->quadrosVisiveis - 1;
    if (atualIdx >= 0 && atualIdx < pag->numQuadros) {
        const QuadroHQ *qAtual = &pag->quadros[atualIdx];
        Rectangle legBox = { 95, 560, 1090, 118 };
        DesenharPainelCyber(legBox, qAtual->corBorda);
        DesenharBadgeCyber(assets->fontes.bold, (Vector2){ legBox.x + 22, legBox.y + 14 }, qAtual->tag, 18, qAtual->corBorda);
        DesenharTextoQuebrado(assets->fontes.regular, qAtual->texto,
            (Rectangle){ legBox.x + 22, legBox.y + 52, legBox.width - 44, 58 }, 22, 1.0f, WHITE);
    }

    float pulso = 0.55f + 0.45f * sinf((float)tempo * 4.5f);
    const char *dica = (cs->quadrosVisiveis < pag->numQuadros)
        ? "CLIQUE OU ESPAÇO PARA O PRÓXIMO QUADRINHO >>"
        : (cs->paginaIdx + 1 < NUM_PAGINAS_HQ)
            ? "CLIQUE OU ESPAÇO PARA A PRÓXIMA PÁGINA >>"
            : "CLIQUE OU ESPAÇO PARA VER O RESULTADO >>";
    DrawTextEx(assets->fontes.bold, dica, (Vector2){ 740, 692 }, 18, 1.0f, ColorAlpha(COR_CIANO_NEON, pulso));

    if (cs->fadePagina < 1.0f) {
        unsigned char af = (unsigned char)((1.0f - cs->fadePagina) * 255.0f);
        DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, af });
    }
}

#endif
