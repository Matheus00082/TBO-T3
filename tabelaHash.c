#include "tabelaHash.h"

struct tabelaHash
{
    noHash **tabela;
    int tamanho;
    int qtdElementos;
    funcaoHash hash;
    funcaoDestruir destruirElemento;
};

tabelaHash *Criar_Tabela_Hash(int tamanho, funcaoHash hash, funcaoDestruir destruirElemento)
{
    tabelaHash *novaTabela = (tabelaHash *)malloc(sizeof(tabelaHash));
    if (novaTabela == NULL)
    {
        printf("Erro ao alocar memória para a tabela hash.\n");
        return NULL;
    }

    novaTabela->tamanho = tamanho;
    novaTabela->qtdElementos = 0;
    novaTabela->hash = hash;
    novaTabela->destruirElemento = destruirElemento;
    novaTabela->tabela = calloc(tamanho, sizeof(noHash *));

    if (novaTabela->tabela == NULL)
    {
        printf("Erro ao alocar memória para a tabela.\n");
        free(novaTabela);
        return NULL;
    }

    return novaTabela;
}

void Inserir_Elemento(tabelaHash *tabela, char *chave, void *elemento)
{
    if (tabela == NULL || elemento == NULL)
    {
        printf("Tabela ou elemento inválido.\n");
        return;
    }

    int indice = tabela->hash(chave, tabela->tamanho);
    noHash *novoNo = Criar_No_Hash(chave, elemento);
    if (novoNo == NULL)
    {
        printf("Erro ao criar novo nó.\n");
        return;
    }

    if (tabela->tabela[indice] == NULL)
        tabela->tabela[indice] = novoNo;
    else
        Conecta_Nos(tabela->tabela[indice], novoNo);

    tabela->qtdElementos++;
}

void *Buscar_Elemento(tabelaHash *tabela, char *chave)
{
    if (tabela == NULL || chave == NULL)
    {
        printf("Tabela ou chave inválida.\n");
        return NULL;
    }

    int indice = tabela->hash(chave, tabela->tamanho);
    noHash *noAtual = tabela->tabela[indice];
    if (noAtual == NULL)
    {
        printf("Nenhum elemento encontrado para a chave: %s\n", chave);
        return NULL;
    }

    while (noAtual != NULL)
    {
        int comparacao = strcmp(noAtual->chave, chave);
        if (comparacao == 0)
        {
            return noAtual->dado;
        }else if (comparacao < 0)
        {
            noAtual = noAtual->esquerda;
        }
        else
        {
            noAtual = noAtual->direita;
        }
    }

    return NULL;
}

void Destruir_Tabela_Hash(tabelaHash *tabela)
{
    if (tabela != NULL)
    {
        for (int i = 0; i < tabela->tamanho; i++)
        {
            if (tabela->tabela[i] != NULL)
            {
                Destruir_Nos_Hash(tabela->tabela[i], tabela->destruirElemento);
                tabela->tabela[i] = NULL;
            }
        }
        
        free(tabela->tabela);
        free(tabela);
    }
}