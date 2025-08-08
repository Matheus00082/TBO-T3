#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Pagina.h"
#include "Palavra.h"
#include "TabelaHash.h"
#include "ArvoreRN.h"

#define CAPACIDADE_INICIAL 50

typedef struct Auxiliar Auxiliar;
typedef struct NomeArquivo NomeArquivo;

// @ Lê um arquivo e retorna um auxiliar com os nomes das páginas e a quantidade
Auxiliar *ler_arquivo(const char *nomeArquivo);

// @ Obtém as linhas do auxiliar
char **obter_linhas(Auxiliar *a);

// @ Obtém a quantidade de linhas do auxiliar
int obter_qtd_linhas(Auxiliar *a);

// @ Destrói o auxiliar e libera a memória alocada
void destruir_auxiliar(Auxiliar *a);

// @ Cria uma tabela hash a partir de um arquivo
void cria_arquivos(Auxiliar *a, func_hash hash);

// @ Cria uma tabela hash para stop words
void cria_stop_words(Auxiliar *a, func_hash hash);

// @ Cria uma tabela hash a partir das palavras presentes nos arquivos
void cria_palavras(Auxiliar *a, func_hash hash);

void inicializa_arquivos(const char *nomeArquivo, Auxiliar *a);

// @ Busca nas tabelas a palavra e imprime na tela os arquivos que ela aparece e o pageRank
void realiza_busca_palavras(Auxiliar* a, const char* palavra);

#endif