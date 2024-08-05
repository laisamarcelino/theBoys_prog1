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

struct heroi cria_heroi (int id, struct conjunto *habilidades){
    struct heroi h;

    h.id_heroi = id; 
    h.habilidades = cria_subcjt_cjt(habilidades, aleat(1,3)); 
    h.paciencia = aleat(0, 100);
    h.velocidade = aleat(50, 5000);
    h.experiencia = 0;
    h.id_base = BASE_NULA;

    return h;
};

struct base cria_base (int id){
    int max;
    struct base b;
    struct fila *f;
    struct conjunto *cjt;

    max = aleat(3,10);
    f = fila_cria();
    cjt = cria_cjt(max);                        

    b.id_base = id;
    b.lotacao = max;
    b.presentes = cjt;
    b.espera = f; 
    b.local.x = aleat(0, N_TAMANHO_MUNDO-1);
    b.local.y = aleat(0, N_TAMANHO_MUNDO-1);

    return b;
};

struct missao cria_missao (int id, struct conjunto *habilidades){
    struct missao m;

    m.id_missao = id;
    m.local.x = aleat(0, N_TAMANHO_MUNDO-1);
    m.local.y = aleat(0, N_TAMANHO_MUNDO-1);
    m.habilidades = cria_subcjt_cjt(habilidades, aleat(6,10)); 
    m.tentativas = 0;

    return m;
};

void cria_mundo (struct mundo *mundo){
    int id, base, i;
    struct evento_t *ev_inicial, *ev_missao, *ev_fim;
    struct lef_t *l;
    struct conjunto *habilidades;

    l = cria_lef();
    mundo->lef = l;

    mundo->tamanho_mundo.x = N_TAMANHO_MUNDO;
    mundo->tamanho_mundo.y = N_TAMANHO_MUNDO;
    mundo->n_herois = N_HEROIS;
    mundo->n_bases = N_BASES;
    mundo->n_missoes = N_MISSOES;
    mundo->n_habilidades = N_HABILIDADES;
    mundo->relogio = T_INICIO;
    mundo->n_missoes_cumpridas = 0;

        
    /* Cria um conjunto de habilidades */
    habilidades = cria_cjt(N_HABILIDADES);

    for(i = 0; i < N_HABILIDADES; i++){
        insere_cjt(habilidades, i);
    }

    /* Cria um vetor de structs herois */
    for(id = 0; id < N_HEROIS; id++){
        mundo->herois[id] = cria_heroi(id, habilidades);
    }

    /* Cria um vetor de structs bases */
    for(id = 0; id < N_BASES; id++){
        mundo->bases[id] = cria_base(id);
        printf("base: %d \n", mundo->bases[id].id_base);
        printf("lotacao: %d \n", mundo->bases[id].lotacao);
        printf("local: %d %d \n cjj: ", mundo->bases[id].local.x, mundo->bases[id].local.y);
        imprime_cjt(mundo->bases[id].presentes);
        printf("fila:");
        imprime_fila(mundo->bases[id].espera);
        printf("\n");
    }
    /*imprime_base ();*/

   
    /* Cria um vetor de structs missoes */
    for(id = 0; id < N_MISSOES; id++){
        mundo->missoes[id] = cria_missao(id, habilidades);     
    }

    /* Insere eventos para herois */
    for(id = 0; id < N_HEROIS; id++){
        base = aleat(0, N_BASES - 1);
        ev_inicial = cria_evento(aleat(0, 4320), CHEGA, id, base);
        insere_cjt(mundo->bases[base].presentes, id);
        insere_lef(mundo->lef, ev_inicial);
    }

    /* Insere eventos de missoes */
    for(id = 0; id < N_MISSOES; id++){
        ev_missao = cria_evento(aleat(0, T_FIM_DO_MUNDO), MISSAO, id, DADO_NULO);
        insere_lef(mundo->lef, ev_missao);
    }

    ev_fim = cria_evento(T_FIM_DO_MUNDO, FIM, DADO_NULO, DADO_NULO);
    insere_lef(mundo->lef, ev_fim);

    /*destroi_cjt(habilidades);*/
};

int relogio_mundo (struct mundo *mundo){
    return mundo->relogio;
}

struct lef_t *lef_mundo (struct mundo *mundo){
    return mundo->lef;
}

