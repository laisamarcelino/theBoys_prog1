#include <stdio.h>
#include <stdlib.h>
#include "conjunto.h"
#include "mundo.h"
#include "lef.h"
#include "fila.h"

/* Ciclo basico de funcionamento de um simulador 
1. retirar o primeiro evento de uma lista de eventos
2. atualzar o relogio
3. tratar o evento: a. atualizar o estado do sistema, b. agendar
   novos eventos criados
4. reperit até concluir a simulação
*/

int main (){
    srand (0); 
    
    /*declarar variaveis*/
    /*int relogio = 0;*/

    /* iniciar as entidades e atributos do mundo */
    struct heroi h;
   
    /* criar os eventos iniciais */
    
    

    /* testes */
    h = cria_heroi(h,0);
    printf( "%d", h.habilidades);

    return 0;
}
