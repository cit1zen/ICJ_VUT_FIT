//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

//Najde a vymaze dany riadok
int htab_remove(htab_t *t, const char *key)
{
    //Odkaz na polozku tabulky
    htab_listitem *before;
    htab_listitem *target;
    before=NULL;
    //Hash funckia nam vrati aky index v flexibilnom prvku struktury htab_t mame prehladavat
    target = t->ptr[ hash_function(key, t->htab_size ) ];
    //Ak je zoznam na danom indexe prazdny
    //teda nemusime hladat
    if (target == NULL)
    {
        return 0;
    }
    else
    {
        //Cyklus na hladanie daneho zaznamu o danom slove
        while( strcmp(target->key,key)!=0)
        {
            before=target;
            target=target->next;
        }
        if(target!=NULL)
        {
            if(before!=NULL)
            {
                before->next=target->next;

            }
            //Aby nezostal pointer v str, ked vymazeme jediny zaznam v danom riadku
            //co by mohlo sposobit problem
            else
            {
                t->ptr[ hash_function(key, t->htab_size )]=NULL;
            }
            free(target->key);
            free(target);
        }
    }
    return 0;
}
