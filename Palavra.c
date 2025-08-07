#include "Palavra.h"

struct Palavra {
    Vector* arquivos;
    char* palavra;
};

int cmp_arquivos_vector(const void *a, const void *b) {
    const char *str_a = (const char *)a;
    const char *str_b = (const char *)b;
    return strcasecmp(str_a, str_b);
}

Palavra* palavra_construtor(char* palavra) {
    Palavra *p = (Palavra*)calloc(1, sizeof(Palavra));
    p->palavra = palavra;
    p->arquivos = vector_construct(NULL);
    return p;
}

void palavra_adiciona_arquivo(Palavra* p, Pagina* pag) {
    vector_push_back(p->arquivos, pag);
}

int palavra_busca_arquivo(Palavra* p, Pagina* pag) {
    return vector_find(p->arquivos, pag, cmp_arquivos_vector);
}
Vector* palavra_retorna_arquivos(Palavra* p){
    return p->arquivos;
}
void palavra_destrutor(void* p) {
    Palavra *palavra = (Palavra*)p;
    vector_destroy_func(palavra->arquivos);
    //free(palavra->palavra); Unica e exclusivamente se a palavra for duplicada na criação
    free(palavra);
}
