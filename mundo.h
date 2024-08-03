/*
 * Arquivo cabeçalho que define entidades, eventos
   e funções auxiliares
 *
*/
#ifndef _mundo_H
#define _mundo_H 

#include "lef.h"

#define T_INICIO 0
#define T_FIM_DO_MUNDO 5 /* em minutos -> 525600*/
#define N_TAMANHO_MUNDO 5 /*20000*/
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
    struct base base;
};

/* Representa cada missão */
struct missao {
    int id_missao;
    struct conjunto *habilidades; 
    struct coordenadas local;
};

/* Mundo é deﬁnido pelas entidades acima e algumas informações gerais */
struct mundo {
    struct heroi herois[N_HEROIS]; /* Vetor de structs */
    struct base bases[N_BASES]; 
    struct missao missoes[N_MISSOES]; 
    struct coordenadas tamanho_mundo; /* Dimensão max do mundo */
    int n_herois; 
    int n_bases;
    int n_missoes;
    int n_habilidades;
    int relogio;
};

/* Retorna um número aleatorio entre max e min */
int aleat (int min, int max);

/* Calcula distancia entre dois pontos */
int distancia (struct coordenadas A, struct coordenadas B);

/* Inicializa um herói */
struct heroi cria_heroi (struct heroi h, int id);

/* Inicializa cada base */
struct base cria_base(struct base b, int id);

/* Inicializa cada missão */
struct missao cria_missao(struct missao m, int id);

/* Inicializa o mundo */
void cria_mundo (struct mundo *mundo, struct lef_t *l);

/* Representa um herói H chegando em uma base B no instante T. Ao chegar, 
o herói analisa o tamanho da fila e decide se espera para entrar ou desiste */
void chega (int t, struct mundo m, int h, int b, struct lef_t *l);

/* O herói H entra na fila de espera da base B. Assim que H entrar na fila, 
o porteiro da base B deve ser avisado para verificar a fila */
void espera (int t, struct mundo m, int h, int b, struct lef_t *l);

/* O herói H desiste de entrar na base B, escolhe uma base aleatória D e viaja para D*/
void desiste (int t, struct mundo m, int h, int b, struct lef_t *l);

/* O porteiro da base B trata a fila de espera */
void avisa (int t, struct base b, struct lef_t *l);

/* O herói H entra na base B. Ao entrar, o herói decide 
quanto tempo vai ficar e agenda sua saída da base */
void entra (int t, struct heroi h, struct base b, struct lef_t *l);

/* herói H sai da base B. Ao sair, escolhe uma base de destino para viajar; 
o porteiro de B é avisado, pois uma vaga foi liberada: */
void sai (int t, struct heroi h, struct base b, struct lef_t *l);

/* O herói H se desloca para uma base D */
void viaja (int t, struct heroi h, struct base d, struct lef_t *l);

/* Dispara uma missão m no instante t */
void missao (int t, struct missao m, struct lef_t *l);

/* Encerra a simulação */
void fim (int t);

#endif