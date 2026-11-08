#ifndef PROGRESSO_H
#define PROGRESSO_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


//==== ESTRUTURA DE DADOS DO JOGADOR ====

typedef struct {
    int acertos;
    int erros;
    char evidenciaPerdida[100]; // Armazena o nome da evidência perdida (até 99 caracteres)
} PlayerStats;


// ==== FUNÇÕES DE ARQUIVO ==== 


// Grava o progresso 
static inline void SalvarProgresso(PlayerStats stats) {
    FILE *arquivo = fopen("progresso.txt", "w");
    if (arquivo != NULL) {
        fprintf(arquivo, "%d\n%d\n%s\n", stats.acertos, stats.erros, stats.evidenciaPerdida);
        fclose(arquivo);
    }
}

// Carrega os dados salvos do arquivo
static inline void CarregarProgresso(PlayerStats *stats) {
    FILE *arquivo = fopen("progresso.txt", "r");
    if (arquivo != NULL) {
        fscanf(arquivo, "%d\n%d\n", &stats->acertos, &stats->erros);
        
        // Lê a linha com o nome da evidência perdida
        if (fgets(stats->evidenciaPerdida, sizeof(stats->evidenciaPerdida), arquivo) != NULL) {
            stats->evidenciaPerdida[strcspn(stats->evidenciaPerdida, "\n")] = 0;
        }
        fclose(arquivo);
    }
}

// Reseta todas as estatísticas
static inline void ReiniciarProgresso(PlayerStats *stats) {
    stats->acertos = 0;
    stats->erros = 0;
    strcpy(stats->evidenciaPerdida, "Nenhuma");
    SalvarProgresso(*stats);
}

#endif