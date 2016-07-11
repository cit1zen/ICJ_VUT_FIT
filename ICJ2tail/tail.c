//Autor: Adam Ormandy
//Uloha: Vytvorit program v jazyku c, implementujuci POSIC utilitu tail
//Datum: 27.4.2015
//Prekladac: GNU GCC compiler

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

//MAKRO KTORE IMPLICITNE NASTAVUJE POCET RIADKOV KTORE MA VYPISAT
//ak sa to nenastavilo pomocou -n
#define IMPL_POCET_RIADKOV 10

//MAKRO NA MAXIMALNU DLZKU RIADKA
//dlzka po ktorej sa vypise warning
#define MAX_LENGHT 510

//MAKRA PRE ROZNE PRIPADY PLATNEHO VSTUPU
//pre switch aby nepracoval z magickymi konstantami
#define LEN_SUBOR  1  // tail 'nazov_suboru'
#define LEN_SUBOR_STDIN  2  // tail <'nazov_suboru'
#define N_SUBOR  3  // tail -n cislo 'nazov_suboru'
#define N_SUBOR_STDIN  4  // tail -n cislo <'nazov_suboru'
#define N_OD_SUBOR  5 // tail -n +cislo 'nazov_suboru'
#define N_OD_SUBOR_STDIN  6 // tail -n +cislo <'nazov_suboru'

//MAKRA ERROROV, na vypis hlasok
#define RIADOK_MOC_DLHY -1 //riadok dlhsi ako MAX_LENGHT
#define FAILED_OPEN_FILE -2
#define UNKNOWN_ARGUMENTS -3


//Funkcia spracovava argumenty prikazovej riadky
int what_to_do(int argv,char *argc[]);

//Funckia prijme cislo chyby a vypise patricnu chybovu hlasku
//pripadne program cely ukonci
int error(int intern_error_code);

// tail -n cislo
int tail_n_poslednych( FILE *input,int pocet_riadkov );

// tail -n +cislo
int tail_od_n( FILE *input,int pocet_riadkov );

//Funkcia na to ak su vstupne data v subore, a nenacitame ich z stdin
//je pouzity callback
//funkcia v podstate robi len fopen a fclose
int data_subor( int pocet_riadkov, char *argc, int (*sposob_vypisu)(FILE* ,int ));

//Funckia na overenie cisla ktore hovori od kolkeho riadku alebo kolko poslednych riadkov mame vypisovat
//Relativne primitivna, len sa mi nechcelo mat 4x to iste napisane
//Vracia hodnotu od kolkeho alebo kolko riadkov mame vypisat ako int
//Pouzite napr aby pri -n +3 nam vrati 3
int overenie_konvertovanie( char *argc )
{
    int pocet_riadkov;
    if(  ( pocet_riadkov = atoi(argc) )  <= 0 )
    {
        error(UNKNOWN_ARGUMENTS);
        exit (1);
    }
    return pocet_riadkov;
}


int main(int argv, char *argc[])
{
    //Najprv musime zistit co vlastne robit
    switch(what_to_do(argv,argc))//(what_to_do(argv, argc))
    {
        // tail 'nazov_suboru'
        case LEN_SUBOR:
        {
            data_subor( IMPL_POCET_RIADKOV, argc[1], tail_n_poslednych );
            break;
        }
        // tail <'nazov_suboru'
        case LEN_SUBOR_STDIN:
        {
            tail_n_poslednych(stdin, IMPL_POCET_RIADKOV);
            break;
        }
        case N_SUBOR:
        {
            data_subor( overenie_konvertovanie(argc[2]), argc[3], tail_n_poslednych );
            break;
        }
        case N_SUBOR_STDIN:
        {
            tail_n_poslednych( stdin , overenie_konvertovanie(argc[2]));
            break;
        }
        case N_OD_SUBOR:
        {
            data_subor( overenie_konvertovanie(argc[2]), argc[3], tail_od_n );
            break;
        }
        case N_OD_SUBOR_STDIN:
        {
            tail_od_n( stdin , overenie_konvertovanie(argc[2]));
            break;
        }
        // chyba argumentov
        default:
        {
            break;
        }
    }
}

//Funkcia spracovava argumenty prikazovej riadky
//je mierne primitivna
int what_to_do(int argv,char *argc[])
{
    // tail <'nazov_suboru'
    if(argv==1)
    {
        return LEN_SUBOR_STDIN;
    }
    // tail 'nazov_suboru'
    else if(argv==2)
    {
        return LEN_SUBOR;
    }
    // tail -n +cislo <'nazov_suboru'
    else if(argv==3 && argc[2][0]=='+' && strcmp("-n",argc[1])==0 )
    {
        return N_OD_SUBOR_STDIN;
    }
    // tail -n +cislo 'nazov_suboru'
    else if(argv==4 && argc[2][0]=='+' && strcmp("-n",argc[1])==0  )
    {
        return N_OD_SUBOR;
    }
    // tail -n cislo <'nazov_suboru'
    else if(argv==3 && strcmp("-n",argc[1])==0 )
    {
        return N_SUBOR_STDIN;
    }
    // tail -n cislo 'nazov_suboru'
    else if(argv==4 && strcmp("-n",argc[1])==0  )
    {
        return N_SUBOR;
    }
    //neplatne argumenty prikazoveho riadka
    else
    {
        error(UNKNOWN_ARGUMENTS);
        exit(1);
    }
}


