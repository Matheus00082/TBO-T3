#include "Pagina.h"
#include <math.h>

struct Pagina {
    char *nome;
    double page_rank;
    int qtd_links_entrada;
    int qtd_links_saida;
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

    return nova_pagina;
}

char *pagina_obter_nome(Pagina *p) {
    return p->nome;
}

double pagina_obter_pagerank(Pagina *p) {
    return p->page_rank;
}

void pagina_definir_pagerank(Pagina *p, double pagerank) {
    p->page_rank = pagerank;
}

int pagina_obter_qtd_links_entrada(Pagina *p) {
    return p->qtd_links_entrada;
}

Pagina* pagina_obter_link_entrada(Pagina *p, int index) {
    if (index >= 0 && index < p->qtd_links_entrada) {
        return p->links_entrada[index];
    }
    return NULL;
}

int pagina_obter_qtd_links_saida(Pagina *p) {
    return p->qtd_links_saida;
}

int pagina_compara(const void *a, const void *b){
    const Pagina *pag_a = (const Pagina *)a;
    const Pagina *pag_b = (const Pagina *)b;
    return strcasecmp(pag_a->nome, pag_b->nome);
}

int compara_paginas_por_pagerank(const void *a, const void *b) {
    Pagina *pag_a = *(Pagina **)a;
    Pagina *pag_b = *(Pagina **)b;

    double pagerank_a = pagina_obter_pagerank(pag_a);
    double pagerank_b = pagina_obter_pagerank(pag_b);

    if (pagerank_a < pagerank_b) {
        return 1; // pag_b é maior
    } else if (pagerank_a > pagerank_b) {
        return -1; // pag_a é maior
    }
    
    return strcmp(pag_a->nome, pag_b->nome); // Se os PageRanks forem iguais, compara pelo nome
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

void pagina_adiciona_link_saida(Pagina* pagina1, Pagina* pagina2){
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


        free(a);
    }
}