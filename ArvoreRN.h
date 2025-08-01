#ifndef _ARVORE_RUBRO_NEGRA
#define _ARVORE_RUBRO_NEGRA

#include <stdbool.h>

typedef struct arvoreRN arvoreRN;

arvoreRN* arvoreRN_construtor();
void arvoreRN_destrutor(arvoreRN* rn);

#endif