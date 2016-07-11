//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Funckia vytvori polozku zoznamu
inline htab_listitem * htab_listitem_create(const char *key)
{
    htab_listitem* struktura;
    struktura = malloc(sizeof(htab_listitem));
    if(struktura == NULL )
    {
        return NULL;
    }
    struktura->key=malloc((strlen(key)+1)*sizeof(char));
    if(struktura->key==NULL)
    {
        free(struktura);
        return NULL;
    }
    strcpy(struktura->key,key);
    struktura->data=0;
    struktura->next=NULL;
    return struktura;
}
