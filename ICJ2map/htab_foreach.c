//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Funcia aplikuje fukciu na kazdy prvok zoznamu
int htab_foreach(htab_t *t,void (*function)())
{
    //Prejdeme vsatky "riadky" tabulky
    for( unsigned index_tabulky=0; index_tabulky<t->htab_size;index_tabulky++ )
    {
        htab_listitem *prvok;
        //Ci nie je riadok prazdny
        if ( (prvok=t->ptr[index_tabulky])!= NULL )
        {
            function(prvok);
            //Prechadzame jednotlive bunky
            while (  prvok->next != NULL )
            {
                prvok=prvok->next;
                function(prvok);
            }
        }
    }
    return 0;
}
