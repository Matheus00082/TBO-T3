#ifndef ARVORE_RUBRO_NEGRA_H
#define ARVORE_RUBRO_NEGRA_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Enum para as cores dos nós
typedef enum {
    PRETO,
    VERMELHO
} Cor;

// Estrutura da árvore rubro-negra
typedef struct ArvoreRN ArvoreRN;
typedef struct NodeRN NodeRN;
typedef void (*func_destruir)(void *elemento);
typedef void (*func_destruir_elemento)(void *elemento);

// Construtor e destrutor
ArvoreRN *arvore_rn_criar();
void arvore_rn_destruir(ArvoreRN *arvore, func_destruir destruir_elemento);

// Operações principais
void arvore_rn_inserir(ArvoreRN *arvore, char *chave, void *dado, func_destruir destruir_elemento);
void *arvore_rn_buscar(ArvoreRN *arvore, char *chave);

// Função para debug
void arvore_rn_imprimir(ArvoreRN *arvore);

void *NodeRN_get_valor(NodeRN *node);

#endif