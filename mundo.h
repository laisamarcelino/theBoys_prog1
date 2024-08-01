/*
 * Arquivo cabeçalho que define entidades, eventos
   e funções auxiliares
 *
*/

#ifndef _ev_t_H
#define _ev_t_H 

/* Representa cada herói */
struct heroi {
    int id_heroi;
    struct conjunto *habilidades; /*usar conjuntos*/
    int paciencia;
    int velocidade;
    int experiencia;
    int id_base;
};

/* Localização da base */
struct coordenadas {
    int x;
    int y;
};

/* Representa cada local frequentado pelos heróis para formar equipes */
struct base {
    int id_base;
    int lotacao; 
    int *presentes; /*é um conjunto*/
    int *espera; /*é uma fila*/
    struct coordenadas local;
};

/* Representa cada missão */
struct missao {
    int id_missao;
    int *habilidades; 
    struct coordenadas local;
};

/* Mundo é deﬁnido pelas entidades acima e algumas informações gerais */
struct mundo {
    int n_herois; 
    int *vetor_herois; 
    int n_bases;
    int *vetor_bases; 
    int n_missoes;
    int n_habilidades;
    struct coordenadas tamanho_mundo;
    int relogio;
};

/* Retorna um número aleatorio entre max e min */
int aleat (int min, int max);

/* Inicializa um herói */
struct heroi cria_heroi (struct heroi h, int id);

/* Inicializa cada base */
void cria_base(struct base b, int id, int lot, int pre, int esp, int x, int y);

/* Inicializa cada missão */
void cria_missao(int n_missoes, struct missao m);

/* Representa um herói H chegando em uma base B no instante T. Ao chegar, 
o herói analisa o tamanho da fila e decide se espera para entrar ou desiste */
void chega (int t, struct heroi h, struct base b);

/* O herói H entra na fila de espera da base B. Assim que H entrar na fila, 
o porteiro da base B deve ser avisado para verificar a fila */
void espera (int t, struct heroi h, struct base b);

/* O herói H desiste de entrar na base B, escolhe uma base aleatória D e viaja para D*/
void desiste (int t, struct heroi h, struct base b);

/* O porteiro da base B trata a fila de espera */
void avisa (int t, struct base b);

/* O herói H entra na base B. Ao entrar, o herói decide 
quanto tempo vai ficar e agenda sua saída da base */
void entra (int t, struct heroi h, struct base b);

/* herói H sai da base B. Ao sair, escolhe uma base de destino para viajar; 
o porteiro de B é avisado, pois uma vaga foi liberada: */
void sai (int t, struct heroi h, struct base b);

/* O herói H se desloca para uma base D */
void viaja (int t, struct heroi h, struct base d);

/* Dispara uma missão m no instante t */
void missao (int t, struct missao m);

/* Encerra a simulação */
void fim (int t);

#endif