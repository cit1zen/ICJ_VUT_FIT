//Keby tam kniznicu dal nahodou viackrat


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//MAKRO na velkost tabulky
#define TAB_SIZE 100

//Struktura pre jednotlive prvky
typedef struct htab_listitem htab_listitem;
struct htab_listitem
{
    char* key; //Dynamicky alokovane string, dane slovo
    unsigned data; //Vyskyt daneho slova
    htab_listitem* next; //Ukazatel na dalsiu polozku
};

//Struktura samotnej tabulky
typedef struct htab_t
{
    unsigned htab_size;
    htab_listitem *ptr[];
}
htab_t;

//Sprosto okopirovana hash funckia zo zadania
unsigned int hash_function(const char *str, unsigned htab_size) {
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
    }

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

//Funckia vytvori polozku zoznamu
htab_listitem * htab_listitem_create(const char *key)
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

//Funcia hlada dane slovo v zozname
//ak nie je v tabulke, tak zaznam vytvori
htab_listitem * htab_lookup(htab_t *t, const char *key)
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

//Najde a vymaze dany riadok
int htab_remove(htab_t *t, const char *key)
{
    //Odkaz na polozku tabulky
    htab_listitem *before;
    htab_listitem *target;
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
            before->next=target->next;
            free(target->key);
            free(target);
        }
    }
    return 0;
}

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

//Zrusi celu tabulku
//pointer ukaze na NULL
void htab_free(htab_t *t)
{
    htab_clear(t);
    free(t);
    return;
}

void htab_statistics(htab_t *t)
{
    unsigned max=0,min=0,average=0,current=0,pocet_prvkov=0;
    //Prejdeme vsatky "riadky" tabulky
    for( unsigned index_tabulky=0; index_tabulky<t->htab_size;index_tabulky++ )
    {
        htab_listitem *target;
        //Ci nie je riadok prazdny
        if ( (target=t->ptr[index_tabulky])!= NULL )
        {
            //Prechadzame jednotlive bunky
            while (  target->next != NULL )
            {
                target=target->next;
                current++;
                pocet_prvkov++;
            }
            current++;
            pocet_prvkov++;
        }
        //Skontrolovanie ci sme nenasli nove minimum alebo maximum
        //a pridanie poctu prvkov do average aby sme mohli vypocitat priemer
        if(max<current)
            max=current;
        if( (min>current || min==0 ) && current!=0 )
            min=current;
        average+=current;
        current=0;
    }
    printf("Maximum:%u\nMinimum:%u\nPriemer:%.2f",max,min,(float)pocet_prvkov/t->htab_size);
    return;
}

#endif // LIBTABLE