void chega (int t, struct mundo *mundo, int h, int b){
    bool decide_esperar = false;
    struct evento_t *evento;
    struct heroi *heroi;
    struct base *base;
    struct fila *fila_espera;
    struct conjunto *cjt_presentes;

    /* Atualiza base de h */
    heroi = &mundo->herois[h];
    heroi->id_base = b;

    base = &mundo->bases[b];
    fila_espera = base->espera;
    cjt_presentes = base->presentes;

    /* Verifica vagas na base e se a fila de espera está vazia */
    if (cardinalidade_cjt(cjt_presentes) < 
            base->lotacao && fila_vazia(fila_espera)) {
        decide_esperar = true;
    }
    /* Verifica paciencia do heroi */
    else {
        decide_esperar = heroi->paciencia > 
            (10*fila_tamanho(fila_espera)) ? true : false;
    }

    if (decide_esperar){
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) ESPERA \n", 
			t, h, b, cardinalidade_cjt(cjt_presentes), base->lotacao);
        evento = cria_evento(t, ESPERA, h, b);
        insere_lef(mundo->lef, evento);
    }
    else {
        printf("%6d: CHEGA  HEROI %2d BASE %d (%2d/%2d) DESISTE \n", 
			t, h, b, cardinalidade_cjt(cjt_presentes), base->lotacao);
        evento = cria_evento(t, DESISTE, h, b);
        insere_lef(mundo->lef, evento);
    }
};

void espera (int t, struct mundo *mundo, int h, int b){
    struct evento_t *evento;
    
    /* Adiciona h ao fim da fila de espera */
    enqueue(mundo->bases[b].espera, h);
    evento = cria_evento(t, AVISA, DADO_NULO, b);
    insere_lef(mundo->lef, evento);

	printf("%6d: ESPERA HEROI %2d BASE %d (%2d) \n",
		t, h, b, fila_tamanho(mundo->bases[b].espera) - 1);
};

void desiste (int t, struct mundo *mundo, int h, int b){
    struct evento_t *evento;
    int d;

    /* Escolhe um base de destino aleatória */
    d = aleat(0, N_BASES);

    evento = cria_evento(t, VIAJA, h, d); 
    insere_lef(mundo->lef, evento);

    printf("%6d: DESISTE HEROI %2d BASE %d \n", t, h, b);
};

void avisa (int t, struct mundo *mundo, int b){
    struct evento_t *evento;
    int h_retirado;
    struct base *base;
    struct fila *fila_espera;
    struct conjunto *cjt_presentes;

    base = &mundo->bases[b];
    fila_espera = base->espera;
    cjt_presentes = base->presentes;
    
    if (t != mundo->relogio)
		mundo->relogio = t;
    
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
		t, b, cardinalidade_cjt(cjt_presentes), base->lotacao);
    imprime_fila(fila_espera);
    

    while (cardinalidade_cjt(cjt_presentes) 
            < base->lotacao && !(fila_vazia(fila_espera))) {
        
        /* Retira primeiro heroi da fila de b e insere em presentes */
        dequeue(base->espera, &h_retirado);
        insere_cjt(cjt_presentes, h_retirado);
        cjt_presentes->card++;

        evento = cria_evento(t, ENTRA, h_retirado, b);
        insere_lef(mundo->lef, evento);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d \n", 
			t, b, h_retirado);
    }
};

void entra (int t, struct mundo *mundo, int h, int b){
    struct evento_t *evento;
    struct base *base;

    base = &mundo->bases[b];

    /* Calcula tempo de permanencia da base*/
    int TPB = 15 + mundo->herois[h].paciencia * aleat(1,20);
    
    evento = cria_evento(t + TPB, SAI, h, b);
    insere_lef(mundo->lef, evento); 

	printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d \n", 
		t, h, b, cardinalidade_cjt(base->presentes), base->lotacao, t + TPB);
};

void sai (int t, struct mundo *mundo, int h, int b){
    struct evento_t *viaja, *avisa;
    struct base *base;
    int d;

    base = &mundo->bases[b];

    /* Retira h do conjunto de herois presentes em b*/
    retira_cjt(mundo->bases[b].presentes, h);
    mundo->bases[b].presentes--;

    d = aleat(0, N_BASES - 1);

    viaja = cria_evento(t, VIAJA, h, d);
    insere_lef(mundo->lef, viaja);
    avisa = cria_evento(t, AVISA, DADO_NULO, b);
    insere_lef(mundo->lef, avisa);

	printf("%6d: SAI    HEROI %2d BASE %d (%2d/%2d) \n", 
		t, h, b, cardinalidade_cjt(base->presentes), base->lotacao);
};

void viaja (int t, struct mundo *mundo, int h, int d){
    struct evento_t *evento;
    struct coordenadas base_atual;
    int dis, duracao, id_base_atual;
    
    /* Calcula duração da viagem */
    id_base_atual = mundo->herois[h].id_base;
    base_atual = mundo->bases[id_base_atual].local;
    dis = distancia(base_atual, mundo->bases[d].local);
    duracao = dis / mundo->herois[h].velocidade;

    evento = cria_evento(t + duracao, CHEGA, h, d);
    insere_lef(mundo->lef, evento);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n", 
		t, h, id_base_atual, d, dis, mundo->herois[h].velocidade, t + duracao);
};

