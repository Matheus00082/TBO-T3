#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pagina.h"
#include "TabelaHash.h"

typedef struct Auxiliar Auxiliar;

// @ Lê um arquivo e retorna um auxiliar com os nomes das páginas e a quantidade
Auxiliar *ler_arquivo(const char *nomeArquivo);

// @ Obtém as linhas do auxiliar
char **obter_linhas(Auxiliar *a);

// @ Obtém a quantidade de linhas do auxiliar
int obter_qtd_linhas(Auxiliar *a);

// @ Destrói o auxiliar e libera a memória alocada
void destruir_auxiliar(Auxiliar *a);

// @ Cria uma estrutura Pagina para cada linha do arquivo
Pagina **retornar_paginas(Auxiliar *a);

// @ Cria uma tabela hash a partir de um arquivo
TabelaHash *cria_tabela_arquivos(Auxiliar *a, func_hash hash);

// @ Cria uma tabela hash a partir das palavras presentes nos arquivos
TabelaHash *cria_tabela_palavras(Auxiliar *a, func_hash hash);

#endif