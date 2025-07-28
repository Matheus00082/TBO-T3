#include "noHash.h"

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
    novoNo->numNosNaArvore = 1;
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;

    return novoNo;
}

int Conecta_Nos(noHash *no1, noHash *no2)
{
    if (no1 == NULL || no2 == NULL)
    {
        printf("Nó inválido para conexão.\n");
        return -1;
    }

    no1->numNosNaArvore++;

    // se a chave do no1 for maior que a do no2, conecta no lado direito
    int cmp = strcmp(no1->chave, no2->chave);
    if (cmp > 0)
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
    else if (cmp < 0)
    {
        if (no1->esquerda == NULL)
        {
            no1->esquerda = no2;
        }
        else
        {
            Conecta_Nos(no1->esquerda, no2);
        }
    }else{
        return 0; // Chaves iguais, não conecta
    }

    return 1;
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