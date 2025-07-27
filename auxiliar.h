#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pagina.h"
#include "tabelaHash.h"

typedef struct auxiliar auxiliar;
    
auxiliar *Ler_Arquivo(const char *nomeArquivo);
char **Obter_Linhas(auxiliar *a);
int Obter_QtdLinhas(auxiliar *a);
void Destruir_auxiliar(auxiliar *a);

pagina **Retornar_Paginas(auxiliar *a);
tabelaHash *Cria_Tabela_Arquivos(const char *nomeArquivo, funcaoHash hash);

#endif