//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Funcia hlada dane slovo v zozname
//ak nie je v tabulke, tak zaznam vytvori
inline htab_listitem * htab_lookup(htab_t *t, const char *key)
{
    //Odkaz na polozku tabulky
    htab_listitem *match;
    //Hash funckia nam vrati aky index v flexibilnom prvku struktury htab_t mame prehladavat
    match = t->ptr[ hash_function(key, t->htab_size ) ];
    //Ak je zoznam na danom indexe prazdny
    //vytvorime prvok a dame ju na prve miesto
    if (match == NULL)
    {
        match = htab_listitem_create(key);
        if (match == NULL)
        {
            return NULL;
        }
        t->ptr[ hash_function(key, t->htab_size ) ] = match;
        return match;
    }
    else
    {
        //Cyklus na hladanie daneho zaznamu o danom slove

        do{
            if( strcmp( match->key, key ) == 0 )
            {
                return match;
            }
            //Posuntie na dalsi prvok
            if( match->next != NULL )
                match=match->next;
        }
        while ( match->next != NULL );
        //Ak sme ho nenasli vytvorime novy a pridame ho nakoniec
        //ak sa nepodari alokovat miesto tak sa vrati NULL
        htab_listitem *novy_prvok=htab_listitem_create(key);
        if (novy_prvok==NULL)
        {
            return NULL;
        }
        match->next=novy_prvok;
        return novy_prvok;
    }
    return NULL;
}
