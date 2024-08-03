#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include "mundo.h"
#include "conjunto.h"
#include "lef.h"
#include "fila.h"

/* Funções de teste */

void testa_cria_mundo() {
    struct mundo m;
    int i;
    struct lef_t *lista_eventos = cria_lef();
    srand(0); 

    cria_mundo(&m, lista_eventos);

    printf("Mundo criado:\n");
    printf("Tamanho do mundo: (%d, %d)\n", m.tamanho_mundo.x, m.tamanho_mundo.y);
    printf("Número de heróis: %d\n", m.n_herois);
    printf("Número de bases: %d\n", m.n_bases);
    printf("Número de missões: %d\n", m.n_missoes);

    for (i = 0; i < m.n_herois; i++) {
        printf("Herói %d - Paciência: %d, Velocidade: %d, Base Inicial: %d\n", 
               m.herois[i].id_heroi, m.herois[i].paciencia, m.herois[i].velocidade, m.herois[i].base.id_base);
    }

    for (i = 0; i < m.n_bases; i++) {
        printf("Base %d - Lotação: %d\n", m.bases[i].id_base, m.bases[i].lotacao);
    }

    for (i = 0; i < m.n_missoes; i++) {
        printf("Missão %d - Local: (%d, %d)\n", m.missoes[i].id_missao, m.missoes[i].local.x, m.missoes[i].local.y);
    }

    destroi_lef(lista_eventos); 
}

void testa_chega() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0); 

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 100;
    int id_heroi = 0; 
    int id_base = m.herois[id_heroi].base.id_base;

    printf("\nTestando evento CHEGA para o herói %d na base %d no tempo %d\n", id_heroi, id_base, tempo_evento);
    chega(tempo_evento, &m, id_heroi, id_base, lista_eventos);

    destroi_lef(lista_eventos); 
}

void testa_espera() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0); 

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 200;
    int id_heroi = 0;
    int id_base = m.herois[id_heroi].base.id_base;

    chega(tempo_evento, &m, id_heroi, id_base, lista_eventos);

    espera(tempo_evento + 10, &m, id_heroi, id_base, lista_eventos);

    destroi_lef(lista_eventos);
}

void testa_desiste() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0);

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 300;
    int id_heroi = 0;
    int id_base = m.herois[id_heroi].base.id_base;

    chega(tempo_evento, &m, id_heroi, id_base, lista_eventos);

    espera(tempo_evento + 10, &m, id_heroi, id_base, lista_eventos);

    desiste(tempo_evento + 20, id_heroi, id_base, lista_eventos);

    destroi_lef(lista_eventos);
}

void testa_avisa() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0);

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 400;
    int id_heroi = 0;
    int id_base = m.herois[id_heroi].base.id_base;

    chega(tempo_evento, &m, id_heroi, id_base, lista_eventos);
    espera(tempo_evento + 10, &m, id_heroi, id_base, lista_eventos);

    printf("\nTestando evento AVISA para a base %d no tempo %d\n", id_base, tempo_evento + 20);
    avisa(tempo_evento + 20, &m, id_heroi, id_base, lista_eventos);

    destroi_lef(lista_eventos);
}

void testa_entra() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0);

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 500;
    int id_heroi = 0;
    int id_base = m.herois[id_heroi].base.id_base;

    chega(tempo_evento, &m, id_heroi, id_base, lista_eventos);
    espera(tempo_evento + 10, &m, id_heroi, id_base, lista_eventos);
    
    printf("\nTestando evento ENTRA para o herói %d na base %d no tempo %d\n", id_heroi, id_base, tempo_evento + 30);
    entra(tempo_evento + 30, &m, id_heroi, id_base, lista_eventos);

    destroi_lef(lista_eventos);
}

void testa_viaja() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0);

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 600;
    int id_heroi = 0;
    int id_base_atual = m.herois[id_heroi].base.id_base;

    printf("\nTestando evento VIAJA para o herói %d da base %d no tempo %d\n", id_heroi, id_base_atual, tempo_evento);
    viaja(tempo_evento, &m, id_heroi, aleat(0, m.n_bases - 1), lista_eventos);

    destroi_lef(lista_eventos);
}

void testa_sai() {
    struct mundo m;
    struct lef_t *lista_eventos = cria_lef();
    srand(0);

    cria_mundo(&m, lista_eventos);
    int tempo_evento = 700;
    int id_heroi = 0;
    int id_base = m.herois[id_heroi].base.id_base;

    chega(tempo_evento, &m, id_heroi, id_base, lista_eventos);
    espera(tempo_evento + 10, &m, id_heroi, id_base, lista_eventos);
    entra(tempo_evento + 20, &m, id_heroi, id_base, lista_eventos);

    printf("\nTestando evento SAI para o herói %d da base %d no tempo %d\n", id_heroi, id_base, tempo_evento + 30);
    sai(tempo_evento + 30, &m, id_heroi, id_base, lista_eventos);

    destroi_lef(lista_eventos);
}

int main() {
    printf("Testando a função cria_mundo:\n");
    testa_cria_mundo();

    printf("\nTestando a função chega:\n");
    testa_chega();

    printf("\nTestando a função espera:\n");
    testa_espera();

    printf("\nTestando a função desiste:\n");
    testa_desiste();

    printf("\nTestando a função avisa:\n");
    testa_avisa();

    printf("\nTestando a função entra:\n");
    testa_entra();

    printf("\nTestando a função viaja:\n");
    testa_viaja();

    printf("\nTestando a função sai:\n");
    testa_sai();

    return 0;
}
