#include "noHash.h"

// struct noHash
// {
//     char *chave;
//     void *dado;
//     noHash *esquerda;
//     noHash *direita;
//     int numArquivos;
// };

noHash *Criar_No_Hash(char *chave, void *dado)
{
    noHash *novoNo = (noHash *)malloc(sizeof(noHash));
    if (novoNo == NULL)
    {
        printf("Erro ao alocar memória para o nó.\n");
        return NULL;
    }

    novoNo->chave = strdup(chave);
    if (novoNo->chave == NULL)
    {
        printf("Erro ao alocar memória para a chave.\n");
        free(novoNo);
        return NULL;
    }

    novoNo->dado = dado;
    novoNo->numArquivos = 0;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;

    return novoNo;
}

void Conecta_Nos(noHash *no1, noHash *no2)
{
    if (no1 == NULL || no2 == NULL)
    {
        printf("Nó inválido para conexão.\n");
        return;
    }

    // se a chave do no1 for maior que a do no2, conecta no lado direito
    if (strcmp(no1->chave, no2->chave) > 0)
    {
        if (no1->direita == NULL)
        {
            no1->direita = no2;
        }
        else
        {
            Conecta_Nos(no1->direita, no2);
        }
    }
    else
    {
        if (no1->esquerda == NULL)
        {
            no1->esquerda = no2;
        }
        else
        {
            Conecta_Nos(no1->esquerda, no2);
        }
    }
}

void Destruir_No_Hash(noHash *no, void (*destruirElemento)(void *elemento))
{
    if (no != NULL)
    {
        free(no->chave);
        if (destruirElemento != NULL)
        {
            destruirElemento(no->dado);
        }
        free(no);
    }
}

void Destruir_Nos_Hash(noHash *no, void (*destruirElemento)(void *))
{
    if (no != NULL)
    {
        Destruir_Nos_Hash(no->esquerda, destruirElemento);
        Destruir_Nos_Hash(no->direita, destruirElemento);
        Destruir_No_Hash(no, destruirElemento);
    }
}