//Funckia prijme cislo chyby a vypise patricnu chybovu hlasku
int error(int intern_error_code)
{
    switch(intern_error_code)
    {
        //riadok je dlhsi ako MAX_LENGHT
        case RIADOK_MOC_DLHY:
        {
            fprintf(stderr, "Riadok bol dlhsi ako %d\n", MAX_LENGHT);
            return 0;
        }
        case FAILED_OPEN_FILE:
        {
            fprintf(stderr, "Nepodarilo sa otvorit subor\n");
            return 0;
        }
        case UNKNOWN_ARGUMENTS:
        {
            fprintf(stderr, "Neznamy format argumentov prikazoveho riadka\n");
            return 0;
        }
        default:
        {
            fprintf(stderr, "Neznama chyba\n");
            exit(1);
        }
    }
    return 0;
}

// tail -n +cislo
int tail_od_n( FILE *input,int pocet_riadkov )
{
    //Vytvorime buffer
    char buffer[1];
    int miesto_v_riadku=0;
    int warning_count=0;
    int aktualny_riadok=1;

    //Nacita z input po jednom znaku
    while( ( fread( buffer, 1, 1 , input )) == 1 )
    {
        //Ak sme nacitali koniec riadka
        if(buffer[0]=='\n' )
        {
            //Tento if sa vykona len ked sme v oblasti ktora sa ma vypisovat
            if( aktualny_riadok>=pocet_riadkov )
                printf("%c",'\n');
            miesto_v_riadku=0;
            aktualny_riadok++;
        }
        //Ak sme nacitali nejaky iny charakter
        //je tu implementovane obmedzenie ze riadok nemoze byt vacsi ako MAX_LENGHT
        //ak je riadok vacsi tak sa nevypisuju nacitane charakteri a prechadzame riadok dalej az do konca znaku
        //plus ked sa to stale prvy krat tak sa vypise warning
        else if(miesto_v_riadku<MAX_LENGHT && aktualny_riadok>=pocet_riadkov )
        {
            miesto_v_riadku++;
            printf("%c",buffer[0]);
        }
        //Riadok je dlhsi ako MAX_LENGHT
        else if( miesto_v_riadku >= MAX_LENGHT)
        {
            //Vypise warnign ak sa to stalo prvy krat
            if(warning_count==0)
            {
                error(RIADOK_MOC_DLHY);
                warning_count++;
            }
        }
    }
    return 0;
}

int tail_n_poslednych( FILE *input,int pocet_riadkov )
{
    //Pole stringov kam ukladame x poslednych riadkov
    char lines[pocet_riadkov][MAX_LENGHT+2];
    //Tento cyklus je tu na to keby sme nacitali menej riadkov ako mame vypisat
    //aby to nevypisovalo vselijaky odpad
    for (int i=0;i<pocet_riadkov;i++)
        strcpy(lines[i],"\0");
    //Tu budeme ukradat nacitany riadok
    char line[MAX_LENGHT+2];
    //Vytvorime buffer
    char buffer[1];
    int miesto_v_riadku=0;
    int warning_count=0;

    //Nacita z input po jednom znaku
    while( ( fread( buffer, 1, 1 , input )) == 1 )
    {
        //Ak sme nacitali koniec riadka
        if(buffer[0]=='\n' )
        {
            line[miesto_v_riadku]='\n';
            line[miesto_v_riadku+1]='\0';
            miesto_v_riadku=0;

            //Tento ultrakrutoprisny cyklus je na posunutie zasobniku z n poslednimi stringami
            //Nami nacitany string len da na poslednu bunku v poli a cele pole posunie o 1
            char medziriadok[MAX_LENGHT+1]=" ";
            for(int cislo_riadka=(pocet_riadkov-1); cislo_riadka>=0;cislo_riadka--)
            {
                strcpy(medziriadok,lines[cislo_riadka]);
                strcpy(lines[cislo_riadka],line);
                strcpy(line,medziriadok);
            }
        }
        //Ak sme nacitali nejaky iny charakter
        //je tu implementovane obmedzenie ze riadok nemoze byt vacsi ako MAX_LENGHT
        //ak je riadok vacsi tak sa nevypisuju nacitane charakteri a prechadzame riadok dalej az do konca znaku
        //plus ked sa to stale prvy krat tak sa vypise warning
        else if( miesto_v_riadku<MAX_LENGHT )
        {
            line[miesto_v_riadku]=buffer[0];
            miesto_v_riadku++;
        }
        //Riadok je dlhsi ako MAX_LENGHT
        else if( miesto_v_riadku>=MAX_LENGHT)
        {
            //Vypise warnign ak sa to stalo prvy krat
            if(warning_count==0)
            {
                error(RIADOK_MOC_DLHY);
                warning_count++;
            }
        }
    }

    //Samotny vypis
    for(int cislo_riadka=0; cislo_riadka<pocet_riadkov;cislo_riadka++)
    {
        printf("%s",lines[cislo_riadka]);
    }
    return 0;
}


int data_subor( int pocet_riadkov, char *nazov_suboru, int (*sposob_vypisu)(FILE* ,int ))
{
    FILE *input;
    input = fopen(nazov_suboru,"r");
    if(input==NULL)
    {
        error(FAILED_OPEN_FILE);
        exit(1);
    }
    sposob_vypisu(input,pocet_riadkov);
    fclose(input);
    return 0;
}
