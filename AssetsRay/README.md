# Guia de Conceitos de C no Projeto AI.libi

Este documento explica, de forma direta e didática, os principais conceitos da linguagem **C** e da biblioteca **Raylib** utilizados no código refatorado do jogo **AI.libi** (`jogo_ai.libi/`). Ele foi feito para ajudar toda a equipe na apresentação para o professor.

---

## 1. Modularização com Arquivos `.h` e `#include`
Em vez de colocar tudo em um único arquivo gigante, dividimos o jogo em módulos menores e organizados:
- `assets.h`: Carregamento de fontes, áudios, resolução virtual e **cache de texturas**.
- `roteiro.h`: Todas as falas, personagens e cenários organizados em tabelas.
- `minigames.h`: Desafios de lógica proposicional e tutorial rápido.
- `celular.h`: Interface de mensagens (`NEXUS-MSG`).
- `progresso.h`: Salvamento e leitura de progresso em arquivo (`progresso.txt`).
- `main.c`: O motor principal que conecta tudo no *Game Loop*.

Para evitar que um `.h` seja incluído duas vezes e cause erro de compilação, usamos *include guards* (`#ifndef NOME_H` / `#define NOME_H` / `#endif`).

---

## 2. `struct` (Estruturas de Dados)
Uma `struct` agrupa variáveis diferentes sob um mesmo nome. No jogo, usamos `struct` para representar entidades inteiras:
- **`Fala`** (`roteiro.h`): guarda `falante`, `texto`, `fundo` e `personagem` de cada diálogo.
- **`EstadoJogo`** (`main.c`): guarda em qual tela o jogador está, índice da fala atual, volume da música/SFX, status das provas e veredito.
- **`GerenciadorAssets`** (`assets.h`): guarda as texturas de UI, fontes, músicas e o cache sob demanda.
- **`PlayerStats`** (`progresso.h`): guarda `acertos`, `erros` e `evidenciaPerdida`.

---

## 3. `enum` (Enumerações)
Um `enum` cria nomes legíveis para números inteiros, evitando "números mágicos" no código:
- **`TelaAtual`** (`main.c`): `TELA_MENU`, `TELA_DIALOGO`, `TELA_MINIGAME`, `TELA_DOSSIE`, `TELA_VEREDITO`, etc.
- **`SequenciaDialogo`** (`main.c`): identifica qual cena está rodando (`SEQ_CENA2`, `SEQ_CENA4`, `SEQ_CENA5`, etc.).

---

## 4. Arrays (Vetores) e Programação Dirigida por Dados
No código antigo, cada diálogo era um `enum` e um `case` gigante no `switch`. Na refatoração, usamos **arrays de structs**:
```c
static const Fala falasCena2[] = {
    { "VOCÊ", "Caio, antes de começarmos...", BG_INTERROGATORIO, CAIO_NEUTRO },
    { "CAIO", "Não. Não faço ideia.", BG_INTERROGATORIO, CAIO_NEUTRO }
};
```
Para descobrir automaticamente quantas falas existem em um array, usamos:
```c
sizeof(falasCena2) / sizeof(falasCena2[0])
```
Assim, um único código de diálogo (`TELA_DIALOGO`) consegue rodar **todas as cenas** do jogo apenas trocando o índice `falaIdx`.

---

## 5. Ponteiros (`*` e `&`)
Ponteiros armazenam o **endereço de memória** de outra variável. Eles são usados para:
1. **Alterar variáveis dentro de funções** (passagem por referência):
   - Ex.: `AtualizarCelular(&celular, ...)` recebe `EstadoCelular *c` para modificar o chat ativo diretamente.
2. **Strings em C** (`const char *`):
   - Em C, textos são ponteiros para o primeiro caractere (`char`) terminado por `'\0'`.
3. **Manipulação de arquivos** (`FILE *`):
   - `fopen`, `fprintf`, `fscanf` usam um ponteiro `FILE *arquivo`.

---

## 6. Manipulação de Arquivos (`FILE *`)
Em `progresso.h`, salvamos o desempenho do jogador em `progresso.txt`:
- `fopen("progresso.txt", "w")` abre para **escrita** (*write*).
- `fprintf(...)` grava `acertos`, `erros` e `evidenciaPerdida`.
- `fopen("progresso.txt", "r")` abre para **leitura** (*read*).
- `fclose(arquivo)` sempre fecha o arquivo para liberar recursos do sistema.

---

## 7. Otimização: Cache de Texturas sob Demanda
Antes, o jogo carregava centenas de PNGs inteiros na memória RAM/VRAM (mais de 260 MB).
Agora, `ObterTexturaCache()` (`assets.h`) mantém apenas **1 fundo**, **1 personagem** e **1 evidência** carregados por vez:
- Se o caminho da imagem for igual ao anterior, reutiliza a textura já carregada.
- Se mudar, chama `UnloadTexture()` na antiga antes de chamar `LoadTexture()` na nova.

---

## 8. Game Loop e Resolução Virtual (`1280×720`) em Raylib
Todo jogo em Raylib segue o ciclo dentro de `while (!WindowShouldClose())`:
1. **Entrada e Lógica (Update):** lê cliques do mouse e teclado (`IsMouseButtonPressed`, `IsKeyPressed`) e calcula colisões (`CheckCollisionPointRec`).
2. **Desenho (Draw):** desenha primeiro em um `RenderTexture2D` fixo de `1280×720` (`BeginTextureMode`), depois escala proporcionalmente para a janela ou tela cheia (`BeginDrawing`).

Para que os **hitboxes nunca quebrem** ao redimensionar a janela ou ativar Tela Cheia (`F11`), `ObterMouseVirtual()` converte a posição real do mouse para coordenadas `1280×720`.
