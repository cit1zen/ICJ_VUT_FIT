 // ppm.c
 // Riesenie IJC-DU1, příklad b), 14.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Modul pre pracu z obrazkami formatu ppm

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ppm.h"
#include "error.h"

//Nacita obsah obrazka formatu ppm
//Ulozi jeho data do datovej struktury ppm
//Zaroven kontroluje format
//filename je nazov obrazka ktori ma otvorit
ppm * ppm_read(const char * filename)
{
    //nacitanie suboru
    FILE *obrazok;
    obrazok=fopen(filename,"r");
    if(obrazok==NULL) //ak nastala nejaka chyba
    {
        Warning("Nepodarilo sa nacitat subor %s",filename);
        return NULL;
    }

    //nacitanie zakladnych udajov
    char buffer[10];
    unsigned int farby,xsize,ysize;
    if(fscanf(obrazok,"%s %u %u %u", buffer, &xsize, &ysize, &farby )<0)
    {
        Warning("Chyba pri nacitani suboru");
        fclose(obrazok);
        return NULL;
    }

    //zistovanie
    //Ci je ma obrazok spravny format
    if(strcmp(buffer,"P6")!=0 && farby==255)
    {
        Warning("Chyba formatu");
        fclose(obrazok);
        return NULL;
    }
    //Ci je v medziach na velkost
    if( xsize>=MAX_XSIZE && ysize>=MAX_YSIZE )
    {
        Warning("Obrazok je moc velky");
        fclose(obrazok);
        return NULL;
    }

    //alokovanie struktury
    ppm *Data;
    if((Data=malloc(sizeof(ppm) + sizeof(char)*3*xsize*ysize))==NULL)
    {
        Warning("Nastala chyba pri alokovani struktury");
        fclose(obrazok);
        return NULL;
    }
    //rozmery obrazka
    Data->xsize=xsize;
    Data->ysize=ysize;

    //Nacitame udaje z obrazka
    //preskocenie medzery
    if (fread(Data->data,sizeof(char),1,obrazok)!=1 && Data->data[0]!='\n')
    {
        Warning("Chyba formatu");
        free(Data);
        fclose(obrazok);
        return NULL;
    }
    if (fread(Data->data,sizeof(char), 3 * Data->xsize * Data->ysize,obrazok)!=(3 * Data->xsize * Data->ysize)) //ak sa pocet nacitanych prvkov nerovna poctu prvkov ktore sa mali nacitat
    {
        Warning("Nepodarilo sa nacitat cely obrazok");
        free(Data);
        fclose(obrazok);
        return NULL;
    }


    fclose(obrazok); //zatvorim subor
    return Data;
}



//Nacita obsah datovej struktury ppm
//Ulozi ho do obrazka formatu ppm
//*p je poiter na strukturu
//filename je nazov suboru, je potrebne aj pripojit priponu ppm
int ppm_write(struct ppm *p, const char * filename)
{
    FILE *obrazok;
    obrazok=fopen(filename,"w+"); //vytvorime subor
    if(obrazok==NULL)
    {
        Warning("Chyba pri vytvarani suboru");
        return -1;
    }
    if( fprintf(obrazok,"%s\n%u %u\n%u\n","P6",p->xsize,p->ysize,255)<0 ) //zakladne udaje o type, rozmeroch a hlbke farieb
    {
        Warning("Chyba pri vytvarani suboru");
        fclose(obrazok);
        return -2;
    }
    if(fwrite(p->data,sizeof(char), 3 * p->xsize * p->ysize,obrazok)!=(3 * p->xsize * p->ysize)) //data jednotlivych pixelov
    {
        Warning("Chyba pri vytvarani suboru");
        fclose(obrazok);
        return -3;
    }
    fclose(obrazok);
    return 0;
}


