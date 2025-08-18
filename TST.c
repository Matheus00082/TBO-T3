#include "TST.h"

struct NodeTST {
    void *valor;
    unsigned char caractere;
    TST *esquerda;
    TST *meio;
    TST *direita;
};

TST *tst_criar() {
    TST *tst = (TST *)malloc(sizeof(TST));

    if (tst == NULL) {
        printf("Erro ao alocar memória para a TST.\n");
        return NULL;
    }
    
    tst->valor = NULL;
    tst->esquerda = NULL;
    tst->meio = NULL;
    tst->direita = NULL;

    return tst;
}

void tst_destruir(TST *tst, func_destruir destruir_elemento) {
    if (tst != NULL) {
        // Destrói recursivamente os filhos primeiro
        tst_destruir(tst->esquerda, destruir_elemento);
        tst_destruir(tst->meio, destruir_elemento);
        tst_destruir(tst->direita, destruir_elemento);

        if (destruir_elemento != NULL) {
            destruir_elemento(tst->valor);
        }

        free(tst);
    }
}

TST *tst_inserir_recursivamente(TST *tst, String *chave, void *dado, int d) {
    unsigned char c = get_caractere_string(chave, d);

    if (tst == NULL) { 
        tst = tst_criar(); 
        tst->caractere = c;
    }
    
    if (c < tst->caractere) {
        tst->esquerda = tst_inserir_recursivamente(tst->esquerda, chave, dado, d);
    } else if (c > tst->caractere) {
        tst->direita = tst_inserir_recursivamente(tst->direita, chave, dado, d);
    } else if (d < get_tamanho(chave) -1) {
        tst->meio = tst_inserir_recursivamente(tst->meio, chave, dado, d+1);
    } else {
        tst->valor = dado;
    }

    return tst;
}

TST *tst_inserir(TST *tst, String *chave, void *dado) {
    return tst_inserir_recursivamente(tst, chave, dado, 0);
}

TST *tst_buscar_recursivamente(TST *tst, String *chave, int d) {
    if (tst == NULL) {
        return NULL;
    }

    unsigned char c = get_caractere_string(chave, d);

    if (c < tst->caractere) {
        return tst_buscar_recursivamente(tst->esquerda, chave, d);
    } else if (c > tst->caractere) {
        return tst_buscar_recursivamente(tst->direita, chave, d);
    } else if (d < get_tamanho(chave) -1) {
        return tst_buscar_recursivamente(tst->meio, chave, d+1);
    } else {
        return tst;
    }
}

void *tst_buscar(TST *tst, String *chave) {
    tst = tst_buscar_recursivamente(tst, chave, 0);
    
    if (tst == NULL) {
        return NULL;
    } else {
        return tst->valor;
    }
}

void *get_valor(TST *tst) {
    if (tst != NULL) {
        return tst->valor;
    }
    return NULL;
}