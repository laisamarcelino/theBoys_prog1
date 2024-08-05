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
    struct evento_t *evento = NULL;
    int relogio;
    srand(0);

    /* Inicializa a estrutura mundo com zeros para evitar problemas
    com valores nao inicializados */
    memset(&mundo, 0, sizeof(mundo));
    
    /* Cria eventos iniciais */
    cria_mundo(&mundo);

    /* Relogio = 0 */
    relogio = relogio_mundo(&mundo);

    /* Inicia a simulação até o fim do mundo */
    while (relogio != T_FIM_DO_MUNDO) {
        evento = retira_lef(lef_mundo(&mundo));
        if (evento == NULL){
            break;
        }

        relogio = tempo_evento(evento);

        switch (tipo_evento(evento)) {
            case CHEGA:
                chega(relogio, &mundo, dado1(evento), dado2(evento));
                break;
            
            case ESPERA:
                espera(relogio, &mundo, dado1(evento), dado2(evento));
                break;
            
            case DESISTE:
                desiste(relogio, &mundo, dado1(evento), dado2(evento));
                break;
            
            case AVISA:
                avisa(relogio, &mundo, dado2(evento));
                break;
            
            case ENTRA:
                entra(relogio, &mundo, dado1(evento), dado2(evento));
                break;
            
            case SAI:
                sai(relogio, &mundo, dado1(evento), dado2(evento));
                break;
            
            case VIAJA:
                viaja(relogio, &mundo, dado1(evento), dado2(evento));
                break;
            
            /*
            case MISSAO:
                missao(relogio, &mundo, dado1(evento));
                break;

            case FIM:
                fim(&mundo);
                break;
            */
        }
        destroi_evento(evento);
    }

    destroi_lef(lef_mundo(&mundo));

    return 0;
}

/* missao duas vezes
    nn passa no desiste
 */

