#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define alfa 0.85

typedef struct pagina pagina;

// @ Cria uma nova página
pagina *Criar_pagina(char *nome, double numpaginas);

// @ Retorna o nome da página
char *Obter_Nome_Pagina(pagina *p);

// @ Destrói uma página
void Destruir_pagina(void *dado);



#endif