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

/**
 * @brief Lê um arquivo e carrega todas as linhas em uma estrutura auxiliar
 * @param nomeArquivo Nome do arquivo a ser lido
 * @return Ponteiro para a estrutura Auxiliar criada, ou NULL em caso de erro
 * 
 * Esta função abre o arquivo especificado e lê todas as suas linhas,
 * armazenando-as em uma estrutura dinâmica para uso posterior.
 */
Auxiliar *ler_arquivo(const char *nomeArquivo);

/**
 * @brief Obtém o array de linhas do auxiliar
 * @param a Ponteiro para a estrutura Auxiliar
 * @return Array de strings contendo as linhas do arquivo
 * 
 * Retorna um ponteiro para o array interno de strings que contém
 * todas as linhas lidas do arquivo.
 */
char **obter_linhas(Auxiliar *a);

/**
 * @brief Obtém a quantidade total de linhas do auxiliar
 * @param a Ponteiro para a estrutura Auxiliar
 * @return Número inteiro representando a quantidade de linhas
 * 
 * Retorna o número total de linhas que foram lidas e armazenadas
 * na estrutura auxiliar.
 */
int obter_qtd_linhas(Auxiliar *a);

/**
 * @brief Destrói a estrutura auxiliar e libera toda a memória alocada
 * @param a Ponteiro para a estrutura Auxiliar a ser destruída
 * @return void
 * 
 * Libera a memória alocada para todas as linhas e para a própria
 * estrutura auxiliar, evitando vazamentos de memória.
 */
void destruir_auxiliar(Auxiliar *a);

/**
 * @brief Cria e inicializa a tabela hash de arquivos/páginas
 * @param a Ponteiro para a estrutura Auxiliar contendo os nomes dos arquivos
 * @return void
 * 
 * Cria uma tabela hash global onde cada chave é o nome de um arquivo
 * e cada valor é uma estrutura Pagina correspondente.
 */
void cria_arquivos(Auxiliar *a);

/**
 * @brief Cria e inicializa a tabela hash de palavras de parada (stop words)
 * @param a Ponteiro para a estrutura Auxiliar (pode ser usado para configurações)
 * @return void
 * 
 * Lê o arquivo de stop words e cria uma tabela hash para verificação
 * rápida de palavras que devem ser ignoradas durante a indexação.
 */
void cria_stop_words(Auxiliar *a);

/**
 * @brief Cria e inicializa a tabela hash de palavras e seus arquivos correspondentes
 * @param a Ponteiro para a estrutura Auxiliar contendo os nomes dos arquivos
 * @return void
 * 
 * Processa todos os arquivos listados no auxiliar, extraindo palavras
 * e criando um índice invertido onde cada palavra mapeia para uma lista
 * de arquivos que a contêm.
 */
void cria_palavras(Auxiliar *a);

/**
 * @brief Inicializa e calcula o PageRank dos arquivos baseado no grafo de links
 * @param nomeArquivo Nome do arquivo contendo o grafo de links entre páginas
 * @param a Ponteiro para a estrutura Auxiliar contendo informações dos arquivos
 * @return void
 * 
 * Lê o arquivo de grafo que define as conexões entre páginas e calcula
 * o valor PageRank para cada página, armazenando na estrutura correspondente.
 */
void inicializa_arquivos(const char *nomeArquivo, Auxiliar *a);

/**
 * @brief Realiza busca por uma palavra e exibe resultados ordenados por PageRank
 * @param a Ponteiro para a estrutura Auxiliar
 * @param palavra String contendo a palavra a ser buscada
 * @return void
 * 
 * Busca a palavra especificada na tabela de índice invertido, encontra
 * todos os arquivos que a contêm e exibe os resultados ordenados pelo
 * valor PageRank das páginas.
 */
void realiza_busca_palavras(Auxiliar* a, char* search_palavras);

#endif