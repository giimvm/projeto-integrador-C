# Como Rodar e Contribuir com o Projeto

Este guia explica o passo a passo para baixar, compilar, executar o jogo no seu computador e contribuir com o desenvolvimento.

---

## 1. Pré-requisitos na Máquina

Antes de começar, certifique-se de que o computador possui os seguintes programas instalados:

- [Git](https://git-scm.com/)
- [Visual Studio Code (VS Code)](https://code.visualstudio.com/)
- [Raylib](https://www.raylib.com/) (instalada no caminho padrão `C:\raylib`)

Caso já estejam instalados, abra o **VS Code**.

---

## 2. Clonar o Repositório no VS Code

1. Abra o **VS Code** (sem nenhuma pasta aberta).
2. Na tela inicial, na seção **Iniciar**, clique em **Clonar Repositório Git**.
   - *Alternativa:* caso a aba de boas-vindas não esteja aberta, clique no ícone de **Controle do Código-Fonte** na barra lateral esquerda (ou pressione `Ctrl + Shift + G`) e clique no botão azul **Clonar Repositório**.
3. Na barra superior que abrir no topo da janela, cole o link do repositório e pressione `Enter`:
   ```text
   https://github.com/giimvm/projeto-integrador-C.git
   ```
4. Uma janela do sistema será aberta pedindo onde salvar o projeto. Escolha uma pasta de sua preferência (ex.: *Área de Trabalho* ou *Documentos*) e clique em **Selecionar como Destino do Repositório**.
5. Assim que o download terminar, aparecerá uma notificação no canto inferior direito perguntando se deseja abrir o repositório clonado. Clique em **Abrir**.

---

## 3. Garantir que a branch `main` está atualizada

1. Abra o terminal integrado do VS Code (`Ctrl + '` ou no menu superior: **Terminal > Novo Terminal**).
2. Verifique e mude para a branch `main`:
   ```bash
   git checkout main
   ```
3. Puxe as últimas atualizações do GitHub:
   ```bash
   git pull origin main
   ```

**Verificação:** o terminal deverá exibir `Already on 'main'` (ou `Switched to branch 'main'`) e `Already up to date.` (ou baixar os arquivos mais recentes sem erros).

---

## 4. Compilar o Jogo

1. No explorador de arquivos do VS Code, **abra o arquivo principal em C** (por exemplo, `main.c`) para que ele seja o arquivo ativo no editor.
2. Com ele aberto, pressione **`Ctrl + Shift + B`** para compilar.
3. O terminal integrado executará o compilador `gcc` com as bibliotecas da Raylib e exibirá ao final:
   ```text
   Terminal will be reused by tasks, press any key to close it.
   ```
4. Pressione qualquer tecla para fechar a tarefa e verifique se o arquivo **`main.exe`** foi gerado/atualizado na pasta do projeto.

> **Atenção:** o `Ctrl + Shift + B` compila o arquivo que estiver aberto no momento. Se você pressionar o atalho com um arquivo `.md` ou `.txt` aberto, o terminal mostrará `terminated with exit code: 1`. Caso isso aconteça, abra o arquivo `.c` do jogo e aperte `Ctrl + Shift + B` novamente.

---

## 5. Rodar o Jogo

Com o terminal integrado aberto na pasta onde está o `main.exe` (caso esteja em subpasta, entre nela com `cd nome_da_pasta`), execute:

```powershell
./main.exe
```

A janela do jogo será aberta e você já poderá testá-lo na sua máquina.

---

## 6. Como Contribuir

1. A partir da `main` atualizada, crie uma nova branch para a sua alteração:
   ```bash
   git checkout -b nome-da-sua-alteracao
   ```
2. Faça suas modificações no código, compile e teste com `./main.exe`.
3. Adicione e salve suas alterações no Git:
   ```bash
   git add .
   git commit -m "Descreva brevemente o que foi alterado"
   ```
4. Envie sua branch para o GitHub e abra um **Pull Request** para a `main`:
   ```bash
   git push origin nome-da-sua-alteracao
   ```