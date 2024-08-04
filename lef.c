#include "lef.h"
#include <stdlib.h>
#include <stdio.h>

struct evento_t *cria_evento (int tempo, int tipo, int dado1, int dado2){
     struct evento_t *evento_t;

	evento_t = malloc(sizeof(struct evento_t)); 

	if (evento_t == NULL)
		return NULL;

	evento_t->tempo = tempo;
    evento_t->tipo = tipo; 
    evento_t->dado1 = dado1;  
    evento_t->dado2 = dado2; 

	return evento_t;
}

struct evento_t *destroi_evento (struct evento_t *e){
    free(e);
	(e) = NULL; 

    return NULL;
}

struct lef_t *cria_lef (){ 
    struct lef_t *lef_t;

	lef_t = malloc(sizeof(struct lef_t)); 

	if (lef_t == NULL)
		return NULL;

	lef_t->primeiro = NULL; 

	return lef_t;
}

struct lef_t *destroi_lef (struct lef_t *l){
    struct nodo_lef_t *aux;

	while (l->primeiro != NULL){
		aux = l->primeiro;
        destroi_evento(aux->evento);
		l->primeiro = l->primeiro->prox;
		free(aux);
	}

	free(l);
	(l) = NULL;

    return NULL;
}

int insere_lef (struct lef_t *l, struct evento_t *e){
    struct nodo_lef_t *novo, *aux;

	novo = malloc(sizeof(struct nodo_lef_t));

	if (novo == NULL)
		return 0;

	novo->evento = e;
    
	/* Caso 1: lef vazia ou é 1° elemento*/
	if (vazia_lef(l) || e->tempo <= l->primeiro->evento->tempo){
        novo->prox = l->primeiro;
		l->primeiro = novo;
		return 1;
	}

	/* Caso 2: lef possui apenas 1 elemento */
	if (l->primeiro->prox == NULL){
		novo->prox = NULL;
		l->primeiro->prox = novo;
		return 1;
	} 

	/* Caso 3: lef possui 2 ou mais elementos */
	aux = l->primeiro;
	while (aux->prox != NULL && e->tempo >= aux->prox->evento->tempo)
		aux = aux->prox;

	novo->prox = aux->prox;
	aux->prox = novo;

	return 1;
}

struct evento_t *retira_lef (struct lef_t *l){
    struct nodo_lef_t *aux;
    struct evento_t *e;

        if (vazia_lef(l))
            return NULL;

        e = l->primeiro->evento;
        aux = l->primeiro;
        l->primeiro = l->primeiro->prox;
        free(aux);
        aux = NULL;

        return e;
    }

    int vazia_lef (struct lef_t *l){
        if (l->primeiro == NULL){
            return 1;
        }
        return 0;
    }

void imprime_lef (struct lef_t *l){
    struct nodo_lef_t *aux; 
    int cont = 0;

    if (l->primeiro == NULL){
        printf ("LEF:\n  Total 0 eventos\n");
        return;
    }
    
    aux = l->primeiro;
    printf("LEF:\n");
    while (aux != NULL){
        cont++;
        printf ("  tempo %d tipo %d d1 %d d2 %d\n", aux->evento->tempo, 
                aux->evento->tipo, aux->evento->dado1, aux->evento->dado2);
        aux = aux->prox;
    }
    
    printf ("  Total %d eventos\n", cont);
}

int tempo_evento (struct evento_t *e){
    return e->tempo;
}

int tipo_evento (struct evento_t *e){
    return e->tipo;
}

int dado1 (struct evento_t *e){
    return e->dado1;
}

int dado2 (struct evento_t *e){
    return e->dado2;
}