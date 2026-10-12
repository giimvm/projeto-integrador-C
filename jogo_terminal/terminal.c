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