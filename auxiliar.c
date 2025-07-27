#include "auxiliar.h"

struct auxiliar
{
    char **linhas;
    int qtdLinhas;
};

// Lê um arquivo e retorna um auxiliar com os nomes das páginas e a quantidade
auxiliar *Ler_Arquivo(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return NULL;
    }

    auxiliar *aux = calloc(1, sizeof(auxiliar));
    if (aux == NULL)
    {
        printf("Erro ao alocar memória para auxiliar.\n");
        fclose(arquivo);
        return NULL;
    }

    char *buffer = NULL;
    size_t tamanhoLinha = 0;
    size_t capacidade = 10; // Capacidade inicial

    aux->linhas = calloc(capacidade, sizeof(char *));
    if (aux->linhas == NULL)
    {
        printf("Erro ao alocar memória para linhas.\n");
        fclose(arquivo);
        return NULL;
    }

    while (getline(&buffer, &tamanhoLinha, arquivo) != -1)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        
        if (aux->qtdLinhas >= capacidade)
        {
            capacidade *= 2;
            aux->linhas = realloc(aux->linhas, capacidade * sizeof(char *));
            if (aux->linhas == NULL)
            {
                printf("Erro ao realocar memória para linhas.\n");
                fclose(arquivo);
                return NULL;
            }
        }

        aux->linhas[aux->qtdLinhas] = strdup(buffer);
        if (aux->linhas[aux->qtdLinhas] == NULL)
        {
            printf("Erro ao duplicar a linha.\n");
            fclose(arquivo);
            return NULL;
        }
        aux->qtdLinhas++;
    }

    if (buffer != NULL)
    {
        free(buffer);
    }
    
    
    fclose(arquivo);
    return aux;
}

char **Obter_Linhas(auxiliar *a)
{
    return a->linhas;
}

int Obter_QtdLinhas(auxiliar *a)
{
    return a->qtdLinhas;
}

void Destruir_auxiliar(auxiliar *a)
{
    if (a != NULL)
    {
        for (int i = 0; i < a->qtdLinhas; i++)
        {
            if (a->linhas[i] != NULL)
            {
                free(a->linhas[i]);
            }
        }
        free(a->linhas);
        free(a);
    }
}

// Cria uma estrutura pagina para cada linha do arquivo
pagina **Retornar_Paginas(auxiliar *a){

    int qtdLinhas = Obter_QtdLinhas(a);
    pagina **paginas = malloc(qtdLinhas * sizeof(pagina *));
    if (paginas == NULL)
    {
        printf("Erro ao alocar memória para páginas.\n");
        return NULL;
    }

    char **linhas = Obter_Linhas(a);

    for (int i = 0; i < qtdLinhas; i++)
    {
        char *nomePagina = strdup(linhas[i]);
        paginas[i] = Criar_pagina(nomePagina, qtdLinhas);
    }

    return paginas;
}

tabelaHash *Cria_Tabela_Arquivos(const char *nomeArquivo, funcaoHash hash){

    auxiliar *a = Ler_Arquivo(nomeArquivo);
    int qtdLinhas = Obter_QtdLinhas(a);
    tabelaHash *tabela = Criar_Tabela_Hash(qtdLinhas, hash, Destruir_pagina);
    pagina **paginas = Retornar_Paginas(a);

    for (int i = 0; i < qtdLinhas; i++)
    {
        if (paginas[i] != NULL)
        {
            Inserir_Elemento(tabela, Obter_Nome_Pagina(paginas[i]), paginas[i]);
        }
    }

    Destruir_auxiliar(a);
    free(paginas);

    return tabela;
}