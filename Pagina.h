#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALFA 0.85
#define CAPACIDADE_ENTRADA_INICIAL 10

typedef struct Pagina Pagina;

// @ Cria uma nova página
Pagina *pagina_construtor(char *nome, double num_paginas);

// @ Retorna o nome da página
char *pagina_obter_nome(Pagina *p);

int pagina_compara(const void *a, const void *b);

void aloca_lista_links_saida(Pagina* pagina, int qtd);

void pagina_adiciona_link_entrada(Pagina* pagina1, Pagina* pagina2);

void pagina_adiciona_link_saida(Pagina* pagina1, Pagina* pagina2);

void pagina_calcula_pgrank_total(Pagina* pagina, int total_pags);

// @ Destrói uma página
void pagina_destrutor(void *dado);

#endif