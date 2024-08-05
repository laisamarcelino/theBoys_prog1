#include <stdio.h>
#include <stdlib.h>
#include "mundo.h"
#include "conjunto.h"
#include "lef.h"
#include "fila.h"
#include <string.h>

int main() {

    /* Inicia as entidades e atributos do mundo */
    struct mundo mundo;
    int i;

    /* Inicializa a estrutura mundo com zeros para evitar problemas
    com valores nao inicializados */
    memset(&mundo, 0, sizeof(mundo));
    
    /* Cria eventos iniciais */
    cria_mundo(&mundo);
    for (i=0; i < N_HEROIS; i++){
        printf("id heroi: %d \n cjt hab: ", mundo.herois[i].id_heroi);
        imprime_cjt(mundo.herois[i].habilidades);
        printf("paciencia: %d\n", mundo.herois[i].paciencia);
        printf("velocidade: %d\n", mundo.herois[i].velocidade);
        printf("exp: %d\n", mundo.herois[i].experiencia);
        printf("id base: %d\n", mundo.herois[i].id_base);
        printf("\n");
    }

    fprintf (stderr, "passou HEROI \n\n");
    /* Base -2 é a base vazia - dado nulo */

    for (i=0; i < N_BASES; i++){
        printf("id base: %d \n cjt presentes: \n", mundo.bases[i].id_base);
        imprime_cjt(mundo.bases[i].presentes);
        printf("lotação: %d\n fila espera: ", mundo.bases[i].lotacao);
        imprime_fila(mundo.bases[i].espera);
        printf("local %d %d\n", mundo.bases[i].local.x, mundo.bases[i].local.y);
        printf("\n");
    }

    fprintf (stderr, "passou BASE\n\n");
    
      for (i=0; i < N_MISSOES; i++){
        printf("id missao: %d \n cjt habilidades: \n", mundo.missoes[i].id_missao);
        imprime_cjt(mundo.missoes[i].habilidades);
        printf("local %d %d\n", mundo.missoes[i].local.x, mundo.missoes[i].local.y);
        printf("tentativas: %d \n", mundo.missoes[i].tentativas);
        printf("\n");
    }

    fprintf (stderr, "passou MISSSAO \n\n");
    
    
    return 0;
}

