#include "pagina.h"

struct pagina
{
    char *nome;
    double pageRank;
    int qtdLinksDeEntrada;
    int qtdLinksDeSaida;
};

pagina *Criar_pagina(char *nome, double numpaginas)
{
    pagina *novaPagina = (pagina *)malloc(sizeof(pagina));
    if (novaPagina == NULL)
    {
        printf("Erro ao alocar memória para nova página.\n");
        return NULL;
    }

    novaPagina->nome = nome;
    novaPagina->pageRank = 1/numpaginas; // Inicializa o PageRank com 1/n
    novaPagina->qtdLinksDeEntrada = 0;
    novaPagina->qtdLinksDeSaida = 0;

    return novaPagina;
}

char *Obter_Nome_Pagina(pagina *p)
{
    return p->nome;
}

void Destruir_pagina(void *dado)
{
    pagina *a = (pagina*)dado;
    if (a != NULL)
    {
        if (a->nome != NULL)
        {
            free(a->nome);
        }
        free(a);
    }
}
