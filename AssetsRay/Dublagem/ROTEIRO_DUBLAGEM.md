# 🎙️ Protocolo Moura — Guia Oficial de Dublagem e Áudio

> **Projeto:** AI.libi (Recife, 2045)  
> **Fonte:** Baseado em `jogo_ai.libi/roteiro.h` e `jogo_ai.libi/cutscene.h`  
> **Objetivo:** Documento de referência para gravação, direção de voz e nomeação dos arquivos de áudio do jogo.

---

## 📑 Sumário

1. [Ficha Técnica e Direção de Voz](#-1-ficha-técnica-e-direção-de-voz)
2. [Padrão de Arquivos e Entrega](#-2-padrão-de-arquivos-e-entrega)
3. [Áudio de Evidência — Prova 1 (Obrigatório)](#-3-áudio-de-evidência--prova-1-obrigatório)
4. [Roteiro Principal — Cenas 1 a 6 (Comum às Duas Rotas)](#-4-roteiro-principal--cenas-1-a-6-comum-às-duas-rotas)
   - [Cena 1 — A Ocorrência](#cena-1--a-ocorrência)
   - [Cena 2 — Interrogatório de Caio](#cena-2--interrogatório-de-caio)
   - [Cena 3 — O Pacote Anônimo](#cena-3--o-pacote-anônimo)
   - [Cena 4 — Interrogatório de Juliana](#cena-4--interrogatório-de-juliana)
   - [Cena 5 — O Corpo](#cena-5--o-corpo)
   - [Cena 6 — A Caixa-Preta (Backup na Nuvem)](#cena-6--a-caixa-preta-backup-na-nuvem)
5. [Cena 7 — Confronto Final (Bifurcado por Rota)](#-5-cena-7--confronto-final-bifurcado-por-rota)
   - [Rota A: Culpada = Juliana](#rota-a-culpada--juliana)
   - [Rota B: Culpado = Caio](#rota-b-culpado--caio)
6. [Cena 8 — Veredito](#-6-cena-8--veredito)
7. [Os 6 Finais](#-7-os-6-finais)
   - [Final 1 — Verdade Absoluta (5/5 provas)](#final-1--verdade-absoluta-55-provas)
   - [Final 2 — A Verdade (4/5 provas)](#final-2--a-verdade-45-provas)
   - [Final 3 — A Dúvida (2 ou 3 provas)](#final-3--a-dúvida-2-ou-3-provas)
   - [Final 4 — Colapso Forense (0 ou 1 prova)](#final-4--colapso-forense-0-ou-1-prova)
   - [Final 5 — A Precipitação (Acusação precoce)](#final-5--a-precipitação-acusação-precoce)
   - [Final 6 — A Injustiça (Acusou o inocente)](#final-6--a-injustiça-acusou-o-inocente)
8. [Cutscene HQ (Narração Opcional)](#-8-cutscene-hq-narração-opcional)
9. [Checklist de Prioridade de Gravação](#-9-checklist-de-prioridade-de-gravação)

---

## 🎭 1. Ficha Técnica e Direção de Voz

| Personagem | Perfil / Idade | Tom de Voz e Atuação | Observações |
| :--- | :--- | :--- | :--- |
| **HELENA** | Investigadora (~28 anos) | Firme, analítica, leve cansaço de madrugada. Tom mais íntimo/reflexivo nas narrações. | Protagonista — maior volume de falas. |
| **DELEGADO MEIRELES** | Delegado (~45-55 anos) | Grave, experiente, objetivo e formal. Passa urgência sem gritar. | Chefe de Helena. |
| **CAIO** | Estudante (22 anos) | Educado, reservado, defensivo sob pressão. Mexe no relógio quando nervoso. | Varia entre calma forçada e explosão no Final 1/2. |
| **JULIANA** | Estudante (21 anos) | Articulada, confiante, irônica quando pressionada. Desaba apenas no final. | Muda de postura cínica para vulnerável se confrontada. |
| **GABRIEL** | Vítima (22 anos) | Jovem, idealista, tenso na gravação da festa. | Aparece **apenas no áudio da Prova 1**. |
| **JUIZ** | Magistrado (~50 anos) | Imparcial, solene e seco. | Aparece apenas no Final 3. |
| **ADVOGADO** | Defesa (~35 anos) | Técnico, seguro, ritmo de tribuna. | Aparece apenas no Final 3. |

> **Dica de atuação:** As falas dos jovens (*Helena, Caio, Juliana, Gabriel*) usam português brasileiro natural falado (`"pra"`, `"tava"`, `"a gente"`). Mantenha a naturalidade sem forçar gírias caricatas.

---

## 📁 2. Padrão de Arquivos e Entrega

- **Formato:** `.wav` (44.1 kHz / 16-bit) ou `.ogg` / `.mp3` (192 kbps+).
- **Silêncio:** Cortar silêncio excessivo no início e no fim (~100 ms de margem).
- **Normalização:** Volume consistente em `-3 dB` a `-1 dB` (sem clipar).
- **Nomenclatura sugerida:** `<ID_DA_FALA>_<PERSONAGEM>.wav`  
  *Exemplo:* `C1_01_HELENA.wav`, `C2_04_CAIO.wav`, `F1A_02_JULIANA.wav`

As falas marcadas com **`[DUB PRIORITÁRIA]`** correspondem às falas-chave do roteiro original caso a equipe opte por gravar primeiro apenas o essencial.

---

## 🔊 3. Áudio de Evidência — Prova 1 (Obrigatório)

Esta prova é reproduzida no **Player de Áudio** do jogo (Cena 3 e Dossiê).  
**Atenção:** Precisa de **duas versões** (uma para cada rota sorteada) — ou uma edição limpa + uma edição com defeitos sutis de IA.

| ID | Rota | Falante | Texto | Direção / Efeitos de Áudio |
| :--- | :--- | :--- | :--- | :--- |
| `P1_01` | Ambas | **GABRIEL** | *"Você não vai fazer nada, Caio. Você nunca precisou fazer nada na vida."* | Som ambiente de festa abafado ao fundo, vento leve, voz próxima ao mic. |
| `P1_02_IA` | **Juliana** | **"CAIO" (IA)** | *"Se você abrir a boca, eu juro que acabo com você."* | **Efeito IA:** Sem som de respiração, corte seco nas frequências altas, leve oscilação robótica no meio da frase e música de fundo em loop a cada 4s. |
| `P1_02_REAL` | **Caio** | **CAIO (Real)** | *"Se você abrir a boca, eu juro que acabo com você."* | **Efeito Real:** Ruído de vento contínuo, respiração ofegante audível antes da frase, timbre estável e natural. |

---

## 🎬 4. Roteiro Principal — Cenas 1 a 6 (Comum às Duas Rotas)

### Cena 1 — A Ocorrência
> *Cenário: Delegacia (mesa da investigadora).*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C1_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Recife, 2045. Quem tem tecnologia vive bem. Mas hoje qualquer foto, voz ou vídeo pode ser fabricado em segundos."* | Narração em off, tom reflexivo e grave. |
| `C1_02` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"Helena, a equipe já está ouvindo quem estava na festa. Passe um pente fino nos álibis, nas imagens e nas mensagens."* | Firme, passando instruções. |
| `C1_03` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"E não se engane: não é porque uma prova parece real que ela realmente é."* | Pausa curta antes da frase, tom de alerta. |

---

### Cena 2 — Interrogatório de Caio
> *Cenário: Sala de interrogatório. Caio está nervoso e mexe no relógio.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C2_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Caio, vou direto ao ponto: você sabe onde o Gabriel está?"* | Direta, observando a reação. |
| `C2_02` | `[DUB PRIORITÁRIA]` | **CAIO** | *"Não. Não faço a menor ideia."* | Curto, tenso, rápido demais. |
| `C2_03` | — | **HELENA** | *"Uma testemunha viu vocês dois batendo boca do lado de fora da festa. O que rolou?"* | Pressionando sem levantar a voz. |
| `C2_04` | — | **CAIO** | *"A gente discutiu, sim. Ele tava bêbado... falando que ia contar uma coisa pra Juliana. Do nada começou a jogar na minha cara que eu pagava a faculdade dele pra ele me dever favor, e que a vaga dele na NEXARA só veio por indicação do meu pai. Eu falei que ele tava sendo injusto."* | Começa irritado, ganha ritmo na justificativa, termina defensivo. |
| `C2_05` | — | **HELENA** | *"E depois da briga?"* | Seca. |
| `C2_06` | `[DUB PRIORITÁRIA]` | **CAIO** | *"Fui embora. Era umas duas e quarenta. Peguei meu carro e fui direto pra casa, sozinho."* | Tentando soar calmo e convincente. |
| `C2_07` | — | **HELENA** | *"Então o seu álibi é só a sua palavra."* | Constatação fria. |
| `C2_08` | — | **CAIO** | *"...É. Eu sei como isso soa. Meu relógio até ficou sem bateria na hora, nem registrou o trajeto."* | Hesitação no início, frustrado consigo mesmo. |
| `C2_09` | — | **HELENA** | *"Tudo bem. Vamos puxar a câmera da saída do estacionamento. Se você saiu mesmo às duas e quarenta, ela vai mostrar."* | Encerrando o bloco. |

---

### Cena 3 — O Pacote Anônimo
> *Cenário: Delegacia. Delegado chega com uma caixa lacrada.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C3_01` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"Acabou de chegar numa entrega anônima. Tem um dispositivo com áudio aí dentro. Mandaram um bilhete dizendo que explica a briga entre o Caio e o Gabriel."* | Sério, intrigado. |
| `C3_02` | — | **HELENA** | *"Quem grava uma briga no meio da festa e manda anonimamente pra polícia?"* | Desconfiada. |
| `C3_03` | — | **DELEGADO MEIRELES** | *"Alguém que quer que a gente ouça. Ou alguém que precisa muito que a gente acredite."* | Tom enigmático. |

#### Pós-Análise da Prova 1 (varia conforme a rota)
| ID | Rota | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `C3_POS_A` | **Juliana** | `[DUB PRIORITÁRIA]` | **HELENA** | *"Forjaram uma ameaça com a voz do Caio. Quem fez isso queria desviar o meu olhar pra ele... e eu preciso descobrir por quê."* | Pensamento/narração em off. |
| `C3_POS_B` | **Caio** | `[DUB PRIORITÁRIA]` | **HELENA** | *"A gravação é limpa. O Caio mentiu quando disse que foi só uma discussão boba. Ele ameaçou o Gabriel de verdade."* | Pensamento/narração em off, mais incisiva. |

---

### Cena 4 — Interrogatório de Juliana
> *Cenário: Sala de interrogatório. Juliana chega com celular de tela trincada.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C4_01` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Desculpa, esse celular trava o tempo todo. Comprei de segunda mão, sem garantia nem nada."* | Simpática, tentando parecer simples/desarmada. |
| `C4_02` | — | **HELENA** | *"Você era próxima do Gabriel?"* | Neutra. |
| `C4_03` | — | **JULIANA** | *"Ele é meu melhor amigo. A gente entrou junto na faculdade."* | Natural, firme. |
| `C4_04` | — | **HELENA** | *"Chegou a falar com ele na festa?"* | Investigativa. |
| `C4_05` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"A gente conversou rapidinho, depois eu voltei pra pista e não vi mais ele."* | Casual, ensaiada. |
| `C4_06` | — | **HELENA** | *"O Caio falou que o Gabriel queria te contar alguma coisa naquela noite. Vocês tinham brigado?"* | Jogando a isca. |
| `C4_07` | — | **JULIANA** | *"Brigamos, sim. Eu descobri que o Gabriel conseguiu a vaga na NEXARA porque tava fechado com o professor João Paulo. Eu ralei três anos por aquela vaga e saí de mãos abanando."* | Amargura contida, ressentimento real. |
| `C4_08` | — | **HELENA** | *"E o que você acha do Caio no meio disso tudo?"* | Observando. |
| `C4_09` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Pra quem sempre teve tudo de mão beijada, é muito fácil pedir pra gente 'ouvir o outro lado'."* | Irônica, cínica. |
| `C4_10` | — | **HELENA** | *"Até que horas você ficou na festa?"* | Direta. |
| `C4_11` | — | **JULIANA** | *"Até umas três da manhã. Pedi um carro de aplicativo e fui embora. Até mandei uma foto pra minha mãe de dentro do carro. Pode olhar. Eu faço questão."* | Confiante até demais no final. |

#### Pós-Análise da Prova 2 (varia conforme a rota)
| ID | Rota | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `C4_POS_A` | **Juliana** | `[DUB PRIORITÁRIA]` | **HELENA** | *"Uma prova falsa pra queimar o Caio. Outra falsa pra proteger a Juliana. Alguém tá montando esse tabuleiro peça por peça."* | Narração em off. |
| `C4_POS_B` | **Caio** | `[DUB PRIORITÁRIA]` | **HELENA** | *"A foto é autêntica. Mesmo furiosa com o Gabriel, a Juliana realmente entrou no carro às três e cinco."* | Narração em off. |

---

### Cena 5 — O Corpo
> *Cenário: Delegacia. Delegado entra e fecha a porta.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C5_01` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"Helena... acharam o Gabriel. Infelizmente, encontraram o corpo dele num barranco, a duzentos metros do galpão."* | Pesado, sombrio. |
| `C5_02` | — | **HELENA** | *"Praticamente do lado da festa?"* | Surpresa, tensa. |
| `C5_03` | — | **DELEGADO MEIRELES** | *"A perícia cravou a morte na própria madrugada, entre 2h50 e 3h05. Ele levou um golpe na têmpora antes de cair. A partir de agora, estamos lidando com homicídio."* | Sério, profissional. |

---

### Cena 6 — A Caixa-Preta (Backup na Nuvem)
> *Cenário: Delegacia. Terminal de análise forense.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C6_01` | — | **DELEGADO MEIRELES** | *"A perícia digital conseguiu resgatar o último backup automático da conta do Gabriel, minutos antes de o aparelho perder o sinal."* | Objetivo. |
| `C6_02` | — | **HELENA** | *"Se ele ia abrir o jogo com alguém naquela noite, tem alguma pista guardada aí."* | Focada. |

#### Pós-Análise da Prova 4 (varia conforme a rota)
| ID | Rota | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `C6_POS_A` | **Juliana** | `[DUB PRIORITÁRIA]` | **HELENA** | *"Ele entregou três anos de trabalho da Juliana de bandeja. Se ela soube disso na festa, tinha motivo de sobra pra perder a cabeça."* | Narração em off. |
| `C6_POS_B` | **Caio** | `[DUB PRIORITÁRIA]` | **HELENA** | *"Não era favor nenhum. O Caio usava o Gabriel de laranja pra lavar dinheiro e comprar a vaga na NEXARA. Se o Gabriel falasse, o Caio ia pra cadeia."* | Narração em off. |

---

## ⚡ 5. Cena 7 — Confronto Final (Bifurcado por Rota)

### Rota A: Culpada = Juliana
> *Cenário: Sala de interrogatório. Juliana está visivelmente mais cansada.*

| ID | Bloco | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `C7A_J01` | Pré-Prova 5 | — | **HELENA** | *"Você garantiu que saiu da festa às três, de carro de aplicativo."* | Firme. |
| `C7A_J02` | Pré-Prova 5 | — | **JULIANA** | *"Garanti. E já te mostrei a foto."* | Impaciente. |
| `C7B_J01` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **HELENA** | *"A sua corrida na Mobi só foi pedida às três e quarenta e um. E os carros da frota não têm volante, diferente da foto que você me mostrou."* | Acusação técnica, implacável. |
| `C7B_J02` | Pós-Prova 5 | — | **JULIANA** | *"Tá, eu fiquei mais um pouco... perdi a noção da hora."* | Tentando improvisar. |
| `C7B_J03` | Pós-Prova 5 | — | **HELENA** | *"Quarenta minutos perdidos no relógio?"* | Irônica, pressionando. |
| `C7B_J04` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Eu tava mal, tá? Fiquei sentada lá fora esfriando a cabeça. O Gabriel saiu andando na direção da estrada."* | Alterada, na defensiva. |
| `C7B_J05` | Pós-Prova 5 | — | **HELENA** | *"O celular dele tava parado a duzentos metros do galpão às 3h20. O corpo também. Ele nunca saiu de lá."* | Fria, letal. |
| `C7B_J06` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **JULIANA** | *"...Eu quero um advogado."* | Seca, fechando a guarda na hora. |
| `C7B_J07` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **HELENA** | *"Quem fala a verdade para pra pensar. Ela já tava com a resposta pronta antes de eu terminar a pergunta."* | Narração em off. |

---

### Rota B: Culpado = Caio
> *Cenário: Sala de interrogatório. Caio tenta manter a postura.*

| ID | Bloco | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- | :--- |
| `C7A_C01` | Pré-Prova 5 | — | **HELENA** | *"Você disse que saiu do estacionamento às duas e quarenta."* | Firme. |
| `C7A_C02` | Pré-Prova 5 | — | **CAIO** | *"Falei. Se vocês já puxaram a câmera da saída, já viram meu carro passando lá."* | Confiante. |
| `C7B_C01` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **HELENA** | *"O horário no vídeo até bate com a sua história. Só tem um problema: a imagem da cancela foi adulterada."* | Direta. |
| `C7B_C02` | Pós-Prova 5 | — | **CAIO** | *"Adulterada? Aquela câmera vive quebrada, Helena. Deve ser erro de compressão."* | Desdenhoso, tentando minimizar. |
| `C7B_C03` | Pós-Prova 5 | — | **HELENA** | *"Pode até ser. Só que os repasses bancários na conta do Gabriel não parecem erro de sistema."* | Pressionando. |
| `C7B_C04` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **CAIO** | *"Eu já falei... era uma ajuda que meu pai organizava pra ele se manter no curso. O Gabriel sempre gostou de fazer drama."* | Nervoso, gaguejando leve no início. |
| `C7B_C05` | Pós-Prova 5 | — | **HELENA** | *"O problema é que o celular dele parou de dar sinal às 3h20, no fundo do barranco. Bem depois da briga de vocês."* | Fria, incisiva. |
| `C7B_C06` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **CAIO** | *"...Acho melhor eu chamar o advogado da família antes de falar mais alguma coisa."* | Engole seco, tenta manter pose. |
| `C7B_C07` | Pós-Prova 5 | `[DUB PRIORITÁRIA]` | **HELENA** | *"Quem fala a verdade para pra pensar. Ele já tava com a defesa engatilhada antes de eu terminar a frase."* | Narração em off. |

---

## ⚖️ 6. Cena 8 — Veredito

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `C8_01` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"Chegamos ao fim, Helena. Suspeita não condena ninguém, e uma boa história nem sempre é verdade. Quem matou Gabriel Moura?"* | Solene, aguardando a decisão. |

---

## 🏆 7. Os 6 Finais

### Final 1 — Verdade Absoluta (5/5 provas)

#### Rota Juliana
| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F1J_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"O Gabriel te chamou nos fundos do galpão pra admitir que entregou o seu projeto ao professor. Vocês brigaram. Depois você voltou, gerou uma foto falsa no carro e, no dia seguinte, forjou um áudio com a voz do Caio."* | Firme, montando o quebra-cabeça na cara dela. |
| `F1J_02` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Ele chorou, sabe? Pediu desculpa, disse que ia denunciar o João Paulo... Eu gritei que era tarde demais. Ele segurou meu braço e falou 'me escuta, Ju'. Eu só queria que alguém me escutasse primeiro."* | Voz embargada, raiva misturada com choro. |
| `F1J_03` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Eu bati. Ele caiu no barranco. Fiquei paralisada olhando... e o pânico tomou conta."* | Baixo, confessando. |
| `F1J_04` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Juliana Costa, você está presa pelo homicídio de Gabriel Moura."* | Oficial, solene. |
| `F1J_05` | — | **DELEGADO MEIRELES** | *"Trabalho impecável, Helena. Com todas as provas validadas, o Ministério Público também abriu inquérito contra o professor João Paulo e o processo seletivo da NEXARA."* | Orgulhoso, aliviado. |
| `F1J_06` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Em 2045, qualquer um pode fabricar uma mentira. Mas quando cada peça fecha sem brecha, nem a maior empresa da cidade consegue esconder a verdade."* | Narração final, tom de encerramento. |

#### Rota Caio
| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F1C_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"O Gabriel descobriu que tava sendo usado de laranja no esquema do seu pai e ia entregar tudo naquela noite. Você foi atrás dele até o barranco e depois usou a tecnologia da NEXARA pra forjar sua saída na câmera."* | Firme, implacável. |
| `F1C_02` | `[DUB PRIORITÁRIA]` | **CAIO** | *"Ele ia acabar com a minha família, Helena! Tudo que o meu pai construiu ia pro lixo por causa de um cara que eu ajudei! Eu só empurrei pra calar a boca dele... eu não sabia que tinha uma pedra lá embaixo!"* | Explosão de desespero, voz alta e quebrada. |
| `F1C_03` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Caio Vasconcelos, você está preso pelo homicídio de Gabriel Moura."* | Oficial, solene. |
| `F1C_04` | — | **DELEGADO MEIRELES** | *"Excelente trabalho, Helena. A perícia fechou os cinco laudos sem nenhuma falha. Os extratos da nuvem também derrubaram a empresa de fachada e o esquema do pai dele na NEXARA."* | Satisfeito, respeitoso. |
| `F1C_05` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Em 2045, dinheiro e algoritmo compram quase tudo. Menos um álibi que resista a cinco provas fechadas."* | Narração final. |

---

### Final 2 — A Verdade (4/5 provas)

#### Rota Juliana
| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F2J_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"O Gabriel te chamou nos fundos do galpão pra contar que entregou o seu projeto ao professor. Vocês discutiram. Depois você voltou pra festa, montou uma foto falsa e forjou o áudio com a voz do Caio."* | Firme. |
| `F2J_02` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Ele chorou, sabe? Pediu desculpa, disse que ia acusar o João Paulo. Eu gritei que era tarde demais. Ele me segurou, falou 'me escuta, Ju'... e eu só queria que alguém me escutasse primeiro."* | Emocionada, ressentida. |
| `F2J_03` | `[DUB PRIORITÁRIA]` | **JULIANA** | *"Eu bati. Ele caiu. Fiquei olhando o buraco e me deu um medo absurdo."* | Baixo, trêmula. |
| `F2J_04` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Juliana Costa, você está presa pelo homicídio de Gabriel Moura."* | Oficial. |
| `F2J_05` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Em 2045, qualquer um pode fabricar uma verdade. Mas a gente ainda percebe quando ela não encaixa."* | Narração final. |

#### Rota Caio
| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F2C_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"O Gabriel descobriu que era laranja no esquema do seu pai e ia denunciar tudo naquela noite. Você seguiu ele até o barranco e depois forjou a gravação do estacionamento."* | Firme. |
| `F2C_02` | `[DUB PRIORITÁRIA]` | **CAIO** | *"Ele ia destruir a minha família, Helena! Tudo que o meu pai construiu ia virar pó por causa de um moleque que eu ajudei! Eu só empurrei pra calar a boca dele... não sabia que tinha uma pedra ali embaixo."* | Desesperado. |
| `F2C_03` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Caio Vasconcelos, você está preso pelo homicídio de Gabriel Moura."* | Oficial. |
| `F2C_04` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Em 2045, qualquer um pode fabricar uma verdade. Mas a gente ainda percebe quando ela não encaixa."* | Narração final. |

---

### Final 3 — A Dúvida (2 ou 3 provas)
> *Cenário: Audiência por videochamada na delegacia.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F3_01` | — | **JUIZ** | *"A acusação possui alguma prova técnica que uma máquina não pudesse ter fabricado?"* | Frio, direto. |
| `F3_02` | `[DUB PRIORITÁRIA]` | **ADVOGADO** | *"Excelência, pela Lei da Dúvida Digital, qualquer indício sem cadeia de custódia completa deve ser anulado. A acusação trouxe uma suspeita e uma narrativa. Nada além disso."* | Seguro, articulado, tom jurídico. |
| `F3_03` | `[DUB PRIORITÁRIA]` | **HELENA** | *"...Não, Excelência. Ainda não."* | Derrotada, engolindo a frustração. |
| `F3_04` | — | **JUIZ** | *"Sendo assim, em nome da dúvida razoável, declaro a absolvição."* | Martelo batido, definitivo. |
| `F3_05` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Quatro meses depois, a NEXARA anunciou sua nova liderança de Engenharia de IA. Em 2045, a verdade sozinha não basta. Ela precisa chegar com a prova certa, na hora certa."* | Narração amarga, melancólica. |

---

### Final 4 — Colapso Forense (0 ou 1 prova)
> *Cenário: Delegacia.*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F4_01` | — | **DELEGADO MEIRELES** | *"Helena, eu olhei o seu dossiê inteiro. Praticamente não temos provas validadas pela perícia."* | Desapontado, sério. |
| `F4_02` | — | **HELENA** | *"Mas eu sei quem foi, Delegado. A história bate."* | Tentando insistir. |
| `F4_03` | — | **DELEGADO MEIRELES** | *"Se eu mandar esse inquérito pro Ministério Público nesse estado, eles devolvem em dez minutos. Sem laudo válido, não existe processo."* | Duro, realista. |
| `F4_04` | — | **HELENA** | *"O inquérito foi arquivado por falta de lastro técnico. A pista tava na minha frente, e eu deixei escapar pelos dedos."* | Narração frustrada. |

---

### Final 5 — A Precipitação (Acusação precoce)
> *Cenário: Delegacia (Cenas 5 ou 6).*

| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F5_01` | — | **HELENA** | *"Delegado, não preciso de mais nada. Quero formalizar a acusação agora."* | Apressada, tensa. |
| `F5_02` | — | **DELEGADO MEIRELES** | *"Agora? A gente acabou de achar o corpo, Helena! Você nem puxou todos os registros nem confrontou o suspeito!"* | Incrédulo, firme. |
| `F5_03` | — | **HELENA** | *"Se a gente esperar mais, vão sumir com o resto das provas."* | Defensiva. |
| `F5_04` | — | **DELEGADO MEIRELES** | *"Uma acusação precipitada destrói a cadeia de custódia e abre brecha pra anulação imediata. Vou te afastar do caso antes que você coloque tudo a perder."* | Autoritário, definitivo. |
| `F5_05` | — | **HELENA** | *"A pressa falou mais alto que o método. Saí do caso antes mesmo de descobrir a verdade inteira."* | Narração melancólica. |

---

### Final 6 — A Injustiça (Acusou o inocente)

#### Acusou Caio (Culpada era Juliana)
| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F6C_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Confirmo minha acusação contra Caio Vasconcelos."* | Firme, porém enganada. |
| `F6C_02` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"O álibi dele parava de pé, o áudio tinha sinais de manipulação... o que a gente tinha de concreto contra ele?"* | Cético, questionando. |
| `F6C_03` | — | **HELENA** | *"Motivo. A dívida, o pai, a briga na festa."* | Seca. |
| `F6C_04` | — | **DELEGADO MEIRELES** | *"Motivo a cidade inteira tem, Helena. Prova, não."* | Ácido, aviso final. |
| `F6C_05` | — | **HELENA** | *"Semanas depois, Caio ouviu a sentença com o olhar vazio. Na delegacia, olhei a foto da Juliana na parede e percebi o tamanho do meu erro."* | Narração pesada, arrependimento. |

#### Acusou Juliana (Culpado era Caio)
| ID | Prioridade | Falante | Texto | Direção |
| :--- | :--- | :--- | :--- | :--- |
| `F6J_01` | `[DUB PRIORITÁRIA]` | **HELENA** | *"Confirmo minha acusação contra Juliana Costa."* | Firme, porém enganada. |
| `F6J_02` | `[DUB PRIORITÁRIA]` | **DELEGADO MEIRELES** | *"A foto dela no carro era autêntica, o álibi fechava... o que a gente tinha de concreto contra ela?"* | Cético, questionando. |
| `F6J_03` | — | **HELENA** | *"Motivo. A vaga na NEXARA, o ressentimento."* | Seca. |
| `F6J_04` | — | **DELEGADO MEIRELES** | *"Motivo a cidade inteira tem, Helena. Prova, não."* | Ácido, aviso final. |
| `F6J_05` | — | **HELENA** | *"Semanas depois, Juliana ouviu a sentença com o olhar vazio. Na delegacia, encarei a foto do Caio na parede e percebi tarde demais quem realmente me enganou."* | Narração pesada, arrependimento. |

---

## 📖 8. Cutscene HQ (Narração Opcional)

Se a equipe quiser narrar também os quadrinhos da **Cutscene HQ** (`cutscene.h`), todos os textos são narrados por **HELENA** (tom de documentário policial / epílogo):

- **Página 1 (Backstory na Faculdade):**
  - `HQ_P1_Q1`: *"No curso de Computação, Gabriel virava madrugadas no laboratório. Idealista, mas pressionado por dívidas e promessas."*
  - `HQ_P1_Q2`: *"Juliana dedicou três anos ao próprio projeto de IA, acreditando que o esforço sozinho garantiria a vaga na NEXARA."*
  - `HQ_P1_Q3`: *"Caio, herdeiro reservado, pagava a mensalidade de Gabriel e abria portas através da influência do pai na NEXARA."*
  - `HQ_P1_Q4`: *"Por fora, eram colegas inseparáveis no campus de Recife em 2045. Por dentro, uma única vaga na NEXARA e segredos de família já rachavam a amizade antes mesmo da festa."*

*(As Páginas 2, 3 e 4 da HQ mudam dinamicamente conforme as provas que o jogador validou ou perdeu — recomenda-se deixar em texto com trilha sonora para evitar dezenas de variações de gravação).*

---

## ✅ 9. Checklist de Prioridade de Gravação

Para fechar uma versão funcional rápida com voz:
1. **Fase 1 (Essencial para o gameplay):** Áudio da **Prova 1** (`P1_01`, `P1_02_IA`, `P1_02_REAL`).
2. **Fase 2 (Falas-chave `[DUB PRIORITÁRIA]`):** ~35 falas distribuídas entre Helena, Delegado Meireles, Caio, Juliana e Advogado.
3. **Fase 3 (Jogo 100% dublado):** Todas as demais falas das Cenas 1 a 8 e dos 6 Finais.
