#include <stdio.h>
#include "conjunto.h"
#include "eventos.h"

#define T_INICIO 0;
#define T_FIM_DO_MUNDO 3; /* Mudar p 525600*/
#define T_INICIO 0;
#define T_FIM_DO_MUNDO 5; /* em minutos -> 525600*/
#define N_TAMANHO_MUNDO 5; /*20000*/
#define N_HABILIDADES 10;
#define N_HEROIS (N_HABILIDADES * 5);
#define N_BASES (N_HEROIS / 6);
#define N_MISSOES (T_FIM_DO_MUNDO / 100);

/* funcoes que voce ache necessarias aqui */
int aleat (int min, int max){ 
	return min + rand() % (max+1 - min);
}

int main (){
    srand (0); /* use zero, nao faca com time (0) */
    
    /*declarar variaveis*/
    int relogio = 0;

    /* iniciar as entidades e atributos do mundo */
    struct heroi *h;
    struct coordenadas *local;
    struct base *b;

    /* criar os eventos iniciais */
    
    

    /* coloque seu codigo aqui */

    return 0;
}
