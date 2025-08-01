#include "tabelaHash.h"

struct tabelaHash
{
    Node **tabela;                 // Array de ponteiros para nós da tabela hash
    int tamanho;                     // Tamanho da tabela hash
    int qtdElementos;                // Quantidade de elementos na tabela hash
    funcaoHash hash;                 // Função de hash
    funcaoDestruir destruirElemento; // Função de destruição de elemento
};

tabelaHash *Criar_Tabela_Hash(int tamanho, funcaoHash hash, funcaoDestruir destruirElemento)
{
    tabelaHash *novaTabela = (tabelaHash *)malloc(sizeof(tabelaHash));
    if (novaTabela == NULL)
    {
        printf("Erro ao alocar memória para a tabela hash.\n");
        return NULL;
    }

    novaTabela->tamanho = tamanho;
    novaTabela->qtdElementos = 0;
    novaTabela->hash = hash;
    novaTabela->destruirElemento = destruirElemento;
    novaTabela->tabela = calloc(tamanho, sizeof(Node *));

    if (novaTabela->tabela == NULL)
    {
        printf("Erro ao alocar memória para a tabela.\n");
        free(novaTabela);
        return NULL;
    }

    return novaTabela;
}

void Inserir_Elemento(tabelaHash *tabela, Node *novoNo)
{
    if (tabela == NULL || novoNo == NULL)
    {
        printf("Tabela ou elemento inválido.\n");
        return;
    }

    int indice = tabela->hash(novoNo->chave, tabela->tamanho);
    if (indice == -1)
    {
        printf("Erro ao calcular índice da tabela hash.\n");
        return;
    }

    if (tabela->tabela[indice] == NULL)
        tabela->tabela[indice] = novoNo;
    else
        Conecta_Node(tabela->tabela[indice], novoNo);

    tabela->qtdElementos++;

    // Redimensionar a tabela hash
    if (tabela->qtdElementos >= tabela->tamanho)
    {
        int novoTamanho = tabela->tamanho * 2;
        Node **novaTabela = calloc(novoTamanho, sizeof(Node *));
        if (novaTabela == NULL)
        {
            printf("Erro ao alocar nova tabela.\n");
            return;
        }

        // Salva dados antigos
        Node **tabelaAntiga = tabela->tabela;
        int tamanhoAntigo = tabela->tamanho;

        // Atualiza a tabela
        tabela->tabela = novaTabela;
        tabela->tamanho = novoTamanho;
        tabela->qtdElementos = 0; // será recontado ao reinserir

        // Reinsere todos os elementos da tabela antiga
        for (int i = 0; i < tamanhoAntigo; i++)
        {
            if (tabelaAntiga[i] != NULL)
            {
                Inserir_Elemento(tabela, tabelaAntiga[i]);
            }
        }

        free(tabelaAntiga);
    }
}

void *Buscar_Elemento(tabelaHash *tabela, char *chave)
{
    if (tabela == NULL || chave == NULL)
    {
        printf("Tabela ou chave inválida.\n");
        return NULL;
    }

    int indice = tabela->hash(chave, tabela->tamanho);
    Node *noAtual = tabela->tabela[indice];
    if (noAtual == NULL)
    {
        printf("Nenhum elemento encontrado para a chave: %s\n", chave);
        return NULL;
    }

    while (noAtual != NULL)
    {
        int comparacao = strcmp(noAtual->chave, chave);
        if (comparacao == 0)
        {
            return noAtual->dado;
        }
        else if (comparacao < 0)
        {
            noAtual = noAtual->esquerda;
        }
        else
        {
            noAtual = noAtual->direita;
        }
    }

    return NULL;
}

void Destruir_Tabela_Hash(tabelaHash *tabela)
{
    if (tabela != NULL)
    {
        for (int i = 0; i < tabela->tamanho; i++)
        {
            if (tabela->tabela[i] != NULL)
            {
                Destruir_Nodes(tabela->tabela[i], tabela->destruirElemento);
                tabela->tabela[i] = NULL;
            }
        }

        free(tabela->tabela);
        free(tabela);
    }
}