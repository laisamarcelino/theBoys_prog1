#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "mundo.h"
#include "conjunto.h"
#include "lef.h"
#include "fila.h"

int main() {

    /* Inicia as entidades e atributos do mundo */
    struct mundo mundo;
    struct evento_t *evento = NULL;
    int relogio;
    srand(0);

    /* Inicializa a estrutura mundo com zeros para evitar problemas
    com valores nao inicializados */
    memset(&mundo, 0, sizeof(mundo));
    
    /* Cria eventos iniciais */
    cria_mundo(&mundo);

    relogio = relogio_mundo(&mundo);

    /* Inicia a simulação até o fim do mundo */
    while (relogio != T_FIM_DO_MUNDO) {
        evento = retira_lef(lef_mundo(&mundo));
        if (evento == NULL){
            break;
        }

        relogio = evento->tempo;

        switch (evento->tipo) {
            case CHEGA:
                chega(&mundo, relogio, evento->dado1, evento->dado2);
                break;
            
            case ESPERA:
                espera(&mundo, relogio, evento->dado1, evento->dado2);
                break;
            
            case DESISTE:
                desiste(&mundo, relogio, evento->dado1, evento->dado2);
                break;
            
            case AVISA:
                avisa(&mundo, relogio, evento->dado2);
                break;
            
            case ENTRA:
                entra(&mundo, relogio, evento->dado1, evento->dado2);
                break;
            
            case SAI:
                sai(&mundo, relogio, evento->dado1, evento->dado2);
                break;
            
            case VIAJA:
                viaja(&mundo, relogio, evento->dado1, evento->dado2);
                break;      
            
            case MISSAO:
                missao(&mundo, relogio, evento->dado1);
                break;
            
            case FIM:
                fim(&mundo);
                break;
            
        }
        destroi_evento(evento);
    }

    destroi_lef(lef_mundo(&mundo));

    return 0;
}
