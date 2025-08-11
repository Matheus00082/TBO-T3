#ifndef PAGINA_H
#define PAGINA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALFA 0.85
#define EPSILON 1e-6
#define CAPACIDADE_ENTRADA_INICIAL 10

typedef struct Pagina Pagina;

// @ Cria uma nova página
Pagina *pagina_construtor(char *nome, double num_paginas);

// @ Retorna o nome da página
char *pagina_obter_nome(Pagina *p);

// @ Compara duas páginas pelo nome (case insensitive)
int pagina_compara(const void *a, const void *b);

// @ Retorna o PageRank da página
double pagina_obter_pagerank(Pagina *p);

// @ Define o PageRank da página
void pagina_definir_pagerank(Pagina *p, double pagerank);

// @ Retorna a quantidade de links de entrada
int pagina_obter_qtd_links_entrada(Pagina *p);

// @ Retorna uma página específica dos links de entrada
Pagina* pagina_obter_link_entrada(Pagina *p, int index);

// @ Retorna a quantidade de links de saída
int pagina_obter_qtd_links_saida(Pagina *p);

void aloca_lista_links_saida(Pagina* pagina, int qtd);

void pagina_adiciona_link_entrada(Pagina* pagina1, Pagina* pagina2);

void pagina_adiciona_link_saida(Pagina* pagina1, Pagina* pagina2);

// @ Destrói uma página
void pagina_destrutor(void *dado);

#endif