// ./main.exe
// FALTA CENTRALIZAR OS BOTÕES E FAZER AS TRANSICOES ENTRE AS TELAS!!

#include "raylib.h"

int main(void) {
    // Declara uma constante inteira para definir a largura da janela em pixels (800px)
    const int larguraTela = 800;
    // Declara uma constante inteira para definir a altura da janela em pixels (600px)
    const int alturaTela = 600;

    // Cria e abre a janela do jogo com as dimensões especificadas e o título "Projeto Menu"
    InitWindow(larguraTela, alturaTela, "Projeto Menu");
    // Define a taxa de quadros (FPS) alvo para 60 quadros por segundo
    SetTargetFPS(60);
    // Carrega a imagem do menu localizada na pasta 'src' diretamente na GPU
    Texture2D minhaImagem = LoadTexture("src/menu.png");

    // Exibe no terminal o diretório de trabalho atual para fins de verificação
    TraceLog(LOG_INFO, "Diretorio de Trabalho: %s", GetWorkingDirectory());
    // Se o ID da textura for 0, significa que o arquivo de imagem não foi encontrado
    if (minhaImagem.id == 0) {
        // Exibe um aviso no terminal informando a falha no carregamento
        TraceLog(LOG_WARNING, "ERRO: A imagem nao foi encontrada no caminho especificado!");
    }

    // Define o retângulo correspondente ao tamanho original do arquivo da imagem
    Rectangle orig = { 0.0f, 0.0f, (float)minhaImagem.width, (float)minhaImagem.height };
    // Define o retângulo de destino cobrindo toda a janela do programa (800x600)
    Rectangle dest = { 0.0f, 0.0f, (float)larguraTela, (float)alturaTela };
    // Define o vetor de origem (ponto de rotação) no canto superior esquerdo (0,0)
    Vector2 pontoZero = { 0.0f, 0.0f };

    // === DEFINIÇÃO DAS ÁREAS DOS BOTÕES DO MENU (AJUSTADAS E CENTRALIZADAS) ===
    // Retângulo centralizado na opção "JOGAR" (incluindo a seta vermelha)
    Rectangle btnJogar = { 115, 271, 106,  35 };
    // Retângulo centralizado na opção "REGRAS"
    Rectangle btnRegras = { 137, 321, 104,  37 };
    // Retângulo centralizado na opção "CONFIGURAÇÕES"
    Rectangle btnConfig = { 102, 372, 181,  43 };
    // Retângulo centralizado na opção "CRÉDITOS"
    Rectangle btnCreditos = { 134, 424, 106,  39 };

    // Variável para armazenar a posição atual do ponteiro do mouse na tela
    Vector2 posMouse = { 0.0f, 0.0f };

    // Inicia o loop principal que executa até o usuário fechar a janela ou pressionar ESC
    while (!WindowShouldClose()) {
        // --- LÓGICA DE ATUALIZAÇÃO ---
        // Obtém a posição x,y do ponteiro do mouse a cada quadro
        posMouse = GetMousePosition();

        // Verifica se a opção JOGAR foi clicada com o botão esquerdo do mouse
        if (CheckCollisionPointRec(posMouse, btnJogar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "Botao JOGAR Clicado!");
            // Adicione a ação/troca de tela do jogo aqui
        }

        // Verifica se a opção REGRAS foi clicada
        if (CheckCollisionPointRec(posMouse, btnRegras) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "Botao REGRAS Clicado!");
        }

        // Verifica se a opção CONFIGURAÇÕES foi clicada
        if (CheckCollisionPointRec(posMouse, btnConfig) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "Botao CONFIGURACOES Clicado!");
        }

        // Verifica se a opção CRÉDITOS foi clicada
        if (CheckCollisionPointRec(posMouse, btnCreditos) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            TraceLog(LOG_INFO, "Botao CREDITOS Clicado!");
        }

        // --- RENDERIZAÇÃO / DESENHO ---
        // Prepara o sistema gráfico para o início da renderização do quadro
        BeginDrawing();

            // Limpa a tela preenchendo o fundo de branco
            ClearBackground(RAYWHITE);
            // Desenha a imagem do menu cobrindo toda a extensão da janela
            DrawTexturePro(minhaImagem, orig, dest, pontoZero, 0.0f, WHITE);
            // Escreve a instrução padrão na parte superior da tela
            DrawText("Pressione ESC ou feche a janela para sair", 10, 10, 20, RAYWHITE);

            // === EFEITO DE HOVER (DESTAQUE AO PASSAR O MOUSE) ===
            // Se o mouse estiver sobre o botão "JOGAR", desenha um retângulo sutil destacado
            if (CheckCollisionPointRec(posMouse, btnJogar)) {
                DrawRectangleLinesEx(btnJogar, 2, RED);
            }

            // Se o mouse estiver sobre o botão "REGRAS"
            if (CheckCollisionPointRec(posMouse, btnRegras)) {
                DrawRectangleLinesEx(btnRegras, 2, RED);
            }

            // Se o mouse estiver sobre o botão "CONFIGURAÇÕES"
            if (CheckCollisionPointRec(posMouse, btnConfig)) {
                DrawRectangleLinesEx(btnConfig, 2, RED);
            }

            // Se o mouse estiver sobre o botão "CRÉDITOS"
            if (CheckCollisionPointRec(posMouse, btnCreditos)) {
                DrawRectangleLinesEx(btnCreditos, 2, RED);
            }

        // Finaliza o desenho e exibe o quadro processado na tela
        EndDrawing();
    }

    // Libera o espaço de memória alocado pela textura da imagem na VRAM
    UnloadTexture(minhaImagem);

    // Encerra a janela gráfica e limpa os recursos da Raylib
    CloseWindow();

    return 0;
}