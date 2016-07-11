//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Zrusi celu tabulku
//pointer ukaze na NULL
void htab_free(htab_t *t)
{
    htab_clear(t);
    free(t);
    return;
}
