#include "Palavra.h"

struct Palavra
{
    Vector* arquivos;
    char* palavra;
};

Palavra* palavra_construtor(Vector* arquivos, char* palavra);
void palavra_destrutor(Palavra* p);
