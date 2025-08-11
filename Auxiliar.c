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

void cria_arquivos(Auxiliar *a) {
    int qtd_linhas = a->qtd_paginas;
    char **linhas = obter_linhas(a);
    ArvoreRN *tabela = arvore_rn_criar();

    for (int i = 0; i < qtd_linhas; i++) {
        char *nome_pagina = linhas[i];
        Pagina *pagina = pagina_construtor(strdup(nome_pagina), qtd_linhas);
        arvore_rn_inserir(tabela, nome_pagina, pagina, pagina_destrutor);
    }

    a->tabela_arquivos = tabela;
}

void cria_stop_words(Auxiliar *a) {
    FILE *arquivo_stop_words = fopen("exemplo/stopwords.txt", "r");
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

void cria_palavras(Auxiliar *a) {
    int qtd_arquivos = a->qtd_paginas;
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

void inicializa_arquivos(const char *nomeArquivo, Auxiliar *a) {
    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", nomeArquivo);
        return;
    }

    char *buffer = NULL;
    size_t tamanho_linha = 0;

    while (getline(&buffer, &tamanho_linha, arquivo) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *nome_pagina = strtok(buffer, " ");
        Pagina *pag = arvore_rn_buscar(a->tabela_arquivos, nome_pagina);

        int qtd_links_saida = atoi(strtok(NULL, " "));
        //aloca_lista_links_saida(pag, qtd_links_saida);

        for (int i = 0; i < qtd_links_saida; i++) {
            char *nome_link_saida = strtok(NULL, " ");
            Pagina *pag_saida = arvore_rn_buscar(a->tabela_arquivos, nome_link_saida);

            if (pag_saida == NULL) {
                continue;
            }

            pagina_adiciona_link_saida(pag, pag_saida);
            pagina_adiciona_link_entrada(pag_saida, pag);
        }
    }

    if (buffer != NULL) {
        free(buffer);
    }

    if (arquivo != NULL) {
        fclose(arquivo);
    }
}

double calcula_pagerank_pagina(Pagina* pagina, int total_paginas) {
    double base = (1.0 - ALFA) / (double)total_paginas;
    double soma_in = 0.0;

    int qtd_links_entrada = pagina_obter_qtd_links_entrada(pagina);

    for (int j = 0; j < qtd_links_entrada; j++) {
        Pagina* origem = pagina_obter_link_entrada(pagina, j);
        int qtd_links_saida = pagina_obter_qtd_links_saida(origem);
        
        if (qtd_links_saida > 0) {
            soma_in += pagina_obter_pagerank(origem) / (double)qtd_links_saida;
        }
    }
    
    return base + (ALFA * soma_in);
}

void calcula_pagerank_global(Auxiliar* a) {
    int total_paginas = a->qtd_paginas;
    char **nomes_paginas = obter_linhas(a);
    
    // Cria array com todas as páginas
    Pagina** todas_paginas = calloc(total_paginas, sizeof(Pagina*));
    
    for (int i = 0; i < total_paginas; i++) {
        todas_paginas[i] = arvore_rn_buscar(a->tabela_arquivos, nomes_paginas[i]);
    }
    
    // Algoritmo do Power Method para calcular PageRank
    double erro = EPSILON + 1;
    
    while (erro >= EPSILON) {
        erro = 0;
        // Calcula novos valores de PageRank para todas as páginas
        double *novos_pageranks = calloc(total_paginas, sizeof(double));
        
        for (int i = 0; i < total_paginas; i++) {
            novos_pageranks[i] = calcula_pagerank_pagina(todas_paginas[i], total_paginas);
        }
        
        // Atualiza PageRanks e calcula erro
        for (int i = 0; i < total_paginas; i++) {
            double antigo = pagina_obter_pagerank(todas_paginas[i]);
            pagina_definir_pagerank(todas_paginas[i], novos_pageranks[i]);
            erro += fabs(novos_pageranks[i] - antigo);
        }
        
        erro /= total_paginas;
        free(novos_pageranks);
    }
    
    free(todas_paginas);
}

void realiza_busca_palavras(Auxiliar* a, char* search_palavras) {
    printf("search:%s\n", search_palavras);
    char *palavra = strtok(search_palavras, " ");
    Vector *palavras = vector_construct(NULL);

    while (palavra != NULL) {
        Palavra *p = (Palavra *)arvore_rn_buscar(a->tabela_palavras, palavra);
        
        if (p == NULL) {
            // Palavra não encontrada - interseção será vazia
            vector_destroy_func(palavras);
            Vector *vazio = vector_construct(NULL);
            printf("pages:\n");
            printf("pr:\n");
            return;
        }
        else if (arvore_rn_buscar(a->stop_words, palavra_obter_nome(p)) != NULL) {
            continue; // Ignora stop words
        }
        
        vector_push_back(palavras, p);
        palavra = strtok(NULL, " ");
    }

    Vector* arquivos =  palavra_retorna_arquivos_intersecao(palavras);
    int qtd_arquivos = vector_size(arquivos);
    Pagina** paginas = calloc(qtd_arquivos, sizeof(Pagina*));
    printf("pages:");

    for (int i = 0; i < qtd_arquivos; i++) {
        paginas[i] = vector_get(arquivos, i);
        printf("%s", pagina_obter_nome(paginas[i]));

        if (i < qtd_arquivos - 1) {
            printf(" ");
        }
        else {
            printf("\n");
        }
    }

    if (qtd_arquivos == 0) {
        printf("\n");
    }

    printf("pr:");

    for (int i = 0; i < qtd_arquivos; i++) {
        double pagerank = pagina_obter_pagerank(paginas[i]);
        printf("%.16lf", pagerank);

        if (i < qtd_arquivos - 1) {
            printf(" ");
        }
    }

    printf("\n");
    vector_destroy_func(palavras);
    vector_destroy_func(arquivos);
    free(paginas);
}