void missao (int t, struct mundo *mundo, int m){
    int menor_dis, i, dis, id_heroi, missao_possivel, base_escolhida;
    struct conjunto *uniao_hab_h, *equipe_escolhida;
    struct evento_t *missao;
    struct missao st_m;
    struct base b;
    struct heroi h;
    
    /* Verifica se m está dentro do limite do vetor
    if (m < 0 || m >= N_MISSOES) {
        return;
    }*/

    st_m = mundo->missoes[m];

    missao_possivel = 0;
    
    printf("%6d: MISSAO %d TENT %d HAB REQ: ", t, st_m.tentativas, m);
	imprime_cjt(st_m.habilidades);

    uniao_hab_h = cria_cjt(N_HABILIDADES);


    /* Calcula distancia de cada base ao local da missao m */
    /* A maior distancia possivel é entre o local da missao e o tam max do mundo */
    menor_dis = distancia(st_m.local, mundo->tamanho_mundo);
    for (i = 0; i < N_BASES; i++){
        dis = distancia(st_m.local, mundo->bases[i].local);
        
        b = mundo->bases[i];

        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS ", t, m, i, dis);
        if (b.presentes == NULL){
            printf("ERRO\n");
            return;
        }
	    imprime_cjt(b.presentes);
       

        /* Une conjuntos de habilidades dos herois na base analisada */
        inicia_iterador_cjt (mundo->bases[i].presentes); 
        while (incrementa_iterador_cjt (mundo->bases[i].presentes, &id_heroi)){
            h = mundo->herois[id_heroi];
            uniao_hab_h = uniao_cjt(uniao_hab_h, mundo->herois[id_heroi].habilidades);
            printf("%6d: MISSAO %d HAB HEROI %2d: ", t, m, id_heroi);
	        imprime_cjt(h.habilidades);
        }
         fprintf (stderr, "passou\n");
        printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", t, m, i);
	    imprime_cjt(uniao_hab_h);

        /* Verifica condicoes de menor distancia e conjunto de habilidades */
        if (dis < menor_dis && contido_cjt(st_m.habilidades, uniao_hab_h)){
            menor_dis = dis;
            equipe_escolhida = mundo->bases[i].presentes;
            missao_possivel = 1;
            base_escolhida = i;
        }
    }

    if (missao_possivel){
        if (vazio_cjt(equipe_escolhida))
            return;

        inicia_iterador_cjt (equipe_escolhida); 
        while (incrementa_iterador_cjt (equipe_escolhida, &id_heroi)){
            mundo->herois[id_heroi].experiencia += 1;
        }
        mundo->n_missoes_cumpridas += 1;
        printf("%6d: MISSAO %d CUMPRIDA BASE %d \n", t, m, base_escolhida);
    }
    else {
        missao = cria_evento(t+24*60, MISSAO, m, DADO_NULO);
        insere_lef(mundo->lef, missao);
        st_m.tentativas += 1;
        printf("%6d: MISSAO %d IMPOSSIVEL \n", t, m);
    }

    /*destroi_cjt(uniao_hab_h);*/
};

void fim (struct mundo *mundo){
    int i, id, pac, vel, exp, min, max, soma;
    double porcentagem, media;
    struct fila *fila;

    fprintf (stderr, "entrei no fim do mundo\n");

    min = max = 1;
    soma = 0;

    printf("%6d: FIM \n", T_FIM_DO_MUNDO);

    for (i = 0; i < N_HEROIS; i++){
        id = mundo->herois[i].id_heroi;
        pac = mundo->herois[i].paciencia;
        vel = mundo->herois[i].velocidade;
        exp = mundo->herois[i].experiencia;
        printf("HEROI %2d PAC %d VEL %d EXP %d HABS ", id, pac, vel, exp);
		imprime_cjt(mundo->herois[i].habilidades);
        destroi_cjt(mundo->herois[i].habilidades);
    }

    for (i = 0; i < N_BASES; i++){
        destroi_cjt(mundo->bases[i].presentes);
        fila = mundo->bases[i].espera;
        fila_destroi(&fila);
    }

    for (i = 0; i < N_MISSOES; i++){
        destroi_cjt(mundo->missoes[i].habilidades);

        if (mundo->missoes[i].tentativas < min){
            min = mundo->missoes[i].tentativas;
        }
        else {
            max = mundo->missoes[i].tentativas;
        }

        soma += mundo->missoes[i].tentativas;
    }

    porcentagem = (mundo->n_missoes_cumpridas * 100) / N_MISSOES;
    media = (double)soma / N_MISSOES;

    printf("MISSOES CUMPRIDAS: %d/%d (%.2f%%)\n", 
            mundo->n_missoes_cumpridas, N_MISSOES, porcentagem);
    printf("TENTATIVAS/MISSAO: MIN %d, MAX %d, MEDIA %.2f\n", 
            min, max, media);

};

