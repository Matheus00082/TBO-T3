#include "TabelaHash.h"
#include "ArvoreRN.h"

struct TabelaHash {
    ArvoreRN **tabela;                   // Array de ponteiros para nós da tabela hash
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
    nova_tabela->tabela = calloc(tamanho, sizeof(ArvoreRN *));

    if (nova_tabela->tabela == NULL) {
        printf("Erro ao alocar memória para a tabela.\n");
        free(nova_tabela);
        return NULL;
    }

    for (int i = 0; i < tamanho; i++) {
        nova_tabela->tabela[i] = arvore_rn_criar();
    }

    return nova_tabela;
}

int tamanho_tabela_hash(TabelaHash *tabela) {
    if (tabela == NULL) {
        printf("Tabela inválida.\n");
        return -1;
    }

    return tabela->tamanho;
}

void inserir_elemento(TabelaHash *tabela, void *chave, void *valor) {
    if (tabela == NULL || chave == NULL || valor == NULL) {
        printf("Tabela ou elemento inválido.\n");
        return;
    }

    int indice = tabela->hash(tabela, chave);

    if (indice == -1) {
        printf("Erro ao calcular índice da tabela hash.\n");
        return;
    }

    arvore_rn_inserir(tabela->tabela[indice], chave, valor, tabela->destruir_elemento);
    tabela->qtd_elementos++;
}

void *buscar_elemento(TabelaHash *tabela, char *chave) {
    if (tabela == NULL || chave == NULL) {
        printf("Tabela ou chave inválida.\n");
        return NULL;
    }

    int indice = tabela->hash(tabela, chave);
    ArvoreRN *arvore = tabela->tabela[indice];
    void *valor = arvore_rn_buscar(arvore, chave);

    if (valor != NULL) {
        return valor;
    }

    return NULL;
}

void destruir_tabela_hash(TabelaHash *tabela) {
    if (tabela != NULL) {
        for (int i = 0; i < tabela->tamanho; i++) {
            if (tabela->tabela[i] != NULL) {
                arvore_rn_destruir(tabela->tabela[i], tabela->destruir_elemento);
            }
        }

        free(tabela->tabela);
        free(tabela);
    }
}

void imprimir_tabela_hash(TabelaHash *tabela) {
    if (tabela == NULL) {
        printf("Tabela ou função de impressão inválida.\n");
        return;
    }

    for (int i = 0; i < tabela->tamanho; i++) {
        ArvoreRN *arvore = tabela->tabela[i];
        if (arvore != NULL) {
            arvore_rn_imprimir(arvore);
        }
    }
}