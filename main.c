#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    char codice_ISBN[18]; // definizione inclusiva di tratttini (13 cifre + 4 trattini + terminatore)
    char titolo[101];
    char autore[51];
    int anno_pubblicazione; // (1800<= ap <=2025)
    int numero_copie;
    char genere[31];
} libro;

typedef struct
{
    int codice_utente;
    char nome[51];
    char cognome[51];
    char email[81];
    char data_iscrizione[11]; // (formato gg//mm//aaaa)
} utente;

typedef struct
{
    int codice_prestito; // (chiave univoca autoincrementata)
    char codice_ISBN_libro[14];
    int codice_utente;
    char data_prestito[11];     // (formato gg//mm//aaaa)
    char data_restituzione[11]; // 30 giorni dopo il prestito
    int restituito;             // restituito=1; mancante=0
} prestito;


void inserisci_libro(libro *ptr,int* n);


int main()
{
    int scelta;
    int* ctr_libri;
    *ctr_libri = 0;

    //Apertura file binario per i libri in lettura scrittura
  /*FILE *fp_libro;  
   size_t flag_libro;
   fp_libro = fopen("libri.bin","r+b");  //controllo apertura file
        if (fp_libro == NULL){
            puts("errore apertura file libri");
            return 1;
        }*/


    // Inizializzazione (simulata)
    printf("=== SISTEMA GESTIONE BIBLIOTECA ===\n\n");
    printf("Inserisci capacita' massima libri: ");
    int caplibri;
    scanf("%d", &caplibri);
    printf("Inserisci capacita' massima utenti: ");
    int caputenti;
    scanf("%d", &caputenti);
    printf("Inserisci capacita' massima prestiti: ");
    int capprestiti;
    scanf("%d", &capprestiti);
    printf("Database inizializzato correttamente!\n\n");

    libro *ptr_libri = (libro *)malloc(caplibri * sizeof(libro));
    if (ptr_libri == NULL)
    {
        printf("Errore: memoria insufficiente!\n");
        return -1;
    }

    utente *ptr_utenti = (utente *)malloc(caputenti * sizeof(utente));
    if (ptr_utenti == NULL)
    {
        printf("Errore: memoria insufficiente!\n");
        return -1;
    }

    // alloco strutture e controllo
    prestito *ptr_prestiti = (prestito *)malloc(capprestiti * sizeof(prestito));
    if (ptr_prestiti == NULL)
    {
        printf("Errore: memoria insufficiente!\n");
        return -1;
    }
    int conta_prestiti = 0; // contatore lunghezza del vettore prestiti

    do
    {
        // Stampa del menù
        printf("=== MENU PRINCIPALE ===\n");
        printf("--- GESTIONE LIBRI ---\n");
        printf("1. Inserisci nuovo libro\n");
        printf("2. Visualizza tutti i libri\n");
        printf("3. Cerca libro per ISBN\n");
        printf("4. Cerca libri per autore\n");
        printf("5. Libri disponibili per prestito\n");
        printf("--- GESTIONE UTENTI ---\n");
        printf("6. Inserisci nuovo utente\n");
        printf("7. Visualizza tutti gli utenti\n");
        printf("8. Cerca utente per codice\n");
        printf("--- GESTIONE PRESTITI ---\n");
        printf("9. Registra nuovo prestito\n");
        printf("10. Registra restituzione libro\n");
        printf("11. Visualizza prestiti attivi\n");
        printf("12. Visualizza storico prestiti utente\n");
        printf("--- STATISTICHE ---\n");
        printf("13. Statistiche generali\n");
        printf("14. Libri per genere\n");
        printf("15. Top 5 libri piu' prestati\n");
        printf("--- FILE ---\n");
        printf("16. Salva database su file binario\n");
        printf("17. Carica database da file binario\n");
        printf("18. Esporta catalogo in formato testo\n");
        printf("19. Esporta report prestiti in formato testo\n");
        printf("20. Esci\n\n");

        printf("Scelta: ");
        scanf("%d", &scelta);

        switch (scelta)
        {
        case 1:
            printf("\n--- Inserisci nuovo libro ---\n");
            inserisci_libro(ptr_libri,ctr_libri);
            break;

        case 2:
            printf("\n--- Visualizza tutti i libri ---\n");
            // Qui implementerai la visualizzazione libri
            break;

        case 3:
            printf("\n--- Cerca libro per ISBN ---\n");
            // Qui implementerai la ricerca per ISBN
            break;

        case 4:
            printf("\n--- Cerca libri per autore ---\n");
            // Qui implementerai la ricerca per autore
            break;

        case 5:
            printf("\n--- Libri disponibili per prestito ---\n");
            // Qui implementerai la visualizzazione libri disponibili
            break;

        case 6:
            printf("\n--- Inserisci nuovo utente ---\n");
            // Qui implementerai l'inserimento utente
            break;

        case 7:
            printf("\n--- Visualizza tutti gli utenti ---\n");
            // Qui implementerai la visualizzazione utenti
            break;

        case 8:
            printf("\n--- Cerca utente per codice ---\n");
            // Qui implementerai la ricerca utente
            break;

        case 9:
            printf("\n--- Registra nuovo prestito ---\n");
            // Qui implementerai la registrazione prestito
            break;

        case 10:
            printf("\n--- Registra restituzione libro ---\n");
            // Qui implementerai la restituzione
            break;

        case 11:
            printf("\n--- Visualizza prestiti attivi ---\n");
            // Qui implementerai la visualizzazione prestiti attivi
            break;

        case 12:
            printf("\n--- Visualizza storico prestiti utente ---\n");
            // Qui implementerai lo storico prestiti
            break;

        case 13:
            printf("\n--- Statistiche generali ---\n");
            // Qui implementerai le statistiche
            break;

        case 14:
            printf("\n--- Libri per genere ---\n");
            // Qui implementerai i libri per genere
            break;

        case 15:
            printf("\n--- Top 5 libri piu' prestati ---\n");
            // Qui implementerai la top 5
            break;

        case 16:
            printf("\n--- Salva database su file binario ---\n");
            // Qui implementerai il salvataggio
            break;

        case 17:
            printf("\n--- Carica database da file binario ---\n");
            // Qui implementerai il caricamento
            break;

        case 18:
            printf("\n--- Esporta catalogo in formato testo ---\n");
            // Qui implementerai l'esportazione catalogo
            break;

        case 19:
            printf("\n--- Esporta report prestiti in formato testo ---\n");
            // Qui implementerai l'esportazione prestiti
            break;

        case 20:
            printf("\nArrivederci!\n");
            break;

        default:
            printf("\nScelta non valida! Inserisci un numero tra 1 e 20.\n");
            break;
        }

        // per fare felice il puffetto
        if (scelta != 20)
        {
            printf("\nPremi INVIO per continuare...");
            getchar(); // Pulisce il buffer
            getchar(); // Aspetta l'input dell'utente
        }

    } while (scelta != 20);

    // libero memoria
    free(ptr_prestiti);
    free(ptr_utenti);
    free(ptr_libri);

    return 0;
}

