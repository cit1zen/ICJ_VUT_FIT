 // bit-array.h
 // Riesenie IJC-DU1, příklad a), 14.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Zakladne rozhranie na riesenie ulohy

#include "error.h"

//Struktura bitoveho pola
//Z prvu som pouzival strukturu ale to sposobovalo neocakavane chovanie tak som ho nahradil polom, kde 1 long obsahuje informaciu o velkosti pola
//a ostatna cast je samotne bitove pole
typedef unsigned long BitArray_t[];


/*MAKRA na pracu z bitovym polom*/

//Aky datovy typ je pouziti ako zaklad bitoveho pola
#define POUZITY_TYP unsigned long

//Makro zabezpecuje ze preskocime prvu bunku, kde je velkost pola
#define SPRAVNY_INDEX(i,p) (i/(sizeof(*p)*8)+1)

//Na pocitanie shiftu pre DU1_SET_BIT_ a DU1_GET_BIT_ makra
#define POSUN(i,p) ( i%(sizeof(p)*8) )

//Kontrola medzi
#define OHRANICENIE(i,p) (( (unsigned)BA_size(p)>(unsigned)i)? 1 : 0 )

//Vypocita ako velke musi byt bitove pole vzhladom na zaklad pola
#define VELKOST_V_LONG(velikost)  ( ( velikost%(sizeof(POUZITY_TYP)*8) ) ?  (velikost/(sizeof(POUZITY_TYP)*8)+2) : (velikost/(sizeof(POUZITY_TYP)*8)+2))

//Vracia hodnotu daneho bitu
//p je pole bitov, i index bitu v bitovom poli
#define DU1_GET_BIT_(p,i) \
    ((( p[ SPRAVNY_INDEX (i,p) ] & ( 1LU <<  POSUN (i,p) ) )>0) ? 1 : 0)

//Zmeni hodnotu daneho bitu,
//p je pole bitov, i index bitu v bitovom poli, b je hodnota na ktoru chceme zmenit bit
#define DU1_SET_BIT_(p,i,b)\
   ( (b==1) ? ( p[ SPRAVNY_INDEX(i,p) ]|= ( 1LU <<  POSUN (i,p) ) )  : ( p[ SPRAVNY_INDEX(i,p) ]&=~( 1LU<< POSUN (i,p) )) )


//Vracia hodnotu daneho bitu
//p je pole bitov, i index bitu v bitovom poli
#define GET_BIT_(p,i) \
    ((( p & ( 1 <<  i%(sizeof(p)*8) ) )>0) ? 1 : 0)

//Zmeni hodnotu daneho bitu,
//p je pole bitov, i index bitu v bitovom poli, b je hodnota na ktoru chceme zmenit bit
#define SET_BIT_(p,i,b) \
    ( (b==1)?  p|=1<<i%(sizeof(p)*8)  : (p&=~(1<<i%(sizeof(p)*8))) )


//Vytvori pole o danou velkostou bitov
//jmeno_pole je BitArray_t je pole ktore mame inicializovat,velikost je velkost bitoveho pola v bitoch
#define BA_create(jmeno_pole,velikost) \
     unsigned long jmeno_pole[ VELKOST_V_LONG(velikost) ]= {velikost,0}



//Podmieneny preklad, ak nieje definovane USE_INLINE budu sa pouzivat tieto makra
#ifndef USE_INLINE

//Vracia velkost bitoveho pola v bitoch
 #define BA_size(jmeno_pole) \
    (jmeno_pole[0])

//Zmeni hodnotu daneho bitu,
//p je pole bitov, i index bitu v bitovom poli, b je hodnota na ktoru chceme zmenit bit
//Ak zajdeme mimo hranic tak ukonci program cez Fatalerror
#define BA_set_bit(jmeno_pole,index,vyraz) \
    ( (OHRANICENIE(index,jmeno_pole))==1 ? (DU1_SET_BIT_(jmeno_pole,index,vyraz)) : (FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)BA_size(jmeno_pole)-1), 2) )


//Vracia hodnotu daneho bitu
//p je pole bitov, i index bitu v bitovom poli
//Ak zajdeme mimo hranic tak ukonci program cez Fatalerror
#define BA_get_bit(jmeno_pole,index) \
    ( (OHRANICENIE(index,jmeno_pole))==1 ? (DU1_GET_BIT_(jmeno_pole,index)) : (FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)BA_size(jmeno_pole)>index-1), 2) )





/*INlINE funkcie na pracu z bitovym polom*/
#else
#include <stdio.h>
#include <stdlib.h>

//Vracia velkost bitoveho pola v bitoch
static inline int BA_size(BitArray_t jmeno_pole)
{
    return jmeno_pole[0];
}

//Zmeni hodnotu daneho bitu,
//p je pole bitov, i index bitu v bitovom poli, b je hodnota na ktoru chceme zmenit bit
//Ak zajdeme mimo hranic tak ukonci program cez Fatalerror
static inline void BA_set_bit(BitArray_t jmeno_pole,int index,int vyraz)
{
    if( (OHRANICENIE((unsigned)index,jmeno_pole))==1 ) //sme v hraniciach bitoveho pola
    {
        DU1_SET_BIT_(jmeno_pole,index,(unsigned)vyraz);
    }
    else //ak ideme za hranice bitoveho pola
    {
        FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)BA_size(jmeno_pole)-1);
    }
}

//Vracia hodnotu daneho bitu
//p je pole bitov, i index bitu v bitovom poli
//Ak zajdeme mimo hranic tak ukonci program cez Fatalerror
static inline int BA_get_bit(BitArray_t jmeno_pole,int index)
{
    if( (OHRANICENIE((unsigned)index,jmeno_pole))==1 ) //sme v hraniciach bitoveho pola
    {
        return (DU1_GET_BIT_(jmeno_pole,(unsigned)index));
    }
    else //ak ideme za hranice bitoveho pola
    {
        FatalError("Index %ld mimo rozsah 0..%ld", (long)index, (long)BA_size(jmeno_pole)-1);
    }
    return EXIT_FAILURE;
}

//koniec podmieneneho prekladu ak mame pouzit inline funkcii
#endif //USE_INLINE



