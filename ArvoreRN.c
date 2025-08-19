#include "ArvoreRN.h"

// Estrutura do nó da árvore rubro-negra
struct NodeRN {
    char *chave;
    void *valor;
    struct NodeRN *esquerda;
    struct NodeRN *direita;
    Cor cor;
    int tamanho; // Número de nós na subárvore
};

struct ArvoreRN {
    struct NodeRN *raiz;
};

NodeRN *criar_node_rn(char *chave, void *valor) {
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
    
    novo_no->valor = valor;
    novo_no->esquerda = NULL;
    novo_no->direita = NULL;
    novo_no->cor = VERMELHO; // Novos nós sempre começam vermelhos
    novo_no->tamanho = 1; // Inicializa o número de nós na subárvore como 1 (o próprio nó)
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
            destruir_elemento(node->valor);
        }

        free(node);
    }
}

int NodeRN_get_tamanho(NodeRN *node) {
    if (node == NULL) {
        return 0;
    }

    return node->tamanho;
}

NodeRN *rotacao_esquerda(NodeRN *x) {
    NodeRN *y = x->direita;
    x->direita = y->esquerda;
    y->esquerda = x;
    y->cor = x->cor;
    x->cor = VERMELHO; 
    y->tamanho = x->tamanho;
    x->tamanho = 1 + NodeRN_get_tamanho(x->esquerda) + NodeRN_get_tamanho(x->direita);
    return y;
}

NodeRN *rotacao_direita(NodeRN *x) {
    NodeRN *y = x->esquerda;
    x->esquerda = y->direita;
    y->direita = x;
    y->cor = x->cor;
    x->cor = VERMELHO; 
    y->tamanho = x->tamanho;
    x->tamanho = 1 + NodeRN_get_tamanho(x->esquerda) + NodeRN_get_tamanho(x->direita);
    return y;
}

void mudar_cor(NodeRN *x) {
    x->cor = VERMELHO;
    x->esquerda->cor = PRETO;
    x->direita->cor = PRETO;
}

int eh_vermelho(NodeRN *node) {
    return node != NULL && node->cor == VERMELHO;
}

int eh_preto(NodeRN *node) {
    return node == NULL || node->cor == PRETO;
}

NodeRN *buscar_node(NodeRN *node, char *chave) {
    while (node != NULL && strcasecmp(chave, node->chave) != 0) {
        if (strcasecmp(chave, node->chave) < 0) {
            node = node->esquerda;
        } 
        else {
            node = node->direita;
        }
    }

    if (node == NULL) {
        return NULL;
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

NodeRN *inserir_node(NodeRN *x, char *chave, void *valor, func_destruir destruir_elemento) {
    if (x == NULL) {
        return criar_node_rn(chave, valor);
    }

    int cmp = strcasecmp(chave, x->chave);

    if (cmp < 0) {
        x->esquerda = inserir_node(x->esquerda, chave, valor, destruir_elemento);
    } 
    else if (cmp > 0) {
        x->direita = inserir_node(x->direita, chave, valor, destruir_elemento);
    } 
    else {
        if (x->valor != NULL) {
            destruir_elemento(x->valor);
        }
        
        x->valor = valor;
    }

    if (eh_vermelho(x->direita) && !eh_vermelho(x->esquerda)) {
        x = rotacao_esquerda(x);
    }

    if (eh_vermelho(x->esquerda) && eh_vermelho(x->esquerda->esquerda)) {
        x = rotacao_direita(x);
    }

    if (eh_vermelho(x->esquerda) && eh_vermelho(x->direita)) {
        mudar_cor(x);
    }

    x->tamanho = NodeRN_get_tamanho(x->esquerda) + NodeRN_get_tamanho(x->direita) + 1;
    return x;
}

void arvore_rn_inserir(ArvoreRN *arvore, char *chave, void *valor, func_destruir destruir_elemento) {
    arvore->raiz = inserir_node(arvore->raiz, chave, valor, destruir_elemento);
    arvore->raiz->cor = PRETO;
}

void *arvore_rn_buscar(ArvoreRN *arvore, char *chave) {
    if (arvore == NULL || chave == NULL) {
        return NULL;
    }

    NodeRN *no = buscar_node(arvore->raiz, chave);
    return NodeRN_get_valor(no);
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

void *NodeRN_get_valor(NodeRN *node) {
    if (node != NULL) {
        return node->valor;
    }
    return NULL;
}
