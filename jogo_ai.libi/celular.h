#ifndef CELULAR_H
#define CELULAR_H

#include "raylib.h"
#include "minigames.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

typedef struct {
    bool inicializado;
    bool janelaAberta;
    int chatAtivo;     // 0 = Chefe Marcos, 1 = AI.LIBE, 2..4 = Casos antigos
    bool lido[5];
    float tempo;
} EstadoCelular;

static inline void IniciarCelular(EstadoCelular *c) {
    memset(c, 0, sizeof(*c));
    c->inicializado = true;
    c->janelaAberta = true;
    c->chatAtivo = 0;
    c->lido[0] = true;
}

// Retorna 1 quando o jogador clicar para avançar para o interrogatório (STATE_C1_6)
static inline int AtualizarCelular(EstadoCelular *c, Vector2 mouse, float dt) {
    if (!c->inicializado) IniciarCelular(c);
    c->tempo += dt;

    // Ícone NEXUS-MSG na área de trabalho do notebook
    Rectangle iconeMsg = { 100.0f, 54.0f, 68.0f, 68.0f };
    // Aba NEXUS-MSG na barra de tarefas do notebook
    Rectangle tabMsg   = { 172.0f, 482.0f, 118.0f, 28.0f };

    if (MGBotaoClicado(iconeMsg, mouse)) {
        c->janelaAberta = true;
        return 0;
    }
    if (MGBotaoClicado(tabMsg, mouse)) {
        c->janelaAberta = !c->janelaAberta;
        return 0;
    }

    if (c->janelaAberta) {
        Rectangle btnMin = { 680.0f, 53.0f, 22.0f, 20.0f };
        Rectangle btnFechar = { 706.0f, 53.0f, 22.0f, 20.0f };
        if (MGBotaoClicado(btnMin, mouse) || MGBotaoClicado(btnFechar, mouse)) {
            c->janelaAberta = false;
            return 0;
        }

        // Lista de conversas e casos antigos na lateral esquerda da janela
        for (int i = 0; i < 5; i++) {
            Rectangle item = { 190.0f, 88.0f + i * 64.0f, 178.0f, 58.0f };
            if (MGBotaoClicado(item, mouse)) {
                c->chatAtivo = i;
                c->lido[i] = true;
                break;
            }
        }

        // Botão para avançar para o interrogatório
        Rectangle btnAvancar = { 494.0f, 416.0f, 226.0f, 38.0f };
        if (MGBotaoClicado(btnAvancar, mouse)) {
            return 1;
        }
    } else {
        // Notificação flutuante no desktop quando a janela estiver minimizada
        Rectangle notifDesk = { 482.0f, 392.0f, 254.0f, 76.0f };
        if (MGBotaoClicado(notifDesk, mouse)) {
            c->janelaAberta = true;
            c->chatAtivo = 0;
            c->lido[0] = true;
        }
    }

    return 0;
}

