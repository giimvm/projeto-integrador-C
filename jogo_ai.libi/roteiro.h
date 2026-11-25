#ifndef ROTEIRO_H
#define ROTEIRO_H

typedef struct {
    const char *falante;
    const char *texto;
    const char *fundo;
    const char *personagem;
    const char *audioPath;
} Fala;

// Cenarios
#define BG_INTERROGATORIO "AssetsRay/Locais/SalaInterrogatorio.png"
#define BG_EVIDENCIAS     "AssetsRay/Locais/SalaEvidencias.png"
#define BG_JUIZ           "AssetsRay/Locais/SalaJuiz.png"
#define BG_ESCRITORIO     "AssetsRay/Locais/escritorioplayer.png"
#define BG_MURAL          "AssetsRay/Locais/muralevidencia.png"
#define BG_SUSPEITOS      "AssetsRay/Locais/salasuspeitos.png"
#define BG_TRIBUNAL       "AssetsRay/Locais/tribunal.png"

// Poses Caio
#define CAIO_NEUTRO       "AssetsRay/Personagens/Caio/CaioNeutro.png"
#define CAIO_DESCONFIADO  "AssetsRay/Personagens/Caio/CaioDesconfiado.png"
#define CAIO_RELUTANTE    "AssetsRay/Personagens/Caio/CaioRelutante.png"
#define CAIO_NERVOSO      "AssetsRay/Personagens/Caio/CaioNervoso.png"
#define CAIO_ESTRESSADO   "AssetsRay/Personagens/Caio/CaioEstressado.png"
#define CAIO_OFENDIDO     "AssetsRay/Personagens/Caio/CaioOfendido.png"
#define CAIO_CONFIANTE    "AssetsRay/Personagens/Caio/CaioConfiante.png"
#define CAIO_ASSUSTADOR   "AssetsRay/Personagens/Caio/CaioAssustador.png"
#define CAIO_CORPO        "AssetsRay/Personagens/Caio/caiofullcorpo.png"

// Poses Delegado Meireles
#define DEL_NEUTRO        "AssetsRay/Personagens/Delegado/DelegadoNeutro.png"
#define DEL_ASSERTIVO     "AssetsRay/Personagens/Delegado/DelegadoAssertivo.png"
#define DEL_CONFIANTE     "AssetsRay/Personagens/Delegado/DelegadoConfiante.png"
#define DEL_EVID1         "AssetsRay/Personagens/Delegado/DelegadoEvidencia.png"
#define DEL_EVID2         "AssetsRay/Personagens/Delegado/DelegadoEvidencia2.png"

// Poses Juliana
#define JULIA_NEUTRO      "AssetsRay/Personagens/Julia/JuliaNeutro.png"
#define JULIA_CONFIANTE   "AssetsRay/Personagens/Julia/JuliaConfiante.png"
#define JULIA_CINICA      "AssetsRay/Personagens/Julia/JuliaCinica.png"
#define JULIA_CORPO       "AssetsRay/Personagens/Julia/julia_corpo.png"

// Poses Juiz e Advogada
#define JUIZ_NEUTRO       "AssetsRay/Personagens/Juiz/JuizNeutro.png"
#define JUIZ_ASSERTIVO    "AssetsRay/Personagens/Juiz/JuizAssertivo.png"
#define JUIZ_TRIBUNAL     "AssetsRay/Personagens/Juiz/juizinteiro.png"
#define ADVOGADA_SPRITE   "AssetsRay/Personagens/Advogada/AdvogadaNeutro.png"

// Pensamento interno nao usa sprite de personagem
#define SEM_SPRITE        ""

// ============================================================================
// CENA 1 - Abertura do Delegado + Pensamento no Escritorio
// ============================================================================
static const Fala falasCena1Abertura[] = {
    { "DELEGADO MEIRELES", "Acabou de entrar uma denúncia nova no sistema. Um universitário desapareceu depois de uma festa.", BG_EVIDENCIAS, DEL_NEUTRO, "src/audio/vozes/del_c1a_01_denuncia_nova.ogg" },
    { "DELEGADO MEIRELES", "A equipe já está ouvindo quem estava lá. Dê uma olhada nos dados iniciais no seu computador.", BG_EVIDENCIAS, DEL_ASSERTIVO, "src/audio/vozes/del_c1a_02_dados_iniciais.ogg" },
    { "DELEGADO MEIRELES", "E não se engane: hoje qualquer foto, voz ou vídeo pode ser fabricado. Não confie em prova só porque ela parece real.", BG_EVIDENCIAS, DEL_CONFIANTE, "src/audio/vozes/del_c1a_03_nao_se_engane.ogg" }
};
static const int NUM_FALAS_CENA1_ABERTURA = sizeof(falasCena1Abertura) / sizeof(falasCena1Abertura[0]);

