 // ppm.h
 // Riesenie IJC-DU1, příklad b), 14.3.2015
 // Autor: Adam Ormandy, xorman00@stud.fit.vutbr.cz FIT
 // Rozhranie pre funkcie modulu ppm


#define MAX_XSIZE 2000
#define MAX_YSIZE 2000

 typedef struct ppm{
 unsigned xsize;
 unsigned ysize;
 char data[]; // RGB bajty, celkem 3*xsize*ysize
 }ppm;

//Nacita obsah obrazka formatu ppm
//Ulozi jeho data do datovej struktury ppm
//Zaroven kontroluje format
//filename je nazov obrazka ktori ma otvorit
ppm * ppm_read(const char * filename);

//Nacita obsah datovej struktury ppm
//Ulozi ho do obrazka formatu ppm
//*p je poiter na strukturu
//filename je nazov suboru, je potrebne aj pripojit priponu ppm
int ppm_write(struct ppm *p, const char * filename);


