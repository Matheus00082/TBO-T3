#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "TabelaHash.h"
#include "Auxiliar.h"
#include "Pagina.h"

int main(int argc, char const *argv[]) {
    // 3.1 Indexador
    clock_t inicio, fim;
    inicio = clock();
    char const *nome_arquivo = argv[1];
    char caminho_arquivo[256];
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/index.txt", nome_arquivo);
    
    Auxiliar *aux = ler_arquivo(caminho_arquivo);
    cria_arquivos(aux);
    cria_stop_words(aux, nome_arquivo);
    cria_palavras(aux, nome_arquivo);

    // 3.2 Google's PageRank
    caminho_arquivo[0] = '\0';  // limpa o buffer
    snprintf(caminho_arquivo, sizeof(caminho_arquivo), "%s/graph.txt", nome_arquivo);
    inicializa_arquivos(caminho_arquivo, aux);
    calcula_pagerank_global(aux);   // Calcula PageRank global de todas as páginas
    
    // 3.3 Processador de Consultas
    char *buffer = NULL;
    size_t tamanho_buffer = 0;

    while (getline(&buffer, &tamanho_buffer, stdin) != -1) {
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

        if (buffer[0] == '\0') {
            break; // Se a linha estiver vazia, sai do loop
        }
        
        realiza_busca_palavras(aux, buffer);
    }

    free(buffer);
    destruir_auxiliar(aux);
    fim = clock();
    double tempo_total = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("TEMPO TOTAL:%lf\n",tempo_total );
    return 0;
}