# 🎙️ Protocolo Moura — Guia Oficial de Dublagem (IA & Voz)

> **Projeto:** AI.libi (Recife, 2045)  
> **Fonte oficial:** Sincronizado diretamente com `jogo_ai.libi/roteiro.h`, `jogo_ai.libi/main.c` (Player de Áudio da Prova 1) e `jogo_ai.libi/cutscene.h`  
> **Total:** **106 falas** (3 do áudio da Prova 1 + 103 do roteiro principal e finais) + Cutscene HQ opcional (5 páginas)

---

## 📑 Sumário

1. [Perfis de Voz para IA (Casting & Prompts)](#-1-perfis-de-voz-para-ia-casting--prompts)
2. [Regras de Ouro para Enviar na IA (Sem Erros de Leitura)](#-2-regras-de-ouro-para-enviar-na-ia-sem-erros-de-leitura)
3. [Padrão de Arquivos e Entrega](#-3-padrão-de-arquivos-e-entrega)
4. [Áudio de Evidência — Prova 1 (Obrigatório)](#-4-áudio-de-evidência--prova-1-obrigatório)
5. [Roteiro Principal — Cenas 1 a 6 (Comum às Duas Rotas)](#-5-roteiro-principal--cenas-1-a-6-comum-às-duas-rotas)
   - [Cena 1 — Abertura do Delegado e Escritório](#cena-1--abertura-do-delegado-e-escritório)
   - [Cena 2 — Interrogatório de Caio](#cena-2--interrogatório-de-caio)
   - [Cena 3 — O Pacote Anônimo](#cena-3--o-pacote-anônimo)
   - [Cena 4 — Interrogatório de Juliana](#cena-4--interrogatório-de-juliana)
   - [Cena 5 — O Corpo](#cena-5--o-corpo)
   - [Cena 6 — A Caixa-Preta (Backup na Nuvem)](#cena-6--a-caixa-preta-backup-na-nuvem)
6. [Cena 7 — Confronto Final (Bifurcado por Rota)](#-6-cena-7--confronto-final-bifurcado-por-rota)
   - [Rota A: Culpada = Juliana](#rota-a-culpada--juliana)
   - [Rota B: Culpado = Caio](#rota-b-culpado--caio)
7. [Cena 8 — Chamado para a Sala de Suspeitos](#-7-cena-8--chamado-para-a-sala-de-suspeitos)
8. [Tribunal e os 6 Finais](#-8-tribunal-e-os-6-finais)
   - [Final 1 — Verdade Absoluta (5/5 provas)](#final-1--verdade-absoluta-55-provas)
   - [Final 2 — A Verdade (4/5 provas)](#final-2--a-verdade-45-provas)
   - [Final 3 — A Dúvida (2 ou 3 provas)](#final-3--a-dúvida-2-ou-3-provas)
   - [Final 4 — Colapso Forense (0 ou 1 prova)](#final-4--colapso-forense-0-ou-1-prova)
   - [Final 5 — A Precipitação (Acusação precoce)](#final-5--a-precipitação-acusação-precoce)
   - [Final 6 — A Injustiça (Acusou o inocente)](#final-6--a-injustiça-acusou-o-inocente)
9. [Cutscene HQ — 5 Páginas (Narração Opcional)](#-9-cutscene-hq--5-páginas-narração-opcional)
10. [Resumo de Falas por Voz e Checklist](#-10-resumo-de-falas-por-voz-e-checklist)

---

## 🎭 1. Perfis de Voz para IA (Casting & Prompts)

Use estas descrições ao escolher ou configurar as **7 vozes** na ferramenta de dublagem por IA (ElevenLabs, Fish Audio, Google/Azure TTS, etc.), sempre em **Português do Brasil (PT-BR)**:

| Falante no Código | Personagem | Quant. Falas | Perfil / Prompt sugerido para a IA |
| :--- | :--- | :---: | :--- |
| **`JOGADOR`** e **`PENSAMENTO`** | **Helena** (Investigadora, ~28 anos) | **46** *(28 falas + 18 pensamentos)* | **Voz feminina adulta jovem**, firme, analítica, ritmo claro e levemente cansado de madrugada. **Atenção:** `PENSAMENTO` usa a **mesma voz da Helena**, só que em tom mais baixo, íntimo/reflexivo (narração em *off*). |
| **`DELEGADO MEIRELES`** | **Delegado Meireles** (~50 anos) | **13** | **Voz masculina madura**, grave, experiente, autoridade calma e objetiva. Passa seriedade sem gritar. |
| **`CAIO`** | **Caio Vasconcelos** (22 anos) | **12** *(10 roteiro + 2 Prova 1)* | **Voz masculina jovem**, educada e controlada no início, ficando tensa, defensiva e desesperada nos confrontos/finais. |
| **`JULIANA`** | **Juliana Costa** (21 anos) | **12** | **Voz feminina jovem**, articulada, confiante, levemente irônica/cínica sob pressão, quebrando para voz embargada na confissão. |
| **`JUIZ`** | **Juiz** (~50 anos) | **14** | **Voz masculina madura** (diferente do Delegado), solene, seca, imparcial, ritmo pausado de audiência. |
| **`ADVOGADA`** | **Advogada de Defesa** (~35 anos) | **8** | **Voz feminina adulta**, técnica, incisiva, dicção impecável e ritmo firme de tribuna. *(Atenção: no jogo atual é **Advogada**, mulher.)* |
| **`GABRIEL`** | **Gabriel Moura** (Vítima, 22 anos) | **1** *(só Prova 1)* | **Voz masculina jovem**, tensa e firme, com leve ruído de ambiente ao fundo. |

---

## 🤖 2. Regras de Ouro para Enviar na IA (Sem Erros de Leitura)

A coluna **"Texto Pronto para IA (Copiar e Colar)"** das tabelas abaixo já foi limpa para você só copiar e colar sem a IA engasgar:

1. **Sem parênteses nem aspas externas:** No código, os pensamentos vêm entre `( )`. Aqui já removemos os parênteses externos para a IA não mudar a entonação de forma estranha.
2. **Sem abreviações:** Abreviações como `Prof.` já foram escritas por extenso (`Professor`) para a IA não ler *"prof ponto"*.
3. **Horários e siglas preparados para fala:** Horários numéricos (como `02h42`, `03h20`, `03h41`) estão escritos por extenso na coluna pronta para IA (ex.: *"duas e quarenta e dois"*, *"três e vinte"*, *"três e quarenta e um"*).
4. **Guia rápido de pronúncia** (se a IA ler errado, use a grafia fonética ao lado):
   - **AI.libi** → leia *"Ái-líbi"* (ou *"A-I-líbi"*)
   - **NEXARA** → leia *"Nexára"*
   - **Mobi** → leia *"Móbi"*
   - **Meireles** → leia *"Meirêles"*
   - **Vasconcelos** → leia *"Vasconcélos"*
5. **Pontuação controla a pausa:** Reticências (`...`) indicam hesitação/pausa dramática. Se a IA acelerar demais, adicione uma vírgula ou quebra de linha.

---

## 📁 3. Padrão de Arquivos e Entrega

- **Formato:** `.wav` (44.1 kHz / 16-bit) ou `.mp3` / `.ogg` (192 kbps+).
- **Corte:** Tirar silêncio longo no início e no fim (deixar ~100 ms de respiro).
- **Volume:** Normalizado entre `-3 dB` e `-1 dB` (sem estourar/clipar).
- **Nome do arquivo:** `<ID>_<PERSONAGEM>.wav`  
  *Exemplos:* `C1A_01_DELEGADO.wav`, `C1_01_HELENA.wav`, `C2_04_CAIO.wav`, `F1J_02_ADVOGADA.wav`.

Falas marcadas com **`[PRIORITÁRIA]`** são as essenciais caso queiram gerar primeiro uma versão rápida para testar no jogo.

---

## 🔊 4. Áudio de Evidência — Prova 1 (Obrigatório)

Esta prova toca no **Player de Áudio** do jogo (Cena 3 e Mural/Dossiê).  
**Importante:** O jogo sorteia o culpado a cada partida, então precisamos das **duas versões** da fala do Caio (ou uma limpa + uma com defeito sutil de IA).

| ID | Rota | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Efeito de Áudio |
| :--- | :--- | :--- | :--- | :--- |
| `P1_01` | Ambas | **GABRIEL** | Você não vai fazer nada, Caio. Você nunca precisou fazer nada na vida. | Tenso, firme. Adicionar som abafado de festa e vento leve ao fundo. |
| `P1_02_IA` | **Juliana** | **CAIO (IA)** | Se você abrir a boca, eu juro que acabo com você. | **Versão IA:** Voz mais plana, sem respiração antes de falar, leve oscilação robótica no meio da frase. |
| `P1_02_REAL` | **Caio** | **CAIO (Real)** | Se você abrir a boca, eu juro que acabo com você. | **Versão Real:** Respiração pesada/ofegante antes da frase, raiva natural, ruído contínuo de vento/festa. |

---

## 🎬 5. Roteiro Principal — Cenas 1 a 6 (Comum às Duas Rotas)

### Cena 1 — Abertura do Delegado e Escritório
> *Abertura (`falasCena1Abertura` na Sala de Evidências) + Reflexão (`falasCena1` no Escritório).*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C1A_01` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | Acabou de entrar uma denúncia nova no sistema. Um universitário desapareceu depois de uma festa. | Sério, direto, informando o caso. |
| `C1A_02` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | A equipe já está ouvindo quem estava lá. Dê uma olhada nos dados iniciais no seu computador. | Firme, passando instrução. |
| `C1A_03` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | E não se engane: hoje qualquer foto, voz ou vídeo pode ser fabricado. Não confie em prova só porque ela parece real. | Pausa curta antes de "hoje"; tom de alerta. |
| `C1_01` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Recife, 2045. Olhando os registros e as manchetes de hoje... quem tem tecnologia vive bem. Mas qualquer foto, voz ou vídeo pode ser inventado em segundos. | Narração em off, tom reflexivo, íntimo e calmo. |

---

### Cena 2 — Interrogatório de Caio
> *Sala de interrogatório (`falasCena2`). Caio está nervoso e na defensiva.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C2_01` | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | Caio, vou direto ao ponto: você sabe onde o Gabriel está? | Direta, firme, observando a reação. |
| `C2_02` | `[PRIORITÁRIA]` | **CAIO** | Não. Não faço a menor ideia. | Curto, tenso, resposta rápida demais. |
| `C2_03` | — | **JOGADOR (Helena)** | Uma testemunha viu vocês dois batendo boca do lado de fora da festa. O que rolou? | Pressionando sem levantar a voz. |
| `C2_04` | — | **CAIO** | A gente discutiu, sim. Ele tava bêbado... falando que ia contar uma coisa pra Juliana. Do nada começou a jogar na minha cara que eu pagava a faculdade dele pra ele me dever favor, e que a vaga dele na Nexára só veio por indicação do meu pai. Eu falei que ele tava sendo injusto. | Começa irritado, acelera na justificativa, termina defensivo. |
| `C2_05` | — | **JOGADOR (Helena)** | E depois da briga? | Seca, curta. |
| `C2_06` | `[PRIORITÁRIA]` | **CAIO** | Fui embora. Era umas duas e quarenta. Peguei meu carro e fui direto pra casa, sozinho. | Tentando soar calmo e convincente. |
| `C2_07` | — | **JOGADOR (Helena)** | Então o seu álibi é só a sua palavra. | Constatação fria. |
| `C2_08` | — | **CAIO** | ...É. Eu sei como isso soa. Meu relógio até ficou sem bateria na hora, nem registrou o trajeto. | Hesitação no início, frustrado. |
| `C2_09` | — | **JOGADOR (Helena)** | Tudo bem. Vamos puxar a câmera da saída do estacionamento. Se você saiu mesmo às duas e quarenta, ela vai mostrar. | Encerrando o interrogatório, objetiva. |

---

### Cena 3 — O Pacote Anônimo
> *Sala de Evidências (`falasCena3`) + Reflexão no Mural após analisar a Prova 1.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C3_01` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | Acabou de chegar numa entrega anônima. Tem um dispositivo com áudio aí dentro. Mandaram um bilhete dizendo que explica a briga entre o Caio e o Gabriel. | Sério, intrigado. |
| `C3_02` | — | **JOGADOR (Helena)** | Quem grava uma briga no meio da festa e manda anonimamente pra polícia? | Desconfiada, analítica. |
| `C3_03` | — | **DELEGADO MEIRELES** | Alguém que quer que a gente ouça. Ou alguém que precisa muito que a gente acredite. | Tom enigmático, pausado. |

#### Pós-Análise da Prova 1 (depende da rota sorteada)
| ID | Rota | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :--- | :---: | :--- | :--- | :--- |
| `C3_POS_J` | **Juliana** | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Forjaram uma ameaça com a voz do Caio. Quem fez isso queria desviar o meu olhar pra ele... e eu preciso descobrir por quê. | Narração em off, pensativa. |
| `C3_POS_C` | **Caio** | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | A gravação é limpa. O Caio mentiu quando disse que foi só uma discussão boba. Ele ameaçou o Gabriel de verdade. | Narração em off, incisiva. |

---

### Cena 4 — Interrogatório de Juliana
> *Sala de interrogatório (`falasCena4`) + Reflexão no Mural após analisar a Prova 2.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C4_01` | `[PRIORITÁRIA]` | **JULIANA** | Desculpa, esse celular trava o tempo todo. Comprei de segunda mão, sem garantia nem nada. | Simpática, tentando parecer desarmada. |
| `C4_02` | — | **JOGADOR (Helena)** | Você era próxima do Gabriel? | Neutra. |
| `C4_03` | — | **JULIANA** | Ele é meu melhor amigo. A gente entrou junto na faculdade. | Natural, firme. |
| `C4_04` | — | **JOGADOR (Helena)** | Chegou a falar com ele na festa? | Investigativa. |
| `C4_05` | `[PRIORITÁRIA]` | **JULIANA** | A gente conversou rapidinho, depois eu voltei pra pista e não vi mais ele. | Casual, levemente ensaiada. |
| `C4_06` | — | **JOGADOR (Helena)** | O Caio falou que o Gabriel queria te contar alguma coisa naquela noite. Vocês tinham brigado? | Jogando a isca, atenta. |
| `C4_07` | — | **JULIANA** | Brigamos, sim. Eu descobri que o Gabriel conseguiu a vaga na Nexára porque tava fechado com o professor João Paulo. Eu ralei três anos por aquela vaga e saí de mãos abanando. | Amargura contida, ressentimento real. |
| `C4_08` | — | **JOGADOR (Helena)** | E o que você acha do Caio no meio disso tudo? | Observando. |
| `C4_09` | `[PRIORITÁRIA]` | **JULIANA** | Pra quem sempre teve tudo de mão beijada, é muito fácil pedir pra gente "ouvir o outro lado". | Irônica, cínica. |
| `C4_10` | — | **JOGADOR (Helena)** | Até que horas você ficou na festa? | Direta. |
| `C4_11` | — | **JULIANA** | Até umas três da manhã. Pedi um carro de aplicativo e fui embora. Até mandei uma foto pra minha mãe de dentro do carro. Pode olhar. Eu faço questão. | Confiante até demais no final. |

#### Pós-Análise da Prova 2 (depende da rota sorteada)
| ID | Rota | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :--- | :---: | :--- | :--- | :--- |
| `C4_POS_J` | **Juliana** | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Uma prova falsa pra queimar o Caio. Outra falsa pra proteger a Juliana. Alguém tá montando esse tabuleiro peça por peça. | Narração em off, calculista. |
| `C4_POS_C` | **Caio** | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | A foto é autêntica. Mesmo furiosa com o Gabriel, a Juliana realmente entrou no carro às três e cinco. | Narração em off, objetiva. |

---

### Cena 5 — O Corpo
> *Sala de Evidências (`falasCena5`). O caso vira homicídio.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C5_01` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | Acharam o Gabriel. Infelizmente, encontraram o corpo dele num barranco, a duzentos metros do galpão. | Pesado, sombrio. |
| `C5_02` | — | **JOGADOR (Helena)** | Praticamente do lado da festa? | Surpresa, tensa. |
| `C5_03` | — | **DELEGADO MEIRELES** | A perícia cravou a morte na própria madrugada, entre duas e cinquenta e três e cinco. Ele levou um golpe na têmpora antes de cair. A partir de agora, estamos lidando com homicídio. | Sério, profissional. |

---

### Cena 6 — A Caixa-Preta (Backup na Nuvem)
> *Sala de Evidências (`falasCena6`) + Reflexão no Mural após analisar a Prova 4.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C6_01` | — | **DELEGADO MEIRELES** | A perícia digital conseguiu resgatar o último backup automático da conta do Gabriel, minutos antes de o aparelho perder o sinal. | Objetivo. |
| `C6_02` | — | **JOGADOR (Helena)** | Se ele ia abrir o jogo com alguém naquela noite, tem alguma pista guardada aí. | Focada. |

#### Pós-Análise da Prova 4 (depende da rota sorteada)
| ID | Rota | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :--- | :---: | :--- | :--- | :--- |
| `C6_POS_J` | **Juliana** | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Ele entregou três anos de trabalho da Juliana de bandeja. Se ela soube disso na festa, tinha motivo de sobra pra perder a cabeça. | Narração em off. |
| `C6_POS_C` | **Caio** | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Não era favor nenhum. O Caio usava o Gabriel de laranja pra lavar dinheiro e comprar a vaga na Nexára. Se o Gabriel falasse, o Caio ia pra cadeia. | Narração em off. |

---

## ⚡ 6. Cena 7 — Confronto Final (Bifurcado por Rota)

### Rota A: Culpada = Juliana
> *Sala de interrogatório (`falasCena7A_Juliana` e `falasCena7B_Juliana`).*

| ID | Bloco | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :--- | :---: | :--- | :--- | :--- |
| `C7A_J01` | Pré-Prova 5 | — | **JOGADOR (Helena)** | Você garantiu que saiu da festa às três, de carro de aplicativo. | Firme. |
| `C7A_J02` | Pré-Prova 5 | — | **JULIANA** | Garanti. E já te mostrei a foto. | Impaciente, confiante. |
| `C7B_J01` | Pós-Prova 5 | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | A sua corrida na Móbi só foi pedida às três e quarenta e um. E os carros da frota não têm volante, diferente da foto que você me mostrou. | Acusação técnica, implacável. |
| `C7B_J02` | Pós-Prova 5 | — | **JULIANA** | Tá, eu fiquei mais um pouco... perdi a noção da hora. | Tentando improvisar, desconcertada. |
| `C7B_J03` | Pós-Prova 5 | — | **JOGADOR (Helena)** | Quarenta minutos perdidos no relógio? | Irônica, pressionando. |
| `C7B_J04` | Pós-Prova 5 | `[PRIORITÁRIA]` | **JULIANA** | Eu tava mal, tá? Fiquei sentada lá fora esfriando a cabeça. O Gabriel saiu andando na direção da estrada. | Alterada, na defensiva. |
| `C7B_J05` | Pós-Prova 5 | — | **JOGADOR (Helena)** | O celular dele tava parado a duzentos metros do galpão às três e vinte. O corpo também. Ele nunca saiu de lá. | Fria, letal. |
| `C7B_J06` | Pós-Prova 5 | `[PRIORITÁRIA]` | **JULIANA** | ...Eu quero um advogado. | Seca, fechando a guarda na hora. |
| `C7B_J07` | Pós-Prova 5 | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Quem fala a verdade para pra pensar. Ela já tava com a resposta pronta antes de eu terminar a pergunta. | Narração em off. |

---

### Rota B: Culpado = Caio
> *Sala de interrogatório (`falasCena7A_Caio` e `falasCena7B_Caio`).*

| ID | Bloco | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :--- | :---: | :--- | :--- | :--- |
| `C7A_C01` | Pré-Prova 5 | — | **JOGADOR (Helena)** | Você disse que saiu do estacionamento às duas e quarenta. | Firme. |
| `C7A_C02` | Pré-Prova 5 | — | **CAIO** | Falei. Se vocês já puxaram a câmera da saída, já viram meu carro passando lá. | Confiante. |
| `C7B_C01` | Pós-Prova 5 | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | O horário no vídeo até bate com a sua história. Só tem um problema: a imagem da cancela foi adulterada. | Direta. |
| `C7B_C02` | Pós-Prova 5 | — | **CAIO** | Adulterada? Aquela câmera vive quebrada, Helena. Deve ser erro de compressão. | Desdenhoso, tentando minimizar. |
| `C7B_C03` | Pós-Prova 5 | — | **JOGADOR (Helena)** | Pode até ser. Só que os repasses bancários na conta do Gabriel não parecem erro de sistema. | Pressionando. |
| `C7B_C04` | Pós-Prova 5 | `[PRIORITÁRIA]` | **CAIO** | Eu já falei... era uma ajuda que meu pai organizava pra ele se manter no curso. O Gabriel sempre gostou de fazer drama. | Nervoso, hesitante no início. |
| `C7B_C05` | Pós-Prova 5 | — | **JOGADOR (Helena)** | O problema é que o celular dele parou de dar sinal às três e vinte, no fundo do barranco. Bem depois da briga de vocês. | Fria, incisiva. |
| `C7B_C06` | Pós-Prova 5 | `[PRIORITÁRIA]` | **CAIO** | ...Acho melhor eu chamar o advogado da família antes de falar mais alguma coisa. | Engole seco, tenta manter a pose. |
| `C7B_C07` | Pós-Prova 5 | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Quem fala a verdade para pra pensar. Ele já tava com a defesa engatilhada antes de eu terminar a frase. | Narração em off. |

---

## ⚖️ 7. Cena 8 — Chamado para a Sala de Suspeitos

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `C8_01` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | Chegamos ao fim da coleta. Os dois suspeitos estão na sala de reconhecimento. Revise seu mural e dê seu veredito. | Solene, firme, passando a decisão final. |

---

## 🏆 8. Tribunal e os 6 Finais

> **Nota:** Nos Finais 1, 2, 3, 4 e 6, a cena se passa no **Tribunal** com **JUIZ** e **ADVOGADA**. O Final 5 se passa na **Sala de Evidências** com o **DELEGADO MEIRELES**.

### Final 1 — Verdade Absoluta (5/5 provas)

#### Rota Juliana (`falasFinalPerfeitoJuliana`)
| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F1J_01` | — | **JUIZ** | Declaro aberta a audiência de instrução contra Juliana Costa. A defesa tem a palavra. | Solene, formal. |
| `F1J_02` | `[PRIORITÁRIA]` | **ADVOGADA** | Excelência, em 2045 qualquer mídia pode ser manipulada. Pela Lei da Dúvida Digital, peço a anulação das provas eletrônicas contra minha cliente. | Técnica, firme, tom jurídico. |
| `F1J_03` | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | Todas as cinco cadeias de custódia estão intactas: a foto do carro e o áudio anônimo foram forjados por inteligência artificial, o GPS e a nuvem são íntegros e a corrida da Móbi às três e quarenta e um derruba o álibi. | Segura, implacável, ritmo claro. |
| `F1J_04` | `[PRIORITÁRIA]` | **JULIANA** | Chega! Ele entregou meu projeto de três anos pro João Paulo... eu só queria que alguém me escutasse! Eu bati, ele caiu no barranco... eu fiquei com medo! | Explosão emocional, voz embargada, desespero. |
| `F1J_05` | — | **JUIZ** | Diante das cinco provas técnicas validadas e da confissão, condeno Juliana Costa e determino inquérito contra o Professor João Paulo na Nexára. | Definitivo, autoridade máxima. |
| `F1J_06` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Em 2045, qualquer um pode fabricar uma mentira. Mas quando todas as peças fecham sem brecha, nem a maior empresa da cidade esconde a verdade. | Narração final, tom de encerramento. |

#### Rota Caio (`falasFinalPerfeitoCaio`)
| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F1C_01` | — | **JUIZ** | Declaro aberta a audiência de instrução contra Caio Vasconcelos. A defesa tem a palavra. | Solene, formal. |
| `F1C_02` | `[PRIORITÁRIA]` | **ADVOGADA** | Excelência, meu cliente apresentou vídeo de saída às duas e quarenta e dois. O restante é suposição sobre ajuda financeira estudantil. | Segura, técnica. |
| `F1C_03` | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | O vídeo da cancela foi adulterado por inteligência artificial. Os cinco laudos estão fechados: o áudio da ameaça é real, o GPS crava três e vinte e os extratos da nuvem provam a lavagem de dinheiro. | Firme, implacável. |
| `F1C_04` | `[PRIORITÁRIA]` | **CAIO** | Ele ia destruir a minha família! Tudo que meu pai construiu ia pro lixo! Eu só empurrei pra calar a boca dele... não sabia que tinha uma pedra lá embaixo! | Explosão de desespero, voz alta e quebrada. |
| `F1C_05` | — | **JUIZ** | Com os cinco laudos periciais íntegros, condeno Caio Vasconcelos e determino o bloqueio da empresa de fachada ligada à Nexára. | Definitivo, solene. |
| `F1C_06` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Em 2045, dinheiro e algoritmo compram quase tudo. Menos um álibi que resista a cinco provas fechadas. | Narração final. |

---

### Final 2 — A Verdade (4/5 provas)

#### Rota Juliana (`falasFinalVerdadeJuliana`)
| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F2J_01` | — | **JUIZ** | Iniciamos a audiência contra Juliana Costa. Defesa, manifeste-se. | Formal, direto. |
| `F2J_02` | — | **ADVOGADA** | Excelência, falta uma peça na perícia completa da acusação. Há margem para questionar a ordem cronológica. | Técnica, tentando abrir brecha. |
| `F2J_03` | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | Quatro provas técnicas validadas sustentam a cadeia de custódia. A foto tem volante inexistente, a Móbi registra três e quarenta e um e o GPS confirma que Gabriel nunca saiu do galpão. | Firme, objetiva. |
| `F2J_04` | `[PRIORITÁRIA]` | **JULIANA** | Ele chorou... pediu desculpa. Eu gritei que era tarde demais. Eu bati e ele caiu... fiquei apavorada. | Voz trêmula, ressentida e assustada. |
| `F2J_05` | — | **JUIZ** | As quatro provas convergem sem contradição. Juliana Costa está condenada pelo homicídio de Gabriel Moura. | Firme, sentença final. |
| `F2J_06` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Em 2045, qualquer um pode fabricar uma verdade. Mas a gente ainda percebe quando ela não encaixa. | Narração final. |

#### Rota Caio (`falasFinalVerdadeCaio`)
| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F2C_01` | — | **JUIZ** | Iniciamos a audiência contra Caio Vasconcelos. Defesa, manifeste-se. | Formal, direto. |
| `F2C_02` | — | **ADVOGADA** | Excelência, a acusação não fechou todos os cinco laudos do inquérito. Peço a rejeição da denúncia. | Firme, jurídica. |
| `F2C_03` | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | Quatro provas validadas fecham o cerco. Os extratos, o GPS e a inconsistência na imagem da cancela derrubam o álibi de Caio. | Firme, segura. |
| `F2C_04` | `[PRIORITÁRIA]` | **CAIO** | Ele ia acabar com tudo que meu pai construiu! Eu só empurrei pra calar a boca dele! | Desesperado, sem saída. |
| `F2C_05` | — | **JUIZ** | Com quatro laudos periciais consistentes e a confissão em audiência, Caio Vasconcelos está condenado. | Solene, definitivo. |
| `F2C_06` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Em 2045, qualquer um pode fabricar uma verdade. Mas a gente ainda percebe quando ela não encaixa. | Narração final. |

---

### Final 3 — A Dúvida (2 ou 3 provas)
> *`falasFinalDuvida` — Réu absolvido por falta de provas completas.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F3_01` | — | **JUIZ** | A acusação possui provas técnicas suficientes que uma máquina não pudesse ter fabricado? | Frio, direto. |
| `F3_02` | `[PRIORITÁRIA]` | **ADVOGADA** | Excelência, pela Lei da Dúvida Digital, qualquer indício sem cadeia de custódia completa deve ser anulado. Faltam laudos essenciais no mural da acusação. Há apenas uma narrativa. | Segura, articulada, triunfante. |
| `F3_03` | `[PRIORITÁRIA]` | **JOGADOR (Helena)** | ...Os laudos restantes ficaram pelo caminho, Excelência. | Derrotada, engolindo a frustração. |
| `F3_04` | — | **JUIZ** | Sendo assim, em nome da dúvida razoável, declaro o réu absolvido. | Seco, definitivo. |
| `F3_05` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Quatro meses depois, a Nexára anunciou sua nova liderança de Engenharia de Inteligência Artificial. Em 2045, a verdade sozinha não basta. Ela precisa chegar com a prova certa. | Narração amarga, melancólica. |

---

### Final 4 — Colapso Forense (0 ou 1 prova)
> *`falasFinalColapso` — Caso arquivado antes mesmo da instrução.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F4_01` | — | **JUIZ** | Olhando os autos enviados ao tribunal... onde estão as provas periciais validadas? | Desapontado, severo. |
| `F4_02` | — | **ADVOGADA** | Não há nenhuma sustentação técnica aqui, Excelência! A acusação trouxe um mural praticamente vazio. Peço o arquivamento sumário por falta de justa causa. | Indignada, firme. |
| `F4_03` | — | **JOGADOR (Helena)** | Mas eu sei o que aconteceu naquela noite... | Tentando insistir, sem força. |
| `F4_04` | — | **JUIZ** | Sem lastro pericial, este tribunal não pode sequer abrir instrução. Caso arquivado. | Duro, definitivo. |
| `F4_05` | — | **PENSAMENTO (Helena)** | O inquérito morreu no tribunal antes mesmo de começar. As pistas tavam na minha frente, e eu deixei escapar. | Narração frustrada. |

---

### Final 5 — A Precipitação (Acusação precoce)
> *`falasFinalPrecipitado` — Acusou cedo demais antes do confronto.*

| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F5_01` | — | **JOGADOR (Helena)** | Delegado, não preciso de mais nada. Quero levar a acusação pro tribunal agora. | Apressada, tensa. |
| `F5_02` | — | **DELEGADO MEIRELES** | Agora? A gente acabou de achar o corpo! Você nem puxou todos os registros nem confrontou o suspeito! | Incrédulo, firme. |
| `F5_03` | — | **DELEGADO MEIRELES** | Se você entrar no tribunal assim, a defesa anula tudo em cinco minutos. Vou te afastar do caso antes que você coloque tudo a perder. | Autoritário, definitivo. |
| `F5_04` | — | **PENSAMENTO (Helena)** | A pressa falou mais alto que o método. Saí do caso antes mesmo de descobrir a verdade inteira. | Narração melancólica. |

---

### Final 6 — A Injustiça (Acusou o inocente)

#### Acusou Caio (Culpada era Juliana) — `falasFinalInjusticaCaio`
| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F6C_01` | — | **JUIZ** | A acusação sustenta a denúncia contra Caio Vasconcelos. A defesa tem a palavra. | Solene, formal. |
| `F6C_02` | `[PRIORITÁRIA]` | **ADVOGADA** | Excelência, o álibi do meu cliente tem base sólida, e a acusação ignorou sinais claros de fabricação digital nas provas contra ele. Há apenas motivo aparente. | Firme, incisiva. |
| `F6C_03` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | Motivo a cidade inteira tem. Você trouxe alguém ao tribunal sem fechar as provas técnicas. | Ácido, decepcionado. |
| `F6C_04` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Semanas depois, Caio ouviu a sentença injusta com o olhar vazio. No mural da delegacia, encarei a foto da Juliana e percebi o tamanho do meu erro. | Narração pesada, arrependimento. |

#### Acusou Juliana (Culpado era Caio) — `falasFinalInjusticaJuliana`
| ID | Prioridade | Voz | Texto Pronto para IA (Copiar e Colar) | Direção / Emoção |
| :--- | :---: | :--- | :--- | :--- |
| `F6J_01` | — | **JUIZ** | A acusação sustenta a denúncia contra Juliana Costa. A defesa tem a palavra. | Solene, formal. |
| `F6J_02` | `[PRIORITÁRIA]` | **ADVOGADA** | Excelência, a foto da minha cliente no carro é autêntica e o horário bate. A acusação trouxe ressentimento, não prova. | Firme, incisiva. |
| `F6J_03` | `[PRIORITÁRIA]` | **DELEGADO MEIRELES** | Motivo a cidade inteira tem. Você trouxe alguém ao tribunal sem fechar as provas técnicas. | Ácido, decepcionado. |
| `F6J_04` | `[PRIORITÁRIA]` | **PENSAMENTO (Helena)** | Semanas depois, Juliana ouviu a sentença injusta com o olhar vazio. No mural da delegacia, encarei a foto do Caio e percebi tarde demais quem realmente me enganou. | Narração pesada, arrependimento. |

---

## 📖 9. Cutscene HQ — 5 Páginas (Narração Opcional)

No código atual (`jogo_ai.libi/cutscene.h`), a Cutscene HQ final tem **5 páginas**. As páginas 2, 3, 4 e 5 mudam dinamicamente conforme as provas validadas (`P1` a `P5`) e o final alcançado.

- **Recomendação para IA:** Se quiserem dublar só a parte fixa (estilo epílogo), comecem pela **Página 1/5 (Backstory)** narrada por **Helena (`PENSAMENTO`)**:
  - `HQ_P1_Q1`: Gabriel na faculdade: idealista no laboratório, mas pressionado por dívidas e promessas.
  - `HQ_P1_Q2`: Juliana: três anos dedicados ao próprio projeto de inteligência artificial pela vaga na Nexára.
  - `HQ_P1_Q3`: Caio: reservado e influente, bancava despesas de Gabriel pela família Vasconcelos.
  - `HQ_P1_Q4`: Por fora, colegas inseparáveis no campus de Recife em 2045.
  - `HQ_P1_Q5`: Por dentro, uma única vaga na Nexára e segredos já rachavam o trio antes da festa.
- *(Páginas 2 a 5 possuem dezenas de combinações de texto dinâmico por rota e prova validada — recomenda-se manter em legenda + trilha sonora).*

---

## ✅ 10. Resumo de Falas por Voz e Checklist

Para gerar em lote na ferramenta de IA (uma voz de cada vez):

| Ordem Sugerida | Voz | Quantidade | Observação |
| :---: | :--- | :---: | :--- |
| **1º (Essencial)** | **GABRIEL + CAIO (Prova 1)** | **3** | `P1_01`, `P1_02_IA`, `P1_02_REAL` — indispensável para o Player de Áudio do jogo. |
| **2º** | **JOGADOR + PENSAMENTO (Helena)** | **46** | Maior volume (28 falas + 18 pensamentos em off). |
| **3º** | **DELEGADO MEIRELES** | **13** | Abertura, descobertas de provas, Cena 8 e Finais 5/6. |
| **4º** | **CAIO** | **10** | *(+2 da Prova 1 acima)* — Cena 2, Cena 7 (Rota Caio) e Finais 1C/2C. |
| **5º** | **JULIANA** | **12** | Cena 4, Cena 7 (Rota Juliana) e Finais 1J/2J. |
| **6º** | **JUIZ** | **14** | Finais 1, 2, 3, 4 e 6 no Tribunal. |
| **7º** | **ADVOGADA** | **8** | Finais 1, 2, 3, 4 e 6 no Tribunal. |
| **Total** | **7 vozes** | **106** | **100% do roteiro principal + Prova 1 cobertos.** |
