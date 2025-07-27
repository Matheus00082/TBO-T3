#ifndef TABELA_HASH_H
#define TABELA_HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "noHash.h"

typedef struct tabelaHash tabelaHash;

typedef unsigned long (*funcaoHash)(char *chave, int tamanho);
typedef void (*funcaoDestruir)(void *elemento);

tabelaHash *Criar_Tabela_Hash(int tamanho, funcaoHash hash, funcaoDestruir destruirElemento);
void Inserir_Elemento(tabelaHash *tabela, char *chave, void *elemento);
void *Buscar_Elemento(tabelaHash *tabela, char *chave);
void Destruir_Tabela_Hash(tabelaHash *tabela);

#endif