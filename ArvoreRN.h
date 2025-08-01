#ifndef ARVORE_RUBRO_NEGRA_H
#define ARVORE_RUBRO_NEGRA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para as cores dos nós
typedef enum {
    VERMELHO,
    PRETO
} Cor;

// Estrutura do nó da árvore rubro-negra
typedef struct NodeRN {
    char *chave;
    void *valor;
    struct NodeRN *esquerda;
    struct NodeRN *direita;
    struct NodeRN *pai;
    Cor cor;
} NodeRN;

// Estrutura da árvore rubro-negra
typedef struct ArvoreRN ArvoreRN;
typedef void (*func_destruir)(void *elemento);

// Construtor e destrutor
ArvoreRN *arvore_rn_criar();
void arvore_rn_destruir(ArvoreRN *arvore, func_destruir destruir_elemento);

// Operações principais
void arvore_rn_inserir(ArvoreRN *arvore, char *chave, void *dado);
void *arvore_rn_buscar(ArvoreRN *arvore, char *chave);

// Função para debug
void arvore_rn_imprimir(ArvoreRN *arvore);

#endif