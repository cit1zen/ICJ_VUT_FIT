//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Vymaze prvky tabulky
//mierne upraveny htab_foreach()
int htab_clear(htab_t *t)
{
    //Prejdeme vsatky "riadky" tabulky
    for( unsigned index_tabulky=0; index_tabulky<t->htab_size;index_tabulky++ )
    {
        htab_listitem *target;
        htab_listitem *next;
        //Ci nie je riadok prazdny
        if ( (target=t->ptr[index_tabulky])!= NULL )
        {
            //Prechadzame jednotlive bunky
            while (  target->next != NULL )
            {
                next=target->next;
                free(target->key);
                free(target);
                target=next;
            }
            //posledny prvok daneho riadku
            free(target->key);
            free(target);
        }
        t->ptr[index_tabulky]=NULL;
    }
    return 0;
}
