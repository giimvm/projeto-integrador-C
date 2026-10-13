#include <stdio.h>
#include <stdlib.h>

#define TOTAL_EVIDENCIAS 6
#define CULPADA_REAL 2   // 1 = Caio, 2 = Juliana

int pontos = 0;           // pontuação do jogador
int erros = 0;            // desafios de logica errados
int pausasLigadas = 1;    // 1 = espera ENTER nas telas
int abandonou = 0;        // 1 = jogador voltou ao menu no meio do caso
int finalAntecipado = 0;  // 1 = jogador acusou Caio na cena 2 (final sem solucao)

/* estado de cada evidencia: 0 = bloqueada, 1 = perdida,
 2 = marcada como autentica, 3 = marcada como IA */
int evidencia1 = 0, evidencia2 = 0, evidencia3 = 0;
int evidencia4 = 0, evidencia5 = 0, evidencia6 = 0;

void limparTela() { //pra limpar a tela do terminal após uma execução
#ifdef _WIN32
    system("cls"); //pra windows
#else
    system("clear"); //pra linux/mac
#endif
}

// Descarta os caracteres restantes da entrada até encontrar ENTER ou EOF.
void limparBuffer() {
    int c;
    do {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

// Limpa o buffer somente quando as pausas estão ativadas.
void esperarEnter() {
    if (pausasLigadas == 1) {
        limparBuffer();
    }
}

// Exibe uma mensagem e aguarda a limpeza da entrada para continuar.
void pausar() {
    if (pausasLigadas == 1) {
        printf("\n[ENTER para continuar]");
        limparBuffer();
    }
}

//Lê um número entre mínimo e máximo e repete até ser válido
int lerOpcao(int minimo, int maximo) {
    int numero = 0;
    int lidos;
    int valido = 0;

    while (valido == 0) {
        lidos = scanf("%d", &numero);
        if (lidos == EOF) {
            exit(0); // Encerra o programa se não houver mais entrada.
        }
        limparBuffer();
        if (lidos == 1 && numero >= minimo && numero <= maximo) {
            valido = 1;
        } else {
            printf("Entrada invalida! Digite um numero de %d a %d: ", minimo, maximo);
        }
    }
    return numero;
}

// Retorna o estado da evidência correspondente ao ID informado.
int pegarEstado(int id) {
    switch (id) {
        case 1: return evidencia1;
        case 2: return evidencia2;
        case 3: return evidencia3;
        case 4: return evidencia4;
        case 5: return evidencia5;
        default: return evidencia6; //"else" do switch case
    }
}

// Atualiza o estado da evidência escolhida durante a investigação.
void mudarEstado(int id, int estado) {
    switch (id) {
        case 1: evidencia1 = estado; break;
        case 2: evidencia2 = estado; break;
        case 3: evidencia3 = estado; break;
        case 4: evidencia4 = estado; break;
        case 5: evidencia5 = estado; break;
        default: evidencia6 = estado; break;
    }
}

// Define quais evidências foram criadas por IA no gabarito do jogo. 
// Evidências 2, 4 e 6 são consideradas geradas por IA.
int ehFalsa(int id) {
    return (id == 2 || id == 4 || id == 6);
}

// Exibe o título correspondente ao número da evidência selecionada.
void mostrarTitulo(int id) {
    switch (id) {
        case 1: printf("Camera de seguranca do estacionamento"); break;
        case 2: printf("Audio da discussao"); break;
        case 3: printf("Mensagens de Caio e Gabriel"); break;
        case 4: printf("Foto de Juliana no carro"); break;
        case 5: printf("Registro de localizacao do celular de Gabriel"); break;
        case 6: printf("Fotografia recebida por Gabriel"); break;
    }
}

// Exibe o conteúdo da evidência desbloqueada pelo jogador.
void mostrarEvidencia(int id) {
    printf("\nPROVA - ");
    // Mostra o nome da evidência antes de apresentar seus detalhes.
    mostrarTitulo(id);
    printf("\n\n");
    // Cada caso apresenta as informações específicas de uma evidência.
    switch (id) {
        case 1:
            printf("Camera de seguranca que mostra Caio indo embora da festa.\n");
            printf("Horario da camera: 2h48 da manha.\n");
            break;
        case 2:
            printf("Audio gravado em um video da festa, enviado anonimamente.\n");
            printf("O audio e super distorcido: tem glitches, mudancas na voz\n");
            printf("e trechos robotizados.\n");
            break;
        case 3:
            printf("Origem: celular de Caio, extraido pela pericia com autorizacao judicial.\n\n");
            printf("GABRIEL (02:11): cara, eu vou falar. as pessoas precisam saber o que ela fez\n");
            printf("CAIO (02:12): fala com ela primeiro. na frente de todo mundo nao\n");
            printf("GABRIEL (02:12): ela nao falou com ninguem quando pegou o premio\n");
            printf("CAIO (02:14): to pedindo. so isso\n");
            printf("GABRIEL (02:15): desce aqui que a gente resolve\n");
            printf("CAIO (02:31): faz o que quiser entao. eu vou embora\n");
            printf("GABRIEL (02:33): vai nessa\n");
            printf("(sem mensagens depois disso)\n");
            break;
        case 4:
            printf("Foto de Juliana dentro do carro, com horario de 3 horas e pouco.\n");
            printf("Ha distorcoes visiveis nas maos e no reflexo do vidro.\n");
            break;
        case 5:
            printf("O registro mostra que o celular de Gabriel esteve proximo a estrada\n");
            printf("onde seu corpo foi encontrado, por volta das 3h20 da manha.\n");
            printf("Entretanto, o arquivo do envelope nao possui todos os dados tecnicos\n");
            printf("necessarios para confirmar sua origem.\n");
            break;
        case 6:
            printf("A imagem mostra Juliana perto do estacionamento da festa. No canto,\n");
            printf("ha uma indicacao de horario proxima das 3h da manha. A primeira vista,\n");
            printf("a foto parece confirmar o depoimento dela.\n");
            printf("Mas a iluminacao no rosto de Juliana nao combina com a do ambiente,\n");
            printf("partes do fundo apresentam deformacoes e o horario do canto esta\n");
            printf("escrito com uma fonte diferente da usada pelo celular.\n");
            break;
    }
}

// Informa ao jogador que uma nova evidência está disponível.
void anunciarEvidencia() {
    printf("\n!! Nova evidencia disponivel !!\n");
    printf("[ENTER] Desbloquear nova evidencia\n");
    // Aguarda a confirmação antes de apresentar o desafio
    esperarEnter();
}

// Uma tentativa so: se errar a questao, a evidencia é perdida
// respostaAcusacao = opcao que acusa alguem sem provas (0 se a questao nao tem)
void desbloquearEvidencia(int id, int respostaCorreta, int respostaAcusacao) {
    int resposta;
    int escolha;
    int confirmou;
    int respondeu = 0;   // 1 quando o jogador fecha sua resposta

    // Se escolher a opcao que acusa alguem, pede confirmacao antes de aceitar
    while (respondeu == 0) {
        printf("Sua resposta: ");
        resposta = lerOpcao(1, 4);
        respondeu = 1;
        
        // Verifica se a opção escolhida encerra a investigação antecipadamente.
        if (resposta == respostaAcusacao) {
            printf("\nEssa opcao acusa diretamente um suspeito e encerra a investigacao.\n");
            printf("Confirmar? [1] Sim [2] Nao: ");
            confirmou = lerOpcao(1, 2);

            // Permite voltar atrás e escolher outra resposta.
            if (confirmou == 2) {
                respondeu = 0;   // volta a perguntar a resposta
                printf("\nVoce reconsidera. Escolha outra resposta.\n\n");
            }
        }
    }

    // Encerra a investigação se o jogador confirmar a acusação antecipada.
    if (resposta == respostaAcusacao) {
        finalAntecipado = 1;
        printf("\nVoce decide que Caio e o culpado e encerra a investigacao aqui.\n");
        pausar();
        return;
    }

    // Se errar o desafio, registra o erro e marca a evidência como perdida.
    if (resposta != respostaCorreta) {
        erros++;
        mudarEstado(id, 1);
        printf("\nResposta errada. A evidencia foi perdida!\n");
        printf("A investigacao continua sem ela.\n");
        pausar();
        return;
    }

    // Adiciona pontos quando o jogador acerta a questão de lógica.
    pontos += 10;
    printf("\nCorreto! Evidencia desbloqueada.\n");
    // Apresenta o conteúdo da evidência liberada.
    mostrarEvidencia(id);
    // Solicita que o jogador classifique a evidência como verdadeira ou gerada por IA.
    printf("\nEssa prova e:\n[1] Verdadeira\n[2] Manipulada por IA\nSua decisao: ");
    escolha = lerOpcao(1, 2);
    // Converte a escolha em estado: 2 = verdadeira, 3 = IA.
    mudarEstado(id, escolha + 1);
    pausar();
}

// Exibe o dossiê com todas as evidências e seus estados atuais.
void consultarDossie() {
    int id;
    int escolha;

    do {
        limparTela();
        printf("===== DOSSIE =====\n");
        // Percorre as seis evidências para mostrar sua situação.
        for (id = 1; id <= TOTAL_EVIDENCIAS; id++) {
            printf("[%d] ", id);
            if (pegarEstado(id) == 0) {
                printf("(ainda bloqueada)\n");
            } else {
                mostrarTitulo(id);
                // Traduz o número do estado em uma descrição para o jogador.
                printf(" -> %s\n", pegarEstado(id) == 1 ? "PERDIDA" :
                                   pegarEstado(id) == 2 ? "marcada como VERDADEIRA" :
                                                          "marcada como IA");
            }
        }
        // Permite escolher uma evidência para consultar ou voltar ao menu anterior.
        printf("[0] Voltar\nQual prova quer rever? ");
        escolha = lerOpcao(0, TOTAL_EVIDENCIAS);

        if (escolha != 0) {
            // Evidências bloqueadas ou perdidas não podem ser consultadas novamente.
            if (pegarEstado(escolha) < 2) {
                printf("Essa prova nao esta disponivel.\n");
            } else {
                mostrarEvidencia(escolha);
            }
            pausar();
        }
    } while (escolha != 0);
}

// Mostra as opções disponíveis entre as cenas da investigação. 
// Retorna 1 para continuar e 0 para voltar ao menu principal.
int menuEntreCenas() {
    int escolha;

    do {
        printf("\n[1] Continuar a investigacao\n[2] Ver dossie\n[3] Voltar ao menu principal\nOpcao: ");
        escolha = lerOpcao(1, 3);
        // Permite consultar o dossiê sem interromper a investigação.
        if (escolha == 2) {
            consultarDossie();
        }
    } while (escolha == 2);

    // Converte a escolha em um resultado que controla o fluxo do jogo.
    return escolha == 1;
}

// ---------------- CENAS ----------------

void cena1() {
    limparTela();
    printf("=== CENA 1 ===\n\n");
    printf("(A tela esta escura. Ouve-se um telefone tocando.)\nTrrrr...Trrrr...\n");
    pausar();
    printf("\nA tela mostra o interior de uma delegacia. Sobre uma mesa, um\n");
    printf("computador recebe uma nova ocorrencia.\n");
    printf("\n   [!] NOVA OCORRENCIA - aperte para abrir\n");
    pausar();
    printf("\nO arquivo do caso aparece:\n\n");
    printf("DESAPARECIMENTO - GABRIEL MOURA\n");
    printf("22 anos\n");
    printf("Estudante universitario\n");
    printf("Ultima localizacao: Festa Universitaria\n");
    printf("Horario estimado: 03:00\n");
    printf("[FOTO DE GABRIEL]\n");
    pausar();
    printf("\nUltima informacao conhecida: ele foi visto tendo uma discussao\n");
    printf("com seu colega, Caio Vasconcelos.\n");
    printf("[FOTO DE CAIO]\n");
    pausar();
    printf("\nO investigador recebe uma mensagem do delegado:\n");
    printf("\"A equipe ja comecou a recolher os depoimentos das pessoas que\n");
    printf("estavam na festa.\"\n");
    printf("\"Precisamos descobrir o que aconteceu naquela noite. Analise os\n");
    printf("alibis, as imagens, as mensagens e tudo que conseguirmos encontrar.\"\n");
    printf("\"E nao confie em uma evidencia so porque ela parece verdadeira.\"\n");
    printf("\n[ENTER] Ir interrogar Caio\n");
    esperarEnter();
}

void cena2() {
    limparTela();
    printf("=== CENA 2 - DEPOIMENTO DE CAIO ===\n\n");
    printf("INTERROGADOR: Caio, antes de comecarmos, voce sabe onde o Gabriel esta?\n");
    printf("CAIO: Nao. Nao faco ideia.\n");
    printf("INTERROGADOR: Tem algum lugar onde voce acha que poderiamos procura-lo?\n");
    printf("Algum lugar que ele costumava frequentar ou onde poderia ter ido depois da festa?\n");
    printf("CAIO: Nao sei... talvez algum lugar que ele costumava ir com os amigos.\n");
    printf("Mas nao tenho certeza.\n");
    printf("INTERROGADOR: Entendo. Entao vamos falar sobre o que aconteceu naquela noite.\n");
    pausar();
    printf("\nINTERROGADOR: Uma testemunha afirmou que viu voce discutindo com Gabriel\n");
    printf("do lado de fora da festa. O que aconteceu?\n");
    printf("CAIO: A gente discutiu, sim.\n");
    printf("INTERROGADOR: Sobre o que?\n");
    printf("CAIO: Assuntos pessoais. Prefiro nao dizer.\n");
    printf("INTERROGADOR: E depois da discussao?\n");
    printf("CAIO: Quando acabou, eu fui embora.\n");
    printf("INTERROGADOR: Voce nao voltou para a festa?\n");
    printf("CAIO: Nao. Eu estava chateado, entao fui direto para casa.\n");
    printf("INTERROGADOR: Que horas voce foi embora?\n");
    printf("CAIO: Eram mais ou menos 2h40.\n");
    pausar();
    printf("\nINTERROGADOR: Entendido. Agora precisamos confirmar se o que voce esta\n");
    printf("dizendo e verdade.\n");
    printf("CAIO: Como?\n");
    printf("INTERROGADOR: Recebemos as imagens da camera que registra a saida da festa.\n");
    printf("Elas podem mostrar se o que voce esta dizendo realmente aconteceu.\n");
    printf("CAIO: Voces vao olhar as cameras?\n");
    printf("INTERROGADOR: E claro. Por que? Esta nervoso?\n");
    printf("CAIO: Nao...\n");
    printf("INTERROGADOR: Se voce realmente foi embora depois da discussao, as imagens\n");
    printf("devem confirmar isso, nao e mesmo?\n");
    printf("CAIO: E... sim.\n");
    printf("INTERROGADOR: Por enquanto, encerramos o depoimento.\n");
    printf("\n(A investigadora sai. Caio permanece na sala de interrogatorio.)\n");
    pausar();

    // Evidência 1; a opção 4 encerra a investigação se for confirmada.
    anunciarEvidencia();
    printf("\nDESAFIO (Condicional)\n");
    printf("Caio disse: \"SE eu fui embora as 2h40, ENTAO a camera da saida me registrou.\"\n");
    printf("Sabendo que ele realmente foi embora as 2h40, o que se conclui?\n");
    printf("[1] A camera deve ter registrado Caio saindo\n");
    printf("[2] A camera nao registrou Caio\n");
    printf("[3] Nada se conclui\n");
    printf("[4] Caio e culpado\n");
    desbloquearEvidencia(1, 1, 4);
}

void cena3() {
    limparTela();
    printf("=== CENA 3 ===\n\n");
    printf("CHEFE: Investigadora, acabamos de receber uma nova prova enviada\n");
    printf("anonimamente. Ela contem um audio que foi gravado em um video da festa.\n");
    printf("No envelope dizia que mostrava o assunto da discussao entre Caio e Gabriel.\n");
    pausar();

    anunciarEvidencia();
    printf("\nDESAFIO (Negacao)\n");
    printf("Qual e a negacao de \"O audio e autentico E a voz e de Gabriel\"?\n");
    printf("[1] O audio nao e autentico E a voz nao e de Gabriel\n");
    printf("[2] O audio nao e autentico OU a voz nao e de Gabriel\n");
    printf("[3] O audio e autentico OU a voz nao e de Gabriel\n");
    printf("[4] O audio e autentico E a voz nao e de Gabriel\n");
    // Evidência 2: o áudio pode ser classificado como verdadeiro ou gerado por IA.
    desbloquearEvidencia(2, 2, 0);

    printf("\n[ENTER] Voltar a sala de interrogatorio\n");
    esperarEnter();
}

void cena4() {
    limparTela();
    printf("=== CENA 4 - DE VOLTA AO INTERROGATORIO ===\n\n");
    printf("INVESTIGADORA: Caio, eu preciso entender uma coisa. Por que voce e Gabriel\n");
    printf("estavam discutindo naquela noite?\n");
    printf("CAIO: Ja falei... era uma coisa pessoal.\n");
    printf("INVESTIGADORA: Eu sei o que voce disse. Mas agora Gabriel esta desaparecido.\n");
    printf("Essa discussao pode acabar sendo muito importante para a investigacao.\n");
    printf("CAIO: Eu nao queria que aquilo tivesse acontecido.\n");
    printf("INVESTIGADORA: Entao me conta. O que aconteceu?\n");
    printf("CAIO: O Gabriel tinha descoberto uma coisa sobre a Juliana.\n");
    printf("INVESTIGADORA: Que coisa?\n");
    printf("CAIO: Ele disse que ela tinha falsificado um trabalho importante da faculdade.\n");
    printf("INVESTIGADORA: E ele tinha provas?\n");
    printf("CAIO: Disse que tinha. Eu nao cheguei a ver.\n");
    pausar();
    printf("\nINVESTIGADORA: E por que isso fez voce discutir com ele?\n");
    printf("CAIO: Porque ele queria contar para todo mundo naquela noite. Eu achei que\n");
    printf("ele devia conversar com ela primeiro, em particular.\n");
    printf("INVESTIGADORA: Voce estava tentando proteger a Juliana?\n");
    printf("CAIO: Eu so nao queria que ele acabasse com a vida dela sem nem dar a chance\n");
    printf("de ela explicar.\n");
    printf("INVESTIGADORA: E o Gabriel concordou com voce?\n");
    printf("CAIO: Nao. Ele ficou dizendo que ela tinha enganado todo mundo e que as\n");
    printf("pessoas precisavam saber.\n");
    printf("INVESTIGADORA: Foi ai que voces comecaram a discutir?\n");
    printf("CAIO: Foi.\n");
    pausar();
    printf("\nINVESTIGADORA: Voce ficou com raiva dele?\n");
    printf("CAIO: Fiquei. Mas so discutimos.\n");
    printf("INVESTIGADORA: Voce ameacou o Gabriel?\n");
    printf("CAIO: Nao.\n");
    printf("INVESTIGADORA: Tem certeza?\n");
    printf("CAIO: Tenho. Eu nunca faria uma coisa dessas.\n");
    printf("INVESTIGADORA: Entendi.\n");
    printf("CAIO: Voce acha que eu fiz alguma coisa com ele?\n");
    printf("INVESTIGADORA: Eu estou tentando descobrir o que aconteceu. E, para isso,\n");
    printf("preciso ouvir todos os lados.\n");
    printf("INVESTIGADORA: Inclusive o da Juliana.\n");
    printf("CAIO: Voce vai chamar ela?\n");
    printf("INVESTIGADORA: Vou. Quero saber se ela vai contar a mesma historia que voce.\n");
    printf("CAIO: Ela vai.\n");
    printf("INVESTIGADORA: Vamos descobrir.\n");
    pausar();

    anunciarEvidencia();
    printf("\nDESAFIO (Bicondicional)\n");
    printf("Gabriel disse: \"Eu conto para todo mundo SE, E SOMENTE SE, ela nao falar\n");
    printf("com ninguem antes.\" Suponha que ela FALOU com alguem antes.\n");
    printf("O que se conclui?\n");
    printf("[1] Gabriel vai contar para todo mundo\n");
    printf("[2] Gabriel nao vai contar para todo mundo\n");
    printf("[3] E indeterminado\n");
    printf("[4] Gabriel ja contou\n");
    // Evidência 3: mensagens que ajudam a esclarecer a relação entre os suspeitos.
    desbloquearEvidencia(3, 2, 0);
}

void cena5() {
    limparTela();
    printf("=== LINHA DE INVESTIGACAO - JULIANA COSTA ===\n\n");
    printf("=== CENA 5 ===\n\n");
    printf("INVESTIGADORA: Juliana Costa?\n");
    printf("JULIANA: Sim.\n");
    printf("INVESTIGADORA: Voce conhecia Gabriel?\n");
    printf("JULIANA: Sim. Nos estudavamos juntos.\n");
    printf("INVESTIGADORA: Voce conversou com ele durante a festa?\n");
    printf("JULIANA: Sim.\n");
    printf("INVESTIGADORA: Como foi a conversa?\n");
    printf("JULIANA: Normal.\n");
    printf("INVESTIGADORA: Voces discutiram?\n");
    printf("JULIANA: Nao.\n");
    printf("INVESTIGADORA: Voce tem certeza?\n");
    printf("JULIANA: Tenho.\n");
    pausar();
    printf("\nINVESTIGADORA: E depois da conversa?\n");
    printf("JULIANA: Eu voltei pra festa e nao vi mais ele.\n");
    printf("INVESTIGADORA: Voce ficou na festa ate que horas?\n");
    printf("JULIANA: Ate umas tres da manha, depois fui pra casa.\n");
    printf("INVESTIGADORA: Voce saiu sozinha?\n");
    printf("JULIANA: Sim.\n");
    printf("INVESTIGADORA: Voce tem algo que possa provar isso? Uma testemunha ou foto?\n");
    printf("JULIANA: Eu tenho uma foto que mandei pra minha mae, mostra o horario.\n");
    printf("INVESTIGADORA: Entendi, pode me mostrar.\n");
    pausar();

    anunciarEvidencia();
    printf("\nDESAFIO (Conjuncao)\n");
    printf("Juliana disse: \"Eu sai sozinha da festa E mandei a foto para a minha mae.\"\n");
    printf("Suponha que ela realmente saiu sozinha, mas NAO mandou a foto para a mae.\n");
    printf("O que se conclui sobre a frase dela?\n");
    printf("[1] A frase dela e verdadeira\n");
    printf("[2] E indeterminado\n");
    printf("[3] A frase dela e falsa\n");
    printf("[4] Juliana saiu acompanhada\n");
    // Evidência 4: permite investigar a foto apresentada por Juliana.
    desbloquearEvidencia(4, 3, 0);

    printf("\n[ENTER] Sair da evidencia\n");
    esperarEnter();
}

void cena6() {
    limparTela();
    printf("=== CENA 6 - DESAPARECIMENTO = HOMICIDIO ===\n\n");
    printf("(O telefone toca. A investigadora atende.)\n");
    printf("INVESTIGADORA: Alo?\n");
    printf("CHEFE: Investigadora, encontramos o Gabriel.\n");
    printf("INVESTIGADORA: Encontraram? Onde ele esta?\n");
    printf("CHEFE: Encontraram o corpo dele em um barranco, as margens de uma estrada.\n");
    printf("INVESTIGADORA: Meu Deus...\n");
    printf("CHEFE: A equipe ja esta no local. A pericia tambem foi acionada.\n");
    printf("INVESTIGADORA: Ja sabem o que aconteceu?\n");
    printf("CHEFE: Ainda nao temos todas as respostas. Mas existem indicios de que\n");
    printf("nao foi uma morte acidental.\n");
    printf("INVESTIGADORA: Entao...\n");
    printf("CHEFE: Sim. A partir de agora, o caso passa a ser tratado como homicidio.\n");
    pausar();
    printf("\nINVESTIGADORA: Entendido. Vou revisar as evidencias.\n");
    printf("CHEFE: Precisamos descobrir o que aconteceu naquela noite. Revise todos os\n");
    printf("depoimentos, as imagens, as mensagens e os registros que ja temos.\n");
    printf("CHEFE: E tenha cuidado com as conclusoes. Uma evidencia falsa pode mudar\n");
    printf("completamente o rumo da investigacao.\n");
    printf("INVESTIGADORA: Entendido.\n");
    printf("\n[A ligacao termina]\n");
    printf("[Algumas horas de investigacao se passam...]\n");
    pausar();
    printf("\nUm policial entra na sala com um envelope.\n");
    printf("POLICIAL: Investigadora, a pericia encontrou novos materiais relacionados\n");
    printf("ao caso.\n");
    printf("INVESTIGADORA: O que encontraram?\n");
    printf("POLICIAL: Um registro de localizacao do celular de Gabriel e uma fotografia\n");
    printf("recuperada do aparelho dele. Ainda precisamos confirmar a autenticidade dos\n");
    printf("arquivos.\n");
    printf("INVESTIGADORA: Deixe os documentos aqui. Vou analisar tudo.\n");
    printf("POLICIAL: Certo. O chefe pediu que voce tenha atencao especial aos horarios.\n");
    printf("Talvez eles ajudem a reconstruir os ultimos momentos de Gabriel.\n");
    printf("\n[O policial sai da sala.]\n");
    pausar();

    anunciarEvidencia();
    printf("\nDESAFIO (Condicional - Modus Tollens)\n");
    printf("\"SE o celular de Gabriel estava perto da estrada as 3h20, ENTAO ele ja tinha\n");
    printf("saido da festa.\" Suponha que Gabriel NAO tinha saido da festa as 3h20.\n");
    printf("O que se conclui?\n");
    printf("[1] O celular estava perto da estrada as 3h20\n");
    printf("[2] O celular nao estava perto da estrada as 3h20\n");
    printf("[3] Nada se conclui\n");
    printf("[4] Gabriel saiu da festa\n");
    // Evidência 5: registro de localização do celular de Gabriel.
    desbloquearEvidencia(5, 2, 0);

    printf("\n[ENTER] CONTINUAR INVESTIGACAO\n");
    esperarEnter();

    printf("\nA pericia conseguiu recuperar uma fotografia que Gabriel havia recebido\n");
    printf("naquela noite.\n");
    printf("INVESTIGADORA: Uma fotografia recebida por Gabriel pouco antes de\n");
    printf("desaparecer? Vamos ver.\n");
    pausar();

    anunciarEvidencia();
    printf("\nDESAFIO (Contrapositiva)\n");
    printf("\"SE a foto e autentica, ENTAO o horario dela bate com o registro do celular.\"\n");
    printf("Qual afirmacao e logicamente equivalente a essa frase?\n");
    printf("[1] SE o horario bate com o registro, ENTAO a foto e autentica\n");
    printf("[2] SE a foto nao e autentica, ENTAO o horario nao bate\n");
    printf("[3] SE o horario nao bate com o registro, ENTAO a foto nao e autentica\n");
    printf("[4] SE a foto e autentica, ENTAO o horario nao bate\n");
    // Evidência 6: última prova antes da acusação final.
    desbloquearEvidencia(6, 3, 0);

    printf("\n[ENTER] Voltar a investigacao\n");
    esperarEnter();
}

// ---------------- FINAL ----------------

// Apresenta o resultado da investigação e calcula os acertos na análise das provas.
void relatorioFinal() {
    int id;
    int estado;
    int acertou;

    printf("\n=== RELATORIO FINAL ===\n");
    // Percorre todas as evidências para comparar a resposta do jogador com o gabarito.
    for (id = 1; id <= TOTAL_EVIDENCIAS; id++) {
        estado = pegarEstado(id);
        printf("[%d] ", id);
        mostrarTitulo(id);
        // Informa qual é a classificação correta de cada evidência.
        printf("\n    Era: %s | ", ehFalsa(id) ? "MANIPULADA POR IA" : "VERDADEIRA");
        if (estado == 0) {
            printf("voce nao chegou a desbloquear\n");
        } else if (estado == 1) {
            printf("voce nao analisou (evidencia perdida)\n");
        } else {
            // Confere se a classificação escolhida corresponde ao gabarito.
            acertou = (estado == 3 && ehFalsa(id)) || (estado == 2 && !ehFalsa(id));
            // Acrescenta pontos quando o jogador identifica corretamente a evidência.
            if (acertou) {
                pontos += 10;
            }
            printf("voce %s\n", acertou ? "acertou" : "errou");
        }
    }
    // Mostra o desempenho final do jogador.
    printf("\nDesafios errados: %d\n", erros);
    printf("Pontuacao final: %d\n", pontos);
    // Apresenta a mensagem educativa central do jogo.
    printf("\nA IA foi usada para distorcer um audio e forjar fotos.\n");
    printf("Uma evidencia nao e verdadeira so porque parece verdadeira.\n");
}

// Apresenta o final em que o caso não é solucionado corretamente.
void finalSemSolucao() {
    limparTela();
    printf("=== FINAL: CASO SEM SOLUCAO ===\n\n");
    // Diferencia uma acusação antecipada de uma acusação incorreta no final.
    if (finalAntecipado == 1) {
        printf("Voce acusou Caio sem ter provas. A investigacao foi encerrada cedo\n");
        printf("demais e o verdadeiro responsavel nunca foi descoberto.\n");
    } else {
        printf("Caio e acusado injustamente. As evidencias falsas enganaram a\n");
        printf("investigacao e Juliana continua livre.\n");
    }
    // Exibe o desempenho do jogador mesmo quando o caso não é resolvido.
    relatorioFinal();
}

// Solicita ao jogador que escolha quem considera responsável pela morte de Gabriel.
void acusacaoFinal() {
    int suspeito;
    int confirmou;

    limparTela();
    printf("=== ACUSACAO FINAL ===\n");
    do {
        // Apresenta os suspeitos e permite consultar o dossiê antes da decisão.
        printf("\nQuem e o responsavel pela morte de Gabriel?\n");
        printf("[1] Caio Vasconcelos\n[2] Juliana Costa\n[3] Ver dossie\nEscolha: ");
        suspeito = lerOpcao(1, 3);
        if (suspeito == 3) {
            // Abre o dossiê e depois retorna à escolha do suspeito.
            consultarDossie();
        } else {
            // Confirma a acusação para evitar uma escolha acidental.
            printf("Confirmar acusacao? [1] Sim [2] Nao: ");
            confirmou = lerOpcao(1, 2);
            // Se cancelar, retorna à seleção de suspeitos.
            if (confirmou == 2) {
                suspeito = 3;
            }
        }
    } while (suspeito == 3);
    // Compara o suspeito escolhido com a culpada definida no programa.
    if (suspeito == CULPADA_REAL) {
        limparTela();
        // Adiciona a pontuação por solucionar o caso.
        pontos += 30;
        printf("=== FINAL: CASO RESOLVIDO ===\n\n");
        printf("Juliana nao consegue sustentar o alibi quando as fotos sao expostas como\n");
        printf("falsas. Ela confessa que usou IA para forjar as imagens e o audio,\n");
        printf("tentando culpar Caio, e acaba admitindo o crime.\n");
        // Apresenta o relatório após a resolução do caso.
        relatorioFinal();
    } else {
        // Se o jogador acusar Caio, apresenta o final sem solução.
        finalSemSolucao();
    }
}

// Controla a ordem das cenas e reinicia os dados para uma nova partida.
void jogar() {
    // zera tudo para uma nova partida
    pontos = 0;
    erros = 0;
    abandonou = 0;
    finalAntecipado = 0;
    // Reinicia o estado de todas as evidências para uma nova investigação.
    evidencia1 = evidencia2 = evidencia3 = 0;
    evidencia4 = evidencia5 = evidencia6 = 0;

    // Inicia a história e apresenta o depoimento de Caio.
    cena1();
    cena2();
    // Se o jogador acusar Caio antecipadamente, encerra o caso.
    if (finalAntecipado == 1) {   // acusou Caio sem provas: vai direto ao final
        finalSemSolucao();
        return;
    }
    // Entre as cenas, permite continuar, consultar o dossiê ou voltar ao menu.
    if (menuEntreCenas() == 0) { abandonou = 1; return; }
    cena3();
    if (menuEntreCenas() == 0) { abandonou = 1; return; }
    cena4();
    if (menuEntreCenas() == 0) { abandonou = 1; return; }
    cena5();
    if (menuEntreCenas() == 0) { abandonou = 1; return; }
    // Apresenta as últimas evidências e solicita a acusação final.
    cena6();
    acusacaoFinal();
}

// ---------------- MENU ----------------

// Explica as regras necessárias para jogar e solucionar a investigação.
void mostrarRegras() {
    limparTela();
    printf("=== REGRAS ===\n\n");
    printf("1. Interrogue os suspeitos e acompanhe o caso.\n");
    printf("2. Resolva a questao de logica para desbloquear cada evidencia.\n");
    printf("   Voce so tem UMA tentativa: se errar, a evidencia e perdida.\n");
    printf("3. Decida se a prova e VERDADEIRA ou MANIPULADA POR IA.\n");
    printf("4. Use o dossie para comparar as provas.\n");
    printf("5. Acuse o responsavel. Cuidado: nem toda prova e verdadeira!\n");
    pausar();
}

// Permite ativar ou desativar as pausas entre as telas do jogo.
void configuracoes() {
    int escolha;

    do {
        limparTela();
        printf("=== CONFIGURACOES ===\n\n");
        // Mostra a configuração atual das pausas.
        printf("[1] Pausas entre telas: %s\n", pausasLigadas ? "LIGADO" : "DESLIGADO");
        printf("[0] Voltar\nOpcao: ");
        escolha = lerOpcao(0, 1);
        // Alterna entre pausas ativadas e desativadas.
        if (escolha == 1) {
            pausasLigadas = !pausasLigadas;
        }
    } while (escolha != 0);
}

// Apresenta o nome do projeto e os integrantes da equipe.
void creditos() {
    limparTela();
    printf("=== CREDITOS ===\n\n");
    printf("IA.libi - projeto integrador\n");
    printf("Equipe: Giovana Monteiro, Luma Frazao, Lanna Marculino,\n");
    printf("Leticia Minucelli, Victor Pina, Raysha Neide, Julia Aquino, Vitoria Thayna\n");
    pausar();
}

// Função principal: exibe o menu e direciona o jogador para cada opção.
int main() {
    int opcao;
    int jogarDeNovo;

    // Mantém o menu ativo até o jogador confirmar que deseja sair.
    do {
        limparTela();
        printf("==============================\n");
        printf("           IA.LIBI\n");
        printf("==============================\n\n");
        printf("[1] Jogar\n[2] Regras\n[3] Configuracoes\n[4] Creditos\n[0] Sair\n\nOpcao: ");
        // Lê a opção escolhida e impede números fora do menu.
        opcao = lerOpcao(0, 4);

        if (opcao == 1) {
            // Permite iniciar uma nova partida após terminar a anterior.
            do {
                jogar();
                jogarDeNovo = 0;
                // Não oferece outra partida se o jogador abandonou a investigação.
                if (abandonou == 0) {
                    printf("\nJogar novamente? [1] Sim [2] Voltar ao menu: ");
                    jogarDeNovo = (lerOpcao(1, 2) == 1);
                }
            } while (jogarDeNovo == 1);
        } else if (opcao == 2) {
            mostrarRegras();
        } else if (opcao == 3) {
            configuracoes();
        } else if (opcao == 4) {
            creditos();
        } else {
            printf("\nTem certeza que quer sair? [1] Sim [2] Nao: ");
            if (lerOpcao(1, 2) == 2) {
                opcao = -1;
            }
        }
    } while (opcao != 0);

    printf("\nAte a proxima!\n");
    return 0;
}