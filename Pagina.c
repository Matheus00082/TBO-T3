#include "Pagina.h"

struct Pagina {
    char *nome;
    double page_rank;
    int qtd_links_entrada;
    int qtd_links_saida;
    int *links_saida;
    int *links_entrada;
};

Pagina *criar_pagina(char *nome, double num_paginas) {
    Pagina *nova_pagina = (Pagina *)malloc(sizeof(Pagina));

    if (nova_pagina == NULL) {
        printf("Erro ao alocar memória para nova página.\n");
        return NULL;
    }

    nova_pagina->nome = nome;
    nova_pagina->page_rank = 1 / num_paginas; // Inicializa o PageRank com 1/n
    nova_pagina->qtd_links_entrada = 0;
    nova_pagina->qtd_links_saida = 0;
    return nova_pagina;
}

char *obter_nome_pagina(Pagina *p) {
    return p->nome;
}

void destruir_pagina(void *dado) {
    Pagina *a = (Pagina *)dado;

    if (a != NULL) {
        if (a->nome != NULL) {
            free(a->nome);
        }

        free(a);
    }
}