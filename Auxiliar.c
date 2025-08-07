#include "Auxiliar.h"
#include "Vector.h"

struct Auxiliar {
    char **linhas;
    int qtd_paginas;
    ArvoreRN *stop_words;
    ArvoreRN *tabela_palavras;
    ArvoreRN *tabela_arquivos;
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

        if (aux->qtd_paginas >= capacidade) {
            capacidade *= 2;
            aux->linhas = realloc(aux->linhas, capacidade * sizeof(char *));

            if (aux->linhas == NULL) {
                printf("Erro ao realocar memória para linhas.\n");
                fclose(arquivo);
                return NULL;
            }
        }

        aux->linhas[aux->qtd_paginas] = strdup(buffer);

        if (aux->linhas[aux->qtd_paginas] == NULL) {
            printf("Erro ao duplicar a linha.\n");
            fclose(arquivo);
            return NULL;
        }

        aux->qtd_paginas++;
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

int obter_qtd_paginas(Auxiliar *a) {
    return a->qtd_paginas;
}

void destruir_auxiliar(Auxiliar *a) {
    if (a != NULL) {
        for (int i = 0; i < a->qtd_paginas; i++) {
            if (a->linhas[i] != NULL) {
                free(a->linhas[i]);
            }
        }
        if (a->stop_words != NULL) {
            arvore_rn_destruir(a->stop_words, free);
        }

        if (a->tabela_palavras != NULL) {
            arvore_rn_destruir(a->tabela_palavras, palavra_destrutor);
        }

        if (a->tabela_arquivos != NULL) {
            arvore_rn_destruir(a->tabela_arquivos, pagina_destrutor);
        }
        
        free(a->linhas);
        free(a);
    }
}

void cria_tabela_arquivos(Auxiliar *a, func_hash hash) {
    int qtd_linhas = obter_qtd_linhas(a);
    char **linhas = obter_linhas(a);
    ArvoreRN *tabela = arvore_rn_criar();

    for (int i = 0; i < qtd_linhas; i++) {
        char *nome_pagina = linhas[i];
        Pagina *pagina = pagina_construtor(strdup(nome_pagina), qtd_linhas);
        arvore_rn_inserir(tabela, nome_pagina, pagina, pagina_destrutor);
    }

    a->tabela_arquivos = tabela;
}

void cria_tabela_stop_words(Auxiliar *a, func_hash hash) {
    FILE* arquivo_stop_words = fopen("exemplo/stopwords.txt", "r");
    ArvoreRN *tabela = arvore_rn_criar();
    char *buffer = NULL;
    size_t tamanho_linha = 0;
    while (getline(&buffer, &tamanho_linha, arquivo_stop_words) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        arvore_rn_inserir(tabela, buffer, strdup(buffer), free);
    }
    fclose(arquivo_stop_words);
    if (buffer != NULL) {
        free(buffer);
    }
    
    a->stop_words = tabela;
}

// Abre e lê cada arquivo e insere as palavras na tabela hash
void cria_tabela_palavras(Auxiliar *a, func_hash hash) {
    int qtd_arquivos = obter_qtd_linhas(a);
    char **nome_arquivos = obter_linhas(a);
    ArvoreRN *tabela = arvore_rn_criar();

    for (int i = 0; i < qtd_arquivos; i++) {
        char caminho_arquivo[256];
        char *nome_arquivo = nome_arquivos[i];
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "exemplo/pages/%s", nome_arquivo);
        FILE *arquivo = fopen(caminho_arquivo, "r");

        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo: %s\n", caminho_arquivo);
            continue;
        }

        char *buffer = NULL;
        size_t tamanho_buffer = 0;

        while (getline(&buffer, &tamanho_buffer, arquivo) != -1) {
            char *string_palavra = strtok(buffer, " \t\n"); // Tokeniza a linha por espaços, tabulações ou quebras de linha

            while (string_palavra != NULL) {
                if (arvore_rn_buscar(a->stop_words, string_palavra) == NULL) {
                    Palavra *p = (Palavra *)arvore_rn_buscar(tabela, string_palavra);
                    Pagina *pag = arvore_rn_buscar(a->tabela_arquivos, nome_arquivo);

                    if (p == NULL) {
                        p = palavra_construtor(string_palavra);
                        palavra_adiciona_arquivo(p, pag);
                        arvore_rn_inserir(tabela, string_palavra, p, palavra_destrutor);
                    }
                    else {
                        if (palavra_busca_arquivo(p, pag) == -1) {
                            palavra_adiciona_arquivo(p, pag);
                        }
                    }
                }
                string_palavra = strtok(NULL, " \t\n");
            }
        }

        if (buffer != NULL) free(buffer);
        if (arquivo != NULL) fclose(arquivo);
    }
    
    a->tabela_palavras = tabela;
}

void le_arquivo_graph(Auxiliar* a) {
    
}

void realiza_busca_palavras(Auxiliar* a, const char* palavra) {
    // pega na arvoreRN de palavras a palavra ( se existir)
    // seria algo do tipo:
    // printf("search:%s\n", palavra);
    // verificar se tem mais de uma palavra e dp buscar na RN: 
    // Palavra* palavras = (Palavra*)arvore_rn_buscar(a->tabela_palavras, palavra); (caso so tenha 1 seria assim)
    // calcula o page rank de CADA palavra
    // printa os arquivos e o page rank
    //for(int i = 0; i < num_palavra_busca; i++){
    //      if(i == 0){
    //          printf("pages:");  
    //      }      
    //      Vector* arquivos = retorna_arquivos_palavra(palavras[i]);
    //      for(int i = 0; i < vector_size(arquivos); i++){
    //          Pagina* p = vector_get(arquivos, i);
    //              printf("%s ", obter_nome_pagina(p));          
    //      }
    //          
    //      
    //}
    // printf("\n");
    //for(int i = 0; i < num_palavra_busca; i++){
    //      if(i == 0){
    //          printf("pr:");  
    //      }      
    //      if(pg_rank[i] != 0){
    //          printf("%f ",pg_rank[i]);
    //      }
    //}
    //

}