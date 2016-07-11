//Autor: Adam Ormandy
//Datum: 27.4.2015
#include "libhtable.h"

//Sprosto okopirovana hash funckia zo zadania
unsigned int hash_function(const char *str, unsigned htab_size) {
    unsigned int h=0;
    const unsigned char *p;
    for(p=(const unsigned char*)str; *p!='\0'; p++)
        h = 65599*h + *p;
    return h % htab_size;
    }
