#ifndef ARVORE_RUBRO_NEGRA_H
#define ARVORE_RUBRO_NEGRA_H

#include <stdbool.h>

typedef struct ArvoreRN ArvoreRN;

ArvoreRN *arvoreRN_construtor();

void arvoreRN_destrutor(ArvoreRN *rn);

#endif