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
    char buffer[256];

    while(scanf("%[^\n]", buffer) == 1) {
        realiza_busca_palavras(aux, buffer);
        getchar();
    }
    
    destruir_auxiliar(aux);
    fim = clock();
    double tempo_total = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("TEMPO TOTAL:%lf\n",tempo_total );
    return 0;
}