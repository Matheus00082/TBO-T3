#include "Palavra.h"
#include <string.h>

struct Palavra {
    Vector *arquivos;
    char *palavra;
};

int cmp_arquivos_vector(const void *a, const void *b) {
    const Pagina *pag_a = (const Pagina *)a;
    const Pagina *pag_b = (const Pagina *)b;
    return pagina_compara(pag_a, pag_b);
}

Palavra* palavra_construtor(char* palavra) {
    Palavra *p = (Palavra*)calloc(1, sizeof(Palavra));
    p->palavra = strdup(palavra);
    p->arquivos = vector_construct(NULL);
    return p;
}

void palavra_adiciona_arquivo(Palavra* p, Pagina* pag) {
    if (palavra_busca_arquivo(p, pag) == -1) {
        if(vector_size(p->arquivos)){
            vector_sort(p->arquivos, pagina_compara);
        }
        vector_push_back(p->arquivos, pag);
    }
}

int palavra_busca_arquivo(Palavra* p, Pagina* pag) {
    int size = vector_size(p->arquivos);

    if (size == 0) {
        return -1; // Não encontrado
    }
    
    Pagina *ultima_pagina_vector = vector_get(p->arquivos, size - 1);

    if (pagina_compara(ultima_pagina_vector, pag) != 0) {
        return -1; // Não encontrado
    }

    return 0;
}

Vector* palavra_retorna_arquivos(Palavra* p){
    return p->arquivos;
}

void palavra_destrutor(void* p) {
    Palavra *palavra = (Palavra*)p;
    vector_destroy_func(palavra->arquivos);

    if (palavra->palavra != NULL) {
        free(palavra->palavra);
    }

    free(palavra);
}

char *palavra_obter_nome(Palavra *p) {
    return p->palavra;
}

int palavra_compara(const void *a, const void *b){
    Palavra *pal_a = *(Palavra **)a;
    Palavra *pal_b = *(Palavra **)b;
    return strcasecmp(pal_a->palavra, pal_b->palavra);
}

Vector *intersecao_arquivos(Vector *v1, Vector *v2) {
    Vector *resultado = vector_construct(NULL);
    int size_v1 = vector_size(v1);
    int size_v2 = vector_size(v2);

    // Vetor de flags para marcar elementos já usados de v2
    int *usado = calloc(size_v2, sizeof(int));

    for (int i = 0; i < size_v1; i++) {
        Pagina *pag_v1 = vector_get(v1, i);

        for (int j = 0; j < size_v2; j++) {
            Pagina *pag_v2 = vector_get(v2, j);

            if (!usado[j] && pagina_compara(pag_v1, pag_v2) == 0) {
                vector_push_back(resultado, pag_v1);
                usado[j] = 1;
                break;
            }
        }
    }

    free(usado);
    vector_destroy_func(v1); // libera o anterior
    return resultado;
}

Vector *palavra_retorna_arquivos_intersecao(Vector *palavras) {
    int qtd_palavras = vector_size(palavras);
    if (qtd_palavras == 0) return vector_construct(NULL);

    // Começa com os arquivos da primeira palavra (faz uma cópia)
    Palavra *primeira = vector_get(palavras, 0);
    Vector *intersecao = vector_clone(primeira->arquivos);

    // Agora vai refinando a interseção com as próximas
    for (int i = 1; i < qtd_palavras; i++) {
        Palavra *p = vector_get(palavras, i);
        Vector *nova_intersecao = intersecao_arquivos(intersecao, p->arquivos);
        intersecao = nova_intersecao;
    }

    return intersecao;
}
