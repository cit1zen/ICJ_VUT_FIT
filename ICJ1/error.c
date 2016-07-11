 // error.c
 // Riesenie IJC-DU1, příklad b), 21.3.2015
 // Autor: Adam Ormandy, FIT
 // Modul na vy pisovanie chybovych hlaseni

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "error.h"

//Vypise chybu do stderr
void Warning(const char *fmt, ...)
{
    va_list argumenty;
    va_start(argumenty, fmt);
    vfprintf(stderr, fmt, argumenty);
    va_end(argumenty);
    return;
}

//Vypise chybu do stderr, a ukonci program exit(1);
void FatalError(const char *fmt, ...)
{
    va_list argumenty;
    va_start(argumenty, fmt);
    vfprintf(stderr, fmt, argumenty);
    va_end(argumenty);
    exit(1); // ukonci program
}
