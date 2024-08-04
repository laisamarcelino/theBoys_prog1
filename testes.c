#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "conjunto.h"
#include "mundo.h"
#include "lef.h"
#include "fila.h"



void insere_eventos_iniciais(struct mundo *mundo);

int main() {

    srand(time(NULL));


    struct mundo mundo;
    cria_mundo(&mundo);


    insere_eventos_iniciais(&mundo);

    
    fim(&mundo);

 
    destroi_lef(mundo.lef);

    return 0;
}

void insere_eventos_iniciais(struct mundo *mundo) {
    int i;
    for (i = 0; i < N_HEROIS; i++) {
        int base = aleat(0, N_BASES - 1);
        struct evento_t *evento_chega = cria_evento(aleat(0, T_FIM_DO_MUNDO), CHEGA, i, base);
        insere_lef(mundo->lef, evento_chega);
    }

    for (i = 0; i < N_MISSOES; i++) {
        struct evento_t *evento_missao = cria_evento(aleat(0, T_FIM_DO_MUNDO), MISSAO, i, DADO_NULO);
        insere_lef(mundo->lef, evento_missao);
    }
}
