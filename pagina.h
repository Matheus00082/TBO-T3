#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define alfa 0.85

typedef struct pagina pagina;

pagina *Criar_pagina(char *nome, double numpaginas);
char *Obter_Nome_Pagina(pagina *p);
void Destruir_pagina(void *dado);



#endif