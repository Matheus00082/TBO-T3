#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALFA 0.85

typedef struct Pagina Pagina;

// @ Cria uma nova página
Pagina *criar_pagina(char *nome, double num_paginas);

// @ Retorna o nome da página
char *obter_nome_pagina(Pagina *p);

// @ Destrói uma página
void destruir_pagina(void *dado);

#endif