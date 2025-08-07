#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALFA 0.85

typedef struct Pagina Pagina;

// @ Cria uma nova página
Pagina *pagina_construtor(char *nome, double num_paginas);

// @ Retorna o nome da página
char *pagina_obter_nome(Pagina *p);

int pagina_compara(const void *a, const void *b);

// @ Destrói uma página
void pagina_destrutor(void *dado);

#endif