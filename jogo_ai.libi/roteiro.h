#ifndef ROTEIRO_H
#define ROTEIRO_H

typedef struct {
    const char *falante;
    const char *texto;
    const char *fundo;
    const char *personagem;
} Fala;

#define BG_INTERROGATORIO "AssetsRay/Locais/SalaInterrogatorio.png"
#define BG_EVIDENCIAS     "AssetsRay/Locais/SalaEvidencias.png"
#define BG_JUIZ           "AssetsRay/Locais/SalaJuiz.png"

// Poses Caio
#define CAIO_NEUTRO       "AssetsRay/Personagens/Caio/CaioNeutro.png"
#define CAIO_DESCONFIADO  "AssetsRay/Personagens/Caio/CaioDesconfiado.png"
#define CAIO_RELUTANTE    "AssetsRay/Personagens/Caio/CaioRelutante.png"
#define CAIO_NERVOSO      "AssetsRay/Personagens/Caio/CaioNervoso.png"
#define CAIO_ESTRESSADO   "AssetsRay/Personagens/Caio/CaioEstressado.png"
#define CAIO_OFENDIDO     "AssetsRay/Personagens/Caio/CaioOfendido.png"
#define CAIO_CONFIANTE    "AssetsRay/Personagens/Caio/CaioConfiante.png"

// Poses Delegado / Chefe
#define DEL_NEUTRO        "AssetsRay/Personagens/Delegado/DelegadoNeutro.png"
#define DEL_ASSERTIVO     "AssetsRay/Personagens/Delegado/DelegadoAssertivo.png"
#define DEL_CONFIANTE     "AssetsRay/Personagens/Delegado/DelegadoConfiante.png"
#define DEL_EVID1         "AssetsRay/Personagens/Delegado/DelegadoEvidencia.png"
#define DEL_EVID2         "AssetsRay/Personagens/Delegado/DelegadoEvidencia2.png"

// Poses Juliana
#define JULIA_NEUTRO      "AssetsRay/Personagens/Julia/JuliaNeutro.png"
#define JULIA_CONFIANTE   "AssetsRay/Personagens/Julia/JuliaConfiante.png"
#define JULIA_CINICA      "AssetsRay/Personagens/Julia/JuliaCinica.png"

// Poses Juiz / Policial
#define JUIZ_NEUTRO       "AssetsRay/Personagens/Juiz/JuizNeutro.png"
#define JUIZ_ASSERTIVO    "AssetsRay/Personagens/Juiz/JuizAssertivo.png"

