#include "Auxiliar.h"
#include "Vector.h"

struct Auxiliar {
    char **linhas;
    int qtd_paginas;
    TST *stop_words;
    TST *tabela_palavras;
    TST *tabela_arquivos;
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
        buffer[strcspn(buffer, "\n")] = 0;      // Remove newline character

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
            tst_destruir(a->stop_words, free);
        }

        if (a->tabela_palavras != NULL) {
            tst_destruir(a->tabela_palavras, palavra_destrutor);
        }

        if (a->tabela_arquivos != NULL) {
            tst_destruir(a->tabela_arquivos, pagina_destrutor);
        }
        
        free(a->linhas);
        free(a);
    }
}

void cria_arquivos(Auxiliar *a) {
    int qtd_linhas = a->qtd_paginas;
    char **linhas = obter_linhas(a);
    TST *tabela = tst_criar();

    for (int i = 0; i < qtd_linhas; i++) {
        char *nome_pagina = linhas[i];
        String *string = string_criar(nome_pagina);
        //printf("%c\n", get_caractere_string(string, 0));
        Pagina *pagina = pagina_construtor(nome_pagina, qtd_linhas);
        tst_inserir(tabela, string, pagina);
        //string_destruir(string);
    }
    a->tabela_arquivos = tabela;
}

void cria_stop_words(Auxiliar *a, char const *pasta) {
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/stopwords.txt", pasta);

    FILE *arquivo_stop_words = fopen(caminho_arquivo, "r");
    TST *tabela = tst_criar();
    char *buffer = NULL;
    size_t tamanho_linha = 0;

    while (getline(&buffer, &tamanho_linha, arquivo_stop_words) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        String *string = string_criar(strdup(buffer));
        tst_inserir(tabela, string, strdup(buffer));
    }

    fclose(arquivo_stop_words);

    if (buffer != NULL) {
        free(buffer);
    }
    
    a->stop_words = tabela;
}

void cria_palavras(Auxiliar *a, char const *pasta) {
    int qtd_arquivos = a->qtd_paginas;
    char **nome_arquivos = obter_linhas(a);
    TST *tabela = tst_criar();

    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s/pages", pasta);

    for (int i = 0; i < qtd_arquivos; i++) {
        char caminho_arquivo[512];
        char *nome_arquivo = nome_arquivos[i];
        String* string_arquivo = string_criar(strdup(nome_arquivo));
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/%s", caminho, nome_arquivo);
        FILE *arquivo = fopen(caminho_arquivo, "r");

        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo: %s\n", caminho_arquivo);
            continue;
        }

        char *buffer = NULL;
        size_t tamanho_buffer = 0;

        while (getline(&buffer, &tamanho_buffer, arquivo) != -1) {
            char *buffer_palavra = strtok(buffer, " \t\n"); // Tokeniza a linha por espaços, tabulacoes ou quebras de linha
            String *string = string_criar(strdup(buffer_palavra));
            
            while (buffer_palavra != NULL) {
                if (tst_buscar(a->stop_words, string) == NULL) {  // Se a palavra nao eh stop word, ela eh adicionada na tabela de palavras 
                    String* string_palavra = string_criar(strdup(buffer_palavra));
                    Palavra* p = (Palavra*)tst_buscar(a->tabela_palavras, string_palavra);
                    Pagina* pag = tst_buscar(a->tabela_arquivos, string_arquivo);
                    //Palavra *p = (Palavra *)arvore_rn_buscar(tabela, buffer_palavra);
                    //Pagina *pag = arvore_rn_buscar(a->tabela_arquivos, nome_arquivo);

                    if (p == NULL) {    // Se a palavra ainda nao esta na tabela, adiciona com o arquivo em que se encontra
                        p = palavra_construtor(buffer_palavra);
                        palavra_adiciona_arquivo(p, pag);
                        tst_inserir(a->tabela_palavras, string_palavra, p);
                        //arvore_rn_inserir(tabela, buffer_palavra, p, palavra_destrutor);
                    }
                    else {      // Se a palavra ja esta na tabela, so precisa adicionar mais um arquivo em que ela se encontra
                        if (palavra_busca_arquivo(p, pag) == -1) {
                            palavra_adiciona_arquivo(p, pag);
                        }
                    }
                    //string_destruir(string_palavra);
                }

                buffer_palavra = strtok(NULL, " \t\n");
            }
        }

        if (buffer != NULL) free(buffer);
        if (arquivo != NULL) fclose(arquivo);
        //string_destruir(string_arquivo);
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
        String* string_pagina = string_criar(strdup(nome_pagina));
        Pagina *pag = tst_buscar(a->tabela_arquivos, string_pagina);

        int qtd_links_saida = atoi(strtok(NULL, " "));

        for (int i = 0; i < qtd_links_saida; i++) {
            char *nome_link_saida = strtok(NULL, " ");
            String* string_pag_saida = string_criar(strdup(nome_link_saida));
            Pagina* pag_saida = tst_buscar(a->tabela_arquivos, string_pag_saida);
            //Pagina *pag_saida = arvore_rn_buscar(a->tabela_arquivos, nome_link_saida);

            if (pag_saida == NULL) {
                continue;
            }

            pagina_adiciona_link_saida(pag, pag_saida);
            pagina_adiciona_link_entrada(pag_saida, pag);
            //string_destruir(string_pag_saida);
        }
        //string_destruir(string_pagina);
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
        String* string_pagina = string_criar(strdup(nomes_paginas[i]));
        todas_paginas[i] = tst_buscar(a->tabela_arquivos, string_pagina);
        //string_destruir(string_pagina);
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

int compara_strings(const void *a, const void *b) {
    const char * const *sa = a;  // ponteiro para elemento (char*)
    const char * const *sb = b;
    return strcasecmp(*sa, *sb); // ou strcmp se quiser case-sensitive
}

void realiza_busca_palavras(Auxiliar* a, char* search_palavras) {
    printf("search:%s\n", search_palavras);

    char *palavra = strtok(search_palavras, " ");
    Vector *palavras = vector_construct(NULL); // para armazenar as palavras encontradas

    while (palavra != NULL) {
        String* string_palavra = string_criar(strdup(palavra));
        if (tst_buscar(a->stop_words, string_palavra) != NULL) {
            palavra = strtok(NULL, " ");
            continue; // Ignora stop words
        }

        Palavra *p = (Palavra *)tst_buscar(a->tabela_palavras, string_palavra);

        if (p == NULL) {
            // Palavra não encontrada - interseção será vazia
            vector_destroy_func(palavras);
            string_destruir(string_palavra);
            printf("pages:\n");
            printf("pr:\n");
            return;
        }
        
        vector_push_back(palavras, p);
        //string_destruir(string_palavra);
        palavra = strtok(NULL, " ");
    }
    
    Vector* arquivos =  palavra_retorna_arquivos_intersecao(palavras);
    vector_sort(arquivos, compara_paginas_por_pagerank);
    int qtd_arquivos = vector_size(arquivos);
    Pagina** paginas = calloc(qtd_arquivos, sizeof(Pagina*));
    printf("pages:");
    double pagerank[qtd_arquivos];

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
        printf("%lf", pagerank);

        if (i < qtd_arquivos - 1) {
            printf(" ");
        }
    }

    printf("\n");
    vector_destroy_func(palavras);
    vector_destroy_func(arquivos);
    free(paginas);
}