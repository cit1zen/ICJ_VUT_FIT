//Autor: Adam Ormandy
//Datum: 27.4.2015
//Prekladac: GCC compiler 4.8.2

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "io.h"

//Nacita jedno slovo z FILE *f
//Slovo je postupnost znakov oddelena whitespasom
int fgetw(char *s, int max, FILE *f)
{
    //Toto je tu na to aby som vytvoril dynamicky format
    //lebo podporovat stringy je too mainstream
    char string[max];
    sprintf(string, "%d", max);
    //dlzka max pre 127 je to 3, potom +1 pre % +1 pre s a +1 pre \0
    char medziprodukt[strlen(string)+3];//+1 for the zero-terminator
    medziprodukt[0]='%';
    strcat(medziprodukt, string);
    medziprodukt[strlen(string)+1]='s';
    medziprodukt[strlen(string)+2]='\0';

    //A vznikol nam format pre fscanf
    const char *format=medziprodukt;

    s[max]='\0';

    //Nacitanie nasho slova
    //kedze je slovo definovane ako postupnost znakov oddelena whitespace znakmi
    //da sa pouzit format %s
    if(fscanf(f,format,s)==EOF)
    {
        return EOF;
    }

    //Toto tu je len na to aby odstranilo veci ako su bodky, ciarky ent.
    //co bol problem ak som programom nechal nasat knihu
    //nevyhoda je ze ahoj+ahoj vrati len 1 ahoj
    //ale je to asi nad ramec ulohy tak to je zakomentovane
    /*
    for(int index=0; index<MAX_LENGHT; index++)
        if( !isalpha( (int)s[index] ) && s[index]!='-' && s[index]!='_' )
           s[index]=' ';
    sscanf(s,"%127s",s);
    */

    //Ci sme nahodou nepresli az za MAX hranicu pola
    for(int index=0; index<max; index++)
        if(s[index]=='\0')
            return index;
    //Ak sme presli za MAX hranicu slova
    //tak nas tento fscanf posunie za nu
    if(fscanf(f,"%*s")==EOF)
    {
        return EOF;
    }
    return max;
}
