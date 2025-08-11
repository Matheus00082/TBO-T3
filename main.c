#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TabelaHash.h"
#include "Auxiliar.h"
#include "Pagina.h"

int main(int argc, char const *argv[]) {
    // 3.1 Indexador
    Auxiliar *aux = ler_arquivo("exemplo/index.txt");
    cria_arquivos(aux);
    cria_stop_words(aux);
    cria_palavras(aux);
    char *buffer = NULL;
    size_t tamanho_buffer = 0;

    // 3.2 Google's PageRank
    inicializa_arquivos("exemplo/graph.txt", aux);
    calcula_pagerank_global(aux); // Calcula PageRank global de todas as páginas
    
    FILE *arquivo = fopen("exemplo/searches.txt", "r");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    
    // 3.3 Processador de Consultas
    while(getline(&buffer, &tamanho_buffer, arquivo) != -1) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        realiza_busca_palavras(aux, buffer);
    }

    free(buffer);
    fclose(arquivo);
    destruir_auxiliar(aux);
    return 0;
}