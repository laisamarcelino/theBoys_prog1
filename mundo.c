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
    if (habilidades == NULL) {
        return h;
    }
    for(i = 0; i < N_HABILIDADES; i++){
        insere_cjt(habilidades, i);
    }

    h.id_heroi = id; 
    h.habilidades = cria_subcjt_cjt(habilidades, aleat(1,3)); 
    h.paciencia = aleat(0, 100);
    h.velocidade = aleat(50, 5000);
    h.experiencia = 0;
    h.base.id_base = BASE_NULA;

    destroi_cjt(habilidades);

    return h;
};

struct base cria_base (struct base b, int id){
    int max;
    max = aleat(3,10);

    b.id_base = id;
    b.lotacao = max;
    b.presentes = cria_cjt(max);
    if (b.presentes == NULL) {
        return b;
    }
    b.espera = fila_cria(); 
    if (b.espera == NULL) {
        destroi_cjt(b.presentes);
        return b;
    }
    b.local.x = aleat(0, N_TAMANHO_MUNDO-1);
    b.local.y = aleat(0, N_TAMANHO_MUNDO-1);

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
    m.tentativas = 0;

    destroi_cjt(habilidades);

    return m;
};

void cria_mundo (struct mundo *mundo, struct lef_t *l){
    int id, base, t_missao;
    struct evento_t *ev_inicial, *ev_missao, *ev_fim;

    /* Cria um vetor de herois */
    for(id = 0; id < N_HEROIS; id++){
        mundo->herois[id] = cria_heroi(mundo->herois[id], id);
    }

    /* Cria um vetor de bases */
    for(id = 0; id < N_BASES; id++){
        mundo->bases[id] = cria_base(mundo->bases[id], id);
    }
   
    /* Cria um vetor de missoes */
    for(id = 0; id < N_MISSOES; id++){
        mundo->missoes[id] = cria_missao(mundo->missoes[id], id);     
    }

    /* Insere eventos para herois */
    for(id = 0; id < N_HEROIS; id++){
        base = aleat(0, N_BASES - 1);
        mundo->herois[id].base.id_base = base;
        ev_inicial = cria_evento(aleat(0, 4320), CHEGA, id, base);
        insere_lef(l, ev_inicial);
    }

    /* Insere eventos de missoes */
    for(id = 0; id < N_MISSOES; id++){
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
    mundo->n_missoes_cumpridas = 0;

    ev_fim = cria_evento(T_FIM_DO_MUNDO, FIM, DADO_NULO, DADO_NULO);
    insere_lef(l, ev_fim);
};

int relogio_mundo (struct mundo *mundo){
    return mundo->relogio;
}

void chega (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    bool decide_esperar;
    struct evento_t *evento;

    /* Atualiza base de h */
    mundo->herois[h].base.id_base = b;

    /* Verifica vagas na base e se a fila de espera está vazia */
    if (!vazio_cjt(mundo->bases[b].presentes) && mundo->bases[b].presentes->card < 
            mundo->bases[b].lotacao && fila_vazia(mundo->bases[b].espera)) {
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
};

void espera (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *avisa;
    
    /* Adiciona h ao fim da fila de espera */
    enqueue(mundo->bases[b].espera, h);
    avisa = cria_evento(t, AVISA, b, DADO_NULO);
    insere_lef(l, avisa);

	printf("%6d: ESPERA HEROI %2d BASE %d (%2d) \n",
		t, h, b, fila_tamanho(mundo->bases[b].espera) - 1);
};

void desiste (int t, int h, int b, struct lef_t *l){
    struct evento_t *viaja;
    int d;

    /* Escolhe um base de destino aleatória */
    d = aleat(0, N_BASES);

    viaja = cria_evento(t, VIAJA, h, d); 
    insere_lef(l, viaja);

    printf("%6d: DESISTE HEROI %2d BASE %d \n", t, h, b);
};

void avisa (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *entra;
    int h_retirado;
    struct base *base = &mundo->bases[b];
    if (base == NULL){
        return;
    }
    if (base->presentes == NULL){
        return;
    }
    
    printf("%6d: AVISA  PORTEIRO BASE %d (%2d/%2d) FILA ",
		t, b, base->presentes->card, base->lotacao);
    imprime_fila(base->espera);
    

    while (!vazio_cjt(base->presentes) && base->presentes->card 
            < base->lotacao && !fila_vazia(base->espera)){
        
        /* Retira primeiro heroi da fila de b e insere em presentes */
        dequeue(base->espera, &h_retirado);
        h = h_retirado;
        insere_cjt(base->presentes, h);

        entra = cria_evento(t, ENTRA, h_retirado, b);
        insere_lef(l, entra);

        printf("%6d: AVISA  PORTEIRO BASE %d ADMITE %2d \n", 
			t, b, h);
    }
};

void entra (int t, struct mundo *mundo, int h, int b, struct lef_t *l){
    struct evento_t *sai;

    /* Calcula tempo de permanencia da base*/
    int TPB = 15 + mundo->herois[h].paciencia * aleat(1,20);
    
    sai = cria_evento(t + TPB, SAI, h, b);
    insere_lef(l, sai); 

	printf("%6d: ENTRA  HEROI %2d BASE %d (%2d/%2d) SAI %d \n", 
		t, h, b, mundo->bases[b].presentes->card, mundo->bases[b].lotacao, t + TPB);
};

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
};

void viaja (int t, struct mundo *mundo, int h, int d, struct lef_t *l){
    struct evento_t *chega;
    struct coordenadas base_atual;
    int dis, duracao, id_base_atual;
    
    /* Calcula duração da viagem */
    id_base_atual = mundo->herois[h].base.id_base;
    base_atual = mundo->bases[id_base_atual].local;
    dis = distancia(base_atual, mundo->bases[d].local);
    duracao = dis / mundo->herois[h].velocidade;

    chega = cria_evento(t + duracao, CHEGA, h, d);
    insere_lef(l, chega);

    printf("%6d: VIAJA  HEROI %2d BASE %d BASE %d DIST %d VEL %d CHEGA %d \n", 
		t, h, id_base_atual, d, dis, mundo->herois[h].velocidade, t + duracao);
};

void missao (int t, struct mundo *mundo, int m, struct lef_t *l){
    int menor_dis, i, dis, id_heroi, missao_possivel, base_escolhida;
    struct conjunto *uniao_hab_h, *equipe_escolhida;
    struct evento_t *missao;
    struct missao *st_m;
    
    /* Verifica se m está dentro do limite do vetor*/
    if (m < 0 || m >= N_MISSOES) {
        return;
    }

    st_m = &mundo->missoes[m];
    if (st_m == NULL){
        return;
    }

    missao_possivel = 0;
    
    printf("%6d: MISSAO %d TENT %d HAB REQ: ", t, st_m->tentativas, m);
	imprime_cjt(st_m->habilidades);

    uniao_hab_h = cria_cjt(N_HABILIDADES);
    if (uniao_hab_h == NULL) {
        return; 
    }

    /* Calcula distancia de cada base ao local da missao m */
    /* A maior distancia possivel é entre o local da missao e o tam max do mundo */
    menor_dis = distancia(st_m->local, mundo->tamanho_mundo);
    for (i = 0; i < N_BASES; i++){
        dis = distancia(st_m->local, mundo->bases[i].local);
        
        printf("%6d: MISSAO %d BASE %d DIST %d HEROIS ", t, m, i, dis);
	    imprime_cjt(mundo->bases[i].presentes);

        /* Une conjuntos de habilidades dos herois na base analisada */
        inicia_iterador_cjt (mundo->bases[i].presentes); 
        while (incrementa_iterador_cjt (mundo->bases[i].presentes, &id_heroi)){
            uniao_hab_h = uniao_cjt(uniao_hab_h, mundo->herois[id_heroi].habilidades);
            printf("%6d: MISSAO %d HAB HEROI %2d: ", t, m, id_heroi);
	        imprime_cjt(mundo->herois[id_heroi].habilidades);
        }

        printf("%6d: MISSAO %d UNIAO HAB BASE %d: ", t, m, i);
	    imprime_cjt(mundo->bases[i].presentes);

        /* Verifica condicoes de menor distancia e conjunto de habilidades */
        if (dis < menor_dis && contido_cjt(st_m->habilidades, uniao_hab_h)){
            menor_dis = dis;
            equipe_escolhida = mundo->bases[i].presentes;
            missao_possivel = 1;
            base_escolhida = i;
        }
    }

    if (missao_possivel){
        inicia_iterador_cjt (equipe_escolhida); 
        while (incrementa_iterador_cjt (equipe_escolhida, &id_heroi)){
            mundo->herois[id_heroi].experiencia += 1;
        }
        mundo->n_missoes_cumpridas += 1;
        printf("%6d: MISSAO %d CUMPRIDA BASE %d \n", t, m, base_escolhida);
    }
    else {
        missao = cria_evento(t+24*60, MISSAO, m, DADO_NULO);
        insere_lef(l, missao);
        st_m->tentativas += 1;
        printf("%6d: MISSAO %d IMPOSSIVEL \n", t, m);
    }
    destroi_cjt(uniao_hab_h);
};

void fim (struct mundo *mundo){
    int i, id, pac, vel, exp, min, max, soma;
    double porcentagem, media;
    struct fila *fila;

    min = max = 1;
    soma = 0;

    printf("%6d: FIM \n", T_FIM_DO_MUNDO);

    for (i = 0; i < N_HEROIS; i++){
        id = mundo->herois[i].id_heroi;
        pac = mundo->herois[i].paciencia;
        vel = mundo->herois[i].velocidade;
        exp = mundo->herois[i].experiencia;
        printf("HEROI %2d PAC %d VEL %d EXP %d HABS", id, pac, vel, exp);
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