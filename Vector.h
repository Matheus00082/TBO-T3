
#ifndef VECTOR_H
#define VECTOR_H

typedef void *data_type;
typedef int (*compare)(const void *, const void *);
typedef void (*destroy_fn)(void *);
typedef struct Vector Vector;

// Cria um vetor vazio
Vector *vector_construct(destroy_fn dst_fn);

// Adiciona um elemento no final do vetor
void vector_push_back(Vector *v, data_type val);

// Retorna o i-ésimo elemento do vetor
data_type vector_get(Vector *v, int i);

// Atribui o valor val ao i-ésimo elemento do vetor
data_type vector_set(Vector *v, int i, data_type val);

// Retorna o tamanho do vetor
int vector_size(Vector *v);

// Retorna o índice do primeiro elemento com valor val. Retorna -1 se nao encontrado.
int vector_find(Vector *v, data_type val, compare c);

// Remove o i-ésimo elemento do vetor.
data_type vector_remove(Vector *v, int i);

// Remove o ultimo elemento
data_type vector_pop_back(Vector *v);

// Insere o elemento na i-esima posicao
void vector_insert(Vector *v, int i, data_type val);

// Troca os elementos das posições i e j (i vira j e j vira i)
void vector_swap(Vector *v, int i, int j);

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v, compare c);

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val, compare c);

// Copia os elementos do vetor v2 para o vetor v1
void vector_copy(Vector *v1, Vector *v2);

Vector *vector_clone(Vector *v);

// Libera a memoria alocada para o vetor
void vector_destroy_func(void *v);

#endif