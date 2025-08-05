#ifndef _PALAVRA_H_
#define _PALAVRA_H_

#include "Vector.h"
#include "Pagina.h"

typedef struct Palavra Palavra;

Palavra* palavra_construtor(char* palavra);
void adiciona_arquivo_palavra(Palavra* p, Pagina* pag);
int busca_arquivo_palavra(Palavra* p, Pagina* pag);
void palavra_destrutor(void* p);

#endif