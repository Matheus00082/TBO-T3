#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *chave;             // chave do nó
    void *dado;              // dado associado ao nó
    struct Node *esquerda; // ponteiro para o nó à esquerda
    struct Node *direita;  // ponteiro para o nó à direita
    int numNosNaArvore;     // número de nós na subárvore
} Node;


// @ Cria um novo nó hash
Node *Criar_Node(char *nome, void *dado);

// @ Conecta dois nós hash que colidem na mesma posição da tabela hash
int Conecta_Node(Node *no1, Node *no2);

// @ Destrói um nó hash
void Destruir_Nodes(Node *no, void (*destruirElemento)(void *elemento));

#endif