//Autor: Adam Ormandy
//Uloha: Program pocita vyskyt jednotlivych slov
//Datum: 27.4.2015
//Prekladac: GCC compiler 4.8.2

//Preco prave 100
//V fgetw aby sa to robilo podla max

#include "libhtable.h"
#include "io.h"

//MAKRO na velkost tabulky
//Najprv som dal len 100, co fungovalo dobre, pre knihy atd
//ale po zatazovom teste ktory trval 5 minut som to zvysil na 1000
#define TAB_SIZE 1000

//MAKRA ERROROV, na vypis hlasok
#define RIADOK_MOC_DLHY -1 //riadok dlhsi ako MAX_LENGHT
#define MALLOC_FAILURE -2 //nepodarilo sa alokovat nejaku strukturu

//MAKRO NA MAXIMALNU DLZKU RIADKA
//dlzka po ktorej sa vypise warning
#define MAX_LENGHT 127

//Callback funckia na vypis jednotlivych prvkov
void vypis_prvkov(htab_listitem *t)
{
    printf("%s\t%u\n",t->key,t->data);
}

int main(void)
{
    //map<string,int> m;
    htab_t *tabulka_vyskytov;
    htab_listitem *vyskyt;
    tabulka_vyskytov=htab_init(TAB_SIZE);
    if(tabulka_vyskytov==NULL)
    {
        return MALLOC_FAILURE;
    }

    //C++ string word;
    char string[MAX_LENGHT+1];
    string[MAX_LENGHT]='\0';

    unsigned max_len_error_amount=0;
    int pocet_znakov=0;
    //C++  while (cin >> word)
    while( ( pocet_znakov=fgetw( string, MAX_LENGHT, stdin) ) != EOF )
    {
        //C++ m[word]++;
        //Pocitanie vyskytov slova
        vyskyt=htab_lookup(tabulka_vyskytov, string);
        if(vyskyt==NULL)
        {
            return MALLOC_FAILURE;
        }
        vyskyt->data+=1;

        //Ak sme za hranicu slova presli prvy krat
        if(pocet_znakov>=MAX_LENGHT && max_len_error_amount<1)
        {
            fprintf(stderr,"Slovo bolo dlhsie ako %d",MAX_LENGHT);
        }
    }

    //Vypis slov a pocet ich vyskytu
    htab_foreach(tabulka_vyskytov,vypis_prvkov);

    //Statistika
    //htab_statistics(tabulka_vyskytov);

    //Upracem si po sebe
    htab_free(tabulka_vyskytov);
}
