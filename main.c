// ./main.exe
//FAZENDO AS TRANSICOES ENTRE AS TELAS!!

#include "raylib.h"

//Definidores do switch case para as telas do jogo
typedef enum TelaAtual {
    TELA_MENU = 0,
    TELA_JOGO,
    TELA_REGRAS,
    TELA_CONFIG,
    TELA_CREDITOS
} TelaAtual;


//função para a verificação dos cliques do botão
bool BotaoClicado(Rectangle btn, Vector2 posMouse) {
    return CheckCollisionPointRec(posMouse, btn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}


//função para att o valor dos sliders de volume
float AtualizarSlider(Rectangle slider, Vector2 posMouse, float valorAtual) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(posMouse, slider)) {
        float novoValor = (posMouse.x - slider.x) / slider.width;
        if (novoValor < 0.0f) novoValor = 0.0f;
        if (novoValor > 1.0f) novoValor = 1.0f;
        return novoValor;
    }
    return valorAtual;
}

int main(void) {
    // Declara uma constante inteira para definir a largura e da altura da janela em pixels (800px)
    const int larguraTela = 800;
    const int alturaTela = 600;

    // Cria e abre a janela do jogo com as dimensões especificadas e o título "Projeto Menu"
    InitWindow(larguraTela, alturaTela, "Projeto Menu");
    SetTargetFPS(60);

    //======= CARREGAMENTO DE TEXTURAS =======
    //========================================

    //Textura menu principal
    Texture2D minhaImagem = LoadTexture("src/menu.png");

    //texturas das configurações
    Texture2D texConfigAtivado = LoadTexture("src/configuracoes 1.png");   
    Texture2D texConfigDesativado = LoadTexture("src/configuracoes.png"); 

    //texturas dos creditos 
    Texture2D texCreditos = LoadTexture("src/créditos.png");

    //texturas das regras
    Texture2D texRegras[5];
    texRegras[0] = LoadTexture("src/REGRAS 1.jpg");
    texRegras[1] = LoadTexture("src/REGRAS 2.jpg");
    texRegras[2] = LoadTexture("src/REGRAS 3.jpg");
    texRegras[3] = LoadTexture("src/REGRAS 4.jpg");
    texRegras[4] = LoadTexture("src/REGRAS 5.jpg");

    int paginaRegras = 0;


    // Exibe no terminal o diretório de trabalho atual para fins de verificação
    TraceLog(LOG_INFO, "Diretorio de Trabalho: %s", GetWorkingDirectory());

    // Verificação das texturas, se o ID da textura for 0, significa que o arquivo de imagem não foi encontrado
   if (minhaImagem.id == 0 || texConfigAtivado.id == 0 || texConfigDesativado.id == 0 || texCreditos.id == 0) {
        TraceLog(LOG_WARNING, "ERRO: Uma ou mais imagens nao foram encontradas no caminho especificado!");
    }

    // Define o retângulo correspondente ao tamanho original do arquivo da imagem
    Rectangle origMenu = { 0.0f, 0.0f, (float)minhaImagem.width, (float)minhaImagem.height };
    Rectangle origConfig = { 0.0f, 0.0f, (float)texConfigAtivado.width, (float)texConfigAtivado.height };
    Rectangle origCreditos = { 0.0f, 0.0f, (float)texCreditos.width, (float)texCreditos.height };
    Rectangle origRegras = { 0.0f, 0.0f, (float)texRegras[0].width, (float)texRegras[0].height };

    // Define o retângulo de destino cobrindo toda a janela do programa (800x600)
    Rectangle dest = { 0.0f, 0.0f, (float)larguraTela, (float)alturaTela };


    // Define o vetor de origem (ponto de rotação) no canto superior esquerdo (0,0)
    Vector2 pontoZero = { 0.0f, 0.0f };

    // ===== DEFINIÇÃO DAS ÁREAS DOS BOTÕES =======
    // ============================================

    
    // ========= MENU PRINCIPAL BOTÕES ==========

    // Retângulo centralizado na opção "JOGAR" (incluindo a seta vermelha)
    Rectangle btnJogar = { 115, 271, 106,  35 };
    Rectangle btnRegras = { 137, 321, 104,  37 };
    Rectangle btnConfig = { 102, 372, 181,  43 };
    Rectangle btnCreditos = { 134, 424, 106,  39 };
   
    // ========= TELA CONFIGURAÇÕES BOTÕES ==========
    Rectangle btnVoltar = { 100, 520, 90, 30 };
    Rectangle btnTelaCheia = { 610, 415, 70, 25 };
    Rectangle sliderVolumeGeral = { 250, 245, 385, 15 };
    Rectangle sliderMusica      = { 250, 283, 385, 15 };
    Rectangle sliderEfeitos     = { 250, 323, 385, 15 };

    // ========= TELA REGRAS BOTÕES ==========
    Rectangle btnVoltarRegras   = { 90, 520, 110, 35 };
    Rectangle btnContinuarRegras = { 630, 520, 130, 35 };


    //Variaveis de estado dos botões 

    TelaAtual telaAtual = TELA_MENU;
    bool telaCheia = true;             // Inicia ativado conforme 'configurações 1.jpg'
    float volGeral = 0.75f;           // 75%
    float volMusica = 0.55f;          // 55%
    float volEfeitos = 0.65f;         // 65%

    // Variável para armazenar a posição atual do ponteiro do mouse na tela
    Vector2 posMouse = { 0.0f, 0.0f };

    // Inicia o loop principal que executa até o usuário fechar a janela ou pressionar ESC
    while (!WindowShouldClose()) {
        

        // Obtém a posição x,y do ponteiro do mouse a cada quadro
        posMouse = GetMousePosition();


        // =====TRANSIÇÕES=====
        //=====================
        switch (telaAtual){

         //switch case para a tela do menu principal
         case TELA_MENU:
         
         //chamando a função para verificar os botões da tela princpal
            if (BotaoClicado(btnJogar, posMouse))         telaAtual = TELA_JOGO;
            else if (BotaoClicado(btnRegras, posMouse))   telaAtual = TELA_REGRAS;
            else if (BotaoClicado(btnConfig, posMouse))   telaAtual = TELA_CONFIG;
            else if (BotaoClicado(btnCreditos, posMouse)) telaAtual = TELA_CREDITOS;
          
          break;

         //switch case para a tela de configurações
         case TELA_CONFIG:

          // Tela cheia ajuste
            if (BotaoClicado(btnTelaCheia, posMouse)) {
                telaCheia = !telaCheia;
                ToggleFullscreen();
            }

            // Sliders ajuste
            volGeral   = AtualizarSlider(sliderVolumeGeral, posMouse, volGeral);
            volMusica  = AtualizarSlider(sliderMusica, posMouse, volMusica);
            volEfeitos = AtualizarSlider(sliderEfeitos, posMouse, volEfeitos);

            // Aplica o volume geral no Raylib
            SetMasterVolume(volGeral);

            // Voltar ao menu principal
            if (BotaoClicado(btnVoltar, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                telaAtual = TELA_MENU;
            }
            break;

            //switch case para a tela de creditos
            case TELA_CREDITOS:
               if (BotaoClicado(btnVoltar, posMouse) || IsKeyPressed(KEY_ESCAPE)) {
                telaAtual = TELA_MENU;
            }
            break;

            //Switch case para a tela de jogo 
            case TELA_JOGO:
            
            //Switch case para a tela de regras
            case TELA_REGRAS:

                if (BotaoClicado(btnContinuarRegras, posMouse)) {
                        if (paginaRegras < 4) {
                            paginaRegras++; // Avança para a próxima imagem
                        } else {
                            paginaRegras = 0;       // Reseta o tutorial para a próxima vez
                            telaAtual = TELA_JOGO;  // Na última página (REGRAS 5), o clique inicia o jogo
                        }
                    }

                    // Voltar página ou Retornar ao Menu ("VOLTAR")
                    if (BotaoClicado(btnVoltarRegras, posMouse)) {
                        if (paginaRegras > 0) {
                            paginaRegras--; // Volta para a imagem anterior
                        } else {
                            telaAtual = TELA_MENU; // Na primeira imagem, retorna ao menu principal
                        }
                    }

                    // Atalho ESC (Volta direto para o Menu Principal)
                    if (IsKeyPressed(KEY_ESCAPE)) {
                        paginaRegras = 0;
                        telaAtual = TELA_MENU;
                    }
                    break;
            


             

        
        
        
        
        
        
        
        
        
        
        
        }

        // ========== RENDERIZAÇÃO DAS IMAGENS =========
        // =============================================
    
        BeginDrawing();
         ClearBackground(RAYWHITE);

         switch (telaAtual) {
          
            case TELA_MENU:
               



















         }
            
        EndDrawing();
    }

    for (int i = 0; i < 5; i++) {
        UnloadTexture(texRegras[i]);
    }

    // Libera o espaço de memória alocado pela textura da imagem na VRAM
    UnloadTexture(minhaImagem);

    // Encerra a janela gráfica e limpa os recursos da Raylib
    CloseWindow();

    return 0;
}