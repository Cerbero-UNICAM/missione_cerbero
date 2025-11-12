#include <stdio.h>
#include <stdlib.h>

typedef struct 
{
    char codice_ISBN[18]; //definizione inclusiva di tratttini (13 cifre + 4 trattini + terminatore)
    char titolo[101];
    char autore[51]; 
    int anno_pubblicazione; // (1800<= ap <=2025)
    int numero_copie;
    char genere[31];
} Libro;

typedef struct {
    int codice_utente;
    char nome[51]; 
    char cognome[51];
    char email[81];
    char data_iscrizione[11]; // (formato gg//mm//aaaa)
} Utente;

typedef struct {
    int codice_prestito; // (chiave univoca autoincrementata)
    char codice_ISBN_libro[14];
    int codice_utente;
    char data_prestito[11]; // (formato gg//mm//aaaa)
    char data_restituzione[11]; // 30 giorni dopo il prestito
    int restituito; // restituito=1; mancante=0
} Prestito;






int main()
{
    return 0;
}