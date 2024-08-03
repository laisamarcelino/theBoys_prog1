#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include "conjunto.h"
#include "mundo.h"
#include "lef.h"
#include "fila.h"

int aleat (int min, int max){ 
	return min + rand() % (max+1 - min);
};

int distancia (struct coordenadas A, struct coordenadas B){

    double termo1 = (B.x - A.x)*(B.x - A.x); 
    double termo2 = (B.y - A.y)*(B.y - A.y);

    return (int)sqrt(termo1 + termo2);
};

struct heroi cria_heroi (struct heroi h, int id){
    struct conjunto *habilidades;
    int i;
    
    /* Cria um conjunto de habilidades */
    habilidades = cria_cjt(N_HABILIDADES);
    for(i = 0; i < N_HABILIDADES; i++){
        insere_cjt(habilidades, i);
    }

    h.id_heroi = id; 
    h.habilidades = cria_subcjt_cjt(habilidades, aleat(1,3)); 
    h.paciencia = aleat(0, 100);
    h.velocidade = aleat(50, 5000);
    h.experiencia = 0;
    h.base.id_base = BASE_NULA;

    destroi_cjt(habilidades); /*Destroi habilidades?*/
    return h;
};

struct base cria_base (struct base b, int id){

    b.id_base = id;
    b.lotacao = aleat(3, 10);
    b.presentes = cria_cjt(b.lotacao);
    b.espera = fila_cria(); /*Verificar a capacidade da fila? */
    b.local.x = aleat(0, N_TAMANHO_MUNDO-1);
    b.local.y = aleat(0, N_TAMANHO_MUNDO-1);

    /*Destroi presentes?*/
    return b;
};

struct missao cria_missao (struct missao m, int id){
    struct conjunto *habilidades;
    int i;
    
    /* Cria um conjunto de habilidades */
    habilidades = cria_cjt(N_HABILIDADES);
    for(i = 0; i < N_HABILIDADES; i++){
        insere_cjt(habilidades, i);
    }

    m.id_missao = id;
    m.local.x = aleat(0, N_TAMANHO_MUNDO-1);
    m.local.y = aleat(0, N_TAMANHO_MUNDO-1);
    m.habilidades = cria_subcjt_cjt(habilidades, aleat(6,10)); 

    destroi_cjt(habilidades); /*Destroi habilidades?*/
    return m;
}

void cria_mundo (struct mundo *mundo, struct lef_t *l){
    int id, base, t_missao;
    struct evento_t *ev_inicial, *ev_missao, *ev_fim;

    /* Cria um vetor de herois */
    for(id = 0; id < N_HEROIS; id++){
        mundo->herois[id] = cria_heroi(mundo->herois[id], id);
        base = aleat(0, N_BASES - 1);
        mundo->herois[id].base.id_base = base;
        ev_inicial = cria_evento(aleat(0, 4320), CHEGA, id, base);
        insere_lef(l, ev_inicial);
    }

    /* Cria um vetor de bases */
    for(id = 0; id < N_BASES; id++){
        mundo->bases[id] = cria_base(mundo->bases[id], id);
    }

    /* Cria um vetor de missoes */
    for(id = 0; id < N_MISSOES; id++){
        mundo->missoes[id] = cria_missao(mundo->missoes[id], id);
        t_missao = aleat(0, T_FIM_DO_MUNDO);
        ev_missao = cria_evento(t_missao, MISSAO, id, DADO_NULO);
        insere_lef(l, ev_missao);
    }

    mundo->tamanho_mundo.x = N_TAMANHO_MUNDO;
    mundo->tamanho_mundo.y = N_TAMANHO_MUNDO;
    mundo->n_herois = N_HEROIS;
    mundo->n_bases = N_BASES;
    mundo->n_missoes = N_MISSOES;
    mundo->n_habilidades = N_HABILIDADES;
    mundo->relogio = T_INICIO;

    ev_fim = cria_evento(T_FIM_DO_MUNDO, FIM, DADO_NULO, DADO_NULO);
    insere_lef(l, ev_fim);
}

