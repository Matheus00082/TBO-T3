#ifndef NO_HASH_H
#define NO_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct noHash
{
    char *chave;             // chave do nó
    void *dado;              // dado associado ao nó
    struct noHash *esquerda; // ponteiro para o nó à esquerda
    struct noHash *direita;  // ponteiro para o nó à direita
    int numNosNaArvore;     // número de nós na subárvore
} noHash;

// @ Cria um novo nó hash
noHash *Criar_No_Hash(char *nome, void *dado);

// @ Conecta dois nós hash que colidem na mesma posição da tabela hash
int Conecta_Nos(noHash *no1, noHash *no2);

// @ Destrói um nó hash
void Destruir_Nos_Hash(noHash *no, void (*destruirElemento)(void *elemento));

#endif