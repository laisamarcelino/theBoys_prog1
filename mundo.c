#include <stdlib.h>
#include "conjunto.h"
#include "mundo.h"
#include "lef.h"
#include "fila.h"


#define T_INICIO 0
#define T_INICIO 0
#define T_FIM_DO_MUNDO 5 /* em minutos -> 525600*/
#define N_TAMANHO_MUNDO 5 /*20000*/
#define N_HABILIDADES 10
#define N_HEROIS (N_HABILIDADES * 5)
#define N_BASES (N_HEROIS / 6)
#define N_MISSOES (T_FIM_DO_MUNDO / 100)
#define DADO_VAZIO -1

int aleat (int min, int max){ 
	return min + rand() % (max+1 - min);
}

struct heroi cria_heroi (struct heroi h, int id){ //ok?
    h.id_heroi = id; /* sequencial */
    h.habilidades = cria_cjt(aleat(1,3));
    h.paciencia = aleat(0, 100);
    h.velocidade = aleat(50, 5000);
    h.experiencia = 0;
    /*h.id_base = id_base;*/

    return h;
};
/*
void cria_base (struct base b, int id, int lot, int pre, int esp, int x, int y){
    struct coordenadas local;

    b.id_base = id;
    b.lotacao = lot;
    b.presentes = pre;
    b.espera = esp;
    b->local.x = x;
    b->local.y = y;

};
*/


/*Adicionar missoes na lef
vetor de missoes
adicionar o evento fim do mundo
*/

/* LEMBRAR
destruir conjunto de habilidades
*/