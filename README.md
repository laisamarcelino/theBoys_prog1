# 🌍 Simulação de Mundo com TADs em C

Este projeto implementa uma **simulação de mundo fictício** utilizando **estruturas de dados abstratas (TADs)** em linguagem C.  
Ele combina **conjuntos, filas, listas de eventos futuros (LEF)** e um módulo principal que organiza entidades (heróis, bases e missões) para simular interações ao longo do tempo.

## 📂 Estrutura do Projeto

- **`conjunto.h / conjunto.c`**  
  Implementa o TAD **conjunto**, permitindo operações como união, interseção, diferença e iteração sobre elementos.  

- **`fila.h / fila.c`**  
  Implementa o TAD **fila** (política FIFO), usado para gerenciar heróis aguardando entrada em bases.  

- **`lef.h / lef.c`**  
  Implementa a **Lista de Eventos Futuros (LEF)**, responsável por armazenar e ordenar eventos pelo tempo lógico.  

- **`mundo.h / mundo.c`**  
  Define as entidades principais da simulação:  
  - **Heróis** (habilidades, paciência, velocidade, experiência, base atual).  
  - **Bases** (capacidade, localização, fila de espera).  
  - **Missões** (habilidades necessárias, localização).  
  - **Mundo** (reúne todas as entidades e a LEF).  
  Também implementa as funções que descrevem os eventos do mundo.  

- **`main.c`**  
  Programa principal que:  
  - Inicializa o mundo e entidades.  
  - Cria eventos iniciais.  
  - Executa a simulação até o **fim do mundo**.  
  - Trata cada evento conforme sua lógica (chegar a uma base, esperar, desistir, viajar, iniciar missão etc.).  

---

## ⚙️ Eventos Modelados

Os eventos são armazenados na **LEF** e processados em ordem de tempo. Cada evento possui:  

- `tempo` → instante lógico em que ocorre.  
- `tipo` → categoria do evento.  
- `dado1, dado2` → parâmetros auxiliares (ex: id do herói, id da base).  

### Tipos de eventos:

| Código   | Evento   | Descrição |
|----------|----------|-----------|
| `CHEGA`  | Chegada  | Um herói chega a uma base. |
| `ESPERA` | Espera   | Herói entra na fila da base. |
| `DESISTE`| Desiste  | Herói desiste e procura outra base. |
| `AVISA`  | Aviso    | Porteiro da base trata a fila. |
| `ENTRA`  | Entrada  | Herói entra em uma base. |
| `SAI`    | Saída    | Herói sai da base. |
| `VIAJA`  | Viagem   | Herói viaja para outra base. |
| `MISSAO` | Missão   | Uma missão é disparada no mundo. |
| `FIM`    | Fim      | Encerra a simulação. |

---

## 🛠️ Como Compilar e Executar

No terminal:

```bash
make
./theboys
