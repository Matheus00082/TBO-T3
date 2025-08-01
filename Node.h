#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char *chave;             // chave do nó
    void *dado;              // dado associado ao nó
    struct Node *esquerda;   // ponteiro para o nó à esquerda
    struct Node *direita;    // ponteiro para o nó à direita
    int num_nodes_arvore;    // número de nós na subárvore
} Node;


// @ Cria um novo nó hash
Node *criar_node(char *chave, void *dado);

// @ Conecta dois nós hash que colidem na mesma posição da tabela hash
int conecta_node(Node *node1, Node *node2);

// @ Destrói os nós da árvore binária de busca
void destruir_bst(Node *node, void (*destruir_elemento)(void *elemento));

#endif