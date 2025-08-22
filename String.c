#include "String.h"

struct String {
    char *c;
    int tamanho;
};

String *string_criar(char* c) {
    String *string = (String*)calloc(1,sizeof(String));

    string->c = strdup(c);
    string->tamanho = strlen(c);

    return string;
}

void string_destruir(String *string) {
    if (string != NULL) {
        free(string->c);
        free(string);
    }
}

int string_comparar(String *string1, String *string2, int d) {
    int min = string1->tamanho < string2->tamanho ? string1->tamanho : string2->tamanho;

    for (int i=d; i<min; i++) {
        if (string1->c[i] < string2->c[i]) {
            return -1;
        } 
        if (string1->c[i] > string2->c[i]) {
            return 1;
        }
    }
    return string1->tamanho - string2->tamanho;
}

int string_comparar_inteira(String *string1, String *string2) {
    return string_comparar(string1, string2, 0);
}

char get_caractere_string(String *string, int d) {  return string->c[d];    }
char* get_string(String *string) {                  return string->c;       }
int get_tamanho(String *string) {                   return string->tamanho; }
