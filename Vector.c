#include <stdlib.h>
#include <stdio.h>
#include "Vector.h"

struct Vector {
    data_type *data;
    destroy_fn dst_fn;
    int size;
    int allocated;
};

// Aloca espaço para um elemento do tipo vector e inicializa os seus atributos.
Vector *vector_construct(destroy_fn dst_fn) {
    Vector *v = calloc(1, sizeof(Vector));

    if (dst_fn == NULL) {
        dst_fn = free; // Define uma função de destruição padrão se não for fornecida
    }
    else {
        v->dst_fn = dst_fn;
    }

    v->allocated = 10;
    v->size = 0;
    v->data = calloc(v->allocated, sizeof(data_type));
    return v;
}

// Libera o espaço reservado para o vector.
void vector_destroy(Vector *v)
{
    for (int i = 0; i < v->size; i++) {
        if (v->dst_fn != NULL) {
            if (v->data[i] != NULL) {
                v->dst_fn(v->data[i]);
            }
        }
    }

    free(v->data);
    free(v);
}

// Adiciona o item dado ao final do vector e aumenta o tamanho. Aumenta a capacidade alocada se necessário.
void vector_push_back(Vector *v, data_type val) {

    if (v->allocated == v->size) {
        v->allocated *= 2;
        v->data = realloc(v->data, v->allocated * sizeof(data_type));
    }

    v->data[v->size] = val;
    v->size++;
}

// Retorna o número de elementos atualmente no vector.
int vector_size(Vector *v) {
    return v->size;
}

// Retorna o i-ésimo elemento do vector.
data_type vector_get(Vector *v, int i) {
    if (i < v->size && i >= 0) {
        return v->data[i];
    }
    else {
        exit(1);
    }
}

// Atualiza o i-ésimo elemento do vector  para que ele passe a ter o valor val.
data_type vector_set(Vector *v, int i, data_type val) {
    data_type aux = v->data[i];
    v->data[i] = val;
    return aux;
}

// Retorna o índice da primeira ocorrência do item no Vector ou -1 se o elemento não for encontrado.
int vector_find(Vector *v, data_type val, compare c) {
    for (int i = 0; i < vector_size(v); i++) {
        if (c(val, vector_get(v, i))) {
            return i;
        }
    }

    return -1;
}

// Remove o i-ésimo elemento do vetor.
data_type vector_remove(Vector *v, int i) {
    data_type value = v->data[i];
    if (i >= 0 && i < v->size) {
        value = v->data[i];

        for (int j = i + 1; j < v->size; j++) {
            v->data[j - 1] = v->data[j];
        }

        v->size--;
    }

    return value;
}

// Remove o ultimo elemento
data_type vector_pop_back(Vector *v) {
    data_type value = vector_remove(v, v->size - 1);
    return value;
}

// Insere o elemento na i-esima posicao
void vector_insert(Vector *v, int i, data_type val) {

    if (i >= 0 && i < v->allocated) {
        for (int j = v->size; j > i; j--) {
            v->data[j] = v->data[j - 1];
        }

        v->data[i] = val;
        v->size++;
    }

    if (i >= v->allocated) {
        vector_push_back(v, val);
    }
}

// Troca os elementos das posições i e j (i vira j e j vira i)
void vector_swap(Vector *v, int i, int j) {
    data_type troca = v->data[i];
    v->data[i] = v->data[j];
    v->data[j] = troca;
}

// Ordena o vetor in-place (sem criar um novo vetor)
void vector_sort(Vector *v, compare c) {
    if (v == NULL || v->data == NULL || v->size <= 1) {
        return; // Nothing to sort
    }

    qsort(v->data, v->size, sizeof(void *), c);
}

// Retorna o indice de val usando busca binaria. Retorna -1 se nao encontrado.
int vector_binary_search(Vector *v, data_type val, compare c) {
    int low = 0, high = v->size - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        if (c(val, vector_get(v, mid)) == 0) {
            return mid;
        } else if (c(val, vector_get(v, mid)) < 0) {
            low = mid + 1;
        }
        else {
            high = mid - 1;
        }
    }

    return -1;
}