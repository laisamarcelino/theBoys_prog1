#include "eventos.h"
#include <stdlib.h>

int aleat (int min, int max){ 
	return min + rand() % (max+1 - min);
}

void *cria_heroi (struct heroi *h, int id, int hab, int pac, int vel, int exp, int id_base){
    h->id_heroi = id;
    h->habilidades = hab;
    h->paciencia = pac;
    h->velocidade = vel;
    h->experiencia = exp;
    h->id_base = id_base;
};

void *cria_base (struct base *b, int id, int lot, int pre, int esp, int x, int y){
    b->id_base = id;
    b->lotacao = lot;
    b->presentes = pre;
    b->espera = esp;
    b->local->x = x;
    b->local->y = y;

};

/*Adicionar missoes na lef
vetor de missoes
adicionar o evento fim do mundo
*/