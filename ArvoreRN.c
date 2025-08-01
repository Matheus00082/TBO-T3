#include "ArvoreRN.h"

struct ArvoreRN {
    NodeRN *raiz;
};

NodeRN *criar_node_rn(char *chave, void *dado, NodeRN *nil) {
    NodeRN *novo_no = (NodeRN *)malloc(sizeof(NodeRN));

    if (novo_no == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        return NULL;
    }
    
    novo_no->chave = strdup(chave);

    if (novo_no->chave == NULL) {
        printf("Erro ao alocar memória para a chave.\n");
        free(novo_no);
        return NULL;
    }
    
    novo_no->dado = dado;
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    novo_no->pai = NULL;
    novo_no->cor = VERMELHO; // Novos nós sempre começam vermelhos
    return novo_no;
}

void destruir_node_rn(NodeRN *node, func_destruir destruir_elemento) {
    if (node != NULL) {
        // Destrói recursivamente os filhos primeiro
        destruir_node_rn(node->esquerda, destruir_elemento);
        destruir_node_rn(node->direita, destruir_elemento);

        // Destrói o nó atual
        free(node->chave);

        if (destruir_elemento != NULL) {
            destruir_elemento(node->dado);
        }

        free(node);
    }
}

void rotacao_esquerda(ArvoreRN *arvore, NodeRN *x) {
    //pass
}

void rotacao_direita(ArvoreRN *arvore, NodeRN *x) {
   //pass
}

void color_flip(NodeRN *node) {
    //pass
}

NodeRN *buscar_node(NodeRN *node, char *chave) {
    while (node != NULL && strcmp(chave, node->chave) != 0) {
        if (strcmp(chave, node->chave) < 0) {
            node = node->esquerda;
        } 
        else {
            node = node->direita;
        }
    }

    return node;
}

void imprimir_node(NodeRN *node, int nivel) {
    if (node != NULL) {
        imprimir_node(node->direita, nivel + 1);

        for (int i = 0; i < nivel; i++) {
            printf("    ");
        }

        printf("%s (%s)\n", node->chave, node->cor == VERMELHO ? "V" : "P");
        imprimir_node(node->esquerda, nivel + 1);
    }
}

ArvoreRN *arvore_rn_criar() {
    ArvoreRN *arvore = (ArvoreRN *)malloc(sizeof(ArvoreRN));

    if (arvore == NULL) {
        printf("Erro ao alocar memória para a árvore.\n");
        return NULL;
    }

    arvore->raiz = NULL; 
    return arvore;
}

void arvore_rn_destruir(ArvoreRN *arvore, func_destruir destruir_elemento) {
    if (arvore != NULL) {
        destruir_node_rn(arvore->raiz, destruir_elemento);
        free(arvore);
    }
}

void arvore_rn_inserir(ArvoreRN *arvore, char *chave, void *dado) {
    //pass
}

void *arvore_rn_buscar(ArvoreRN *arvore, char *chave) {
    if (arvore == NULL || chave == NULL) {
        return NULL;
    }

    NodeRN *no = buscar_node(arvore->raiz, chave);
    
    if (no != NULL) {
        return no->dado;
    }
    
    return NULL;
}

void arvore_rn_imprimir(ArvoreRN *arvore) {
    if (arvore != NULL) {
        printf("Árvore Rubro-Negra:\n");

        if (arvore->raiz == NULL) {
            printf("(vazia)\n");
        } 
        else {
            imprimir_node(arvore->raiz, 0);
        }

        printf("\n");
    }
}