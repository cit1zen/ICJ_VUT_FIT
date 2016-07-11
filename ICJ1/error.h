 // error.h
 // Riesenie IJC-DU1, příklad b), 21.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Rozhranie pre modul na vypisovanie chybovych hlasenich

//vypise chybu do stderr
void Warning(const char *fmt, ...);

//vypise chybu do stderr, a ukonci program exit(1);
void FatalError(const char *fmt, ...);

