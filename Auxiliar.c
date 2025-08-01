#include "Auxiliar.h"
#include "Vector.h"

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
    size_t capacidade = CAPACIDADE_INICIAL;
    aux->linhas = calloc(capacidade, sizeof(char *));

    if (aux->linhas == NULL) {
        printf("Erro ao alocar memória para linhas.\n");
        fclose(arquivo);
        return NULL;
    }

    while (getline(&buffer, &tamanho_linha, arquivo) != -1) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

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

TabelaHash *cria_tabela_arquivos(Auxiliar *a, func_hash hash) {
    int qtd_linhas = obter_qtd_linhas(a);
    char **linhas = obter_linhas(a);
    TabelaHash *tabela = criar_tabela_hash(qtd_linhas, hash, destruir_pagina);

    for (int i = 0; i < qtd_linhas; i++) {
        char *nome_pagina = strdup(linhas[i]);
        Pagina *pagina = criar_pagina(nome_pagina, qtd_linhas);
        inserir_elemento(tabela, nome_pagina, pagina);
    }

    return tabela;
}

TabelaHash *cria_tabela_stop_words(func_hash hash) {
    Auxiliar *aux = ler_arquivo("exemplo/stopwords.txt");
    int qtd_linhas = obter_qtd_linhas(aux);
    char **linhas = obter_linhas(aux);
    TabelaHash *tabela = criar_tabela_hash(qtd_linhas, hash, free);

    for (int i = 0; i < qtd_linhas; i++) {
        char *palavra = strdup(linhas[i]);
        inserir_elemento(tabela, palavra, palavra);
    }

    destruir_auxiliar(aux);
    return tabela;
}

// Abre e lê cada arquivo e insere as palavras na tabela hash
TabelaHash *cria_tabela_palavras(Auxiliar *a, TabelaHash *stop_words, func_hash hash) {
    int qtd_arquivos = obter_qtd_linhas(a);
    char **nome_arquivos = obter_linhas(a);
    TabelaHash *tabela = criar_tabela_hash(qtd_arquivos, hash, vector_destroy);

    for (int i = 0; i < qtd_arquivos; i++) {
        char caminho_arquivo[256];
        char *nome_arquivo = strdup(nome_arquivos[i]);
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "exemplo/pages/%s", nome_arquivo);
        FILE *arquivo = fopen(caminho_arquivo, "r");

        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo: %s\n", caminho_arquivo);
            continue;
        }

        char *buffer = NULL;
        size_t tamanho_buffer = 0;

        while (getline(&buffer, &tamanho_buffer, arquivo) != -1) {
            // buffer[strcspn(buffer, "\n")] = 0;     // Remove newline character
            char *palavra = strtok(buffer, " \t\n"); // Tokeniza a linha por espaços, tabulações ou quebras de linha

            while (palavra != NULL) {
                if (buscar_elemento(stop_words, palavra) == NULL) {
                    Vector *v = (Vector *)buscar_elemento(tabela, palavra);

                    if (v == NULL) {
                        v = vector_construct(free);
                        vector_push_back(v, nome_arquivo);
                        char *palavra_copia = strdup(palavra);
                        inserir_elemento(tabela, palavra_copia, v);
                    }
                    else {
                        if (vector_find(v, nome_arquivo, strcmp) == -1) {
                            vector_push_back(v, nome_arquivo);
                        }
                    }
                }

                palavra = strtok(NULL, " \t\n");
            }
        }

        free(buffer);
        fclose(arquivo);
    }
    
    return NULL;
}