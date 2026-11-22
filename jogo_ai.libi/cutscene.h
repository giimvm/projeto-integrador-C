#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "raylib.h"
#include "assets.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

#define NUM_PAGINAS_HQ 4
#define MAX_QUADROS_PAG 4

typedef struct {
    Rectangle rect;       // Posicao e tamanho do quadradinho na pagina (coordenadas virtuais 1280x720)
    const char *tag;      // Etiqueta curta no topo do quadrinho
    char texto[320];      // Legenda narrativa do quadrinho
    const char *imagem;   // Caminho da imagem do quadrinho
    Color corBorda;       // Cor de destaque da borda
} QuadroHQ;

typedef struct {
    const char *tituloPagina;
    int numQuadros;
    QuadroHQ quadros[MAX_QUADROS_PAG];
} PaginaHQ;

typedef struct {
    int paginaIdx;          // 0..3
    int quadrosVisiveis;    // 1..numQuadros (aparecem um por vez)
    float fadeQuadro;       // 0.0 -> 1.0 (fade in do ultimo quadro revelado)
    float fadePagina;       // 1.0 -> 0.0 ao trocar de pagina ou sair
    int estadoTransicao;    // 0=revelando quadro, 1=pagina parada, 2=fade out prox pagina, 3=fade out fim
    float hoverPular;
    CacheTextura caches[MAX_QUADROS_PAG];
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
    for (int i = 0; i < MAX_QUADROS_PAG; i++) {
        if (cs->caches[i].tex.id != 0) {
            UnloadTexture(cs->caches[i].tex);
            cs->caches[i].tex = (Texture2D){0};
        }
        cs->caches[i].caminho[0] = '\0';
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
    // PAGINA 1: BACKSTORY NA FACULDADE (4 quadradinhos separados)
    // =========================================================================
    cs->paginas[0].tituloPagina = "PÁGINA 1/4 // OS DIAS DE FACULDADE";
    cs->paginas[0].numQuadros = 4;

    cs->paginas[0].quadros[0] = (QuadroHQ){
        { 75, 76, 365, 345 }, "GABRIEL",
        "", "AssetsRay/Cutscenes/Q_GabrielFaculdade.png", COR_CIANO_NEON
    };
    snprintf(cs->paginas[0].quadros[0].texto, 320,
        "No curso de Computação, Gabriel virava madrugadas no laboratório. Idealista, mas pressionado por dívidas e promessas.");

    cs->paginas[0].quadros[1] = (QuadroHQ){
        { 458, 76, 365, 345 }, "JULIANA",
        "", "AssetsRay/Cutscenes/Q_JulianaFaculdade.png", COR_ROSA_NEON
    };
    snprintf(cs->paginas[0].quadros[1].texto, 320,
        "Juliana dedicou três anos ao próprio projeto de IA, acreditando que o esforço sozinho garantiria a vaga na NEXARA.");

    cs->paginas[0].quadros[2] = (QuadroHQ){
        { 841, 76, 364, 345 }, "CAIO",
        "", "AssetsRay/Evidências/CaioTrueEvidence.png", COR_ROXO_NEON
    };
    snprintf(cs->paginas[0].quadros[2].texto, 320,
        "Caio, herdeiro reservado, pagava a mensalidade de Gabriel e abria portas através da influência do pai na NEXARA.");

    cs->paginas[0].quadros[3] = (QuadroHQ){
        { 75, 438, 1130, 232 }, "O TRIO ANTES DA QUEDA",
        "", "AssetsRay/Cutscenes/Q_TrioCampus.png", COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[0].quadros[3].texto, 320,
        "Por fora, eram colegas inseparáveis no campus de Recife em 2045. Por dentro, uma única vaga na NEXARA e segredos de família já rachavam a amizade antes mesmo da festa.");

    // =========================================================================
    // PAGINA 2: A FESTA E AS PISTAS DA INVESTIGACAO (3 quadradinhos)
    // =========================================================================
    cs->paginas[1].tituloPagina = "PÁGINA 2/4 // A FESTA E OS RASTROS";
    cs->paginas[1].numQuadros = 3;

    cs->paginas[1].quadros[0] = (QuadroHQ){
        { 75, 76, 555, 310 }, "BRIGA DO LADO DE FORA",
        "", "AssetsRay/Cutscenes/Q_FestaDiscussao.png", COR_ROXO_NEON
    };
    snprintf(cs->paginas[1].quadros[0].texto, 320,
        "Na madrugada da festa, Caio e Gabriel discutiram feio do lado de fora. Testemunhas ouviram acusações sobre a vaga na NEXARA.");

    cs->paginas[1].quadros[1] = (QuadroHQ){
        { 650, 76, 555, 310 },
        p4 ? "NUVEM VALIDADA" : "NUVEM NÃO VALIDADA",
        "", "AssetsRay/Cutscenes/Painel2_TensaoFesta.png",
        p4 ? COR_VERDE_OK : COR_AMARELO_AVISO
    };
    if (ehJu) {
        snprintf(cs->paginas[1].quadros[1].texto, 320, p4
            ? "O backup da nuvem que você validou revelou: Gabriel entregou o código de Juliana ao Prof. João Paulo em troca da vaga."
            : "Sem o laudo do backup na nuvem, o roubo do projeto de Juliana apareceu só nas falas — sem prova documental no dossiê.");
    } else {
        snprintf(cs->paginas[1].quadros[1].texto, 320, p4
            ? "Os extratos da nuvem que você validou provaram: o pai de Caio usava Gabriel de laranja para lavar dinheiro e comprar a vaga."
            : "Sem os extratos da nuvem validados, o esquema financeiro por trás da vaga na NEXARA ficou sem comprovação técnica.");
    }

    cs->paginas[1].quadros[2] = (QuadroHQ){
        { 75, 404, 1130, 266 },
        (p1 && p2) ? "MÁSCARAS DE IA DESMASCARADAS" : "LACUNAS NAS PROVAS DIGITAIS",
        "", "AssetsRay/Evidências/JuliaDeepfake.png",
        (p1 && p2) ? COR_CIANO_NEON : COR_VERMELHO_ERR
    };
    if (ehJu) {
        snprintf(cs->paginas[1].quadros[2].texto, 320, (p1 && p2)
            ? "Sua Lupa Forense provou que tanto o áudio contra Caio quanto a foto de Juliana no carro (com volante inexistente) foram gerados por IA."
            : "Juliana usou IA para criar uma foto no carro e um áudio incriminando Caio — parte dessa farsa escapou da sua validação (%s/%s).",
            p1 ? "Áudio OK" : "Áudio Perdido", p2 ? "Foto OK" : "Foto Perdida");
    } else {
        snprintf(cs->paginas[1].quadros[2].texto, 320, (p1 && p2)
            ? "Você confirmou que o áudio da ameaça de Caio e a foto de Juliana saindo às 03:05 eram reais — desmontando a defesa dele."
            : "O áudio real da briga e a foto autêntica de Juliana eram peças-chave contra Caio, mas nem todas foram validadas (%s/%s).",
            p1 ? "Áudio OK" : "Áudio Perdido", p2 ? "Foto OK" : "Foto Perdida");
    }

    // =========================================================================
    // PAGINA 3: A CENA DO BARRANCO E A SAIDA (3 quadradinhos)
    // =========================================================================
    cs->paginas[2].tituloPagina = "PÁGINA 3/4 // O BARRANCO (02:50 - 03:41)";
    cs->paginas[2].numQuadros = 3;

    cs->paginas[2].quadros[0] = (QuadroHQ){
        { 75, 76, 1130, 275 }, "ATRÁS DO GALPÃO",
        "", "AssetsRay/Cutscenes/Painel3_Barranco.png", COR_ROSA_NEON
    };
    snprintf(cs->paginas[2].quadros[0].texto, 320, ehJu
        ? "Nos fundos do galpão, perto do barranco, Gabriel pediu desculpas e segurou o braço de Juliana. Tomada pela raiva, ela o golpeou na têmpora; ele caiu no escuro."
        : "Na estrada atrás do galpão, Gabriel ameaçou denunciar o esquema. Caio o empurrou à beira do barranco — e a queda contra a pedra foi fatal.");

    cs->paginas[2].quadros[1] = (QuadroHQ){
        { 75, 368, 555, 302 },
        p3 ? "GPS VALIDADO (03:20)" : "GPS SEM VALIDAÇÃO",
        "", "AssetsRay/Cutscenes/Q_CelularBarranco.png",
        p3 ? COR_VERDE_OK : COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[2].quadros[1].texto, 320, p3
        ? "Você validou o sinal das três torres: o celular de Gabriel nunca saiu da festa e parou no fundo do barranco às 03:20."
        : "Sem validar o registro de localização, a hora exata em que o celular de Gabriel ficou parado no barranco perdeu força técnica.");

    cs->paginas[2].quadros[2] = (QuadroHQ){
        { 650, 368, 555, 302 },
        p5 ? "REGISTRO DE SAÍDA VALIDADO" : "REGISTRO DE SAÍDA PERDIDO",
        "", ehJu ? "AssetsRay/Locais/SalaInterrogatorio.png" : "AssetsRay/Evidências/CaioTrueEvidence.png",
        p5 ? COR_CIANO_NEON : COR_VERMELHO_ERR
    };
    if (ehJu) {
        snprintf(cs->paginas[2].quadros[2].texto, 320, p5
            ? "A corrida da Mobi às 03:41 (em veículo autônomo sem volante) que você validou derrubou de vez o álibi de Juliana."
            : "Sem validar o registro da Mobi das 03:41, faltou a prova final de que Juliana ainda estava no galpão após a morte.");
    } else {
        snprintf(cs->paginas[2].quadros[2].texto, 320, p5
            ? "Você provou que o vídeo da cancela às 02:42 tinha placa distorcida e sombra errada — Caio forjou a própria saída com IA."
            : "Sem validar a fraude no vídeo da cancela, o álibi de Caio das 02:40 permaneceu de pé para a defesa.");
    }

    // =========================================================================
    // PAGINA 4: O VEREDITO E AS CONSEQUENCIAS (3 quadradinhos)
    // =========================================================================
    cs->paginas[3].tituloPagina = "PÁGINA 4/4 // VEREDITO E CONSEQUÊNCIAS";
    cs->paginas[3].numQuadros = 3;

    cs->paginas[3].quadros[0] = (QuadroHQ){
        { 75, 76, 555, 290 }, "PESO DO SEU DOSSIÊ",
        "", "AssetsRay/Locais/SalaEvidencias.png", COR_CIANO_NEON
    };
    snprintf(cs->paginas[3].quadros[0].texto, 320,
        "Você encerrou a investigação com %d/5 provas validadas: P1[%s] P2[%s] P3[%s] P4[%s] P5[%s].",
        totalVal, p1?"OK":"X", p2?"OK":"X", p3?"OK":"X", p4?"OK":"X", p5?"OK":"X");

    cs->paginas[3].quadros[1] = (QuadroHQ){
        { 650, 76, 555, 290 }, "A SENTENÇA",
        "",
        (tipoFinal == 1 || tipoFinal == 2) ? "AssetsRay/Cutscenes/Painel4_FinalPrisao.png"
        : (tipoFinal == 3 || tipoFinal == 4) ? "AssetsRay/Cutscenes/Painel4_FinalAbsolvicao.png"
        : "AssetsRay/Cutscenes/Painel4_FinalInjustica.png",
        (tipoFinal <= 2) ? COR_VERDE_OK : (tipoFinal == 3 || tipoFinal == 5) ? COR_AMARELO_AVISO : COR_VERMELHO_ERR
    };
    if (tipoFinal == 1) {
        snprintf(cs->paginas[3].quadros[1].texto, 320,
            "FINAL 1 - VERDADE ABSOLUTA: Com 5/5 provas fechadas, %s foi preso(a) sem margem para recurso digital.",
            ehJu ? "Juliana" : "Caio");
    } else if (tipoFinal == 2) {
        snprintf(cs->paginas[3].quadros[1].texto, 320,
            "FINAL 2 - A VERDADE: Com %d/5 provas validadas, %s não resistiu às contradições e confessou o crime.",
            totalVal, ehJu ? "Juliana" : "Caio");
    } else if (tipoFinal == 3) {
        snprintf(cs->paginas[3].quadros[1].texto, 320,
            "FINAL 3 - A DÚVIDA: Com apenas %d/5 provas validadas, a defesa invocou a Lei da Dúvida Digital e anulou a acusação.",
            totalVal);
    } else if (tipoFinal == 4) {
        snprintf(cs->paginas[3].quadros[1].texto, 320,
            "FINAL 4 - COLAPSO FORENSE: Com só %d/5 provas validadas, o Ministério Público rejeitou o inquérito antes do tribunal.",
            totalVal);
    } else if (tipoFinal == 5) {
        snprintf(cs->paginas[3].quadros[1].texto, 320,
            "FINAL 5 - A PRECIPITAÇÃO: Você acusou antes da nuvem e do confronto final. O Delegado Meireles te afastou do caso.");
    } else {
        snprintf(cs->paginas[3].quadros[1].texto, 320,
            "FINAL 6 - A INJUSTIÇA: Sem provas contra o verdadeiro culpado, você condenou %s apenas por motivo aparente.",
            ehJu ? "Caio" : "Juliana");
    }

    cs->paginas[3].quadros[2] = (QuadroHQ){
        { 75, 384, 1130, 286 }, "MESES DEPOIS...",
        "",
        (tipoFinal == 1 || tipoFinal == 2) ? "AssetsRay/Cutscenes/Painel4_FinalPrisao.png"
        : (tipoFinal == 3 || tipoFinal == 4) ? "AssetsRay/Cutscenes/Painel4_FinalAbsolvicao.png"
        : "AssetsRay/Cutscenes/Painel4_FinalInjustica.png",
        (tipoFinal <= 2) ? COR_VERDE_OK : COR_ROSA_NEON
    };
    if (tipoFinal == 1) {
        snprintf(cs->paginas[3].quadros[2].texto, 320, ehJu
            ? "O esquema com o Prof. João Paulo caiu e a NEXARA foi investigada. Em Recife, 2045, nenhuma mentira sintética resistiu às 5 provas."
            : "A empresa de fachada da família Vasconcelos foi bloqueada e o esquema na NEXARA desmoronou. Justiça completa.");
    } else if (tipoFinal == 2) {
        snprintf(cs->paginas[3].quadros[2].texto, 320,
            "O caso foi encerrado com sucesso. Atrás do vidro, o inocente respirou aliviado enquanto a verdade prevaleceu.");
    } else if (tipoFinal == 3) {
        snprintf(cs->paginas[3].quadros[2].texto, 320,
            "Quatro meses depois, %s assumiu a nova liderança de Engenharia de IA na NEXARA. A verdade sem prova chegou tarde demais.",
            ehJu ? "Juliana Costa" : "Caio Vasconcelos");
    } else if (tipoFinal == 4) {
        snprintf(cs->paginas[3].quadros[2].texto, 320,
            "O processo foi engavetado por falta de lastro técnico. A família de Gabriel ficou sem respostas e o culpado seguiu livre.");
    } else if (tipoFinal == 5) {
        snprintf(cs->paginas[3].quadros[2].texto, 320,
            "Afastada da investigação, Helena viu o culpado ganhar tempo para apagar rastros. Na perícia digital, pressa custa a verdade.");
    } else {
        snprintf(cs->paginas[3].quadros[2].texto, 320,
            "Enquanto um inocente cumpria pena, Helena encarou a foto de %s na parede da delegacia e percebeu o tamanho do erro.",
            ehJu ? "Juliana Costa" : "Caio Vasconcelos");
    }
}

// Retorna 1 quando terminar todas as 4 paginas ou pular no Modo Dev
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
            DescarregarCutsceneCaches(cs);
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
            // Completa o fade-in do quadro atual imediatamente
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

    // Folha escura da pagina de HQ
    DrawRectangleRounded((Rectangle){ 55, 62, 1170, 624 }, 0.015f, 4, (Color){ 16, 12, 26, 235 });
    DrawRectangleLinesEx((Rectangle){ 55, 62, 1170, 624 }, 2.0f, ColorAlpha(COR_ROXO_NEON, 0.55f));

    // Cabecalho da pagina
    DesenharBadgeCyber(assets->fontes.bold, (Vector2){ 55, 16 }, pag->tituloPagina, 21, COR_CIANO_NEON);

    // Indicador de paginas 1..4
    for (int i = 0; i < NUM_PAGINAS_HQ; i++) {
        DrawRectangleRec((Rectangle){ 590 + i * 28, 28, 20, 8 }, (i == cs->paginaIdx) ? COR_ROSA_NEON : ColorAlpha(GRAY, 0.4f));
    }

    if (devDesbloqueado) {
        Rectangle btnPular = { 985, 18, 265, 40 };
        DesenharBotaoCyber(assets->fontes.bold, btnPular, "PULAR HQ [S]", 19, cs->hoverPular, tempo, COR_AMARELO_AVISO);
    }

    // Desenha cada quadradinho revelado (um por vez com fade-in)
    for (int i = 0; i < cs->quadrosVisiveis && i < pag->numQuadros; i++) {
        const QuadroHQ *q = &pag->quadros[i];
        float alpha = (i == cs->quadrosVisiveis - 1) ? cs->fadeQuadro : 1.0f;
        unsigned char a255 = (unsigned char)(255.0f * alpha);

        // Moldura branca externa de gibi + fundo escuro do quadro
        DrawRectangle((int)q->rect.x - 3, (int)q->rect.y - 3, (int)q->rect.width + 6, (int)q->rect.height + 6, (Color){ 238, 235, 245, a255 });
        DrawRectangleRec(q->rect, (Color){ 12, 8, 22, a255 });

        Texture2D tex = ObterTexturaCache(&cs->caches[i], q->imagem);
        if (tex.id != 0) {
            DesenharTexturaCover(tex, q->rect, (Color){ 255, 255, 255, a255 });
        }

        // Faixa escura inferior dentro do quadradinho para a legenda do gibi
        float legH = (q->rect.height >= 300) ? 104.0f : 92.0f;
        Rectangle legRect = { q->rect.x, q->rect.y + q->rect.height - legH, q->rect.width, legH };
        DrawRectangleRec(legRect, (Color){ 10, 6, 20, (unsigned char)(228 * alpha) });
        DrawLineEx((Vector2){ legRect.x, legRect.y }, (Vector2){ legRect.x + legRect.width, legRect.y }, 2.0f, ColorAlpha(q->corBorda, alpha));

        // Tag superior do quadradinho
        Vector2 szTag = MeasureTextEx(assets->fontes.bold, q->tag, 17, 1.0f);
        Rectangle tagBox = { q->rect.x + 8, q->rect.y + 8, szTag.x + 18, 25 };
        DrawRectangleRec(tagBox, (Color){ 14, 10, 26, (unsigned char)(230 * alpha) });
        DrawRectangleLinesEx(tagBox, 1.5f, ColorAlpha(q->corBorda, alpha));
        DrawTextEx(assets->fontes.bold, q->tag, (Vector2){ tagBox.x + 9, tagBox.y + 4 }, 17, 1.0f, ColorAlpha(q->corBorda, alpha));

        // Texto da legenda do quadradinho
        DesenharTextoQuebrado(assets->fontes.regular, q->texto,
            (Rectangle){ legRect.x + 12, legRect.y + 8, legRect.width - 24, legH - 12 }, 19, 1.0f, (Color){ 255, 255, 255, a255 });

        DrawRectangleLinesEx(q->rect, 2.5f, ColorAlpha(q->corBorda, alpha));
    }

    float pulso = 0.55f + 0.45f * sinf((float)tempo * 4.5f);
    const char *dica = (cs->quadrosVisiveis < pag->numQuadros)
        ? "CLIQUE OU ESPAÇO PARA O PRÓXIMO QUADRINHO >>"
        : (cs->paginaIdx + 1 < NUM_PAGINAS_HQ)
            ? "CLIQUE OU ESPAÇO PARA A PRÓXIMA PÁGINA >>"
            : "CLIQUE OU ESPAÇO PARA VER O RESULTADO >>";
    DrawTextEx(assets->fontes.bold, dica, (Vector2){ 740, 694 }, 18, 1.0f, ColorAlpha(COR_CIANO_NEON, pulso));

    // Fade out de pagina
    if (cs->fadePagina < 1.0f) {
        unsigned char af = (unsigned char)((1.0f - cs->fadePagina) * 255.0f);
        DrawRectangle(0, 0, VIRTUAL_W, VIRTUAL_H, (Color){ 6, 4, 14, af });
    }
}

#endif
