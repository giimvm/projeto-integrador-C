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