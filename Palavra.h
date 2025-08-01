#ifndef _PALAVRA_H_
#define _PALAVRA_H_

#include "Vector.h"

typedef struct Palavra Palavra;

Palavra* palavra_construtor(Vector* arquivos, char* palvra);
void palavra_destrutor(Palavra* p);

#endif