// CENA 2: Primeiro interrogatório de Caio (C2.0 .. C2.7)
static const Fala falasCena2[] = {
    { "VOCÊ", "Caio, antes de começarmos, você sabe onde o Gabriel está?", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Não. Não faço ideia.", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "VOCÊ", "Uma testemunha afirmou que viu você discutindo com Gabriel do lado de fora da festa. O que aconteceu?", BG_INTERROGATORIO, CAIO_DESCONFIADO },
    { "CAIO", "A gente discutiu, sim. Prefiro não falar sobre isso.", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "VOCÊ", "E depois da discussão?", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "CAIO", "Eu fui embora. Eram mais ou menos 2h40.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "VOCÊ", "Entendido. Agora precisamos confirmar se o que você está dizendo é verdade.", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "VOCÊ", "Recebemos as imagens da câmera que registra a saída da festa. Elas podem mostrar se o que você está dizendo realmente aconteceu. Por enquanto, encerramos o depoimento.", BG_INTERROGATORIO, CAIO_NEUTRO }
};
static const int NUM_FALAS_CENA2 = sizeof(falasCena2) / sizeof(falasCena2[0]);

// RAMIFICAÇÃO: Acusação precoce de Caio (F1.1 .. F1.4)
static const Fala falasAcusaCaio[] = {
    { "VOCÊ", "Confirmo minha acusação contra Caio Vasconcelos.", BG_JUIZ, JUIZ_NEUTRO },
    { "DELEGADO", "Sua acusação está...", BG_JUIZ, JUIZ_NEUTRO },
    { "DELEGADO", "Incorreta!", BG_JUIZ, JUIZ_ASSERTIVO },
    { "DELEGADO", "Caio foi falsamente acusado, e as informações reunidas não justificavam responsabilizá-lo pelo desaparecimento de Gabriel.", BG_JUIZ, JUIZ_ASSERTIVO }
};
static const int NUM_FALAS_ACUSA_CAIO = sizeof(falasAcusaCaio) / sizeof(falasAcusaCaio[0]);

// CENA 3: Chefe entrega dispositivo anônimo (C3.1 .. C3.2)
static const Fala falasCena3[] = {
    { "CHEFE", "Investigadora, recebemos este dispositivo numa entrega anônima. Ele contém um vídeo da festa, com uma conversa gravada ao fundo.", BG_EVIDENCIAS, DEL_EVID1 },
    { "CHEFE", "Veio acompanhado de uma mensagem: o áudio revelaria o motivo da discussão entre Caio e Gabriel.", BG_EVIDENCIAS, DEL_EVID2 }
};
static const int NUM_FALAS_CENA3 = sizeof(falasCena3) / sizeof(falasCena3[0]);

// CENA 4: Caio revela o motivo da discussão (C4.1 .. C4.15)
static const Fala falasCena4[] = {
    { "VOCÊ", "Caio, eu preciso entender uma coisa. Por que você e Gabriel estavam discutindo naquela noite?", BG_INTERROGATORIO, CAIO_DESCONFIADO },
    { "CAIO", "Já falei... Prefiro não dizer.", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "VOCÊ", "O que aconteceu?", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "CAIO", "O Gabriel tinha descoberto uma coisa sobre a Juliana.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "Ele disse que ela tinha falsificado um trabalho importante da faculdade.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "VOCÊ", "Você estava tentando proteger a Juliana?", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "CAIO", "Eu só não queria que ele acabasse com a vida dela sem nem dar a chance de ela explicar.", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "VOCÊ", "E o Gabriel concordou com você?", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "CAIO", "Não. Ele ficou dizendo que ela tinha enganado todo mundo e que as pessoas precisavam saber.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "VOCÊ", "Você ameaçou o Gabriel?", BG_INTERROGATORIO, CAIO_OFENDIDO },
    { "CAIO", "Eu nunca faria uma coisa dessas.", BG_INTERROGATORIO, CAIO_OFENDIDO },
    { "VOCÊ", "Estou tentando descobrir o que aconteceu. E, para isso, preciso ouvir todos os lados. Inclusive o da Juliana.", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Você vai chamar ela?", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "VOCÊ", "Vou. Quero saber se ela vai contar a mesma história que você.", BG_INTERROGATORIO, CAIO_CONFIANTE }
};
static const int NUM_FALAS_CENA4 = sizeof(falasCena4) / sizeof(falasCena4[0]);

// CENA 5: Interrogatório de Juliana (C5.1 .. C5.10)
static const Fala falasCena5[] = {
    { "VOCÊ", "Juliana Costa?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Sim.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "VOCÊ", "Você conhecia Gabriel?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Sim. Nós estudávamos juntos.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "VOCÊ", "Você conversou com ele durante a festa?", BG_INTERROGATORIO, JULIA_CINICA },
    { "JULIANA", "Sim. Tivemos uma conversa normal. Depois eu voltei pra festa e não vi mais ele.", BG_INTERROGATORIO, JULIA_CINICA },
    { "VOCÊ", "Você ficou na festa até que horas?", BG_INTERROGATORIO, JULIA_CONFIANTE },
    { "JULIANA", "Até umas três da manhã, depois fui pra casa.", BG_INTERROGATORIO, JULIA_CONFIANTE },
    { "VOCÊ", "Você tem algo que possa provar isso? Uma testemunha ou foto?", BG_INTERROGATORIO, JULIA_CONFIANTE },
    { "JULIANA", "Eu tenho uma foto que mandei pra minha mãe que mostra o horário.", BG_INTERROGATORIO, JULIA_CINICA }
};
static const int NUM_FALAS_CENA5 = sizeof(falasCena5) / sizeof(falasCena5[0]);

// CENA 6 Parte 1: Chefe avisa sobre Gabriel (C6.1 .. C6.7)
static const Fala falasCena6A[] = {
    { "CHEFE", "Investigadora, encontramos o Gabriel.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "VOCÊ", "Encontraram? Onde ele está?", BG_EVIDENCIAS, DEL_NEUTRO },
    { "CHEFE", "Encontraram o corpo dele em um barranco, às margens de uma estrada.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "VOCÊ", "Meu Deus...", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "CHEFE", "A equipe já está no local. A perícia também foi acionada. Existem indícios de que não foi uma morte acidental.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "CHEFE", "A partir de agora, o caso passa a ser tratado como homicídio.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "CHEFE", "E tenha cuidado com as conclusões. Uma evidência falsa pode mudar completamente o rumo da investigação.", BG_EVIDENCIAS, DEL_CONFIANTE }
};
static const int NUM_FALAS_CENA6A = sizeof(falasCena6A) / sizeof(falasCena6A[0]);

// CENA 6 Parte 2: Algumas horas depois (C6.9 .. C6.12)
static const Fala falasCena6B[] = {
    { "POLICIAL", "Investigadora, a perícia enviou novos arquivos para o seu computador. Eles podem ajudar na investigação.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "VOCÊ", "O que encontraram?", BG_EVIDENCIAS, DEL_NEUTRO },
    { "POLICIAL", "Um registro de localização do celular de Gabriel e uma fotografia recuperada do aparelho dele. Ainda precisamos confirmar a autenticidade dos arquivos.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "POLICIAL", "O chefe pediu atenção especial aos horários. Talvez eles ajudem a reconstruir os últimos momentos de Gabriel.", BG_EVIDENCIAS, DEL_ASSERTIVO }
};
static const int NUM_FALAS_CENA6B = sizeof(falasCena6B) / sizeof(falasCena6B[0]);

// CENA 7: Foto recuperada (C7.1 .. C7.2)
static const Fala falasCena7[] = {
    { "CHEFE", "A perícia conseguiu recuperar uma fotografia que Gabriel havia recebido naquela noite.", BG_EVIDENCIAS, DEL_EVID1 },
    { "VOCÊ", "Uma fotografia recebida por Gabriel pouco antes de desaparecer? Vamos ver.", BG_EVIDENCIAS, DEL_NEUTRO }
};
static const int NUM_FALAS_CENA7 = sizeof(falasCena7) / sizeof(falasCena7[0]);

// CENA 8: Decisão final (C8.2 .. C8.4)
static const Fala falasCena8[] = {
    { "DELEGADO", "Você chegou ao fim da investigação. Agora, precisa apresentar seu veredito. Mas pense bem: uma suspeita não é o mesmo que uma prova, e uma história convincente nem sempre é verdadeira.", BG_JUIZ, JUIZ_NEUTRO },
    { "VOCÊ", "Entendido. Vou considerar tudo o que consegui descobrir antes de tomar minha decisão.", BG_JUIZ, JUIZ_NEUTRO },
    { "DELEGADO", "Então, investigadora, quem você acredita ser responsável pela morte de Gabriel Moura?", BG_JUIZ, JUIZ_ASSERTIVO }
};
static const int NUM_FALAS_CENA8 = sizeof(falasCena8) / sizeof(falasCena8[0]);

#endif
