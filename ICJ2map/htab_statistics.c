//Autor: Adam Ormandy
//Datum: 27.4.2015

#include "libhtable.h"

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
    printf("Maximum:%u\nMinimum:%u\nPriemer:%.2f\n",max,min,(float)pocet_prvkov/t->htab_size);
    return;
}
