#include "Auxiliar.h"
#include "Vector.h"
#include <string.h> // Para strdup

typedef struct {
    char *chave_original; // Guarda a cópia da chave que será usada na TST
    void *valor_original; // Guarda a Pagina*, Palavra*, etc.
} nodeAux;

// Destrutor para nodeAuxes que guardam Paginas
void nodeAux_pagina_destrutor(void *ag_void) {
    if (ag_void == NULL) return;
    nodeAux *ag = (nodeAux *)ag_void;
    free(ag->chave_original);
    pagina_destrutor(ag->valor_original);
    free(ag);
}

// Destrutor para nodeAuxes que guardam stop words (char*)
void nodeAux_stopwords_destrutor(void *ag_void) {
    if (ag_void == NULL) return;
    nodeAux *ag = (nodeAux *)ag_void;
    free(ag->chave_original);
    //free(ag->valor_original); // O valor aqui também é um strdup
    free(ag);
}

// Destrutor para nodeAuxes que guardam Palavras
void nodeAux_palavra_destrutor(void *ag_void) {
    if (ag_void == NULL) return;
    nodeAux *ag = (nodeAux *)ag_void;
    palavra_destrutor(ag->valor_original);
    free(ag->chave_original);
    free(ag);
}

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

char **obter_linhas(Auxiliar *a) { return a->linhas; }
int obter_qtd_paginas(Auxiliar *a) { return a->qtd_paginas; }

void destruir_auxiliar(Auxiliar *a) {
    if (a != NULL) {
        free(a->linhas);

        // CORRIGIDO: Usa os destrutores de nodeAux corretos.
        if (a->stop_words != NULL) {
            tst_destruir(a->stop_words, nodeAux_stopwords_destrutor);
        }
        if (a->tabela_palavras != NULL) {
            tst_destruir(a->tabela_palavras, nodeAux_palavra_destrutor);
        }
        if (a->tabela_arquivos != NULL) {
            tst_destruir(a->tabela_arquivos, nodeAux_pagina_destrutor);
        }
        
        free(a);
    }
}

void cria_arquivos(Auxiliar *a) {
    int qtd_linhas = a->qtd_paginas;
    char **linhas = obter_linhas(a);
    TST *tabela = tst_criar();

    for (int i = 0; i < qtd_linhas; i++) {
        char *nome_pagina = linhas[i];
        
        // CORRIGIDO: Cria o valor e o nodeAux para inserção segura.
        Pagina *pagina = pagina_construtor(nome_pagina, qtd_linhas);
        
        nodeAux *ag = malloc(sizeof(nodeAux));
        ag->chave_original = strdup(nome_pagina); // A TST agora tem sua própria cópia da chave.
        ag->valor_original = pagina;

        tst_inserir(tabela, ag->chave_original, ag);
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
        
        if (tst_buscar(tabela, buffer) != NULL) {
            // A palavra já foi inserida. Pule para a próxima.
            continue;
        }
        
        nodeAux *ag = calloc(1, sizeof(nodeAux));
        ag->chave_original = strdup(buffer);
        ag->valor_original = ag->chave_original;

        tst_inserir(tabela, ag->chave_original, ag);
    }
    fclose(arquivo_stop_words);
    if (buffer != NULL) free(buffer);
    a->stop_words = tabela;
}