static const Fala falasCena1[] = {
    { "PENSAMENTO", "(Recife, 2045. Olhando os registros e as manchetes de hoje... quem tem tecnologia vive bem. Mas qualquer foto, voz ou vídeo pode ser inventado em segundos.)", BG_ESCRITORIO, SEM_SPRITE }
};
static const int NUM_FALAS_CENA1 = sizeof(falasCena1) / sizeof(falasCena1[0]);

// ============================================================================
// CENA 2 - Interrogatorio de Caio
// ============================================================================
static const Fala falasCena2[] = {
    { "JOGADOR", "Caio, vou direto ao ponto: você sabe onde o Gabriel está?", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Não. Não faço a menor ideia.", BG_INTERROGATORIO, CAIO_NERVOSO, "src/audio/vozes/caio_c2_01_nao_faco_ideia.ogg" },
    { "JOGADOR", "Uma testemunha viu vocês dois batendo boca do lado de fora da festa. O que rolou?", BG_INTERROGATORIO, CAIO_DESCONFIADO },
    { "CAIO", "A gente discutiu, sim. Ele tava bêbado... falando que ia contar uma coisa pra Juliana.", BG_INTERROGATORIO, CAIO_ESTRESSADO, "src/audio/vozes/caio_c2_02_a_gente_discutiu.ogg" },
    { "CAIO", "Do nada começou a jogar na minha cara que eu pagava a faculdade dele pra ele me dever favor, e que a vaga dele na NEXARA só veio por indicação do meu pai. Eu falei que ele tava sendo injusto.", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "JOGADOR", "E depois da briga?", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "CAIO", "Fui embora. Era umas duas e quarenta. Peguei meu carro e fui direto pra casa, sozinho.", BG_INTERROGATORIO, CAIO_NERVOSO, "src/audio/vozes/caio_c2_03_fui_embora.ogg" },
    { "JOGADOR", "Então o seu álibi é só a sua palavra.", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "CAIO", "...É. Eu sei como isso soa. Meu relógio até ficou sem bateria na hora, nem registrou o trajeto.", BG_INTERROGATORIO, CAIO_OFENDIDO, "src/audio/vozes/caio_c2_04_sei_como_soa.ogg" },
    { "JOGADOR", "Tudo bem. Vamos puxar a câmera da saída do estacionamento. Se você saiu mesmo às duas e quarenta, ela vai mostrar.", BG_INTERROGATORIO, CAIO_NEUTRO }
};
static const int NUM_FALAS_CENA2 = sizeof(falasCena2) / sizeof(falasCena2[0]);

// ============================================================================
// CENA 3 - O pacote anonimo + reflexao no mural
// ============================================================================
static const Fala falasCena3[] = {
    { "DELEGADO MEIRELES", "Acabou de chegar numa entrega anônima. Tem um dispositivo com áudio aí dentro. Mandaram um bilhete dizendo que explica a briga entre o Caio e o Gabriel.", BG_EVIDENCIAS, DEL_EVID1, "src/audio/vozes/del_c3_01_entrega_anonima.ogg" },
    { "JOGADOR", "Quem grava uma briga no meio da festa e manda anonimamente pra polícia?", BG_EVIDENCIAS, DEL_EVID2 },
    { "DELEGADO MEIRELES", "Alguém que quer que a gente ouça. Ou alguém que precisa muito que a gente acredite.", BG_EVIDENCIAS, DEL_ASSERTIVO, "src/audio/vozes/del_c3_03_alguem_que_quer.ogg" }
};
static const int NUM_FALAS_CENA3 = sizeof(falasCena3) / sizeof(falasCena3[0]);

static const Fala falasCena3PosJuliana[] = {
    { "PENSAMENTO", "(Forjaram uma ameaça com a voz do Caio. Quem fez isso queria desviar o meu olhar pra ele... e eu preciso descobrir por quê.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA3_POS_JULIANA = sizeof(falasCena3PosJuliana) / sizeof(falasCena3PosJuliana[0]);

static const Fala falasCena3PosCaio[] = {
    { "PENSAMENTO", "(A gravação é limpa. O Caio mentiu quando disse que foi só uma discussão boba. Ele ameaçou o Gabriel de verdade.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA3_POS_CAIO = sizeof(falasCena3PosCaio) / sizeof(falasCena3PosCaio[0]);

// ============================================================================
// CENA 4 - Interrogatorio de Juliana + reflexao no mural
// ============================================================================
static const Fala falasCena4[] = {
    { "JULIANA", "Desculpa, esse celular trava o tempo todo. Comprei de segunda mão, sem garantia nem nada.", BG_INTERROGATORIO, JULIA_NEUTRO, "src/audio/vozes/ju_c4_01_celular_trava.ogg" },
    { "JOGADOR", "Você era próxima do Gabriel?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Ele é meu melhor amigo. A gente entrou junto na faculdade.", BG_INTERROGATORIO, JULIA_CONFIANTE, "src/audio/vozes/ju_c4_02_melhor_amigo.ogg" },
    { "JOGADOR", "Chegou a falar com ele na festa?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "A gente conversou rapidinho, depois eu voltei pra pista e não vi mais ele.", BG_INTERROGATORIO, JULIA_CINICA, "src/audio/vozes/ju_c4_03_conversou_rapidinho.ogg" },
    { "JOGADOR", "O Caio falou que o Gabriel queria te contar alguma coisa naquela noite. Vocês tinham brigado?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Brigamos, sim. Eu descobri que o Gabriel conseguiu a vaga na NEXARA porque tava fechado com o professor João Paulo. Eu ralei três anos por aquela vaga e saí de mãos abanando.", BG_INTERROGATORIO, JULIA_CINICA, "src/audio/vozes/ju_c4_04_brigamos_sim.ogg" },
    { "JOGADOR", "E o que você acha do Caio no meio disso tudo?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Pra quem sempre teve tudo de mão beijada, é muito fácil pedir pra gente \"ouvir o outro lado\".", BG_INTERROGATORIO, JULIA_CONFIANTE, "src/audio/vozes/ju_c4_05_mao_beijada.ogg" },
    { "JOGADOR", "Até que horas você ficou na festa?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Até umas três da manhã. Pedi um carro de aplicativo e fui embora. Até mandei uma foto pra minha mãe de dentro do carro. Pode olhar. Eu faço questão.", BG_INTERROGATORIO, JULIA_CINICA, "src/audio/vozes/ju_c4_06_tres_da_manha.ogg" }
};
static const int NUM_FALAS_CENA4 = sizeof(falasCena4) / sizeof(falasCena4[0]);

static const Fala falasCena4PosJuliana[] = {
    { "PENSAMENTO", "(Uma prova falsa pra queimar o Caio. Outra falsa pra proteger a Juliana. Alguém tá montando esse tabuleiro peça por peça.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA4_POS_JULIANA = sizeof(falasCena4PosJuliana) / sizeof(falasCena4PosJuliana[0]);

static const Fala falasCena4PosCaio[] = {
    { "PENSAMENTO", "(A foto é autêntica. Mesmo furiosa com o Gabriel, a Juliana realmente entrou no carro às três e cinco.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA4_POS_CAIO = sizeof(falasCena4PosCaio) / sizeof(falasCena4PosCaio[0]);

// ============================================================================
// CENA 5 - O corpo
// ============================================================================
static const Fala falasCena5[] = {
    { "DELEGADO MEIRELES", "Acharam o Gabriel. Infelizmente, encontraram o corpo dele num barranco, a duzentos metros do galpão.", BG_EVIDENCIAS, DEL_NEUTRO, "src/audio/vozes/del_c5_01_acharam_gabriel.ogg" },
    { "JOGADOR", "Praticamente do lado da festa?", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "DELEGADO MEIRELES", "A perícia cravou a morte na própria madrugada, entre 2h50 e 3h05. Ele levou um golpe na têmpora antes de cair. A partir de agora, estamos lidando com homicídio.", BG_EVIDENCIAS, DEL_ASSERTIVO, "src/audio/vozes/del_c5_03_pericia_cravou.ogg" }
};
static const int NUM_FALAS_CENA5 = sizeof(falasCena5) / sizeof(falasCena5[0]);

// ============================================================================
// CENA 6 - A Caixa-Preta / Backup na Nuvem + reflexao no mural
// ============================================================================
static const Fala falasCena6[] = {
    { "DELEGADO MEIRELES", "A perícia digital conseguiu resgatar o último backup automático da conta do Gabriel, minutos antes de o aparelho perder o sinal.", BG_EVIDENCIAS, DEL_EVID1, "src/audio/vozes/del_c6_01_backup_nuvem.ogg" },
    { "JOGADOR", "Se ele ia abrir o jogo com alguém naquela noite, tem alguma pista guardada aí.", BG_EVIDENCIAS, DEL_NEUTRO }
};
static const int NUM_FALAS_CENA6 = sizeof(falasCena6) / sizeof(falasCena6[0]);

static const Fala falasCena6PosJuliana[] = {
    { "PENSAMENTO", "(Ele entregou três anos de trabalho da Juliana de bandeja. Se ela soube disso na festa, tinha motivo de sobra pra perder a cabeça.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA6_POS_JULIANA = sizeof(falasCena6PosJuliana) / sizeof(falasCena6PosJuliana[0]);

static const Fala falasCena6PosCaio[] = {
    { "PENSAMENTO", "(Não era favor nenhum. O Caio usava o Gabriel de laranja pra lavar dinheiro e comprar a vaga na NEXARA. Se o Gabriel falasse, o Caio ia pra cadeia.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA6_POS_CAIO = sizeof(falasCena6PosCaio) / sizeof(falasCena6PosCaio[0]);

// ============================================================================
// CENA 7 - Confronto Final
// ============================================================================
static const Fala falasCena7A_Juliana[] = {
    { "JOGADOR", "Você garantiu que saiu da festa às três, de carro de aplicativo.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Garanti. E já te mostrei a foto.", BG_INTERROGATORIO, JULIA_CONFIANTE, "src/audio/vozes/ju_c7a_01_garanti.ogg" }
};
static const int NUM_FALAS_CENA7A_Juliana = sizeof(falasCena7A_Juliana) / sizeof(falasCena7A_Juliana[0]);

static const Fala falasCena7B_Juliana[] = {
    { "JOGADOR", "A sua corrida na Mobi só foi pedida às três e quarenta e um. E os carros da frota não têm volante, diferente da foto que você me mostrou.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Tá, eu fiquei mais um pouco... perdi a noção da hora.", BG_INTERROGATORIO, JULIA_CINICA, "src/audio/vozes/ju_c7b_01_fiquei_mais.ogg" },
    { "JOGADOR", "Quarenta minutos perdidos no relógio?", BG_INTERROGATORIO, JULIA_CINICA },
    { "JULIANA", "Eu tava mal, tá? Fiquei sentada lá fora esfriando a cabeça. O Gabriel saiu andando na direção da estrada.", BG_INTERROGATORIO, JULIA_CINICA, "src/audio/vozes/ju_c7b_02_tava_mal.ogg" },
    { "JOGADOR", "O celular dele tava parado a duzentos metros do galpão às 3h20. O corpo também. Ele nunca saiu de lá.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "...Eu quero um advogado.", BG_INTERROGATORIO, JULIA_NEUTRO, "src/audio/vozes/ju_c7b_03_quero_advogado.ogg" },
    { "PENSAMENTO", "(Quem fala a verdade para pra pensar. Ela já tava com a resposta pronta antes de eu terminar a pergunta.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA7B_Juliana = sizeof(falasCena7B_Juliana) / sizeof(falasCena7B_Juliana[0]);

static const Fala falasCena7A_Caio[] = {
    { "JOGADOR", "Você disse que saiu do estacionamento às duas e quarenta.", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Falei. Se vocês já puxaram a câmera da saída, já viram meu carro passando lá.", BG_INTERROGATORIO, CAIO_CONFIANTE, "src/audio/vozes/caio_c7a_01_falei.ogg" }
};
static const int NUM_FALAS_CENA7A_Caio = sizeof(falasCena7A_Caio) / sizeof(falasCena7A_Caio[0]);

static const Fala falasCena7B_Caio[] = {
    { "JOGADOR", "O horário no vídeo até bate com a sua história. Só tem um problema: a imagem da cancela foi adulterada.", BG_INTERROGATORIO, CAIO_DESCONFIADO },
    { "CAIO", "Adulterada? Aquela câmera vive quebrada. Deve ser erro de compressão.", BG_INTERROGATORIO, CAIO_OFENDIDO, "src/audio/vozes/caio_c7b_01_adulterada.ogg" },
    { "JOGADOR", "Pode até ser. Só que os repasses bancários na conta do Gabriel não parecem erro de sistema.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "Eu já falei... era uma ajuda que meu pai organizava pra ele se manter no curso. O Gabriel sempre gostou de fazer drama.", BG_INTERROGATORIO, CAIO_ESTRESSADO, "src/audio/vozes/caio_c7b_02_ja_falei_ajuda.ogg" },
    { "JOGADOR", "O problema é que o celular dele parou de dar sinal às 3h20, no fundo do barranco. Bem depois da briga de vocês.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "...Acho melhor eu chamar o advogado da família antes de falar mais alguma coisa.", BG_INTERROGATORIO, CAIO_RELUTANTE, "src/audio/vozes/caio_c7b_03_chamar_advogado.ogg" },
    { "PENSAMENTO", "(Quem fala a verdade para pra pensar. Ele já tava com a defesa engatilhada antes de eu terminar a frase.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_CENA7B_Caio = sizeof(falasCena7B_Caio) / sizeof(falasCena7B_Caio[0]);

// ============================================================================
// CENA 8 - Chamado para a Sala de Suspeitos
// ============================================================================
static const Fala falasCena8[] = {
    { "DELEGADO MEIRELES", "Chegamos ao fim da coleta. Os dois suspeitos estão na sala de reconhecimento. Revise seu mural e dê seu veredito.", BG_JUIZ, DEL_ASSERTIVO }
};
static const int NUM_FALAS_CENA8 = sizeof(falasCena8) / sizeof(falasCena8[0]);

// ============================================================================
// TRIBUNAL + FINAIS (Advogada defende o reu conforme qtd de provas)
// ============================================================================
// FINAL 1 - FINAL PERFEITO (Veredito correto + >=4 logicas + >=2 extras)
static const Fala falasFinalPerfeitoJuliana[] = {
    { "JUIZ", "Declaro aberta a audiência de instrução contra Juliana Costa. A defesa tem a palavra.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, em 2045 qualquer mídia pode ser manipulada. Pela Lei da Dúvida Digital, peço a anulação das provas eletrônicas contra minha cliente.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f1_ju_duvida_digital.ogg" },
    { "JOGADOR", "Doutora, aqui não tem brecha nenhuma. O áudio do Caio e a foto no carro são falsos, o GPS e a nuvem fecham o horário, e a corrida da Mobi às 3h41 derruba o álibi inteiro.", BG_TRIBUNAL, ADVOGADA_SPRITE },
    { "JULIANA", "Chega, para com isso! Eu ralei três anos por aquela vaga e ele entregou tudo de mão beijada pro João Paulo! Eu perdi a cabeça... quando vi, ele já tinha caído no barranco!", BG_TRIBUNAL, JULIA_CORPO, "src/audio/vozes/ju_f1_chega.ogg" },
    { "JUIZ", "Diante das cinco provas técnicas validadas e da confissão, condeno Juliana Costa e determino inquérito contra o Prof. João Paulo na NEXARA.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "PENSAMENTO", "(Em 2045, qualquer um fabrica uma mentira em segundos. Mas quando todas as peças fecham, nem a maior empresa da cidade segura a barra.)", BG_TRIBUNAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_PERF_JULIANA = sizeof(falasFinalPerfeitoJuliana) / sizeof(falasFinalPerfeitoJuliana[0]);

static const Fala falasFinalPerfeitoCaio[] = {
    { "JUIZ", "Declaro aberta a audiência de instrução contra Caio Vasconcelos. A defesa tem a palavra.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, meu cliente apresentou vídeo de saída às 02h42. O restante é suposição sobre ajuda financeira estudantil.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f1_caio_video_saida.ogg" },
    { "JOGADOR", "Aquele vídeo da cancela foi forjado por IA, Excelência. As cinco provas batem: a ameaça no áudio é real, o GPS crava 3h20 no barranco e a nuvem mostra o esquema de lavagem.", BG_TRIBUNAL, ADVOGADA_SPRITE },
    { "CAIO", "Ele ia acabar com a minha família! Tudo que meu pai construiu ia pro ralo! Eu só empurrei pra ele calar a boca... não sabia que tinha pedra lá embaixo!", BG_TRIBUNAL, CAIO_CORPO, "src/audio/vozes/caio_f1_foi_sem_querer.ogg" },
    { "JUIZ", "Com os cinco laudos periciais íntegros, condeno Caio Vasconcelos e determino o bloqueio da empresa de fachada ligada à NEXARA.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "PENSAMENTO", "(Em 2045, grana e algoritmo compram quase tudo. Menos um álibi que aguente cinco provas fechadas.)", BG_TRIBUNAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_PERF_CAIO = sizeof(falasFinalPerfeitoCaio) / sizeof(falasFinalPerfeitoCaio[0]);

// FINAL 2 - FINAL BOM (Veredito correto + >=3 logicas + >=2 extras)
static const Fala falasFinalVerdadeJuliana[] = {
    { "JUIZ", "Iniciamos a audiência contra Juliana Costa. Defesa, manifeste-se.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, falta uma peça na perícia completa da acusação. Há margem para questionar a ordem cronológica.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f2_ju_falta_peca.ogg" },
    { "JOGADOR", "Pode até faltar um laudo, mas os quatro que estão aqui fecham a conta: a foto no carro é falsa, a Mobi marca 3h41 e o GPS prova que o Gabriel nunca saiu do galpão.", BG_TRIBUNAL, ADVOGADA_SPRITE },
    { "JULIANA", "Ele ainda tentou pedir desculpa... eu gritei que era tarde demais. Bati nele e ele caiu... fiquei desesperada.", BG_TRIBUNAL, JULIA_CORPO, "src/audio/vozes/ju_f2_ele_chorou.ogg" },
    { "JUIZ", "As quatro provas convergem sem contradição. Juliana Costa está condenada pelo homicídio de Gabriel Moura.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "PENSAMENTO", "(Em 2045, qualquer um fabrica uma história bonita. Mas a gente ainda saca quando ela não fecha.)", BG_TRIBUNAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_VERD_JULIANA = sizeof(falasFinalVerdadeJuliana) / sizeof(falasFinalVerdadeJuliana[0]);

static const Fala falasFinalVerdadeCaio[] = {
    { "JUIZ", "Iniciamos a audiência contra Caio Vasconcelos. Defesa, manifeste-se.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, a acusação não fechou todos os cinco laudos do inquérito. Peço a rejeição da denúncia.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f2_caio_cinco_laudos.ogg" },
    { "JOGADOR", "Quatro laudos fechados já bastam pra derrubar esse álibi. Os extratos, o GPS e a falha no vídeo da cancela colocam o Caio lá atrás do galpão.", BG_TRIBUNAL, ADVOGADA_SPRITE },
    { "CAIO", "Ele ia jogar tudo no ventilador! Eu só empurrei pra calar a boca dele, não era pra acontecer aquilo!", BG_TRIBUNAL, CAIO_CORPO, "src/audio/vozes/caio_f2_ele_ia_acabar.ogg" },
    { "JUIZ", "Com quatro laudos periciais consistentes e a confissão em audiência, Caio Vasconcelos está condenado.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "PENSAMENTO", "(Em 2045, qualquer um fabrica uma história bonita. Mas a gente ainda saca quando ela não fecha.)", BG_TRIBUNAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_VERD_CAIO = sizeof(falasFinalVerdadeCaio) / sizeof(falasFinalVerdadeCaio[0]);

// FINAL 3 - INCONCLUSIVO (Veredito correto sem provas suficientes)
static const Fala falasFinalDuvida[] = {
    { "JUIZ", "A acusação possui provas técnicas suficientes que uma máquina não pudesse ter fabricado?", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, pela Lei da Dúvida Digital, qualquer indício sem cadeia de custódia completa deve ser anulado. Faltam laudos essenciais aqui, há apenas uma narrativa.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f3_cadeia_custodia.ogg" },
    { "JOGADOR", "...Os laudos que faltavam ficaram pelo caminho, Excelência.", BG_TRIBUNAL, ADVOGADA_SPRITE },
    { "JUIZ", "Sendo assim, em nome da dúvida razoável, declaro o réu absolvido.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "PENSAMENTO", "(Quatro meses depois, tava lá a notícia da nova liderança de IA na NEXARA. Em 2045, saber a verdade não basta, tem que provar sem deixar ponta solta.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_DUVIDA = sizeof(falasFinalDuvida) / sizeof(falasFinalDuvida[0]);

// FINAL 4 - INJUSTICA E AFASTAMENTO (Acusou o inocente sem provas suficientes para prender)
static const Fala falasFinalInjusticaAfastamento[] = {
    { "JUIZ", "A acusação denunciou um suspeito sem lastro pericial suficiente para sustentar a prisão.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, não há cadeia de custódia nem provas consistentes contra meu cliente! Peço a rejeição imediata da denúncia.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f4_sem_sustentacao.ogg" },
    { "DELEGADO MEIRELES", "Você apontou pra pessoa errada e ainda entrou no tribunal de mãos vazias. A corregedoria mandou te afastar na hora.", BG_TRIBUNAL, DEL_ASSERTIVO },
    { "JUIZ", "Denúncia rejeitada por falta de provas. O verdadeiro culpado pelo crime segue em liberdade.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "PENSAMENTO", "(Acusei quem não devia sem nem fechar as provas. Perdi o caso e deixei o culpado sair andando pela porta da frente.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_INJ_AFAST = sizeof(falasFinalInjusticaAfastamento) / sizeof(falasFinalInjusticaAfastamento[0]);

// FINAL 5 - A PRECIPITACAO
static const Fala falasFinalPrecipitado[] = {
    { "JOGADOR", "Delegado, já deu. Quero levar a acusação pro tribunal agora mesmo.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "DELEGADO MEIRELES", "Agora? A gente acabou de achar o corpo, bicho! Você nem puxou os registros direito nem apertou o suspeito!", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "DELEGADO MEIRELES", "Se você entrar lá assim, a defesa desmonta tudo em cinco minutos. Vou te tirar do caso antes que você afunde a investigação.", BG_EVIDENCIAS, DEL_CONFIANTE },
    { "PENSAMENTO", "(A pressa atropelou o método. Saí do caso antes mesmo de juntar as peças de verdade.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_PRECIPITADO = sizeof(falasFinalPrecipitado) / sizeof(falasFinalPrecipitado[0]);

// FINAL 6 - A INJUSTICA (acusou o inocente no Tribunal)
static const Fala falasFinalInjusticaCaio[] = {
    { "JUIZ", "A acusação sustenta a denúncia contra Caio Vasconcelos. A defesa tem a palavra.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, o álibi do meu cliente tem base sólida, e a acusação ignorou sinais claros de fabricação digital nas provas contra ele.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f6_caio_alibi_solido.ogg" },
    { "DELEGADO MEIRELES", "Motivo a cidade inteira tem. Você trouxe alguém pro banco dos réus sem fechar a prova técnica.", BG_TRIBUNAL, DEL_ASSERTIVO },
    { "PENSAMENTO", "(Semanas depois, o Caio ouviu a sentença injusta com o olhar vazio. Quando olhei de novo pra foto da Juliana no mural, caiu a ficha do tamanho do meu erro.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_INJ_CAIO = sizeof(falasFinalInjusticaCaio) / sizeof(falasFinalInjusticaCaio[0]);

static const Fala falasFinalInjusticaJuliana[] = {
    { "JUIZ", "A acusação sustenta a denúncia contra Juliana Costa. A defesa tem a palavra.", BG_TRIBUNAL, JUIZ_TRIBUNAL },
    { "ADVOGADA", "Excelência, a foto da minha cliente no carro é autêntica e o horário bate. A acusação trouxe ressentimento, não prova.", BG_TRIBUNAL, ADVOGADA_SPRITE, "src/audio/vozes/adv_f6_ju_foto_autentica.ogg" },
    { "DELEGADO MEIRELES", "Motivo a cidade inteira tem. Você trouxe alguém pro banco dos réus sem fechar a prova técnica.", BG_TRIBUNAL, DEL_ASSERTIVO },
    { "PENSAMENTO", "(Semanas depois, a Juliana ouviu a sentença injusta com o olhar vazio. Quando encarei a foto do Caio no mural, percebi tarde demais quem realmente me passou a perna.)", BG_MURAL, SEM_SPRITE }
};
static const int NUM_FALAS_FINAL_INJ_JULIANA = sizeof(falasFinalInjusticaJuliana) / sizeof(falasFinalInjusticaJuliana[0]);

#endif
