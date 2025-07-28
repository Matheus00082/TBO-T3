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
    auxiliar *aux = Ler_Arquivo("exemplo/index.txt");
    tabelaHash *tabelaDeArquivos = Cria_Tabela_Arquivos(aux, hash); // Ainda não sei se o nome do arquivo é fixo ou variável
    tabelaHash *tabelaDePalavras = Cria_Tabela_Palavras(aux, hash);

    char *buffer = NULL;
    size_t tamanhoBuffer = 0;
    FILE *arquivo = fopen("exemplo/searches.txt", "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    while(getline(&buffer, &tamanhoBuffer, arquivo) != -1)
    {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        printf("Linha lida: %s\n", buffer);
    }

    free(buffer);
    fclose(arquivo);
    Destruir_auxiliar(aux);
    Destruir_Tabela_Hash(tabelaDeArquivos);
    Destruir_Tabela_Hash(tabelaDePalavras);
    return 0;
}