void cria_palavras(Auxiliar *a, char const *pasta) {
    int qtd_arquivos = a->qtd_paginas;
    char **nome_arquivos = obter_linhas(a);
    TST *tabela = tst_criar(); // CORRIGIDO: Usar a tabela local para construção.

    char caminho[256];
    snprintf(caminho, sizeof(caminho), "%s/pages", pasta);

    for (int i = 0; i < qtd_arquivos; i++) {
        char caminho_arquivo[512];
        char *nome_arquivo = nome_arquivos[i];
        snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/%s", caminho, nome_arquivo);
        FILE *arquivo = fopen(caminho_arquivo, "r");
        if (arquivo == NULL) continue;

        // CORRIGIDO: Pega a página usando o nodeAux.
        nodeAux* ag_pagina = (nodeAux*)tst_buscar(a->tabela_arquivos, nome_arquivo);
        if (ag_pagina == NULL) { fclose(arquivo); continue; }
        Pagina* pag = (Pagina*)ag_pagina->valor_original;

        char *buffer = NULL;
        size_t tamanho_buffer = 0;
        while (getline(&buffer, &tamanho_buffer, arquivo) != -1) {
            char *buffer_palavra = strtok(buffer, " \t\n");
            while (buffer_palavra != NULL) {
                if (tst_buscar(a->stop_words, buffer_palavra) == NULL) {
                    // CORRIGIDO: Busca na tabela local e extrai valor do nodeAux.
                    nodeAux* ag_palavra = (nodeAux*)tst_buscar(tabela, buffer_palavra);
                    
                    if (ag_palavra == NULL) {
                        // Palavra nova, precisa criar chave permanente, valor e nodeAux.
                        char* chave_permanente = strdup(buffer_palavra);
                        Palavra* p_novo = palavra_construtor(chave_permanente);
                        palavra_adiciona_arquivo(p_novo, pag);
                        
                        nodeAux* ag_novo = malloc(sizeof(nodeAux));
                        ag_novo->chave_original = chave_permanente;
                        ag_novo->valor_original = p_novo;
                        
                        tst_inserir(tabela, chave_permanente, ag_novo);
                    } else {
                        Palavra* p_existente = (Palavra*)ag_palavra->valor_original;
                        if (palavra_busca_arquivo(p_existente, pag) == -1) {
                            palavra_adiciona_arquivo(p_existente, pag);
                        }
                    }
                }
                buffer_palavra = strtok(NULL, " \t\n");
            }
        }
        if (buffer != NULL) free(buffer);
        fclose(arquivo);
    }
    a->tabela_palavras = tabela; // CORRIGIDO: Atribui a tabela construída ao final.
}


void inicializa_arquivos(const char *nomeArquivo, Auxiliar *a) {
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) return;
    char *buffer = NULL;
    size_t tamanho_linha = 0;
    while (getline(&buffer, &tamanho_linha, arquivo) != -1) {
        buffer[strcspn(buffer, "\n")] = 0;
        char *nome_pagina = strtok(buffer, " ");
        
        // CORRIGIDO: Extrai valor do nodeAux.
        nodeAux* ag_pag = (nodeAux*)tst_buscar(a->tabela_arquivos, nome_pagina);
        if (ag_pag == NULL) continue;
        Pagina *pag = (Pagina*)ag_pag->valor_original;

        int qtd_links_saida = atoi(strtok(NULL, " "));
        for (int i = 0; i < qtd_links_saida; i++) {
            char *nome_link_saida = strtok(NULL, " ");
            
            // CORRIGIDO: Extrai valor do nodeAux.
            nodeAux* ag_pag_saida = (nodeAux*)tst_buscar(a->tabela_arquivos, nome_link_saida);
            if (ag_pag_saida == NULL) continue;
            Pagina* pag_saida = (Pagina*)ag_pag_saida->valor_original;

            pagina_adiciona_link_saida(pag, pag_saida);
            pagina_adiciona_link_entrada(pag_saida, pag);
        }
    }
    if (buffer != NULL) free(buffer);
    fclose(arquivo);
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
    Pagina** todas_paginas = calloc(total_paginas, sizeof(Pagina*));
    
    for (int i = 0; i < total_paginas; i++) {
        // CORRIGIDO: Extrai valor do nodeAux.
        nodeAux* ag = (nodeAux*)tst_buscar(a->tabela_arquivos, nomes_paginas[i]);
        if (ag != NULL) {
            todas_paginas[i] = (Pagina*)ag->valor_original;
        }
    }
    
    // O resto da lógica do PageRank permanece a mesma...
    double erro = 1000; // EPSILON + 1
    while (erro >= 0.0001) { // EPSILON
        erro = 0;
        double *novos_pageranks = calloc(total_paginas, sizeof(double));
        for (int i = 0; i < total_paginas; i++) {
            novos_pageranks[i] = calcula_pagerank_pagina(todas_paginas[i], total_paginas);
        }
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
    char *palavra_token = strtok(search_palavras, " ");
    Vector *palavras_encontradas = vector_construct(NULL);

    while (palavra_token != NULL) {
        if (tst_buscar(a->stop_words, palavra_token) != NULL) {
            palavra_token = strtok(NULL, " ");
            continue;
        }

        // CORRIGIDO: Extrai valor do nodeAux.
        nodeAux *ag = (nodeAux *)tst_buscar(a->tabela_palavras, palavra_token);
        if (ag == NULL) {
            vector_destroy_func(palavras_encontradas);
            printf("pages:\npr:\n");
            return;
        }
        
        vector_push_back(palavras_encontradas, ag->valor_original);
        palavra_token = strtok(NULL, " ");
    }
    
    // O resto da lógica da busca permanece a mesma...
    Vector* arquivos =  palavra_retorna_arquivos_intersecao(palavras_encontradas);
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
    vector_destroy_func(palavras_encontradas);
    vector_destroy_func(arquivos);
    free(paginas);
}