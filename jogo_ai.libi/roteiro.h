#ifndef ROTEIRO_H
#define ROTEIRO_H

typedef struct {
    const char *falante;
    const char *texto;
    const char *fundo;
    const char *personagem;
} Fala;

// Cenarios existentes
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
#define CAIO_ASSUSTADOR   "AssetsRay/Personagens/Caio/CaioAssustador.png"

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

// Poses Juiz
#define JUIZ_NEUTRO       "AssetsRay/Personagens/Juiz/JuizNeutro.png"
#define JUIZ_ASSERTIVO    "AssetsRay/Personagens/Juiz/JuizAssertivo.png"

// TODO: Assets faltando - sprite da protagonista HELENA e do(a) ADVOGADO(A)
#define HELENA_SPRITE     ""
#define ADVOGADO_SPRITE   JUIZ_NEUTRO

// ============================================================================
// CENA 1 - A ocorrencia (comum as duas rotas)
// ============================================================================
static const Fala falasCena1[] = {
    { "HELENA", "Recife, 2045. Quem tem tecnologia vive bem. Mas hoje qualquer foto, voz ou vídeo pode ser fabricado em segundos.", BG_EVIDENCIAS, HELENA_SPRITE },
    { "DELEGADO MEIRELES", "Helena, a equipe já está ouvindo quem estava na festa. Passe um pente fino nos álibis, nas imagens e nas mensagens.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "DELEGADO MEIRELES", "E não se engane: não é porque uma prova parece real que ela realmente é.", BG_EVIDENCIAS, DEL_ASSERTIVO }
};
static const int NUM_FALAS_CENA1 = sizeof(falasCena1) / sizeof(falasCena1[0]);

// ============================================================================
// CENA 2 - Interrogatorio de Caio (comum as duas rotas)
// ============================================================================
static const Fala falasCena2[] = {
    { "HELENA", "Caio, vou direto ao ponto: você sabe onde o Gabriel está?", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Não. Não faço a menor ideia.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "HELENA", "Uma testemunha viu vocês dois batendo boca do lado de fora da festa. O que rolou?", BG_INTERROGATORIO, CAIO_DESCONFIADO },
    { "CAIO", "A gente discutiu, sim. Ele tava bêbado... falando que ia contar uma coisa pra Juliana. Do nada começou a jogar na minha cara que eu pagava a faculdade dele pra ele me dever favor, e que a vaga dele na NEXARA só veio por indicação do meu pai. Eu falei que ele tava sendo injusto.", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "HELENA", "E depois da briga?", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "CAIO", "Fui embora. Era umas duas e quarenta. Peguei meu carro e fui direto pra casa, sozinho.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "HELENA", "Então o seu álibi é só a sua palavra.", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "CAIO", "...É. Eu sei como isso soa. Meu relógio até ficou sem bateria na hora, nem registrou o trajeto.", BG_INTERROGATORIO, CAIO_OFENDIDO },
    { "HELENA", "Tudo bem. Vamos puxar a câmera da saída do estacionamento. Se você saiu mesmo às duas e quarenta, ela vai mostrar.", BG_INTERROGATORIO, CAIO_NEUTRO }
};
static const int NUM_FALAS_CENA2 = sizeof(falasCena2) / sizeof(falasCena2[0]);

// ============================================================================
// CENA 3 - O pacote anonimo (comum + pos-analise bifurcada)
// ============================================================================
static const Fala falasCena3[] = {
    { "DELEGADO MEIRELES", "Acabou de chegar numa entrega anônima. Tem um dispositivo com áudio aí dentro. Mandaram um bilhete dizendo que explica a briga entre o Caio e o Gabriel.", BG_EVIDENCIAS, DEL_EVID1 },
    { "HELENA", "Quem grava uma briga no meio da festa e manda anonimamente pra polícia?", BG_EVIDENCIAS, DEL_EVID2 },
    { "DELEGADO MEIRELES", "Alguém que quer que a gente ouça. Ou alguém que precisa muito que a gente acredite.", BG_EVIDENCIAS, DEL_ASSERTIVO }
};
static const int NUM_FALAS_CENA3 = sizeof(falasCena3) / sizeof(falasCena3[0]);

static const Fala falasCena3PosJuliana[] = {
    { "HELENA", "Forjaram uma ameaça com a voz do Caio. Quem fez isso queria desviar o meu olhar pra ele... e eu preciso descobrir por quê.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA3_POS_JULIANA = sizeof(falasCena3PosJuliana) / sizeof(falasCena3PosJuliana[0]);

static const Fala falasCena3PosCaio[] = {
    { "HELENA", "A gravação é limpa. O Caio mentiu quando disse que foi só uma discussão boba. Ele ameaçou o Gabriel de verdade.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA3_POS_CAIO = sizeof(falasCena3PosCaio) / sizeof(falasCena3PosCaio[0]);

// ============================================================================
// CENA 4 - Interrogatorio de Juliana (comum + pos-analise bifurcada)
// ============================================================================
static const Fala falasCena4[] = {
    { "JULIANA", "Desculpa, esse celular trava o tempo todo. Comprei de segunda mão, sem garantia nem nada.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "HELENA", "Você era próxima do Gabriel?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Ele é meu melhor amigo. A gente entrou junto na faculdade.", BG_INTERROGATORIO, JULIA_CONFIANTE },
    { "HELENA", "Chegou a falar com ele na festa?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "A gente conversou rapidinho, depois eu voltei pra pista e não vi mais ele.", BG_INTERROGATORIO, JULIA_CINICA },
    { "HELENA", "O Caio falou que o Gabriel queria te contar alguma coisa naquela noite. Vocês tinham brigado?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Brigamos, sim. Eu descobri que o Gabriel conseguiu a vaga na NEXARA porque tava fechado com o professor João Paulo. Eu ralei três anos por aquela vaga e saí de mãos abanando.", BG_INTERROGATORIO, JULIA_CINICA },
    { "HELENA", "E o que você acha do Caio no meio disso tudo?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Pra quem sempre teve tudo de mão beijada, é muito fácil pedir pra gente \"ouvir o outro lado\".", BG_INTERROGATORIO, JULIA_CONFIANTE },
    { "HELENA", "Até que horas você ficou na festa?", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Até umas três da manhã. Pedi um carro de aplicativo e fui embora. Até mandei uma foto pra minha mãe de dentro do carro. Pode olhar. Eu faço questão.", BG_INTERROGATORIO, JULIA_CINICA }
};
static const int NUM_FALAS_CENA4 = sizeof(falasCena4) / sizeof(falasCena4[0]);

static const Fala falasCena4PosJuliana[] = {
    { "HELENA", "Uma prova falsa pra queimar o Caio. Outra falsa pra proteger a Juliana. Alguém tá montando esse tabuleiro peça por peça.", BG_INTERROGATORIO, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA4_POS_JULIANA = sizeof(falasCena4PosJuliana) / sizeof(falasCena4PosJuliana[0]);

static const Fala falasCena4PosCaio[] = {
    { "HELENA", "A foto é autêntica. Mesmo furiosa com o Gabriel, a Juliana realmente entrou no carro às três e cinco.", BG_INTERROGATORIO, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA4_POS_CAIO = sizeof(falasCena4PosCaio) / sizeof(falasCena4PosCaio[0]);

// ============================================================================
// CENA 5 - O corpo (comum as duas rotas)
// ============================================================================
static const Fala falasCena5[] = {
    { "DELEGADO MEIRELES", "Helena... acharam o Gabriel. Infelizmente, encontraram o corpo dele num barranco, a duzentos metros do galpão.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "HELENA", "Praticamente do lado da festa?", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "DELEGADO MEIRELES", "A perícia cravou a morte na própria madrugada, entre 2h50 e 3h05. Ele levou um golpe na têmpora antes de cair. A partir de agora, estamos lidando com homicídio.", BG_EVIDENCIAS, DEL_ASSERTIVO }
};
static const int NUM_FALAS_CENA5 = sizeof(falasCena5) / sizeof(falasCena5[0]);

// ============================================================================
// CENA 6 - A Caixa-Preta / Backup na Nuvem (comum + pos-analise bifurcada)
// ============================================================================
static const Fala falasCena6[] = {
    { "DELEGADO MEIRELES", "A perícia digital conseguiu resgatar o último backup automático da conta do Gabriel, minutos antes de o aparelho perder o sinal.", BG_EVIDENCIAS, DEL_EVID1 },
    { "HELENA", "Se ele ia abrir o jogo com alguém naquela noite, tem alguma pista guardada aí.", BG_EVIDENCIAS, DEL_NEUTRO }
};
static const int NUM_FALAS_CENA6 = sizeof(falasCena6) / sizeof(falasCena6[0]);

static const Fala falasCena6PosJuliana[] = {
    { "HELENA", "Ele entregou três anos de trabalho da Juliana de bandeja. Se ela soube disso na festa, tinha motivo de sobra pra perder a cabeça.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA6_POS_JULIANA = sizeof(falasCena6PosJuliana) / sizeof(falasCena6PosJuliana[0]);

static const Fala falasCena6PosCaio[] = {
    { "HELENA", "Não era favor nenhum. O Caio usava o Gabriel de laranja pra lavar dinheiro e comprar a vaga na NEXARA. Se o Gabriel falasse, o Caio ia pra cadeia.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA6_POS_CAIO = sizeof(falasCena6PosCaio) / sizeof(falasCena6PosCaio[0]);

// ============================================================================
// CENA 7 - Confronto Final (Bifurcado: Rota Juliana vs. Rota Caio)
// ============================================================================
static const Fala falasCena7A_Juliana[] = {
    { "HELENA", "Você garantiu que saiu da festa às três, de carro de aplicativo.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Garanti. E já te mostrei a foto.", BG_INTERROGATORIO, JULIA_CONFIANTE }
};
static const int NUM_FALAS_CENA7A_JULIANA = sizeof(falasCena7A_Juliana) / sizeof(falasCena7A_Juliana[0]);

static const Fala falasCena7B_Juliana[] = {
    { "HELENA", "A sua corrida na Mobi só foi pedida às três e quarenta e um. E os carros da frota não têm volante, diferente da foto que você me mostrou.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Tá, eu fiquei mais um pouco... perdi a noção da hora.", BG_INTERROGATORIO, JULIA_CINICA },
    { "HELENA", "Quarenta minutos perdidos no relógio?", BG_INTERROGATORIO, JULIA_CINICA },
    { "JULIANA", "Eu tava mal, tá? Fiquei sentada lá fora esfriando a cabeça. O Gabriel saiu andando na direção da estrada.", BG_INTERROGATORIO, JULIA_CINICA },
    { "HELENA", "O celular dele tava parado a duzentos metros do galpão às 3h20. O corpo também. Ele nunca saiu de lá.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "...Eu quero um advogado.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "HELENA", "Quem fala a verdade para pra pensar. Ela já tava com a resposta pronta antes de eu terminar a pergunta.", BG_INTERROGATORIO, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA7B_JULIANA = sizeof(falasCena7B_Juliana) / sizeof(falasCena7B_Juliana[0]);

static const Fala falasCena7A_Caio[] = {
    { "HELENA", "Você disse que saiu do estacionamento às duas e quarenta.", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Falei. Se vocês já puxaram a câmera da saída, já viram meu carro passando lá.", BG_INTERROGATORIO, CAIO_CONFIANTE }
};
static const int NUM_FALAS_CENA7A_CAIO = sizeof(falasCena7A_Caio) / sizeof(falasCena7A_Caio[0]);

static const Fala falasCena7B_Caio[] = {
    { "HELENA", "O horário no vídeo até bate com a sua história. Só tem um problema: a imagem da cancela foi adulterada.", BG_INTERROGATORIO, CAIO_DESCONFIADO },
    { "CAIO", "Adulterada? Aquela câmera vive quebrada, Helena. Deve ser erro de compressão.", BG_INTERROGATORIO, CAIO_OFENDIDO },
    { "HELENA", "Pode até ser. Só que os repasses bancários na conta do Gabriel não parecem erro de sistema.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "Eu já falei... era uma ajuda que meu pai organizava pra ele se manter no curso. O Gabriel sempre gostou de fazer drama.", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "HELENA", "O problema é que o celular dele parou de dar sinal às 3h20, no fundo do barranco. Bem depois da briga de vocês.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "...Acho melhor eu chamar o advogado da família antes de falar mais alguma coisa.", BG_INTERROGATORIO, CAIO_RELUTANTE },
    { "HELENA", "Quem fala a verdade para pra pensar. Ele já tava com a defesa engatilhada antes de eu terminar a frase.", BG_INTERROGATORIO, HELENA_SPRITE }
};
static const int NUM_FALAS_CENA7B_CAIO = sizeof(falasCena7B_Caio) / sizeof(falasCena7B_Caio[0]);

// ============================================================================
// CENA 8 - Veredito (comum as duas rotas)
// ============================================================================
static const Fala falasCena8[] = {
    { "DELEGADO MEIRELES", "Chegamos ao fim, Helena. Suspeita não condena ninguém, e uma boa história nem sempre é verdade. Quem matou Gabriel Moura?", BG_JUIZ, DEL_ASSERTIVO }
};
static const int NUM_FALAS_CENA8 = sizeof(falasCena8) / sizeof(falasCena8[0]);

// ============================================================================
// FINAL 1 - VERDADE ABSOLUTA (5/5 provas validadas - Caso Perfeito)
// ============================================================================
static const Fala falasFinalPerfeitoJuliana[] = {
    { "HELENA", "O Gabriel te chamou nos fundos do galpão pra admitir que entregou o seu projeto ao professor. Vocês brigaram. Depois você voltou, gerou uma foto falsa no carro e, no dia seguinte, forjou um áudio com a voz do Caio.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Ele chorou, sabe? Pediu desculpa, disse que ia denunciar o João Paulo... Eu gritei que era tarde demais. Ele segurou meu braço e falou \"me escuta, Ju\". Eu só queria que alguém me escutasse primeiro.", BG_INTERROGATORIO, JULIA_CINICA },
    { "JULIANA", "Eu bati. Ele caiu no barranco. Fiquei paralisada olhando... e o pânico tomou conta.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "HELENA", "Juliana Costa, você está presa pelo homicídio de Gabriel Moura.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "DELEGADO MEIRELES", "Trabalho impecável, Helena. Com todas as provas validadas, o Ministério Público também abriu inquérito contra o professor João Paulo e o processo seletivo da NEXARA.", BG_EVIDENCIAS, DEL_CONFIANTE },
    { "HELENA", "Em 2045, qualquer um pode fabricar uma mentira. Mas quando cada peça fecha sem brecha, nem a maior empresa da cidade consegue esconder a verdade.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_PERF_JULIANA = sizeof(falasFinalPerfeitoJuliana) / sizeof(falasFinalPerfeitoJuliana[0]);

static const Fala falasFinalPerfeitoCaio[] = {
    { "HELENA", "O Gabriel descobriu que tava sendo usado de laranja no esquema do seu pai e ia entregar tudo naquela noite. Você foi atrás dele até o barranco e depois usou a tecnologia da NEXARA pra forjar sua saída na câmera.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "Ele ia acabar com a minha família, Helena! Tudo que o meu pai construiu ia pro lixo por causa de um cara que eu ajudei! Eu só empurrei pra calar a boca dele... eu não sabia que tinha uma pedra lá embaixo!", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "HELENA", "Caio Vasconcelos, você está preso pelo homicídio de Gabriel Moura.", BG_INTERROGATORIO, CAIO_ASSUSTADOR },
    { "DELEGADO MEIRELES", "Excelente trabalho, Helena. A perícia fechou os cinco laudos sem nenhuma falha. Os extratos da nuvem também derrubaram a empresa de fachada e o esquema do pai dele na NEXARA.", BG_EVIDENCIAS, DEL_CONFIANTE },
    { "HELENA", "Em 2045, dinheiro e algoritmo compram quase tudo. Menos um álibi que resista a cinco provas fechadas.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_PERF_CAIO = sizeof(falasFinalPerfeitoCaio) / sizeof(falasFinalPerfeitoCaio[0]);

// ============================================================================
// FINAL 2 - A VERDADE (4/5 provas validadas)
// ============================================================================
static const Fala falasFinalVerdadeJuliana[] = {
    { "HELENA", "O Gabriel te chamou nos fundos do galpão pra contar que entregou o seu projeto ao professor. Vocês discutiram. Depois você voltou pra festa, montou uma foto falsa e forjou o áudio com a voz do Caio.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "JULIANA", "Ele chorou, sabe? Pediu desculpa, disse que ia acusar o João Paulo. Eu gritei que era tarde demais. Ele me segurou, falou \"me escuta, Ju\"... e eu só queria que alguém me escutasse primeiro.", BG_INTERROGATORIO, JULIA_CINICA },
    { "JULIANA", "Eu bati. Ele caiu. Fiquei olhando o buraco e me deu um medo absurdo.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "HELENA", "Juliana Costa, você está presa pelo homicídio de Gabriel Moura.", BG_INTERROGATORIO, JULIA_NEUTRO },
    { "HELENA", "Em 2045, qualquer um pode fabricar uma verdade. Mas a gente ainda percebe quando ela não encaixa.", BG_INTERROGATORIO, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_VERD_JULIANA = sizeof(falasFinalVerdadeJuliana) / sizeof(falasFinalVerdadeJuliana[0]);

static const Fala falasFinalVerdadeCaio[] = {
    { "HELENA", "O Gabriel descobriu que era laranja no esquema do seu pai e ia denunciar tudo naquela noite. Você seguiu ele até o barranco e depois forjou a gravação do estacionamento.", BG_INTERROGATORIO, CAIO_NERVOSO },
    { "CAIO", "Ele ia destruir a minha família, Helena! Tudo que o meu pai construiu ia virar pó por causa de um moleque que eu ajudei! Eu só empurrei pra calar a boca dele... não sabia que tinha uma pedra ali embaixo.", BG_INTERROGATORIO, CAIO_ESTRESSADO },
    { "HELENA", "Caio Vasconcelos, você está preso pelo homicídio de Gabriel Moura.", BG_INTERROGATORIO, CAIO_ASSUSTADOR },
    { "HELENA", "Em 2045, qualquer um pode fabricar uma verdade. Mas a gente ainda percebe quando ela não encaixa.", BG_INTERROGATORIO, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_VERD_CAIO = sizeof(falasFinalVerdadeCaio) / sizeof(falasFinalVerdadeCaio[0]);

// ============================================================================
// FINAL 3 - A DUVIDA (2 ou 3 provas validadas - absolvicao no tribunal)
// ============================================================================
static const Fala falasFinalDuvida[] = {
    { "JUIZ", "A acusação possui alguma prova técnica que uma máquina não pudesse ter fabricado?", BG_JUIZ, JUIZ_NEUTRO },
    { "ADVOGADO", "Excelência, pela Lei da Dúvida Digital, qualquer indício sem cadeia de custódia completa deve ser anulado. A acusação trouxe uma suspeita e uma narrativa. Nada além disso.", BG_JUIZ, ADVOGADO_SPRITE },
    { "HELENA", "...Não, Excelência. Ainda não.", BG_JUIZ, JUIZ_NEUTRO },
    { "JUIZ", "Sendo assim, em nome da dúvida razoável, declaro a absolvição.", BG_JUIZ, JUIZ_ASSERTIVO },
    { "HELENA", "Quatro meses depois, a NEXARA anunciou sua nova liderança de Engenharia de IA. Em 2045, a verdade sozinha não basta. Ela precisa chegar com a prova certa, na hora certa.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_DUVIDA = sizeof(falasFinalDuvida) / sizeof(falasFinalDuvida[0]);

// ============================================================================
// FINAL 4 - COLAPSO FORENSE (0 ou 1 prova validada - arquivamento antes do juri)
// ============================================================================
static const Fala falasFinalColapso[] = {
    { "DELEGADO MEIRELES", "Helena, eu olhei o seu dossiê inteiro. Praticamente não temos provas validadas pela perícia.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "HELENA", "Mas eu sei quem foi, Delegado. A história bate.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "DELEGADO MEIRELES", "Se eu mandar esse inquérito pro Ministério Público nesse estado, eles devolvem em dez minutos. Sem laudo válido, não existe processo.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "HELENA", "O inquérito foi arquivado por falta de lastro técnico. A pista tava na minha frente, e eu deixei escapar pelos dedos.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_COLAPSO = sizeof(falasFinalColapso) / sizeof(falasFinalColapso[0]);

// ============================================================================
// FINAL 5 - A PRECIPITACAO (acusacao precoce nas Cenas 5 ou 6)
// ============================================================================
static const Fala falasFinalPrecipitado[] = {
    { "HELENA", "Delegado, não preciso de mais nada. Quero formalizar a acusação agora.", BG_EVIDENCIAS, DEL_NEUTRO },
    { "DELEGADO MEIRELES", "Agora? A gente acabou de achar o corpo, Helena! Você nem puxou todos os registros nem confrontou o suspeito!", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "HELENA", "Se a gente esperar mais, vão sumir com o resto das provas.", BG_EVIDENCIAS, DEL_ASSERTIVO },
    { "DELEGADO MEIRELES", "Uma acusação precipitada destrói a cadeia de custódia e abre brecha pra anulação imediata. Vou te afastar do caso antes que você coloque tudo a perder.", BG_EVIDENCIAS, DEL_CONFIANTE },
    { "HELENA", "A pressa falou mais alto que o método. Saí do caso antes mesmo de descobrir a verdade inteira.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_PRECIPITADO = sizeof(falasFinalPrecipitado) / sizeof(falasFinalPrecipitado[0]);

// ============================================================================
// FINAL 6 - A INJUSTICA (acusou o inocente no Veredito)
// ============================================================================
static const Fala falasFinalInjusticaCaio[] = {
    { "HELENA", "Confirmo minha acusação contra Caio Vasconcelos.", BG_JUIZ, DEL_NEUTRO },
    { "DELEGADO MEIRELES", "O álibi dele parava de pé, o áudio tinha sinais de manipulação... o que a gente tinha de concreto contra ele?", BG_JUIZ, DEL_ASSERTIVO },
    { "HELENA", "Motivo. A dívida, o pai, a briga na festa.", BG_JUIZ, DEL_ASSERTIVO },
    { "DELEGADO MEIRELES", "Motivo a cidade inteira tem, Helena. Prova, não.", BG_JUIZ, DEL_CONFIANTE },
    { "HELENA", "Semanas depois, Caio ouviu a sentença com o olhar vazio. Na delegacia, olhei a foto da Juliana na parede e percebi o tamanho do meu erro.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_INJ_CAIO = sizeof(falasFinalInjusticaCaio) / sizeof(falasFinalInjusticaCaio[0]);

static const Fala falasFinalInjusticaJuliana[] = {
    { "HELENA", "Confirmo minha acusação contra Juliana Costa.", BG_JUIZ, DEL_NEUTRO },
    { "DELEGADO MEIRELES", "A foto dela no carro era autêntica, o álibi fechava... o que a gente tinha de concreto contra ela?", BG_JUIZ, DEL_ASSERTIVO },
    { "HELENA", "Motivo. A vaga na NEXARA, o ressentimento.", BG_JUIZ, DEL_ASSERTIVO },
    { "DELEGADO MEIRELES", "Motivo a cidade inteira tem, Helena. Prova, não.", BG_JUIZ, DEL_CONFIANTE },
    { "HELENA", "Semanas depois, Juliana ouviu a sentença com o olhar vazio. Na delegacia, encarei a foto do Caio na parede e percebi tarde demais quem realmente me enganou.", BG_EVIDENCIAS, HELENA_SPRITE }
};
static const int NUM_FALAS_FINAL_INJ_JULIANA = sizeof(falasFinalInjusticaJuliana) / sizeof(falasFinalInjusticaJuliana[0]);

#endif
