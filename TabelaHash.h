#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Node.h"

typedef struct TabelaHash TabelaHash;

typedef unsigned long (*func_hash)(char *chave, int tamanho); // Ponteiro para função de hash
typedef void (*func_destruir)(void *elemento);                // Ponteiro para função de destruição de elemento

// @ Cria uma nova tabela hash
TabelaHash *criar_tabela_hash(int tamanho, func_hash hash, func_destruir destruir_elemento);

// @ Insere um elemento na tabela hash
void inserir_elemento(TabelaHash *tabela, Node *novo_no);

// @ Busca um elemento na tabela hash
void *buscar_elemento(TabelaHash *tabela, char *chave);

// @ Destrói a tabela hash e libera a memória alocada
void destruir_tabela_hash(TabelaHash *tabela);

#endif