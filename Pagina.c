#include "Pagina.h"
#include <math.h>

struct Pagina {
    char *nome;
    double page_rank;
    int qtd_links_entrada;
    int qtd_links_saida;
    Pagina** links_saida;
    Pagina** links_entrada;
    int capacidade_links_entrada;
};

Pagina *pagina_construtor(char *nome, double num_paginas) {
    Pagina *nova_pagina = (Pagina *)malloc(sizeof(Pagina));

    if (nova_pagina == NULL) {
        printf("Erro ao alocar memória para nova página.\n");
        return NULL;
    }

    nova_pagina->nome = nome;
    nova_pagina->page_rank = 1 / num_paginas; // Inicializa o PageRank com 1/n
    nova_pagina->qtd_links_entrada = 0;
    nova_pagina->links_entrada = (Pagina**)calloc(CAPACIDADE_ENTRADA_INICIAL, sizeof(Pagina*));
    nova_pagina->capacidade_links_entrada = CAPACIDADE_ENTRADA_INICIAL;
    nova_pagina->qtd_links_saida = 0;
    nova_pagina->links_saida = NULL;

    return nova_pagina;
}

char *pagina_obter_nome(Pagina *p) {
    return p->nome;
}

int pagina_compara(const void *a, const void *b){
    const Pagina *pag_a = (const Pagina *)a;
    const Pagina *pag_b = (const Pagina *)b;
    return strcasecmp(pag_a->nome, pag_b->nome);
}

void aloca_lista_links_saida(Pagina* pagina, int qtd){
    pagina->links_saida = (Pagina**) malloc(qtd * sizeof(Pagina*));
    pagina->qtd_links_saida = 0;
}

void pagina_adiciona_link_entrada(Pagina* pagina1, Pagina* pagina2){
    if (pagina1->qtd_links_entrada >= pagina1->capacidade_links_entrada) {
        pagina1->capacidade_links_entrada *= 2;
        pagina1->links_entrada = (Pagina**) realloc(pagina1->links_entrada, pagina1->capacidade_links_entrada * sizeof(Pagina*));

        if (pagina1->links_entrada == NULL) {
            printf("Erro ao realocar memória para links de entrada.\n");
            return;
        }
    }

    pagina1->links_entrada[pagina1->qtd_links_entrada] = pagina2;
    pagina1->qtd_links_entrada++;
}

double pagina_calcula_pgrank(Pagina* pagina, int k, int total_pag) {
    if (k == 0) {
        return 1.0 / (double)total_pag;
    }

    double base = (1.0 - ALFA) / (double) total_pag;

    // soma das contribuições das páginas que apontam para essa
    double soma_in = 0.0;

    for (int j = 0; j < pagina->qtd_links_entrada; j++) {
        Pagina* origem = pagina->links_entrada[j];

        if (origem->qtd_links_saida > 0) {
            soma_in += pagina_calcula_pgrank(origem, k - 1, total_pag) / (double)origem->qtd_links_saida;
        }
    }

    return base + (ALFA * soma_in);
}

void pagina_calcula_pgrank_total(Pagina** paginas, int qtd_paginas, int total_pag) {
    double erro = EPSILON + 1;
    int k = 1;

    while (erro >= EPSILON) {
        erro = 0;
    
        for (int i = 0; i < qtd_paginas; i++) {
            double novo = pagina_calcula_pgrank(paginas[i], k, total_pag);
            erro += fabs(novo - paginas[i]->page_rank);
            paginas[i]->page_rank = novo;
        }
        
        if (qtd_paginas == 0) {
            continue; 
        }

        erro /= qtd_paginas;
        k++;
    }

    for (int i = 0; i < qtd_paginas; i++) {
        if (paginas[i]->page_rank) {
            printf("%.16lf ", paginas[i]->page_rank);
        }
    }
}

void pagina_adiciona_link_saida(Pagina* pagina1, Pagina* pagina2){
    //pagina1->links_saida[pagina1->qtd_links_saida] = pagina2;
    pagina1->qtd_links_saida++;
}

void pagina_destrutor(void *dado) {
    Pagina *a = (Pagina *)dado;

    if (a != NULL) {

        if (a->nome != NULL) {
            free(a->nome);
        }

        if (a->links_entrada != NULL) {
            free(a->links_entrada);
        }

        if (a->links_saida != NULL) {
            free(a->links_saida);
        }

        free(a);
    }
}