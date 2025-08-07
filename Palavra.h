#ifndef _PALAVRA_H_
#define _PALAVRA_H_

#include "Vector.h"
#include "Pagina.h"

typedef struct Palavra Palavra;

// construtor e destrutor
Palavra* palavra_construtor(char* palavra);
void palavra_destrutor(void* p);

// auxiliares
void palavra_adiciona_arquivo(Palavra* p, Pagina* pag);
int palavra_busca_arquivo(Palavra* p, Pagina* pag);
Vector* palavra_retorna_arquivos(Palavra* p);


#endif