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
    const char *marcador; // Sinal visual opcional (ex: "[REFAZER]", "[NOVO]")
    float cropShiftY;     // Ajuste vertical do recorte (0.0=centro, negativo=sobe p/ rostos)
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

static inline void _GarantirTexturaQuadro(QuadroHQ *q) {
    if (q->temTex || q->imgPath[0] == '\0') return;
    char buf[256];
    const char *resolvido = ResolverCaminho(q->imgPath, buf, sizeof(buf));
    if (FileExists(resolvido)) {
        q->tex = LoadTexture(resolvido);
        if (q->tex.id > 0) {
            SetTextureFilter(q->tex, TEXTURE_FILTER_BILINEAR);
            q->temTex = true;
        }
    }
}

// Carrega apenas 1 textura por vez para evitar travadinha ao abrir a cutscene
static inline void _CarregarTexturasCutscene(EstadoCutscene *cs) {
    if (cs->paginas[0].numQuadros > 0) {
        _GarantirTexturaQuadro(&cs->paginas[0].quadros[0]);
    }
}

static inline void _PrecarregarUmaTexturaCutscene(EstadoCutscene *cs) {
    for (int offset = 0; offset < NUM_PAGINAS_HQ; offset++) {
        int p = (cs->paginaIdx + offset) % NUM_PAGINAS_HQ;
        for (int i = 0; i < cs->paginas[p].numQuadros && i < MAX_QUADROS_PAG; i++) {
            QuadroHQ *q = &cs->paginas[p].quadros[i];
            if (!q->temTex && q->imgPath[0] != '\0') {
                _GarantirTexturaQuadro(q);
                return;
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
    (void)statusProva;
    (void)respostaProva;

    // =========================================================================
    // PAGINA 1/5: ANTES DO CASO (Layout TemplateGibi: 2 Esq + 3 Dir)
    // =========================================================================
    cs->paginas[0].tituloPagina = "PÁGINA 1/5 // ANTES DO CASO";
    cs->paginas[0].numQuadros = 5;

    // Q1: Topo Esquerda
    cs->paginas[0].quadros[0] = (QuadroHQ){
        { {95, 66}, {658, 66}, {602, 294}, {95, 326} },
        "01 // GABRIEL E O PROFESSOR", "", COR_CIANO_NEON
    };
    snprintf(cs->paginas[0].quadros[0].texto, 320, "Gabriel e o Prof. João Paulo viviam um caso escondido, e Gabriel sabia muito bem o que pedir em troca.");
    _SetQuadroImg(&cs->paginas[0].quadros[0], "AssetsRay/CutsceneRay/hq/p1_q1_gabriel.png");

    // Q2: Topo Direita
    cs->paginas[0].quadros[1] = (QuadroHQ){
        { {672, 66}, {1185, 66}, {1185, 198}, {622, 250} },
        "02 // JULIANA ESTUDANDO", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[0].quadros[1].texto, 320, "Enquanto isso, Juliana virava noites entre código e café, apostando três anos de ralação numa vaga da NEXARA.");
    _SetQuadroImg(&cs->paginas[0].quadros[1], "AssetsRay/CutsceneRay/hq/p1_q2_juliana.png");

    // Q3: Meio Direita
    cs->paginas[0].quadros[2] = (QuadroHQ){
        { {618, 264}, {1185, 212}, {1185, 404}, {580, 366} },
        "03 // CAIO CHANTAGEANDO", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[0].quadros[2].texto, 320, "Já Caio não movia uma palha nos estudos: preferiu gravar o segredo de Gabriel pra arrancar a vaga na marra.");
    _SetQuadroImg(&cs->paginas[0].quadros[2], "AssetsRay/CutsceneRay/hq/p1_q3_caio.png");

    // Q4: Baixo Esquerda
    cs->paginas[0].quadros[3] = (QuadroHQ){
        { {95, 340}, {598, 308}, {540, 546}, {95, 546} },
        "04 // A AMIZADE ANTES", "", COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[0].quadros[3].texto, 320, "Antes de tudo desandar, os três riam pelos corredores do campus como se nada fosse separar a amizade.");
    _SetQuadroImg(&cs->paginas[0].quadros[3], "AssetsRay/CutsceneRay/hq/p1_q4_amizade.png");

    // Q5: Baixo Direita
    cs->paginas[0].quadros[4] = (QuadroHQ){
        { {576, 380}, {1185, 418}, {1185, 546}, {554, 546} },
        "05 // A AMIZADE SE DESTRUINDO", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[0].quadros[4].texto, 320, "Quando a vaga escapou das mãos de Juliana, o clima azedou de vez. Entre culpa e chantagem, o trio já tava por um fio.");
    _SetQuadroImg(&cs->paginas[0].quadros[4], "AssetsRay/CutsceneRay/hq/p1_q5_ruptura.png");
    cs->paginas[0].quadros[4].cropShiftY = -0.28f;

    // =========================================================================
    // PAGINA 2/5: O QUE LEVOU AO CASO E A CULPA (4 quadros 2x2)
    // =========================================================================
    cs->paginas[1].tituloPagina = "PÁGINA 2/5 // O QUE LEVOU AO CASO";
    cs->paginas[1].numQuadros = 4;

    cs->paginas[1].quadros[0] = (QuadroHQ){
        { {95, 66}, {655, 66}, {605, 308}, {95, 282} },
        "01 // BRIGA NA FESTA", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[1].quadros[0].texto, 320, "Na saída da festa, a paciência acabou. Gabriel e Caio bateram boca na frente de todo mundo.");
    _SetQuadroImg(&cs->paginas[1].quadros[0], "AssetsRay/CutsceneRay/hq/p2_q1_briga.png");
    cs->paginas[1].quadros[0].cropShiftY = -0.25f;

    cs->paginas[1].quadros[1] = (QuadroHQ){
        { {669, 66}, {1185, 66}, {1185, 336}, {619, 308} },
        ehJu ? "02 // A CONFISSÃO" : "02 // A AMEAÇA", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[1].quadros[1].texto, 320, ehJu
        ? "Naquela madrugada, a verdade escapou: Gabriel admitiu pra Juliana que tinha passado a perna nela."
        : "Bêbado e sem filtro, Gabriel ameaçou abrir o bico sobre a chantagem e o esquema da família Vasconcelos.");
    _SetQuadroImg(&cs->paginas[1].quadros[1], ehJu ? "AssetsRay/CutsceneRay/hq/p2_q2_nuvem_ju.png" : "AssetsRay/CutsceneRay/hq/p2_q2_nuvem_caio.png");

    cs->paginas[1].quadros[2] = (QuadroHQ){
        { {95, 296}, {595, 322}, {655, 546}, {95, 546} },
        "03 // ATRÁS DO GALPÃO", "", COR_VERMELHO_ERR
    };
    snprintf(cs->paginas[1].quadros[2].texto, 320, ehJu
        ? "Atrás do galpão, três anos de mágoa falaram mais alto, Juliana perdeu a cabeça e partiu pra cima dele."
        : "Encurralado na estrada escura, Caio empurrou Gabriel na beira do barranco só pra calar a boca dele.");
    _SetQuadroImg(&cs->paginas[1].quadros[2], ehJu ? "AssetsRay/CutsceneRay/hq/p3_q1_galpao_ju.png" : "AssetsRay/CutsceneRay/hq/p3_q1_galpao_caio.png");

    cs->paginas[1].quadros[3] = (QuadroHQ){
        { {609, 322}, {1185, 350}, {1185, 546}, {669, 546} },
        "04 // O PESO DA CULPA", "", COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[1].quadros[3].texto, 320, ehJu
        ? "O barulho da queda gelou o sangue na hora. Ali, no escuro, Juliana entendeu que não tinha mais volta."
        : "Entre raiva e pânico, Caio olhou pro fundo do barranco e sentiu o chão sumir debaixo dos pés.");
    _SetQuadroImg(&cs->paginas[1].quadros[3], ehJu ? "AssetsRay/CutsceneRay/hq/p3_q3_saida_ju.png" : "AssetsRay/CutsceneRay/hq/p3_q3_saida_caio.png");

    // =========================================================================
    // PAGINA 3/5: LUTO, JULGAMENTO E SENTENCA (3 quadros)
    // =========================================================================
    cs->paginas[2].tituloPagina = "PÁGINA 3/5 // O PESO DO JULGAMENTO";
    cs->paginas[2].numQuadros = 3;

    cs->paginas[2].quadros[0] = (QuadroHQ){
        { {95, 66}, {615, 66}, {555, 546}, {95, 546} },
        "01 // O QUE RESTOU DA AMIZADE", "", COR_CIANO_NEON
    };
    snprintf(cs->paginas[2].quadros[0].texto, 320, ehJu
        ? "Com o amigo morto, o que sobrou pro Caio foi uma foto velha na mão e a pergunta de onde tudo desandou."
        : "Com o amigo morto, tudo o que restou pra Juliana foi uma foto velha e um silêncio que não passa.");
    _SetQuadroImg(&cs->paginas[2].quadros[0], ehJu ? "AssetsRay/CutsceneRay/hq/p3_q1_inocente_foto_caio.png" : "AssetsRay/CutsceneRay/hq/p3_q1_inocente_foto_ju.png");

    cs->paginas[2].quadros[1] = (QuadroHQ){
        { {629, 66}, {1185, 66}, {1185, 290}, {598, 315} },
        "02 // O JULGAMENTO", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[2].quadros[1].texto, 320, (tipoFinal <= 2)
        ? "No tribunal, a defesa tentou jogar tudo na conta da dúvida digital, mas as provas não deram folga."
        : "No tribunal, cada ponta solta da investigação virou munição nas mãos da defesa.");
    _SetQuadroImg(&cs->paginas[2].quadros[1], (tipoFinal <= 2) ? "AssetsRay/CutsceneRay/hq/p4_q2_audiencia_vitoria.png" : "AssetsRay/CutsceneRay/hq/p4_q2_audiencia_derrota.png");

    cs->paginas[2].quadros[2] = (QuadroHQ){
        { {596, 329}, {1185, 304}, {1185, 546}, {569, 546} },
        "03 // SENTENÇA E PRISÃO", "", (tipoFinal <= 2) ? COR_VERDE_OK : COR_VERMELHO_ERR
    };
    snprintf(cs->paginas[2].quadros[2].texto, 320,
        (tipoFinal == 1) ? "O culpado foi condenado sem brechas, e todo o esquema escondido na NEXARA veio abaixo." :
        (tipoFinal == 2) ? "Diante das provas, o culpado confessou no tribunal e acabou condenado pelo crime." :
        (tipoFinal == 3) ? "Sem provas técnicas fechadas, o juiz anulou a acusação e o réu saiu livre." :
        (tipoFinal == 4) ? "Com a acusação errada e sem lastro, o caso foi rejeitado antes mesmo da sentença." :
        (tipoFinal == 5) ? "A pressa falou mais alto que o método, e a investigação foi encerrada cedo demais." :
                           "No fim do julgamento, o inocente pagou a conta atrás das grades no lugar do verdadeiro culpado.");
    _SetQuadroImg(&cs->paginas[2].quadros[2],
        (tipoFinal <= 2) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_ju_condenada.png" : "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_caio_condenado.png") :
        (tipoFinal == 3) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p3_q3_sentenca_absolvido.png" : "AssetsRay/CutsceneRay/hq/p3_q3_f3_caio_sentado_mesa.png") :
        (tipoFinal == 4 || tipoFinal == 5) ? "AssetsRay/CutsceneRay/hq/p3_q3_sentenca_afastamento.png" :
                           (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_injustica_caio.png" : "AssetsRay/CutsceneRay/hq/p4_q3_sentenca_injustica_ju.png"));

    // =========================================================================
    // PAGINA 4/5: CONSEQUENCIA POS-CASO (2 quadros divididos na diagonal)
    // =========================================================================
    cs->paginas[3].tituloPagina = "PÁGINA 4/5 // CONSEQUÊNCIA PÓS-CASO";
    cs->paginas[3].numQuadros = 2;

    cs->paginas[3].quadros[0] = (QuadroHQ){
        { {95, 66}, {680, 66}, {585, 546}, {95, 546} },
        "01 // CONSEQUÊNCIA PÓS-CASO", "", (tipoFinal <= 2) ? COR_VERDE_OK : COR_AMARELO_AVISO
    };
    snprintf(cs->paginas[3].quadros[0].texto, 320,
        (tipoFinal == 1) ? "O escândalo explodiu na mídia, a NEXARA foi pro brejo e o culpado acabou atrás das grades." :
        (tipoFinal == 2) ? (ehJu ? "Caio seguiu na empresa tentando expor os podres de dentro e foi se despedir do amigo." : "Juliana mergulhou nos estudos com mais força do que nunca e foi se despedir de Gabriel.") :
        (tipoFinal == 3) ? (ehJu ? "Juliana saiu livre pela brecha da lei, mas o alívio durou pouco, o vazio ficou." : "Absolvido por falta de provas, Caio voltou pra NEXARA, mas já não tinha paz em lugar nenhum.") :
        (tipoFinal == 4) ? (ehJu ? "Com o susto de quase ver Caio cair por ela, Juliana largou o cargo e tentou se reaproximar." : "As ligações não param de chegar, mas Caio só consegue encarar a foto antiga dos três.") :
        (tipoFinal == 5) ? (ehJu ? "No alto da NEXARA à noite, Juliana olha a cidade inteira, e nunca se sentiu tão sozinha." : "Mesmo livre, o peso do barranco não dá trégua: toda madrugada termina em pesadelo.") :
                           (ehJu ? "Do lado de fora das grades, Juliana encara a prisão sabendo quem realmente devia tá lá dentro." : "Sozinha na cela, Juliana esperou uma visita que o Caio nunca teve coragem de fazer."));
    _SetQuadroImg(&cs->paginas[3].quadros[0],
        (tipoFinal == 1) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f1_perfeito_ju.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f1_perfeito_caio.png") :
        (tipoFinal == 2) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f2_bom_inocente_caio.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f2_bom_inocente_ju.png") :
        (tipoFinal == 3) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f3_inconclusivo_ju.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f3_inconclusivo_caio.png") :
        (tipoFinal == 4) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q1_f4_ju_afasta_reaproxima.png" : "AssetsRay/CutsceneRay/hq/p4_q1_f4_caio_retrato_celular.png") :
        (tipoFinal == 5) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q1_f5_ju_escritorio_noite.png" : "AssetsRay/CutsceneRay/hq/p4_q1_f5_caio_cama_pesadelo.png") :
                           (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q1_f6_ju_encara_prisao.png" : "AssetsRay/CutsceneRay/hq/p4_q1_f6_juliana_prisao_sozinha.png"));

    cs->paginas[3].quadros[1] = (QuadroHQ){
        { {694, 66}, {1185, 66}, {1185, 546}, {599, 546} },
        "02 // DESDOBRAMENTO", "", COR_CIANO_NEON
    };
    snprintf(cs->paginas[3].quadros[1].texto, 320,
        (tipoFinal <= 2)
            ? (ehJu ? "Entre o vidro da prisão, não precisou de discurso: Caio e Juliana finalmente fizeram as pazes."
                    : "Abalada por ter perdido a pesquisa, uma borboleta marrom e dourada pousou perto dela como um adeus.") :
        (tipoFinal == 3) ? (ehJu ? "No escuro do quarto, o sorriso daquela foto na faculdade parecia de outra vida." : "No silêncio do escritório, Caio percebeu que conseguiu a vaga, mas perdeu todo mundo.") :
        (tipoFinal == 4) ? (ehJu ? "De roupas leves diante da lápide, os dois dividem o mesmo silêncio e o mesmo arrependimento." : "Na reunião da NEXARA, sem o Gabriel pra segurar as pontas, Caio travou sem saber explicar nada.") :
        (tipoFinal == 5) ? (ehJu ? "Por fora ela tentava manter a pose, mas a culpa acabou escapando no silêncio." : "O sono não vem mais, Caio passa a madrugada inteira encarando o teto no escuro.") :
                           (ehJu ? "De volta à rotina da empresa, o retrato na mesa virou um lembrete impossível de ignorar." : "Enquanto isso, a defesa já batia na porta do Delegado Meireles tentando virar esse jogo."));
    _SetQuadroImg(&cs->paginas[3].quadros[1],
        (tipoFinal <= 2)
            ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q2_reconciliacao_prisao.png" : "AssetsRay/CutsceneRay/hq/p4_q2_juliana_cemiterio_borboleta.png") :
        (tipoFinal == 3) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q2_f3_ju_retrato.png" : "AssetsRay/CutsceneRay/hq/p4_q2_f3_caio_isolado.png") :
        (tipoFinal == 4) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q2_f4_ju_caio_cemiterio.png" : "AssetsRay/CutsceneRay/hq/p4_q2_f4_caio_reuniao_nervoso.png") :
        (tipoFinal == 5) ? (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q2_f5_closeup_olho_lagrima.png" : "AssetsRay/CutsceneRay/hq/p4_q2_f5_caio_olhos_abertos.png") :
                           (ehJu ? "AssetsRay/CutsceneRay/hq/p4_q2_f6_ju_escritorio_retrato.png" : "AssetsRay/CutsceneRay/hq/p4_q2_f6_advogado_meireles.png"));

    // =========================================================================
    // PAGINA 5/5: EPILOGO / NOVO COMECO (2 quadros)
    // =========================================================================
    cs->paginas[4].tituloPagina = "PÁGINA 5/5 // EPÍLOGO";
    cs->paginas[4].numQuadros = 2;

    cs->paginas[4].quadros[0] = (QuadroHQ){
        { {95, 66}, {680, 66}, {585, 546}, {95, 546} },
        "01 // EPÍLOGO", "", COR_ROXO_NEON
    };
    snprintf(cs->paginas[4].quadros[0].texto, 320,
        (tipoFinal == 1 && ehJu)  ? "Caio largou a NEXARA de vez e abriu uma startup de bolsas pra jovens perdidos, com a foto de Gabriel sempre na mesa." :
        (tipoFinal == 1 && !ehJu) ? "Em memória de Gabriel, Juliana ergueu a própria empresa e lançou uma IA pra ajudar a achar pessoas desaparecidas." :
        (tipoFinal == 2 && ehJu)  ? "Com a lição gravada na pele, Caio usou a própria influência pra abrir caminho pra quem realmente precisa." :
        (tipoFinal == 2 && !ehJu) ? "Juliana deu a volta por cima e criou uma startup pra blindar projetos universitários contra roubo de autoria." :
        (tipoFinal == 3)          ? (ehJu ? "Sem conseguir mais vestir aquela camisa, Juliana deixou a NEXARA e sumiu daquela vida." : "O cargo que ele tanto quis não preencheu nada, só sobrou o olhar vazio diante da foto antiga.") :
        (tipoFinal == 4)          ? (ehJu ? "De noite no espelho, a culpa ganhou forma: por um segundo, ela jurava que Gabriel tava logo atrás." : "Livre da acusação injusta, Juliana seguiu o próprio caminho e fez a startup dela decolar.") :
        (tipoFinal == 5)          ? (ehJu ? "Entre a chuva e os lírios vermelhos do cemitério, o peso daquela noite voltou inteiro." : "Cansada de esperar, Juliana bateu à porta da delegacia disposta a ir até o fim.") :
                                    (ehJu ? "Quando Juliana saiu do escritório, parecia que ela mesma tinha deixado a própria história pra trás." : "No fim do expediente, Caio virou as costas e saiu da sala tentando esquecer o retrato na mesa."));
    _SetQuadroImg(&cs->paginas[4].quadros[0],
        (tipoFinal == 1 && ehJu)  ? "AssetsRay/CutsceneRay/hq/p5_q1_caio_startup_bolsas.png" :
        (tipoFinal == 1 && !ehJu) ? "AssetsRay/CutsceneRay/hq/p5_q1_juliana_ia_desaparecidos.png" :
        (tipoFinal == 2 && ehJu)  ? "AssetsRay/CutsceneRay/hq/p5_q1_caio_startup_bolsas.png" :
        (tipoFinal == 2 && !ehJu) ? "AssetsRay/CutsceneRay/hq/p5_q1_juliana_startup_autoral.png" :
        (tipoFinal == 3)          ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f3_ju_deixa_nexara.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f3_caio_olhar_vazio_retrato.png") :
        (tipoFinal == 4)          ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f4_ju_espelho_gabriel.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f4_juliana_startup_sucesso.png") :
        (tipoFinal == 5)          ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f5_ju_cemiterio_spider_lily.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f5_juliana_indo_delegacia.png") :
                                    (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q1_f6_ju_saindo_escritorio.png" : "AssetsRay/CutsceneRay/hq/p5_q1_f6_caio_saindo_escritorio.png"));

    cs->paginas[4].quadros[1] = (QuadroHQ){
        { {694, 66}, {1185, 66}, {1185, 546}, {599, 546} },
        "02 // FECHAMENTO FINAL", "", COR_ROSA_NEON
    };
    snprintf(cs->paginas[4].quadros[1].texto, 320,
        (tipoFinal <= 2 && ehJu)  ? "Anos depois, do lado de fora da prisão, Caio sorriu entre lágrimas e pediu desculpa, enquanto uma borboleta cruzava o céu." :
        (tipoFinal == 1 && !ehJu) ? "Com a verdade de pé, Recife dormiu um pouco mais justa, e o legado de Gabriel seguiu vivo." :
        (tipoFinal == 2 && !ehJu) ? "Na mesa da sua startup, o sorriso de Gabriel naquela foto lembrava todos os dias o porquê de ela não ter desistido." :
        (tipoFinal == 3)          ? (ehJu ? "O retrato estilhaçado na cama dizia tudo: daqueles três amigos, não sobrou ninguém inteiro." : "Caio saiu pela porta deixando o prêmio pra trás. Ganhou tudo, e não levou nada.") :
        (tipoFinal == 4)          ? (ehJu ? "Quando ela se virou, não tinha ninguém lá. Só o silêncio e o choro que ela segurou tempo demais." : "Sem o talento de verdade e afundada em mentiras, a NEXARA começou a ruir por dentro.") :
        (tipoFinal == 5)          ? (ehJu ? "O caso voltou pros jornais na TV, e as malas já estavam prontas pra fuga antes que batessem na porta." : "Diante do Delegado Meireles, Juliana colocou as cartas na mesa pra reabrir o caso do amigo.") :
                                    "A moldura continuava inteira, mas quem escapou da justiça já tinha apagado o próprio rosto.");
    _SetQuadroImg(&cs->paginas[4].quadros[1],
        (tipoFinal <= 2 && ehJu)  ? "AssetsRay/CutsceneRay/hq/p5_q2_caio_espera_juliana.png" :
        (tipoFinal == 1 && !ehJu) ? "AssetsRay/CutsceneRay/hq/p5_q2_reflexao_vitoria.png" :
        (tipoFinal == 2 && !ehJu) ? "AssetsRay/CutsceneRay/hq/p5_q2_f2_caio_mesa_juliana.png" :
        (tipoFinal == 3)          ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q2_f3_retrato_quebrado.png" : "AssetsRay/CutsceneRay/hq/p5_q2_f3_caio_sai_premio.png") :
        (tipoFinal == 4)          ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q2_f4_ju_vira_chorando.png" : "AssetsRay/CutsceneRay/hq/p5_q2_f4_nexara_decadencia.png") :
        (tipoFinal == 5)          ? (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q2_f5_tv_reabertura_malas.png" : "AssetsRay/CutsceneRay/hq/p5_q2_f5_juliana_fala_meireles.png") :
                                    (ehJu ? "AssetsRay/CutsceneRay/hq/p5_q2_f6_retrato_ju_sem_rosto.png" : "AssetsRay/CutsceneRay/hq/p5_q2_f6_retrato_caio_sem_rosto.png"));

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
            float tY = 0.5f + q->cropShiftY;
            if (tY < 0.0f) tY = 0.0f;
            if (tY > 1.0f) tY = 1.0f;
            vOff = (1.0f - vSpan) * tY;
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

    if (q->marcador && q->marcador[0] != '\0') {
        Vector2 tamM = MeasureTextEx(f->bold, q->marcador, 15, 1.0f);
        float mx = fminf(v[1].x, v[2].x) - tamM.x - 20.0f;
        float my = v[0].y + 14.0f;
        DrawRectangleRounded((Rectangle){ mx - 8.0f, my - 4.0f, tamM.x + 16.0f, tamM.y + 8.0f },
                             0.25f, 4, ColorAlpha((Color){ 32, 18, 4, 255 }, 0.90f * alpha));
        DrawRectangleRoundedLinesEx((Rectangle){ mx - 8.0f, my - 4.0f, tamM.x + 16.0f, tamM.y + 8.0f },
                                    0.25f, 4, 1.5f, ColorAlpha(COR_AMARELO_AVISO, alpha));
        DrawTextEx(f->bold, q->marcador, (Vector2){ mx, my }, 15, 1.0f, ColorAlpha(COR_AMARELO_AVISO, alpha));
    }
}

// Retorna 1 quando terminar todas as 5 paginas ou pular no Modo Dev
static inline int AtualizarCutscene(EstadoCutscene *cs, Vector2 mouse, float dt, bool devDesbloqueado, Sound sfxClick, bool temSfx) {
    _PrecarregarUmaTexturaCutscene(cs);
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
        _GarantirTexturaQuadro(&((PaginaHQ *)pag)->quadros[i]);
        float alpha = (i == cs->quadrosVisiveis - 1) ? cs->fadeQuadro : 1.0f;
        DesenharQuadroCyberHQ(&pag->quadros[i], &assets->fontes, alpha, tempo);
    }

    // Legenda narrativa limpa na parte inferior (apenas a historia, sem tags de cena)
    int atualIdx = cs->quadrosVisiveis - 1;
    if (atualIdx >= 0 && atualIdx < pag->numQuadros) {
        const QuadroHQ *qAtual = &pag->quadros[atualIdx];
        Rectangle legBox = { 95, 560, 1090, 118 };
        DesenharPainelCyber(legBox, qAtual->corBorda);
        DesenharTextoQuebrado(assets->fontes.regular, qAtual->texto,
            (Rectangle){ legBox.x + 28, legBox.y + 24, legBox.width - 56, legBox.height - 44 }, 24, 1.0f, WHITE);
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
