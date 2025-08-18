#ifndef STRING_H
#define STRING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct String String;

String *string_criar(char *c);
void string_destruir(String *string);

int string_comparar(String *string1, String *string2, int d);
int string_comparar_inteira(String *string1, String *string2);

char get_caractere_string(String *string, int d);
char* get_string(String *string);
int get_tamanho(String *string);

#endif