//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Funkcia na inicializaciu zoznamu
//vracia pointer na strukturu
htab_t* htab_init(const unsigned size)
{
    htab_t* struktura; //Odkaz na strukturu, ktoru budeme returnovat
    struktura = malloc(sizeof(htab_t) + (size+1)*sizeof(htab_listitem));
    //Ak sa nepodarilo alokovat pamat
    if(struktura==NULL)
    {
        return NULL;
    }
    struktura->htab_size=size;
    return struktura;
}