void chega (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    bool decide_esperar;
    struct evento_t *evento;

    /* Atualiza base de h */
    mundo->herois[h].base.id_base = b;

    /* Verifica vagas na base e se a fila de espera está vazia */
    if (cardinalidade_cjt(mundo->bases[b].presentes) < mundo->bases[b].lotacao 
            && fila_vazia(mundo->bases[b].espera)){
        decide_esperar = true;
    }
    /* Verifica paciencia do heroi */
    else {
        decide_esperar = mundo->herois[h].paciencia > 
            (10*fila_tamanho(mundo->bases[b].espera)) ? true : false;
    }

    if (decide_esperar){
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA \n", 
			t, h, b, mundo->bases[b].presentes->card, mundo->bases[b].lotacao);
        evento = cria_evento(t, ESPERA, h, b);
        insere_lef(l, evento);
    }
    else {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE \n", 
			t, h, b, mundo->bases[b].presentes->card, mundo->bases[b].lotacao);
        evento = cria_evento(t, DESISTE, h, b);
        insere_lef(l, evento);
    }
}

void espera (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *avisa;
    
    /* Adiciona h ao fim da fila de espera */
    enqueue(mundo->bases[b].espera, h);
    avisa = cria_evento(t, AVISA, b, DADO_NULO);
    insere_lef(l, avisa);

	printf("%6d: ESPERA HEROI %2d BASE %d (%2d) \n",
		t, h, b, fila_tamanho(mundo->bases[b].espera) - 1);
}

void desiste (int t, int h, int b, struct lef_t *l){
    struct evento_t *viaja;
    int d;

    /* Escolhe um base de destino aleatória */
    d = aleat(0, N_BASES);

    viaja = cria_evento(t, VIAJA, h, d); 
    insere_lef(l, viaja);

    printf("%6d: DESISTE HEROI %2d BASE %d \n", t, h, b);

}

void avisa (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *entra;
    int h_retirado;


	printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
		t, b, mundo->bases[b].presentes->card, mundo->bases[b].lotacao);
    imprime_fila(mundo->bases[b].espera);

    while (cardinalidade_cjt(mundo->bases[b].presentes) < mundo->bases[b].lotacao 
            && !fila_vazia(mundo->bases[b].espera)){
        
        /* Retira primeiro heroi da fila de b*/
        dequeue(mundo->bases[b].espera, &h_retirado);
        insere_cjt(mundo->bases[b].presentes, h);

        entra = cria_evento(t, ENTRA, h, b);
        insere_lef(l, entra);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d \n", 
			t, b, h);
    }
}

void entra (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *sai;

    /* Calcula tempo de permanencia da base*/
    int TPB = 15 + mundo->herois[h].paciencia * aleat(1,20);
    
    sai = cria_evento(t + TPB, SAI, h, b);
    insere_lef(l, sai); 

	printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d \n", 
		t, h, b, mundo->bases[b].presentes->card, mundo->bases[b].lotacao, t + TPB);
}

void sai (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *viaja, *avisa;
    int d;

    /* Retira h do conjunto de herois presentes em b*/
    retira_cjt(mundo->bases[b].presentes, h);
    d = aleat(0, N_BASES);

    viaja = cria_evento(t, VIAJA, h, d);
    insere_lef(l, viaja);
    avisa = cria_evento(t, AVISA, b, DADO_NULO);
    insere_lef(l, avisa);

	printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d) \n", 
		t, h, b, mundo->bases[b].presentes->card, mundo->bases[b].lotacao);
}

void viaja (int t, struct mundo *mundo, int h, int d, struct lef_t *l){
    struct evento_t *chega;
    struct coordenadas base_atual;
    int dis, duracao, id_base_atual;
    
    /* Calcula duração da viagem */
    base_atual = mundo->herois[h].base.local;
    dis = distancia(base_atual, mundo->bases[d].local);
    duracao = dis / mundo->herois[h].velocidade;

    chega = cria_evento(t + duracao, CHEGA, h, d);
    insere_lef(l, chega);

    id_base_atual = mundo->herois[h].base.id_base;
    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n", 
		t, h, id_base_atual, d, dis, mundo->herois[h].velocidade, t + duracao);
}

void missao (int t, struct mundo *mundo, struct missao m, struct lef_t *l){
    int menor_dis, i, dis, j;
    struct conjunto *cjt_unido;

    /* Calcula distancia de cada base ao local da missao m */
    menor_dis = distancia(m.local, mundo->bases[0].local);
    for (i = 1; i <= N_BASES; i++){
        dis = distancia(m.local, mundo->bases[i].local);
        
        /* Une conjuntos */
        

        if (dis < menor_dis){
            menor_dis = dis;
        }
    }

}