static inline void DesenharCelular(EstadoCelular *c, FontesUI *f, Vector2 mouse, int larguraTela, int alturaTela) {
    if (!c->inicializado) IniciarCelular(c);

    // Fundo cyberpunk / investigação noir com chuva sutil
    DrawRectangle(0, 0, larguraTela, alturaTela, (Color){ 5, 7, 15, 255 });
    for (int x = 0; x < larguraTela; x += 40) {
        DrawLine(x, 0, x, alturaTela, (Color){ 165, 25, 130, 14 });
    }
    for (int y = 0; y < alturaTela; y += 40) {
        DrawLine(0, y, larguraTela, y, (Color){ 0, 205, 255, 12 });
    }
    for (int r = 0; r < 22; r++) {
        float rx = (float)((r * 97 + (int)(c->tempo * 115.0f)) % larguraTela);
        float ry = (float)((r * 61 + (int)(c->tempo * 260.0f)) % alturaTela);
        DrawLineEx((Vector2){ rx, ry }, (Vector2){ rx - 5.0f, ry + 16.0f }, 1.1f, (Color){ 0, 200, 255, 28 });
    }

    // Hardware do Notebook (posicionado em x=78 para não cobrir o botão de pausa no canto superior esquerdo)
    Rectangle carcaTela = { 78.0f, 22.0f, 684.0f, 508.0f };
    Rectangle telaSO    = { 88.0f, 34.0f, 664.0f, 480.0f };
    DrawRectangleRounded(carcaTela, 0.03f, 8, (Color){ 18, 22, 34, 255 });
    DrawRectangleRoundedLinesEx(carcaTela, 0.03f, 8, 2.0f, (Color){ 0, 205, 255, 175 });
    DrawCircle(420, 28, 3.0f, (Color){ 0, 235, 255, 200 }); // Webcam LED

    // Base / teclado estilizado do notebook na parte inferior
    DrawRectangleRounded((Rectangle){ 56.0f, 530.0f, 728.0f, 46.0f }, 0.16f, 6, (Color){ 22, 26, 40, 255 });
    DrawRectangleRoundedLinesEx((Rectangle){ 56.0f, 530.0f, 728.0f, 46.0f }, 0.16f, 6, 1.5f, (Color){ 90, 140, 195, 160 });
    DrawRectangleRounded((Rectangle){ 350.0f, 534.0f, 140.0f, 8.0f }, 0.4f, 4, (Color){ 0, 210, 255, 120 });
    DrawRectangleRounded((Rectangle){ 365.0f, 548.0f, 110.0f, 20.0f }, 0.2f, 4, (Color){ 14, 18, 28, 255 });

    // Área de trabalho (Desktop) do NEURO-OS
    DrawRectangleRec(telaSO, (Color){ 9, 13, 24, 255 });
    for (int gx = 88; gx < 752; gx += 32) {
        DrawLine(gx, 34, gx, 478, (Color){ 0, 180, 230, 12 });
    }
    for (int gy = 34; gy < 478; gy += 32) {
        DrawLine(88, gy, 752, gy, (Color){ 190, 40, 150, 10 });
    }
    DesenharTextoUI(f->bold, "NEURO-OS v4.2 // NOIR-NET", 495.0f, 44.0f, 18.0f, (Color){ 0, 210, 255, 55 });

    // Ícones da área de trabalho (1 interagível + 4 não interagíveis)
    const char *deskNomes[5] = { "NEXUS-MSG", "TERMINAL", "LOGS_RAW", "FIREWALL", "LIXEIRA" };
    Color deskCores[5] = {
        { 0, 225, 255, 255 },
        { 140, 160, 190, 160 },
        { 140, 160, 190, 160 },
        { 180, 110, 190, 160 },
        { 130, 145, 170, 150 }
    };
    float pulso = 0.5f + 0.5f * sinf(c->tempo * 4.5f);

    for (int i = 0; i < 5; i++) {
        Rectangle ic = { 100.0f, 54.0f + i * 82.0f, 68.0f, 68.0f };
        bool interagivel = (i == 0);
        bool hover = interagivel && CheckCollisionPointRec(mouse, ic);
        Color bg = hover ? (Color){ 24, 44, 74, 235 } : (Color){ 14, 20, 36, 210 };
        DrawRectangleRounded(ic, 0.16f, 6, bg);
        DrawRectangleRoundedLinesEx(ic, 0.16f, 6, hover ? 2.0f : 1.2f, deskCores[i]);

        // Mini ícone simbólico
        Rectangle badge = { ic.x + 19.0f, ic.y + 11.0f, 30.0f, 24.0f };
        DrawRectangleRounded(badge, 0.2f, 4, (Color){ deskCores[i].r / 4, deskCores[i].g / 4, deskCores[i].b / 4, 240 });
        DrawRectangleRoundedLinesEx(badge, 0.2f, 4, 1.4f, deskCores[i]);
        DesenharTextoCentralizadoUI(f->bold, interagivel ? "MSG" : (i == 1 ? ">_" : (i == 2 ? "LOG" : (i == 3 ? "FW" : "DEL"))), badge, 14.0f, deskCores[i]);
        DesenharTextoCentralizadoUI(f->regular, deskNomes[i], (Rectangle){ ic.x - 4.0f, ic.y + 42.0f, 76.0f, 20.0f }, 14.0f,
                                    interagivel ? RAYWHITE : (Color){ 145, 165, 190, 190 });

        if (interagivel && !c->janelaAberta) {
            DrawCircle((int)(ic.x + ic.width - 6), (int)(ic.y + 6), 6.0f, (Color){ 255, 65, 175, (unsigned char)(170 + pulso * 85) });
        }
    }

    // Janela do aplicativo NEXUS-MSG
    if (c->janelaAberta) {
        Rectangle win = { 182.0f, 46.0f, 556.0f, 420.0f };
        DrawRectangleRounded(win, 0.03f, 6, (Color){ 12, 16, 30, 250 });
        DrawRectangleRoundedLinesEx(win, 0.03f, 6, 1.8f, (Color){ 0, 215, 255, 220 });

        // Barra de título da janela
        DrawRectangleRounded((Rectangle){ 182.0f, 46.0f, 556.0f, 32.0f }, 0.03f, 6, (Color){ 18, 28, 52, 255 });
        DrawLine(182, 78, 738, 78, (Color){ 0, 210, 255, 140 });
        DrawCircle(198, 62, 5.0f, (Color){ 255, 65, 170, 255 });
        DesenharTextoUI(f->bold, "NEXUS-MSG v2.4  —  Mensageiro Criptografado", 210.0f, 53.0f, 18.0f, (Color){ 170, 245, 255, 255 });

        Rectangle btnMin    = { 680.0f, 52.0f, 22.0f, 20.0f };
        Rectangle btnFechar = { 706.0f, 52.0f, 22.0f, 20.0f };
        MGDesenharBotao(f, btnMin, "_", mouse, false, (Color){ 80, 170, 220, 180 });
        MGDesenharBotao(f, btnFechar, "X", mouse, false, (Color){ 245, 80, 120, 200 });

        // Lista lateral de conversas e casos antigos ("encher salsicha" noir)
        DrawRectangle(184, 79, 190, 385, (Color){ 10, 14, 26, 255 });
        DrawLine(374, 79, 374, 464, (Color){ 0, 200, 255, 90 });

        const char *titulos[5] = {
            "Chefe Marcos",
            "Central AI.LIBE",
            "Caso #041 Neo-Porto",
            "Caso #038 Blackout",
            "Informante K-9"
        };
        const char *subtitulos[5] = {
            "[URGENTE] Logs do Caio",
            "Alerta acesso 03:18",
            "Arquivado - Cais 7",
            "Arquivado - Subestacao",
            "Contato zona baixa"
        };

        for (int i = 0; i < 5; i++) {
            Rectangle item = { 190.0f, 88.0f + i * 64.0f, 178.0f, 58.0f };
            bool ativo = (c->chatAtivo == i);
            bool hover = CheckCollisionPointRec(mouse, item);
            Color fundo = ativo ? (Color){ 18, 52, 78, 250 }
                        : hover ? (Color){ 20, 30, 52, 240 }
                                : (Color){ 14, 20, 36, 220 };
            Color borda = ativo ? (Color){ 0, 230, 255, 240 } : (Color){ 70, 120, 170, 120 };
            DrawRectangleRounded(item, 0.12f, 6, fundo);
            DrawRectangleRoundedLinesEx(item, 0.12f, 6, ativo ? 1.8f : 1.0f, borda);
            DesenharTextoUI(f->bold, titulos[i], item.x + 10.0f, item.y + 8.0f, 17.0f, ativo ? (Color){ 170, 245, 255, 255 } : RAYWHITE);
            DesenharTextoUI(f->regular, subtitulos[i], item.x + 10.0f, item.y + 30.0f, 15.0f,
                            (i == 0) ? (Color){ 255, 130, 200, 255 } : (Color){ 160, 185, 215, 210 });
            if (!c->lido[i]) {
                DrawCircle((int)(item.x + item.width - 12), (int)(item.y + 14), 4.5f, (Color){ 255, 65, 170, 255 });
            }
        }

        // Área do chat selecionado à direita
        DesenharTextoUI(f->bold, titulos[c->chatAtivo], 390.0f, 88.0f, 22.0f, (Color){ 170, 245, 255, 255 });
        DesenharTextoUI(f->regular, "Canal criptografado ponta-a-ponta", 390.0f, 112.0f, 15.0f, (Color){ 130, 175, 205, 200 });
        DrawLine(386, 134, 726, 134, (Color){ 0, 205, 255, 70 });

        Rectangle b1 = { 390.0f, 146.0f, 312.0f, 84.0f };
        Rectangle b2 = { 390.0f, 240.0f, 312.0f, 86.0f };
        Rectangle b3 = { 414.0f, 336.0f, 308.0f, 64.0f };

        DrawRectangleRounded(b1, 0.12f, 6, (Color){ 18, 28, 48, 245 });
        DrawRectangleRoundedLinesEx(b1, 0.12f, 6, 1.2f, (Color){ 0, 200, 245, 140 });
        DrawRectangleRounded(b2, 0.12f, 6, (Color){ 26, 20, 46, 245 });
        DrawRectangleRoundedLinesEx(b2, 0.12f, 6, 1.2f, (Color){ 225, 70, 175, 145 });
        DrawRectangleRounded(b3, 0.12f, 6, (Color){ 12, 52, 74, 245 });
        DrawRectangleRoundedLinesEx(b3, 0.12f, 6, 1.2f, (Color){ 0, 225, 255, 175 });

        if (c->chatAtivo == 0) {
            DesenharTextoUI(f->regular, "[23:39] Detetive, recuperamos logs apagados", b1.x + 12.0f, b1.y + 12.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "do servidor. Caio foi o ultimo a falar", b1.x + 12.0f, b1.y + 34.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "com Gabriel antes do desaparecimento.", b1.x + 12.0f, b1.y + 56.0f, 17.0f, RAYWHITE);

            DesenharTextoUI(f->regular, "[23:41] Va para a sala de interrogatorio.", b2.x + 12.0f, b2.y + 12.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "Fique atento: algumas provas podem ter", b2.x + 12.0f, b2.y + 34.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "sido manipuladas por IA.", b2.x + 12.0f, b2.y + 56.0f, 17.0f, (Color){ 255, 205, 235, 255 });

            DesenharTextoUI(f->regular, "[23:42] VOCE: Entendido, Diretor.", b3.x + 12.0f, b3.y + 12.0f, 17.0f, (Color){ 170, 245, 255, 255 });
            DesenharTextoUI(f->regular, "Seguindo para interrogar Caio agora.", b3.x + 12.0f, b3.y + 34.0f, 17.0f, (Color){ 170, 245, 255, 255 });
        } else if (c->chatAtivo == 1) {
            DesenharTextoUI(f->regular, "[03:18] AI.LIBE: Login incomum detectado", b1.x + 12.0f, b1.y + 12.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "no terminal 04 fora do expediente.", b1.x + 12.0f, b1.y + 34.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "Origem mascarada por 3 saltos de VPN.", b1.x + 12.0f, b1.y + 56.0f, 17.0f, RAYWHITE);

            DesenharTextoUI(f->regular, "[03:19] Pacotes criptografados enviados", b2.x + 12.0f, b2.y + 12.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "para dispositivo externo nao listado", b2.x + 12.0f, b2.y + 34.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "no inventario do laboratorio.", b2.x + 12.0f, b2.y + 56.0f, 17.0f, (Color){ 255, 205, 235, 255 });

            DesenharTextoUI(f->regular, "STATUS: Evidencia em analise forense.", b3.x + 12.0f, b3.y + 22.0f, 17.0f, (Color){ 170, 245, 255, 255 });
        } else if (c->chatAtivo == 2) {
            DesenharTextoUI(f->regular, "[ARQUIVO] Contrabando de implantes", b1.x + 12.0f, b1.y + 12.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "neurais no armazem 7 de Neo-Porto.", b1.x + 12.0f, b1.y + 34.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "Chuva forte apagou rastros termicos.", b1.x + 12.0f, b1.y + 56.0f, 17.0f, RAYWHITE);

            DesenharTextoUI(f->regular, "[RELATORIO] Suspeito escapou de balsa", b2.x + 12.0f, b2.y + 12.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "rumo ao distrito industrial sul.", b2.x + 12.0f, b2.y + 34.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "Caso encerrado sem prisoes.", b2.x + 12.0f, b2.y + 56.0f, 17.0f, (Color){ 255, 205, 235, 255 });

            DesenharTextoUI(f->regular, "NOTA: Sem ligacao com o caso Gabriel.", b3.x + 12.0f, b3.y + 22.0f, 17.0f, (Color){ 170, 245, 255, 255 });
        } else if (c->chatAtivo == 3) {
            DesenharTextoUI(f->regular, "[ARQUIVO] Queda de energia no setor 2", b1.x + 12.0f, b1.y + 12.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "por 40 minutos durante tempestade.", b1.x + 12.0f, b1.y + 34.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "Cameras da avenida ficaram offline.", b1.x + 12.0f, b1.y + 56.0f, 17.0f, RAYWHITE);

            DesenharTextoUI(f->regular, "[PERICIA] Falha causada por sobrecarga", b2.x + 12.0f, b2.y + 12.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "em transformador antigo da rede.", b2.x + 12.0f, b2.y + 34.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "Nenhuma invasao confirmada.", b2.x + 12.0f, b2.y + 56.0f, 17.0f, (Color){ 255, 205, 235, 255 });

            DesenharTextoUI(f->regular, "NOTA: Arquivo historico.", b3.x + 12.0f, b3.y + 22.0f, 17.0f, (Color){ 170, 245, 255, 255 });
        } else {
            DesenharTextoUI(f->regular, "[21:05] K-9: As ruas do setor 9 estao", b1.x + 12.0f, b1.y + 12.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "quietas demais hoje, detetive.", b1.x + 12.0f, b1.y + 34.0f, 17.0f, RAYWHITE);
            DesenharTextoUI(f->regular, "Tem gente vendendo dados falsos.", b1.x + 12.0f, b1.y + 56.0f, 17.0f, RAYWHITE);

            DesenharTextoUI(f->regular, "[21:07] K-9: Se precisar de algo", b2.x + 12.0f, b2.y + 12.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "sobre mercado paralelo, me chame", b2.x + 12.0f, b2.y + 34.0f, 17.0f, (Color){ 255, 205, 235, 255 });
            DesenharTextoUI(f->regular, "no canal antigo depois da meia-noite.", b2.x + 12.0f, b2.y + 56.0f, 17.0f, (Color){ 255, 205, 235, 255 });

            DesenharTextoUI(f->regular, "VOCE: Anotado. Fico de olho.", b3.x + 12.0f, b3.y + 22.0f, 17.0f, (Color){ 170, 245, 255, 255 });
        }

        Rectangle btnAvancar = { 494.0f, 416.0f, 226.0f, 38.0f };
        MGDesenharBotao(f, btnAvancar, "IR INTERROGAR CAIO >>", mouse, true, (Color){ 0, 225, 255, 230 });
    } else {
        // Notificação flutuante estilo SO quando minimizado
        Rectangle notifDesk = { 482.0f, 392.0f, 254.0f, 76.0f };
        bool hover = CheckCollisionPointRec(mouse, notifDesk);
        DrawRectangleRounded(notifDesk, 0.14f, 6, hover ? (Color){ 24, 38, 66, 245 } : (Color){ 16, 22, 40, 240 });
        DrawRectangleRoundedLinesEx(notifDesk, 0.14f, 6, 1.8f, (Color){ 255, 65, 175, (unsigned char)(155 + pulso * 100) });
        DesenharTextoUI(f->bold, "NOTIFICACAO: CHEFE MARCOS", notifDesk.x + 12.0f, notifDesk.y + 10.0f, 17.0f, (Color){ 170, 245, 255, 255 });
        DesenharTextoUI(f->regular, "Nova mensagem urgente no NEXUS-MSG.", notifDesk.x + 12.0f, notifDesk.y + 34.0f, 15.0f, RAYWHITE);
        DesenharTextoUI(f->regular, "Clique para abrir a janela.", notifDesk.x + 12.0f, notifDesk.y + 52.0f, 14.0f, (Color){ 255, 165, 220, 230 });
    }

    // Barra de Tarefas inferior do Sistema Operacional
    DrawRectangle(88, 478, 664, 36, (Color){ 12, 16, 28, 250 });
    DrawLine(88, 478, 752, 478, (Color){ 0, 200, 255, 140 });

    // Botão SYS (decorativo)
    DrawRectangleRounded((Rectangle){ 96.0f, 482.0f, 68.0f, 28.0f }, 0.2f, 4, (Color){ 24, 32, 54, 240 });
    DrawRectangleRoundedLinesEx((Rectangle){ 96.0f, 482.0f, 68.0f, 28.0f }, 0.2f, 4, 1.2f, (Color){ 255, 65, 175, 180 });
    DesenharTextoCentralizadoUI(f->bold, "SYS", (Rectangle){ 96.0f, 482.0f, 68.0f, 28.0f }, 16.0f, (Color){ 255, 160, 220, 255 });

    // Aba interagível NEXUS-MSG na barra de tarefas
    Rectangle tabMsg = { 172.0f, 482.0f, 118.0f, 28.0f };
    MGDesenharBotao(f, tabMsg, "NEXUS-MSG", mouse, c->janelaAberta, (Color){ 0, 215, 255, 210 });

    // Outras abas/apps não interagíveis na barra de tarefas
    const char *appsFixos[3] = { "TERM_04", "SCANNER", "CAM_NET" };
    for (int i = 0; i < 3; i++) {
        Rectangle tab = { 298.0f + i * 94.0f, 482.0f, 86.0f, 28.0f };
        DrawRectangleRounded(tab, 0.2f, 4, (Color){ 18, 24, 40, 220 });
        DrawRectangleRoundedLinesEx(tab, 0.2f, 4, 1.0f, (Color){ 80, 115, 155, 130 });
        DesenharTextoCentralizadoUI(f->regular, appsFixos[i], tab, 15.0f, (Color){ 140, 165, 195, 190 });
    }

    // Relógio e VPN à direita da barra de tarefas
    DesenharTextoUI(f->regular, "VPN-ON  |  23:42", 632.0f, 487.0f, 16.0f, (Color){ 165, 225, 245, 230 });
}

#endif
