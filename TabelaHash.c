#include "TabelaHash.h"

struct TabelaHash {
    Node **tabela;                   // Array de ponteiros para nós da tabela hash
    int tamanho;                     // Tamanho da tabela hash
    int qtd_elementos;               // Quantidade de elementos na tabela hash
    func_hash hash;                  // Função de hash
    func_destruir destruir_elemento; // Função de destruição de elemento
};

TabelaHash *criar_tabela_hash(int tamanho, func_hash hash, func_destruir destruir_elemento) {
    TabelaHash *nova_tabela = (TabelaHash *)malloc(sizeof(TabelaHash));

    if (nova_tabela == NULL) {
        printf("Erro ao alocar memória para a tabela hash.\n");
        return NULL;
    }

    nova_tabela->tamanho = tamanho;
    nova_tabela->qtd_elementos = 0;
    nova_tabela->hash = hash;
    nova_tabela->destruir_elemento = destruir_elemento;
    nova_tabela->tabela = calloc(tamanho, sizeof(Node *));

    if (nova_tabela->tabela == NULL) {
        printf("Erro ao alocar memória para a tabela.\n");
        free(nova_tabela);
        return NULL;
    }

    return nova_tabela;
}

void inserir_elemento(TabelaHash *tabela, Node *novo_no) {
    if (tabela == NULL || novo_no == NULL) {
        printf("Tabela ou elemento inválido.\n");
        return;
    }

    int indice = tabela->hash(novo_no->chave, tabela->tamanho);

    if (indice == -1) {
        printf("Erro ao calcular índice da tabela hash.\n");
        return;
    }

    if (tabela->tabela[indice] == NULL) {
        tabela->tabela[indice] = novo_no;
    }
    else {
        conecta_node(tabela->tabela[indice], novo_no);
    }

    tabela->qtd_elementos++;

    // Redimensionar a tabela hash
    if (tabela->qtd_elementos >= tabela->tamanho) {
        int novo_tamanho = tabela->tamanho * 2;
        Node **nova_tabela = calloc(novo_tamanho, sizeof(Node *));

        if (nova_tabela == NULL) {
            printf("Erro ao alocar nova tabela.\n");
            return;
        }

        // Salva dados antigos
        Node **tabela_antiga = tabela->tabela;
        int tamanho_antigo = tabela->tamanho;

        // Atualiza a tabela
        tabela->tabela = nova_tabela;
        tabela->tamanho = novo_tamanho;
        tabela->qtd_elementos = 0; // será recontado ao reinserir

        // Reinsere todos os elementos da tabela antiga
        for (int i = 0; i < tamanho_antigo; i++) {
            if (tabela_antiga[i] != NULL) {
                inserir_elemento(tabela, tabela_antiga[i]);
            }
        }

        free(tabela_antiga);
    }
}

void *buscar_elemento(TabelaHash *tabela, char *chave) {
    if (tabela == NULL || chave == NULL) {
        printf("Tabela ou chave inválida.\n");
        return NULL;
    }

    int indice = tabela->hash(chave, tabela->tamanho);
    Node *no_atual = tabela->tabela[indice];

    if (no_atual == NULL) {
        printf("Nenhum elemento encontrado para a chave: %s\n", chave);
        return NULL;
    }

    while (no_atual != NULL) {
        int comparacao = strcmp(no_atual->chave, chave);

        if (comparacao == 0) {
            return no_atual->dado;
        }
        else if (comparacao < 0) {
            no_atual = no_atual->esquerda;
        }
        else {
            no_atual = no_atual->direita;
        }
    }

    return NULL;
}

void destruir_tabela_hash(TabelaHash *tabela) {
    if (tabela != NULL) {
        for (int i = 0; i < tabela->tamanho; i++) {
            if (tabela->tabela[i] != NULL) {
                destruir_nodes(tabela->tabela[i], tabela->destruir_elemento);
                tabela->tabela[i] = NULL;
            }
        }

        free(tabela->tabela);
        free(tabela);
    }
}