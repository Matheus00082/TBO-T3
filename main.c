#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelaHash.h"
#include "auxiliar.h"
#include "pagina.h"

unsigned long hash(char *str, int tamanho)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % tamanho;
}

int main(int argc, char const *argv[])
{
    tabelaHash *tabelaDeArquivos = Cria_Tabela_Arquivos("exemplo/index.txt", hash); // Ainda não sei se o nome do arquivo é fixo ou variável
    Destruir_Tabela_Hash(tabelaDeArquivos);
    return 0;
}