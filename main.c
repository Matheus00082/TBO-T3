#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "TabelaHash.h"
#include "Auxiliar.h"
#include "Pagina.h"

unsigned long hash(TabelaHash *tabela, void *chave) {
    char *str = (char *)chave;
    int tamanho_tabela = tamanho_tabela_hash(tabela);
    unsigned long hash_val = 0, base = 27183;
    unsigned long random_val = 31415;
    int tamanho_chave = strlen(str);

    for (int i = 0; i < tamanho_chave; i++) {
        hash_val = (hash_val * random_val + str[i]) % tamanho_tabela;

        random_val = (random_val * base) % (tamanho_tabela - 1);
    }

    return hash_val;
}

int main(int argc, char const *argv[]) {
    // 3.1 Indexador
    Auxiliar *aux = ler_arquivo("exemplo/index.txt");
    cria_tabela_arquivos(aux, hash);
    cria_tabela_stop_words(aux, hash);
    cria_tabela_palavras(aux, hash);
    char *buffer = NULL;
    size_t tamanho_buffer = 0;
    FILE *arquivo = fopen("exemplo/searches.txt", "r");

    // 3.2 Google's PageRank


    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // 3.3 Processador de Consultas
    while(getline(&buffer, &tamanho_buffer, arquivo) != -1) {
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        printf("Linha lida: %s\n", buffer);
    }

    free(buffer);
    fclose(arquivo);
    destruir_auxiliar(aux);
    return 0;
}