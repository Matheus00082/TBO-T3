#ifndef TST_H
#define TST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"

typedef struct NodeTST TST;

typedef void (*func_destruir)(void *elemento);

TST *tst_criar();
void tst_destruir(TST *tst, func_destruir destruir_elemento);

TST *tst_inserir(TST *tst, char *chave, void *dado);
void *tst_buscar(TST *tst, char *chave);

void *get_valor(TST *tst);

#endif