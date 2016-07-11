//Autor: Adam Ormandy
//Uloha: Rozhranie pre funckie pre program pocitajuci vyskyt slov
//Datum: 27.4.2015
//Prekladac: GCC compiler 4.8.2

#ifndef LIBTABLE
#define LIBTABLE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
extern unsigned int hash_function(const char *str, unsigned htab_size);

//Funkcia na inicializaciu zoznamu
//vracia pointer na strukturu
extern htab_t* htab_init(const unsigned size);

//Funckia vytvori polozku zoznamu
extern htab_listitem * htab_listitem_create(const char *key);

//Funcia hlada dane slovo v zozname
//ak nie je v tabulke, tak zaznam vytvori
extern htab_listitem * htab_lookup(htab_t *t, const char *key);

//Funcia aplikuje fukciu na kazdy prvok zoznamu
extern int htab_foreach(htab_t *t,void (*function)());

//Najde a vymaze dany riadok
extern int htab_remove(htab_t *t, const char *key);

//Vymaze prvky tabulky
//mierne upraveny htab_foreach()
extern int htab_clear(htab_t *t);

//Zrusi celu tabulku
//pointer ukaze na NULL
extern void htab_free(htab_t *t);

extern void htab_statistics(htab_t *t);

#endif // LIBTABLE
