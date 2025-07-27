#ifndef NO_HASH_H
#define NO_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noHash
{
    char *chave;
    void *dado;
    struct noHash *esquerda;
    struct noHash *direita;
    int numArquivos;
} noHash;

noHash *Criar_No_Hash(char *nome, void *dado);
void Conecta_Nos(noHash *no1, noHash *no2);
void Destruir_Nos_Hash(noHash *no, void (*destruirElemento)(void *elemento));

#endif