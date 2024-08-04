#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mundo.h"
#include "conjunto.h"
#include "lef.h"
#include "fila.h"
#include <string.h>

int main() {

    /* Inicia as entidades e atributos do mundo */
    struct mundo mundo;
    struct lef_t *lef;
    struct evento_t *evento;
    int relogio;

    /* Inicializa a estrutura mundo com zeros para evitar problemas
    com valores nao inicializados */
    memset(&mundo, 0, sizeof(mundo));
    lef = cria_lef();
    if (lef == NULL){
        return 1;
    }

    /* Cria os eventos iniciais*/
    cria_mundo(&mundo, lef);

    /* Relogio = 0 */
    relogio = relogio_mundo(&mundo);
    
    /* Inicia a simulação até o fim do mundo */
    while (relogio != T_FIM_DO_MUNDO) {
        evento = retira_lef(lef);
        if (evento == NULL){
            break;
        }

        relogio = tempo_evento(evento);

        switch (tipo_evento(evento)) {
            case CHEGA:
                chega(relogio, &mundo, dado1(evento), dado2(evento), lef);
                break;
            
            case ESPERA:
                espera(relogio, &mundo, dado1(evento), dado2(evento), lef);
                break;
            
            case DESISTE:
                desiste(relogio, dado1(evento), dado2(evento), lef);
                break;
            
            case AVISA:
                avisa(relogio, &mundo, dado1(evento), dado2(evento), lef);
                break;
            
            case ENTRA:
                entra(relogio, &mundo, dado1(evento), dado2(evento), lef);
                break;
            
            case SAI:
                sai(relogio, &mundo, dado1(evento), dado2(evento), lef);
                break;
            
            case VIAJA:
                viaja(relogio, &mundo, dado1(evento), dado2(evento), lef);
                break;
            
            case MISSAO:
                missao(relogio, &mundo, dado1(evento), lef);
                break;

            case FIM:
                fim(&mundo);
                break;
        }
        destroi_evento(evento);
    }

    destroi_lef(lef);

    return 0;
}

