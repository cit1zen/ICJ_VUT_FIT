 // eratosthenes.c
 // Riesenie IJC-DU1, příklad a), 14.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Modul ktory pridava funkciu eratostenovho sita

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "bit-array.h"
#include "eratosthenes.h"
#include "error.h"

//Simuluje funkciu eratosenovho sita7
//pole je bitove pole do ktoreho budeme zaznamenavat ci je dane cislo prvocislo alebo nie
//0 v bit. poli je prvocisla, 1 je zlozene cislo
void Eratosthenes(BitArray_t pole)
{
    //0 a 1 si neni prvocisla
    BA_set_bit(pole,0,1);
    BA_set_bit(pole,1,1);

    //cyklus ktory robi sito
    for(unsigned int i=2;i<sqrt((unsigned)BA_size(pole));i++) //prechadzanie eratostenovym sitom, prechadzame do indexu hodnoty druhej odmocniny velkosti pola v bitoch
    {
        if(BA_get_bit(pole,i)==0) //narazili sme na prvocislo
        {
            for(unsigned int nasobky=2;(nasobky*i)<(unsigned)BA_size(pole);nasobky++) //vylucenie vsetkych nasobkov najdeneho prvocisla
            {
                BA_set_bit(pole,nasobky*i,1);
            }
        }
    }
    return;
}