void inserisci_libro(libro* ptr,int *n)
{
int c;
int k = *n;

puts("inserisci i seguenti dati del libro");

printf("codice ISBN(XXX-X-XXXX-XXXX-X): ");
scanf("%s", (ptr+k)->codice_ISBN);
    while ((c = getchar()) != '\n'); // pulire stdin
    for (int i = 0; i < k;i++){
        if(strcmp((ptr+k)->codice_ISBN,(ptr+i)->codice_ISBN) == 0){     //controllo unicità ISBN
            printf("l ISBN è uguale ad un libro gia esistente,inseriscine un altro");
            scanf("%s", (ptr+k)->codice_ISBN);
        }
    }

printf("Titolo: ");
fgets((ptr+k)->titolo, sizeof((ptr+k)->titolo), stdin);
(ptr+k)->titolo[strcspn((ptr+k)->titolo, "\n")] = '\0';

printf("autore: ");
fgets((ptr+k)->autore, sizeof((ptr+k)->autore), stdin);
(ptr+k)->autore[strcspn((ptr+k)->autore, "\n")] = '\0';

printf("anno di pubblicazione: ");
scanf("%d",&(ptr+k)->anno_pubblicazione);
while((ptr+k)->anno_pubblicazione < 1800 || (ptr+k)->anno_pubblicazione > 2025){
    printf("anno non valido, inserire un anno tra il 1800 e il 2025: ");
    scanf("%d",&((ptr+k)->anno_pubblicazione));
}
while ((c = getchar()) != '\n'); // pulire stdin

printf("numero copie: ");
scanf("%d",&((ptr+k)->numero_copie));
while((ptr+k)->numero_copie < 0){
    printf("numero copie non valido, inserire un numero maggiore di 0: ");
    scanf("%d",&((ptr+k)->numero_copie));
}
while ((c = getchar()) != '\n'); // pulire stdin

printf("genere: ");
fgets((ptr+k)->genere, sizeof((ptr+k)->genere), stdin);
(ptr+k)->genere[strcspn((ptr+k)->genere, "\n")] = '\0';

*n += 1;

}