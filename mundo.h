/*
 * Arquivo cabeçalho que define entidades, eventos
   e funções auxiliares
 *
*/
#ifndef _mundo_H
#define _mundo_H 

#include "lef.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO  525600 /* em minutos -> 525600*/
#define N_TAMANHO_MUNDO 20000 
#define N_HABILIDADES 10
#define N_HEROIS N_HABILIDADES * 5
#define N_BASES N_HEROIS / 6
#define N_MISSOES T_FIM_DO_MUNDO / 100
#define DADO_NULO -1
#define BASE_NULA -2
#define CHEGA 0
#define ESPERA 1
#define DESISTE 2
#define AVISA 3
#define ENTRA 4
#define SAI 5
#define VIAJA 6
#define MISSAO 7
#define FIM 8

/* Localização da base */
struct coordenadas {
    int x;
    int y;
};

/* Representa cada local frequentado pelos heróis para formar equipes */
struct base {
    int id_base;
    int lotacao; 
    struct conjunto *presentes; 
    struct fila *espera; 
    struct coordenadas local;
};

/* Representa cada herói */
struct heroi {
    int id_heroi;
    struct conjunto *habilidades; 
    int paciencia;
    int velocidade;
    int experiencia;
    int id_base;
};

/* Representa cada missão */
struct missao {
    int id_missao;
    struct conjunto *habilidades; 
    struct coordenadas local;
    int tentativas;
};

/* Mundo é deﬁnido pelas entidades acima e algumas informações gerais */
struct mundo {
    struct heroi herois[N_HEROIS]; /* Vetor de structs */
    struct base bases[N_BASES]; 
    struct missao missoes[N_MISSOES]; 
    struct coordenadas tamanho_mundo; /* Dimensão max do mundo */
    struct conjunto *habilidades;
    struct lef_t *lef;
    int n_herois; 
    int n_bases;
    int n_missoes;
    int n_habilidades;
    int relogio;
    int n_missoes_cumpridas;
    int min_tentativas;
    int max_tentativas;
    int total_tentativas;
};

/* Retorna um número aleatorio entre max e min */
int aleat (int min, int max);

/* Calcula distancia entre dois pontos */
int distancia (struct coordenadas A, struct coordenadas B);

/* Inicializa um herói */
struct heroi cria_heroi (int id, struct conjunto *habilidades);

/* Inicializa cada base */
struct base cria_base(int id);

/* Inicializa cada missão */
struct missao cria_missao(int id, struct conjunto *habilidades);

/* Inicializa o mundo, inclusive os herois, bases e missoes */
void cria_mundo (struct mundo *mundo);

/* Retorna o valor do relogio do mundo. Evita violar o TAD */
int relogio_mundo (struct mundo *mundo);

/* Retorna a LEF do mundo. Evita violar o TAD */
struct lef_t *lef_mundo (struct mundo *mundo);

/* Representa um herói H chegando em uma base B no instante T. Ao chegar, 
o herói analisa o tamanho da fila e decide se espera para entrar ou desiste */
void chega (int t, struct mundo *mundo, int h, int b);

/* O herói H entra na fila de espera da base B. Assim que H entrar na fila, 
o porteiro da base B deve ser avisado para verificar a fila */
void espera (int t, struct mundo *mundo, int h, int b);

/* O herói H desiste de entrar na base B, escolhe uma base aleatória D e viaja*/
void desiste (int t, struct mundo *mundo, int h, int b);

/* O porteiro da base B trata a fila de espera */
void avisa (int t, struct mundo *mundo, int b);

/* O herói H entra na base B. Ao entrar, o herói decide 
quanto tempo vai ficar e agenda sua saída da base */
void entra (int t, struct mundo *mundo, int h, int b);

/* herói H sai da base B. Ao sair, escolhe uma base de destino para viajar; 
o porteiro de B é avisado, pois uma vaga foi liberada: */
void sai (int t, struct mundo *mundo, int h, int b);

/* O herói H se desloca para uma base D */
void viaja (int t, struct mundo *mundo, int h, int d);

/* Dispara uma missão m no instante t */
void missao (int t, struct mundo *mundo, int m);

/* Encerra a simulação */
void fim (struct mundo *mundo);

#endif