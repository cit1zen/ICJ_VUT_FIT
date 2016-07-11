#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <queue>
#include <cstring>

using namespace std;

//GLOBALNA PREMENNA, KOLKO RIADKOV OD KONCA ALEBO OD ZACIATKU MAME VYPISAT
//ak sa to nenastavilo pomocou -n
unsigned tail_pocet_riadkov=10;

//MAKRA PRE ROZNE PRIPADY PLATNEHO VSTUPU
//pre switch aby nepracoval z magickymi konstantami
#define LEN_SUBOR  1  // tail 'nazov_suboru'
#define LEN_SUBOR_STDIN  2  // tail <'nazov_suboru'
#define N_SUBOR  3  // tail -n cislo 'nazov_suboru'
#define N_SUBOR_STDIN  4  // tail -n cislo <'nazov_suboru'
#define N_OD_SUBOR  5 // tail -n +cislo 'nazov_suboru'
#define N_OD_SUBOR_STDIN  6 // tail -n +cislo <'nazov_suboru'

//MAKRA ERROROV, na vypis hlasok
#define FAILED_OPEN_FILE -1
#define UNKNOWN_ARGUMENTS -2

//Funkcia spracovava argumenty prikazovej riadky
//je mierne primitivna
int what_to_do(int argv,char *argc[]);

// tail -n cislo
int tail_n_poslednych( istream &input,unsigned pocet_riadkov );

// tail -n +cislo
int tail_od_n( istream &input,unsigned pocet_riadkov );


int data_subor( unsigned pocet_riadkov, string &nazov_suboru, int (*sposob_vypisu)(istream& ,unsigned ));

//Funckia prijme cislo chyby a vypise patricnu chybovu hlasku
int error(int intern_error_code);

int main(int argc, char* argv[])
{
    //Najprv musime zistit co vlastne robit
    switch(what_to_do(argc,argv))//(what_to_do(argv, argc))
    {
        // tail 'nazov_suboru'
        case LEN_SUBOR:
        {
            string subor=argv[1];
            data_subor( tail_pocet_riadkov, subor, tail_n_poslednych );
            break;
        }
        // tail <'nazov_suboru'
        case LEN_SUBOR_STDIN:
        {
            tail_n_poslednych(cin, tail_pocet_riadkov);
            break;
        }
        // tail -n cislo subor
        case N_SUBOR:
        {
            string subor=argv[3];
            data_subor( tail_pocet_riadkov, subor, tail_n_poslednych );
            break;
        }
        // tail -n cislo <subor
        case N_SUBOR_STDIN:
        {
            tail_n_poslednych( cin , tail_pocet_riadkov);
            break;
        }
        case N_OD_SUBOR:
        {
            string subor=argv[3];
            data_subor( tail_pocet_riadkov, subor, tail_od_n );
            break;
        }
        case N_OD_SUBOR_STDIN:
        {
            tail_od_n( cin , tail_pocet_riadkov);
            break;
        }
        // chyba argumentov
        default:
        {
            break;
        }
    }
}


// tail -n +cislo
int tail_od_n( istream &input,unsigned pocet_riadkov )
{
    //String ktori bude fungovat ako buffer
    string buffer;
    unsigned aktualny_riadok=1;

    //Nacitanie riadku
    while( ( getline(input,buffer) ) )
    {
        //Ak sme uz v riadkoch ktore mame vypisovat
        if( aktualny_riadok>=pocet_riadkov )
            cout << buffer << '\n';
        aktualny_riadok++;
    }
    return 0;
}

// tail -n cislo
int tail_n_poslednych( istream &input,unsigned pocet_riadkov )
{
    //Buffer
    string buffer;
    //Kontajner
    queue<string> ulozisko;
    unsigned aktualny_riadok=1;

    while( ( getline(input,buffer) ) )
    {
        //Ak uz sme nacitali viac riadkov ako si mame pamatat
        if( aktualny_riadok>pocet_riadkov )
            ulozisko.pop();
        ulozisko.push(buffer);
        aktualny_riadok++;
    }

    //Vypis
    while( !ulozisko.empty() )
    {
        cout << ulozisko.front() << '\n';
        ulozisko.pop();
    }
    return 0;
}

//Funckia ak bol zadany subor, lebo moje funkcie primaju istream
//a subor je potreba otvorit ako ifstream
int data_subor( unsigned pocet_riadkov, string &nazov_suboru, int (*sposob_vypisu)(istream& ,unsigned ))
{
    ifstream ifs;
    ifs.open(nazov_suboru);
    if( !ifs.is_open() )
    {
        error(FAILED_OPEN_FILE);
        exit(1);
    }
    istream &input_funckia = ifs;
    sposob_vypisu(input_funckia,pocet_riadkov);
    ifs.close();
    return 0;
}

//Funckia prijme cislo chyby a vypise patricnu chybovu hlasku
int error(int intern_error_code)
{
    switch(intern_error_code)
    {
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
        tail_pocet_riadkov=atoi(argc[2]);
        return N_OD_SUBOR_STDIN;
    }
    // tail -n +cislo 'nazov_suboru'
    else if(argv==4 && argc[2][0]=='+' && strcmp("-n",argc[1])==0  )
    {
        tail_pocet_riadkov=atoi(argc[2]);
        return N_OD_SUBOR;
    }
    // tail -n cislo <'nazov_suboru'
    else if(argv==3 && strcmp("-n",argc[1])==0 )
    {
        tail_pocet_riadkov=atoi(argc[2]);
        return N_SUBOR_STDIN;
    }
    // tail -n cislo 'nazov_suboru'
    else if(argv==4 && strcmp("-n",argc[1])==0  )
    {
        tail_pocet_riadkov=atoi(argc[2]);
        return N_SUBOR;
    }
    //neplatne argumenty prikazoveho riadka
    else
    {
        error(UNKNOWN_ARGUMENTS);
        exit(1);
    }
}
