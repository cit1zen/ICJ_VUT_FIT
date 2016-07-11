 // steg-decode.c
 // Riesenie IJC-DU1, příklad b), 26.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Algorytmus na precitanie tajne spravy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bit-array.h"
#include "ppm.h"
#include "error.h"
#include "eratosthenes.h"

#define VELKOST_DATA_OBRAZOK (obrazok->xsize*obrazok->ysize*3)

int main(int argc, char **arg)
{
    if(argc!=2) //Skusame ci sme dostali meno suboru
    {
        FatalError("Nespravny pocet argumentov");
    }
    //Nacitame obrazok
    //Funkcia skontroluje format, ci sa dal otvori
    //ak nie vrati NULL
    ppm *obrazok;

    if((obrazok=ppm_read(arg[1]))==NULL)
    {
        FatalError("Chyba suboru");
    }

    //Vytvorime si bitove pole
    //Erastenovym sitom najdeme na bitovom poli prvocisla
    BA_create(prvocisla, MAX_XSIZE*MAX_YSIZE*3  );
    Eratosthenes(prvocisla);

    int pismeno_index=0; //Index pismena
    int pismeno=0; //Pomocna premenna pismeno, sem budeme ukladat jednotlive bity a potom ich vypisovat ako pismeno

    for(unsigned int index=2;index<VELKOST_DATA_OBRAZOK;index++)
    {
        if(BA_get_bit(prvocisla,(int)index)==0) //Ak je dane cislo prvocislo
        {
            int hodnota = GET_BIT_(obrazok->data[index], 0);  //Do hodnoty sa ulozi LSB bajtu
            SET_BIT_(pismeno, pismeno_index, hodnota);
            pismeno_index++;
            if(pismeno_index==8)
            {
                if(isprint(pismeno)) //Ci je rozkodovane pismeno pisatelne
                {
                    fputc(pismeno, stdout);
                    pismeno_index=0;
                    pismeno=0;
                }
                else if(pismeno=='\0') //Ci sme nerozkodovali konec spravy
                {
                    fputc(pismeno, stdout);
                    free(obrazok);
                    exit(EXIT_SUCCESS);
                }
                else //Nasli sme chybu v zakodovanej sprave
                {
                    pismeno_index=0;
                    free(obrazok);
                    FatalError("Chyba dat");
                }

            }

        }
    }
    free(obrazok);
    return EXIT_FAILURE;
}
