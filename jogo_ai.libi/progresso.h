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

typedef struct {
    float volMusica;
    float volSfx;
    bool telaCheia;
    bool modoTechNight;
    bool devDesbloqueado;
    float volGeral;
} ConfigSalva;

static inline void SalvarConfig(ConfigSalva cfg) {
    FILE *arquivo = fopen("config.txt", "w");
    if (arquivo != NULL) {
        fprintf(arquivo, "%.3f\n%.3f\n%d\n%d\n%d\n%.3f\n",
            cfg.volMusica, cfg.volSfx,
            cfg.telaCheia ? 1 : 0,
            cfg.modoTechNight ? 1 : 0,
            cfg.devDesbloqueado ? 1 : 0,
            cfg.volGeral);
        fclose(arquivo);
    }
}

static inline void CarregarConfig(ConfigSalva *cfg) {
    FILE *arquivo = fopen("config.txt", "r");
    if (arquivo != NULL) {
        int tc = 0, tn = 0, dv = 0;
        float vg = cfg->volGeral;
        int lidos = fscanf(arquivo, "%f\n%f\n%d\n%d\n%d\n%f\n", &cfg->volMusica, &cfg->volSfx, &tc, &tn, &dv, &vg);
        if (lidos >= 2) {
            if (cfg->volMusica < 0.0f) cfg->volMusica = 0.0f;
            if (cfg->volMusica > 1.0f) cfg->volMusica = 1.0f;
            if (cfg->volSfx < 0.0f) cfg->volSfx = 0.0f;
            if (cfg->volSfx > 1.0f) cfg->volSfx = 1.0f;
            cfg->telaCheia = (tc != 0);
            cfg->modoTechNight = (tn != 0);
            if (lidos >= 5) cfg->devDesbloqueado = (dv != 0);
            if (lidos >= 6) {
                if (vg < 0.0f) vg = 0.0f;
                if (vg > 1.0f) vg = 1.0f;
                cfg->volGeral = vg;
            }
        }
        fclose(arquivo);
    }
}

#endif