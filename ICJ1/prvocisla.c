 // prvocisla.c
 // Riesenie IJC-DU1, příklad a), 14.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Najde prvocisla zo zadaneho rozsahu a vypise poslednych 10 vzostupne

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bit-array.h"
#include "error.h"
#include "eratosthenes.h"

#define HORNA_MEDZ_HLADANIA 201000000
#define POCET_HLADANYCH_PRVOCISEL 10

int main(void)
{
    BA_create(bit_pole,HORNA_MEDZ_HLADANIA+1); //inicializacia takeho pola

    Eratosthenes(bit_pole); //funkcia nam vyhlada prvocisla, na hodnote indexu ktory je prvocislo bude 1

    unsigned long prvocisla_pole[POCET_HLADANYCH_PRVOCISEL]; //pole kde zaznamename nasich 10 prvocisel
    int index_prvocisla_pole=0; //Pocitadlo kolko sme uz nasli prvocisel


    unsigned int index=HORNA_MEDZ_HLADANIA; //Postupuje od najvyssieho cisla smerom dolu
    while( index>0 && index_prvocisla_pole<POCET_HLADANYCH_PRVOCISEL ) //Samotny cyklus na zostupovanie dolu
    {
        if(BA_get_bit(bit_pole,index)==0) //Ak je zadane cislo prvocislo
        {
            prvocisla_pole[index_prvocisla_pole]=index; //Hodnota premennej index je najdene prvocislo
            index_prvocisla_pole++; //posunieme pocitadlo kolko sme uz nasli prvocisel
        }
        index--;
    }
    index_prvocisla_pole--; // po skonceni hladania ma hodnotu 10 co je mimo pola preto ju musume zmensit na 9

    for(;index_prvocisla_pole>=0;index_prvocisla_pole--) //cyklus na vypisanie
    {
        printf("%lu\n",prvocisla_pole[index_prvocisla_pole]);
    }

    return 0;
}
