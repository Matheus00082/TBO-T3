#include "Node.h"

Node *criar_node(char *chave, void *dado) {
    Node *novo_node = (Node *)malloc(sizeof(Node));

    if (novo_node == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        return NULL;
    }

    novo_node->chave = strdup(chave);
    
    if (novo_node->chave == NULL) {
        printf("Erro ao alocar memória para a chave.\n");
        free(novo_node);
        return NULL;
    }

    novo_node->dado = dado;
    novo_node->num_nodes_arvore = 1;
    novo_node->esquerda = NULL;
    novo_node->direita = NULL;
    return novo_node;
}

int conecta_node(Node *node1, Node *node2) {
    if (node1 == NULL || node2 == NULL) {
        printf("Nó inválido para conexão.\n");
        return -1;
    }

    node1->num_nodes_arvore++;

    // se a chave do no1 for maior que a do no2, conecta no lado direito
    int cmp = strcasecmp(node1->chave, node2->chave);

    if (cmp > 0) {
        if (node1->direita == NULL) {
            node1->direita = node2;
        }
        else {
            conecta_node(node1->direita, node2);
        }
    }
    else if (cmp < 0) {
        if (node1->esquerda == NULL) {
            node1->esquerda = node2;
        }
        else {
            conecta_node(node1->esquerda, node2);
        }
    }
    else {
        return 0; // Chaves iguais, não conecta
    }

    return 1;
}

void destruir_bst(Node *node, void (*destruir_elemento)(void *elemento)) {
    if (node != NULL) {
        // Destrói recursivamente os filhos primeiro
        destruir_bst(node->esquerda, destruir_elemento);
        destruir_bst(node->direita, destruir_elemento);
        
        // Depois destrói o nó atual
        free(node->chave);

        if (destruir_elemento != NULL) {
            destruir_elemento(node->dado);
        }

        free(node);
    }
}