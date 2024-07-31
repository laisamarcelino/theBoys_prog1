/*
 * Arquivo cabeçalho que define entidades e eventos
 *
*/

#ifndef _ev_t_H
#define _ev_t_H /*descobrir pra que isso aq*/

/* Representa cada herói */
struct heroi {
    int id_heroi;
    int habilidades; //deve ser vetor, como?
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
    int presentes; //deve ser vetor, como?
    int espera; //deve ser vetor, como?
    struct coordenadas *local;
};

/* Representa cada missão */
struct missao {
    int id_missao;
    int habilidades; //deve ser vetor, como?
    struct coordenadas *local;
};

/* Mundo é deﬁnido pelas entidades acima e algumas informações gerais */
struct mundo {
    int n_herois;
    int vetor_herois; //deve ser vetor, como?
    int n_bases;
    int vetor_bases; //deve ser vetor, como]
    int n_missoes;
    int n_habilidades;
    struct coordenadas *tamanho_mundo;
    int relogio;
};

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