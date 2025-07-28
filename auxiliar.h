#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagina.h"
#include "tabelaHash.h"

typedef struct auxiliar auxiliar;

// @ Lê um arquivo e retorna um auxiliar com os nomes das páginas e a quantidade
auxiliar *Ler_Arquivo(const char *nomeArquivo);

// @ Obtém as linhas do auxiliar
char **Obter_Linhas(auxiliar *a);

// @ Obtém a quantidade de linhas do auxiliar
int Obter_QtdLinhas(auxiliar *a);

// @ Destrói o auxiliar e libera a memória alocada
void Destruir_auxiliar(auxiliar *a);

// @ Cria uma estrutura pagina para cada linha do arquivo
pagina **Retornar_Paginas(auxiliar *a);

// @ Cria uma tabela hash a partir de um arquivo
tabelaHash *Cria_Tabela_Arquivos(auxiliar *a, funcaoHash hash);

// @ Cria uma tabela hash a partir das palavras presentes nos arquivos
tabelaHash *Cria_Tabela_Palavras(auxiliar *a, funcaoHash hash);

#endif