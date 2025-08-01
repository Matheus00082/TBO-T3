#include "Auxiliar.h"

struct Auxiliar {
    char **linhas;
    int qtd_linhas;
};

Auxiliar *ler_arquivo(const char *nomeArquivo) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    Auxiliar *aux = calloc(1, sizeof(Auxiliar));

    if (aux == NULL) {
        printf("Erro ao alocar memória para auxiliar.\n");
        fclose(arquivo);
        return NULL;
    }

    char *buffer = NULL;
    size_t tamanho_linha = 0;
    size_t capacidade = 10;
    aux->linhas = calloc(capacidade, sizeof(char *));

    if (aux->linhas == NULL) {
        printf("Erro ao alocar memória para linhas.\n");
        fclose(arquivo);
        return NULL;
    }

    while (getline(&buffer, &tamanho_linha, arquivo) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;

        if (aux->qtd_linhas >= capacidade) {
            capacidade *= 2;
            aux->linhas = realloc(aux->linhas, capacidade * sizeof(char *));

            if (aux->linhas == NULL) {
                printf("Erro ao realocar memória para linhas.\n");
                fclose(arquivo);
                return NULL;
            }
        }

        aux->linhas[aux->qtd_linhas] = strdup(buffer);

        if (aux->linhas[aux->qtd_linhas] == NULL) {
            printf("Erro ao duplicar a linha.\n");
            fclose(arquivo);
            return NULL;
        }

        aux->qtd_linhas++;
    }

    if (buffer != NULL) {
        free(buffer);
    }

    fclose(arquivo);
    return aux;
}

char **obter_linhas(Auxiliar *a) {
    return a->linhas;
}

int obter_qtd_linhas(Auxiliar *a) {
    return a->qtd_linhas;
}

void destruir_auxiliar(Auxiliar *a) {
    if (a != NULL) {
        for (int i = 0; i < a->qtd_linhas; i++) {
            if (a->linhas[i] != NULL) {
                free(a->linhas[i]);
            }
        }

        free(a->linhas);
        free(a);
    }
}

Pagina **retornar_paginas(Auxiliar *a) {

    int qtd_linhas = obter_qtd_linhas(a);
    Pagina **paginas = malloc(qtd_linhas * sizeof(Pagina *));

    if (paginas == NULL) {
        printf("Erro ao alocar memória para páginas.\n");
        return NULL;
    }

    char **linhas = obter_linhas(a);

    for (int i = 0; i < qtd_linhas; i++) {
        char *nome_pagina = strdup(linhas[i]);
        paginas[i] = criar_pagina(nome_pagina, qtd_linhas);
    }

    return paginas;
}

TabelaHash *cria_tabela_arquivos(Auxiliar *a, func_hash hash) {
    int qtd_linhas = obter_qtd_linhas(a);
    TabelaHash *tabela = criar_tabela_hash(qtd_linhas, hash, destruir_pagina);
    Pagina **paginas = retornar_paginas(a);

    for (int i = 0; i < qtd_linhas; i++) {
        if (paginas[i] != NULL) {
            Node *novo_node = criar_node(obter_nome_pagina(paginas[i]), paginas[i]);
            inserir_elemento(tabela, novo_node);
        }
    }

    free(paginas);
    return tabela;
}

// Abre e lê cada arquivo e insere as palavras na tabela hash
TabelaHash *cria_tabela_palavras(Auxiliar *a, func_hash hash) {
    int qtdArquivos = obter_qtd_linhas(a);
    char **nome_arquivos = obter_linhas(a);

    for (int i = 0; i < qtdArquivos; i++) {
        char caminho_arquivo[256];
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "exemplo/pages/%s", nome_arquivos[i]);
        FILE *arquivo = fopen(caminho_arquivo, "r");

        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo: %s\n", caminho_arquivo);
            continue;
        }

        char *buffer = NULL;
        size_t tamanho_buffer = 0;

        while (getline(&buffer, &tamanho_buffer, arquivo) != -1) {
            buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
            printf("Linha lida: %s\n", buffer);
        }

        free(buffer);
        fclose(arquivo);
    }

    return NULL;
}