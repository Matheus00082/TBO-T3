#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noHash.h"

typedef struct tabelaHash tabelaHash;

typedef unsigned long (*funcaoHash)(char *chave, int tamanho); // Ponteiro para função de hash
typedef void (*funcaoDestruir)(void *elemento); // Ponteiro para função de destruição de elemento

// @ Cria uma nova tabela hash
tabelaHash *Criar_Tabela_Hash(int tamanho, funcaoHash hash, funcaoDestruir destruirElemento);

// @ Insere um elemento na tabela hash
void Inserir_Elemento(tabelaHash *tabela, noHash *novoNo);

// @ Busca um elemento na tabela hash
void *Buscar_Elemento(tabelaHash *tabela, char *chave);

// @ Destrói a tabela hash e libera a memória alocada
void Destruir_Tabela_Hash(tabelaHash *tabela);

#endif