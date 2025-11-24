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
    char genere[31]; // (Possibili campi: Classico; Fantasy; Giallo; Fantascienza; Saggio; Romanzo)
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
    char codice_ISBN_libro[18];
    int codice_utente;
    char data_prestito[11];     // (formato gg//mm//aaaa)
    char data_restituzione[11]; // 30 giorni dopo il prestito
    int restituito;             // restituito=1; mancante=0
} prestito;

void inserisci_libro(libro *ptr,int cap_libri, int *n);
void cerca_libro_ISBN(libro *ptr, int n);
void stampa_lista_libri(libro *ptr,int n);
void cerca_libro_autore(libro *ptr, int n);
char* stringa_maiuscolo(char *str);
void libri_disponibili_prestito(libro *ptr,int n);
// 6 - inserisci nuovo utente
void inserisci_utente(utente *ptr, int *ptr_num_utenti, int *ptr_capacità);
void visualizza_utenti(utente *ptr, int numero_utenti);
void cerca_utente(utente *ptr, int numero_utenti);
int input_ISBN(char *ptr_ISBN, libro *ptr_libri, int *ind_libro, int conta_libri);
int is_ISBN(char ISBN[]);
int esiste_copia(char ISBN[], libro *ptr_libri, int *ind_libro, int conta_libri);
int input_utente(int *ptr_codUtente, utente *ptr_utenti, int *ind_utente, int conta_utenti);
void input_data(char *ptr_data);
int is_data(char *ptr_data);
void calcoladata(char *data);
void registra_prestito(libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int *conta_prestiti, int capprestiti);
void registra_restituzione(prestito *ptr_prestiti, int conta_prestiti, libro *ptr_libri, int conta_libri);
void visualizza_prestiti_attivi(libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int conta_prestiti);
void visualizza_storico_utente(libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int conta_prestiti);
void statistiche_generali(int numero_libri, int numero_utenti, int numero_prestiti, prestito *ptr_prestiti, libro *ptr_libri);
libro *libro_più_prestato(int numero_libri, int numero_prestiti, prestito *ptr_prestiti, libro *ptr_libri);
char *stringa_maiuscolo(char *str);
void conteggio_libri(libro *ptr_libri, int numero_libri);
int n_volte_libro_prestato(libro *ptr_libro, prestito *ptr_prestiti, int numero_prestiti);
int indice_max(int *ptr, int numero_elementi);
void libri_più_prestati(libro *ptr_libri, int numero_libri, prestito *ptr_prestiti, int numero_prestiti);

int main()
{
    int scelta;
    int *ctr_libri;
    *ctr_libri = 0;

    // Apertura file binario per i libri in lettura scrittura
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
    int conta_utenti = 0;

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
            inserisci_libro(ptr_libri,caplibri,ctr_libri);
            break;

        case 2:
            printf("\n--- Visualizza tutti i libri ---\n");
            
            break;

        case 3:
            printf("\n--- Cerca libro per ISBN ---\n");
            cerca_libro_ISBN(ptr_libri,*ctr_libri);
            break;

        case 4:
            printf("\n--- Cerca libri per autore ---\n");
            cerca_libro_autore(ptr_libri,*ctr_libri);
            break;

        case 5:
            printf("\n--- Libri disponibili per prestito ---\n");
            libri_disponibili_prestito(ptr_libri,*ctr_libri);
            break;

        case 6:
            inserisci_utente(ptr_utenti, &conta_utenti, &caputenti);
            break;

        case 7:
            printf("\n--- Visualizza tutti gli utenti ---\n");
            visualizza_utenti(ptr_utenti, conta_utenti);
            break;

        case 8:
            printf("\n--- Cerca utente per codice ---\n");
            cerca_utente(ptr_utenti, conta_utenti); 
            break;

        case 9:
            printf("\n--- Registra nuovo prestito ---\n");
            registra_prestito(ptr_libri, *ctr_libri, ptr_utenti, conta_utenti, ptr_prestiti, &conta_prestiti, capprestiti);
            break;

        case 10:
            printf("\n--- Registra restituzione libro ---\n");
            registra_restituzione(ptr_prestiti, conta_prestiti, ptr_libri, *ctr_libri);
            break;

        case 11:
            printf("\n--- Visualizza prestiti attivi ---\n");
            visualizza_prestiti_attivi(ptr_libri, *ctr_libri, ptr_utenti, conta_utenti, ptr_prestiti, conta_prestiti);
            break;

        case 12:
            printf("\n--- Visualizza storico prestiti utente ---\n");
            visualizza_storico_utente(ptr_libri, *ctr_libri, ptr_utenti, conta_utenti, ptr_prestiti, conta_prestiti);
            break;

        case 13:
            printf("\n--- Statistiche generali ---\n");
            statistiche_generali(*ctr_libri, conta_utenti, conta_prestiti, ptr_prestiti, ptr_libri);
            break;

        case 14:
            printf("\n--- Libri per genere ---\n");
            conteggio_libri(ptr_libri, *ctr_libri);
            break;

        case 15:
            printf("\n--- Top 5 libri piu' prestati ---\n");
            libri_più_prestati(ptr_libri, *ctr_libri, ptr_prestiti, conta_prestiti);
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

// 1 inserisci nuovo libro

void inserisci_libro(libro *ptr,int cap_libri, int *n)
{
    int c;
    int k = *n;
    int j; //indice scelta switch

    if(k >= cap_libri){  //controllo se posso inserier un altro libro
        puts("spazio insufficiente, impossibile inserire un nuovo libro");
        return;
    }

    puts("inserisci i seguenti dati del libro");

    printf("codice ISBN(XXX-X-XXXX-XXXX-X): ");         //inserimento informazioni libro
    scanf("%s", (ptr + k)->codice_ISBN);
    while ((c = getchar()) != '\n'); // pulire stdin
    for (int i = 0; i < k; i++)
    {
        if (strcmp((ptr + k)->codice_ISBN, (ptr + i)->codice_ISBN) == 0)
        { // controllo unicità ISBN
            puts("l ISBN è uguale ad un libro gia esistente,inseriscine un altro");
            scanf("%s", (ptr + k)->codice_ISBN);
             while ((c = getchar()) != '\n');
        }
    }

    printf("Titolo: ");
    fgets((ptr + k)->titolo, sizeof((ptr + k)->titolo), stdin);
    (ptr + k)->titolo[strcspn((ptr + k)->titolo, "\n")] = '\0';

    printf("autore: ");
    fgets((ptr + k)->autore, sizeof((ptr + k)->autore), stdin);
    (ptr + k)->autore[strcspn((ptr + k)->autore, "\n")] = '\0';

    printf("anno di pubblicazione: ");
    scanf("%d", &(ptr + k)->anno_pubblicazione);
    while ((ptr + k)->anno_pubblicazione < 1800 || (ptr + k)->anno_pubblicazione > 2025)
    {
        printf("anno non valido, inserire un anno tra il 1800 e il 2025: ");
        scanf("%d", &((ptr + k)->anno_pubblicazione));
    }
    while ((c = getchar()) != '\n'); // pulire stdin

    printf("numero copie: ");
    scanf("%d", &((ptr + k)->numero_copie));
    while ((ptr + k)->numero_copie < 0)
    {
        printf("numero copie non valido, inserire un numero maggiore di 0: ");
        scanf("%d", &((ptr + k)->numero_copie));
    }
    while ((c = getchar()) != '\n')
        ; // pulire stdin

    do{
         printf("Scegli uno tra i seguenti generi:\n1 Fantasy\n2 Classico\n3 Saggi\n4 Fantascienza\n5 Giallo\n6 Romanzo\n");
        scanf("%d",&j);
        switch (j)
        {
        case 1:
            strcpy((ptr + k)->genere,"FANTASY");
            break;
        case 2:
            strcpy((ptr + k)->genere,"CLASSICO");
            break;
        case 3:
            strcpy((ptr + k)->genere,"SAGGI");
            break;
        case 4:
            strcpy((ptr + k)->genere,"FANTASCIENZA");
            break;
        case 5:
            strcpy((ptr + k)->genere,"GIALLO");
            break;
        case 6:
           strcpy((ptr + k)->genere,"ROMANZO");
           break;
        default:
           puts("numero inserito non valido");
           break;
        }
    } while(j > 6 || j < 1);
    *n += 1;        //incremento il counter dei libri salvati nel puntatore
}


void cerca_libro_ISBN(libro *ptr, int n)
{
    char temp[18];

    printf("inserisci l ISBN da cercare(XXX-X-XXXX-XXXX-X): ");
    scanf("%s", temp);
    for (int i = 0; i < n; i++)
    {
        if (strcmp(temp, (ptr + i)->codice_ISBN) == 0)  //confronto tra il codice inserito e la lista dei libri
        { 
            printf("Titolo: %s\nAutore: %s\nAnno di pubblicazione: %d\nNumero copie: %d\nGenere: %s", (ptr + i)->titolo, (ptr + i)->autore, (ptr + i)->anno_pubblicazione, (ptr + i)->numero_copie, (ptr + i)->genere);
            return;
        }
    }
    printf("Nessun libro trovato");
}

void stampa_lista_libri(libro *ptr,int n){

    int len_autore = 0;
    int len_titolo = 0;

    for(int i = 0;i < n;i++){       //trovo la massima lunghezza delle stringhe per formattare la tabella 
        int temp0 = strlen((ptr+i)->autore);
        int temp1 = strlen((ptr+i)->titolo);
        if (temp0 > len_autore){
            len_autore = temp0;
        }
        if(temp1 > len_titolo){
            len_titolo = temp1;
        }
    }//segmentatio fault
    printf("%-15s | %-*s | %-*s | %-21s | %-13s | %-12s\n","Codice ISBN",len_autore,"Autore",len_titolo,"Titolo","Anno di pubblicazione","Genere","Numero copie");
    for (int j = 0; j < n;j++){
        printf("%-15s | %-*s | %-*s | %-17d | %-13s | %-12d\n",(ptr+j)->codice_ISBN,len_autore,(ptr+j)->autore,len_titolo,(ptr+j)->titolo,(ptr+j)->anno_pubblicazione,(ptr+j)->genere,(ptr+j)->numero_copie);
    }
}

void cerca_libro_autore(libro *ptr,int n){
    char str_temp0[51];      //immagazzino la stringa da ricercare
    char str_temp1[51];      //var temp per non modificare i nomi nel puntatore
    puts("inserisci il nome dell autore da ricercare");
    scanf("%s",str_temp0);
    printf("i libri di %s sono: ",str_temp0);
    for(int i = 0;i < n;i++){
        strcpy(str_temp1, ptr->autore);           // copia autore in temp
        stringa_maiuscolo(str_temp1);             // converto la copia in maiuscolo
        if(strcmp(str_temp1, str_temp0) == 0){    //confronto le due stringhe per tutti i libri
            printf("%s\n",(ptr+i)->titolo);
        }
    }
}

void libri_disponibili_prestito(libro *ptr,int n){
    libro *ptr_temp[n-1];  //creo un array di puntatori per non ordinare il puntatore originale
    char* temp;        //variabile d appoggio per bubble sort

    for(int i = 0;i < n;i++){
        ptr_temp[i] = (ptr+i);
    }

    for(int i = 0;i < n-1;i++){         //bubble sort(strcmp resituisce > 0 se str1 è dopo str2)
        for(int j = 0;j < n - i - 1;j++){
            if(strcmp(ptr_temp[i]->titolo,ptr_temp[i+1]->titolo) > 0){
                temp = ptr_temp[j];     //scambio gli indirizzi
                ptr_temp[i] = ptr_temp[i+1];
                ptr_temp[i+1] = temp;
            }
        }
    }

    puts("libri disponibili per il prestito");
    for(int k = 0;k < n;k++){       //stampo solo libri disponibili
        if((ptr+k)->numero_copie > 0){
            printf("%s\n",ptr_temp[k]->titolo);
        }
    }
}

// Sezione B

// 6 - Inserisci nuovo utente
void inserisci_utente(utente *ptr, int *ptr_num_utenti, int *ptr_capacità)
{
     if (*ptr_num_utenti >= *ptr_capacità)
    {
        printf("Errore: impossibile registrare nuovo utente! Capacità massima raggiunta!\n");
        return;
    }
    int new_codice_utente;
    char new_nome[51];
    char new_cognome[51];
    char new_email[81];
    char new_data_iscrizione[11];

    // inserisco controlli sulla lunghezza massima dell'input tramite scanf
    printf("\n--- Inserisci nuovo utente ---\n");
    printf("Inserire codice utente:");
    scanf("%d", &new_codice_utente);
    printf("Inserire nome:");
    scanf("%50s", new_nome);
    printf("inserisci cognome");
    scanf("%50s", new_cognome);
    printf("inserisci email");
    scanf("%80s", new_email);
    printf("Inserire data di iscrizione (gg/mm/aaaa):");
    scanf("%10s", new_data_iscrizione);

    // controllo che il codice utente sia univoco

    int codice_valido = 0; // 0 se codice va verificato e 1 se codice supera il controllo

    while (!codice_valido)
    {

        codice_valido = 1; // assume che il codice utente sia valido

        for (int j = 0; j < *ptr_num_utenti; j++)
        {
            if (ptr[j].codice_utente == new_codice_utente)
            {
                printf("Il codice utente inserito è già stato utilizzato!\nInserirne uno diverso:\n");
                scanf("%d", &new_codice_utente); // modifica il valore del codice utente
                codice_valido = 0; // modifico valore perchè devo rientrare nel ciclo al fine di verificare che il nuovo codice appena inserito sia valido
                break; // esce solo dal for
            }
        }
    }

    // se supera il controllo, allora posso registrare il nuovo titolo

    ptr[*ptr_num_utenti].codice_utente = new_codice_utente;
    strcpy(ptr[*ptr_num_utenti].nome, new_nome);
    strcpy(ptr[*ptr_num_utenti].cognome, new_cognome);
    strcpy(ptr[*ptr_num_utenti].email, new_email);
    strcpy(ptr[*ptr_num_utenti].data_iscrizione, new_data_iscrizione);

    *ptr_num_utenti += 1; // aggiorno contatore utenti per la chiamata successiva

    printf("Registrazione del nuovo utente avvenuta con successo!\n");
}

// 7 - Visualizza utenti

void visualizza_utenti(utente *ptr, int numero_utenti)
{
    if (numero_utenti == 0)
    {
        printf("Non ci sono utenti registrati!\n");
        return;
    }
    else
    {
        printf("%-15s %-50s %-50s %-80s %-20s\n", "Codice utente", "Nome", "Cognome", "Email", "Data di iscrizione");
        for (int i = 0; i < numero_utenti; i++)
        {
            printf("%d %-50s %-50s %-80s %-10s\n", ptr[i].codice_utente, ptr[i].nome, ptr[i].cognome, ptr[i].email, ptr[i].data_iscrizione);
            printf("\n");
        }
    }
}

// 8 - Ricerca tramite codice utente

void cerca_utente(utente *ptr, int numero_utenti)
{
    int codice_ricerca;
    int trovato = 0; // se vi è corrispondenza con un codice utente registrato trovato=1; altrimento trovato=0
    printf("Inserire il codice utente da cercare:");
    scanf("%d", &codice_ricerca);

    for (int i = 0; i < numero_utenti; i++)
    {
        if (ptr[i].codice_utente == codice_ricerca)
        {
            printf("%-15s %-50s %-50s %-80s %-20s\n", "Codice utente", "Nome", "Cognome", "Email", "Data di iscrizione");
            printf("%d %-50s %-50s %-80s %-10s\n", ptr[i].codice_utente, ptr[i].nome, ptr[i].cognome, ptr[i].email, ptr[i].data_iscrizione);
            trovato = 1; // confermo la corrispondenza, dunque aggiorno valore trovato
            break;
        }
    }

    if (trovato == 0)
    {
        printf("Il codice utente %d non è associato ad alcun utente registrato!\n", codice_ricerca);
    }
}

// FINE SEZIONE B

// SEZIONE C

// SCELTA 9

void registra_prestito(libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int *conta_prestiti, int capprestiti)
{
    // controllo capacità
    if (*conta_prestiti >= capprestiti)
    {
        printf("\nDatabase prestiti pieno, impossibile aggiungere altri prestiti. (%d/%d prestiti)\n", *conta_prestiti, capprestiti);
        return;
    }

    char ISBN[18], data[11];
    int utente;

    // indici libro e utente
    int ind_libro, ind_utente;

    if (!input_ISBN(ISBN, ptr_libri, &ind_libro, conta_libri))
    {
        return;
    }

    if (!input_utente(&utente, ptr_utenti, &ind_utente, conta_utenti))
    {
        return;
    }

    // ho trovato libro e utente

    // decremento numero copie
    (ptr_libri[ind_libro].numero_copie)--;

    // input data prestito
    input_data(data);

    // inizializzo prestito
    ptr_prestiti[*conta_prestiti].codice_prestito = (*conta_prestiti);                         // inserisco codice prestito
    strcpy(ptr_prestiti[*conta_prestiti].codice_ISBN_libro, ptr_libri[ind_libro].codice_ISBN); // inserisco ISBN
    ptr_prestiti[*conta_prestiti].codice_utente = ptr_utenti[ind_utente].codice_utente;        // inserisco codice utente
    strcpy(ptr_prestiti[*conta_prestiti].data_prestito, data);                                 // inserisco data prestito
    calcoladata(data);                                                                         // calcolo data restituzione
    strcpy(ptr_prestiti[*conta_prestiti].data_restituzione, data);                             // inserisco data restituzione
    ptr_prestiti[*conta_prestiti].restituito = 0;                                              // imposto restituito=0

    // stampo feedback
    printf("\n===============================================\n");
    printf("           PRESTITO REGISTRATO\n");
    printf("===============================================\n");
    printf("  Codice:      %d\n", *conta_prestiti);
    printf("  Libro:       %s\n", ptr_libri[ind_libro].titolo);
    printf("  ISBN:        %s\n", ISBN);
    printf("  Utente:      %d\n", utente);
    printf("  Prestito:    %s\n", ptr_prestiti[*conta_prestiti].data_prestito);
    printf("  Restituzione:%s\n", ptr_prestiti[*conta_prestiti].data_restituzione);
    printf("  Copie rim.:  %d\n", ptr_libri[ind_libro].numero_copie);
    printf("===============================================\n");

    // incremento contaprestiti
    (*conta_prestiti)++;
}

// inizio input_ISBN
int input_ISBN(char *ptr_ISBN, libro *ptr_libri, int *ind_libro, int conta_libri)
{
    printf("\nInserire codice ISBN: ");
    scanf("%17s", ptr_ISBN);

    // pulizia buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // valido input
    while (!is_ISBN(ptr_ISBN)) // finché non è valido ... continuo a chiedere input
    {
        printf("\nISBN non valido! Inserirlo nel formato 'XXX-X-XXXX-XXXX-X':\n");
        scanf("%17s", ptr_ISBN);

        // pulizia buffer
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }

    // ISBN valido

    // controllo esistenza copie e ricavo indice
    return esiste_copia(ptr_ISBN, ptr_libri, ind_libro, conta_libri);
}

int is_ISBN(char *ISBN)
{
    // verifico lunghezza
    if (strlen(ISBN) != 17)
    {
        return 0;
    }

    // Pattern: XXX-X-XXXX-XXXX-X
    int posizioni_cifre[] = {0, 1, 2, 4, 6, 7, 8, 9, 11, 12, 13, 14, 16};
    int posizioni_trattini[] = {3, 5, 10, 15};

    // Verifica che le posizioni siano cifre
    for (int i = 0; i < 13; i++)
    {
        if (!isdigit(ISBN[posizioni_cifre[i]]))
        {
            return 0;
        }
    }

    // Verifica che le posizioni siano trattini
    for (int i = 0; i < 4; i++)
    {
        if (ISBN[posizioni_trattini[i]] != '-')
        {
            return 0;
        }
    }

    return 1;
}

int esiste_copia(char *ptr_ISBN, libro *ptr_libri, int *ind_libro, int conta_libri)
{
    // scorro libri
    for (int i = 0; i < conta_libri; i++)
    {
        if (!strcmp(ptr_libri[i].codice_ISBN, ptr_ISBN)) // se trovo ISBN...
        {
            if (ptr_libri[i].numero_copie > 0) // se ci sono copie...
            {
                *ind_libro = i; // salvo indice
                return 1;       // esiste copia
            }
            else // se NON ci sono copie...
            {
                printf("\nNon ci sono copie disponibili!\n");
                return 0; // NON ci sono copie
            }
        }
    }

    printf("\nL'ISBN non e' registrato");
    return 0; // non ho trovato ISBN
}
// fine input_ISBN

// inizio input_utente
int input_utente(int *ptr_codUtente, utente *ptr_utenti, int *ind_utente, int conta_utenti)
{
    printf("\nInserire codice utente: ");

    // controllo scanf
    if (scanf("%d", ptr_codUtente) != 1)
    {
        printf("Input non valido! Inserire un numero.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        return 0;
    }

    // pulizia buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // cerco utente
    for (int i = 0; i < conta_utenti; i++) // scorro utenti
    {
        if (ptr_utenti[i].codice_utente == *ptr_codUtente) // se trovo codice ...
        {
            // ho trovato utente
            *ind_utente = i;               // salvo indice
            printf("\nUtente trovato!\n"); // stampo feedback
            return 1;
        }
    }

    // NON ho trovato utente
    printf("\nL'utente non esiste!\n"); // stampo feedback
    return 0;
}
// fine input_utente

// inizio input_data
void input_data(char *ptr_data)
{
    printf("\nInserire data del prestito: ");
    scanf("%10s", ptr_data);

    // pulizia buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // valido input
    while (!is_data(ptr_data)) // finché non è valido ... continuo a chiedere input
    {
        printf("\nData non valida! Inserirla nel formato 'gg/mm/aaaa':\n");
        scanf("%10s", ptr_data);

        // pulizia buffer
        while ((c = getchar()) != '\n' && c != EOF)
            ;
    }

    // data valida
}

int is_data(char *data)
{
    // Verifica lunghezza
    if (strlen(data) != 10)
        return 0;

    // Pattern: gg/mm/aaaa
    int posizioni_cifre[] = {0, 1, 3, 4, 6, 7, 8, 9};

    // Verifica che le posizioni siano cifre
    for (int i = 0; i < 8; i++)
    {
        if (!isdigit(data[posizioni_cifre[i]]))
        {
            return 0;
        }
    }

    // Verifica che le posizioni siano slash
    if (data[2] != '/' || data[5] != '/')
    {
        return 0;
    }

    // Estrai giorno, mese, anno
    int giorno = (data[0] - '0') * 10 + (data[1] - '0');
    int mese = (data[3] - '0') * 10 + (data[4] - '0');
    int anno = (data[6] - '0') * 1000 + (data[7] - '0') * 100 +
               (data[8] - '0') * 10 + (data[9] - '0');

    // Validazione logica della data
    if (mese < 1 || mese > 12)
        return 0;
    if (giorno < 1)
        return 0;

    // Giorni per mese
    int giorni_mese[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Aggiusta febbraio per anni bisestili
    if (mese == 2 && ((anno % 4 == 0) && (anno % 100 != 0 || anno % 400 == 0)))
        giorni_mese[1] = 29;

    // Controllo giorno massimo
    if (giorno > giorni_mese[mese - 1])
        return 0;

    return 1;
}
// fine input_data

// inizio calcoladata
void calcoladata(char *data)
{
    // Estrai giorno, mese, anno
    int giorno = (data[0] - '0') * 10 + (data[1] - '0');
    int mese = (data[3] - '0') * 10 + (data[4] - '0');
    int anno = (data[6] - '0') * 1000 + (data[7] - '0') * 100 +
               (data[8] - '0') * 10 + (data[9] - '0');

    // Giorni per mese
    int giorni_mese[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Aggiusta febbraio per anni bisestili
    if (mese == 2 && ((anno % 4 == 0) && (anno % 100 != 0 || anno % 400 == 0)))
        giorni_mese[1] = 29;

    // aggiungo 30 gg
    giorno += 30;

    // aggiusto data
    while (giorno > giorni_mese[mese - 1]) // finché ho troppi giorni per il mese corrente...
    {
        giorno -= giorni_mese[mese - 1]; // gli sottraggo i giorni del mese corrente
        mese++;                          // e passo al mese successivo

        if (mese > 12) // se poi sforo i mesi dell'anno ...
        {
            mese = 1; // torno a gennaio
            anno++;   // e passo all'anno successivo
        }

        // ricalcolo i giorni di febbraio a seconda che l'anno corrente sia bisestile oppure no
        giorni_mese[1] = 28;
        if (mese == 2 && (anno % 4 == 0) && (anno % 100 != 0 || anno % 400 == 0))
        {
            giorni_mese[1] = 29;
        }
    }

    // sostituisco stringa data
    char output[11];
    sprintf(output, "%02d/%02d/%04d", giorno, mese, anno);
    strcpy(data, output);
}
// fine calcoladata

// FINE SCELTA 9

// SCELTA 10

void registra_restituzione(prestito *ptr_prestiti, int conta_prestiti, libro *ptr_libri, int conta_libri)
{
    // input codice
    int cod;
    printf("Inserire codice prestito: ");
    scanf("%d", &cod);

    // pulizia buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // verifico esistenza
    if (0 <= cod && cod < conta_prestiti) // se prestito esiste ...
    {
        if (ptr_prestiti[cod].restituito) // se già restituito ...
        {
            printf("\nIl prestito e' gia' stato restituito!\n");
        }
        else // non ancora restituito ...
        {
            // cerco ISBN corrispondente
            int flag = 0;
            for (int i = 0; i < conta_libri; i++) // scorro componenti occupate
            {
                if (!strcmp(ptr_prestiti[cod].codice_ISBN_libro, ptr_libri[i].codice_ISBN)) // se l'ho trovato ...
                {
                    (ptr_libri[i].numero_copie)++;    // incremento numero copie
                    ptr_prestiti[cod].restituito = 1; // segno restituito

                    flag = 1; // ho trovato il libro
                    break;    // e smetto di scorrere
                }
            }

            // controllo riuscita della ricerca
            if (flag) // se riuscito ...
            {
                printf("\nRestituzione effettuata con successo!\n");
            }
            else // se NON riuscito ...
            {
                printf("\nErrore! Non c'e' un ISBN corrispondente a quello del prestito.\n");
            }
        }
    }
    else // se il prestito non esiste ...
    {
        printf("\nIl prestito non esiste!\n");
    }
}

// FINE SCELTA 10

// SCELTA 11

// prototipi
void salva_titolo(char *ptr_ISBN, libro *ptr_libri, int conta_libri, char *titolo);
void salva_nomeUtente(int cod_utente, utente *ptr_utenti, int conta_utenti, char *nome_utente);

void visualizza_prestiti_attivi(libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int conta_prestiti)
{
    char titolo[101], nome_utente[102]; // nome(50) , spazio , cognome(50) , terminatore
    int trovati = 0;

    printf("\n");
    for (int i = 0; i < conta_prestiti; i++) // scorro prestiti
    {
        if (!(ptr_prestiti[i].restituito)) // se restituito=0 ...
        {
            // salvo titolo e nome_utente corrispondenti
            salva_titolo(ptr_prestiti[i].codice_ISBN_libro, ptr_libri, conta_libri, titolo);
            salva_nomeUtente(ptr_prestiti[i].codice_utente, ptr_utenti, conta_utenti, nome_utente);

            // Stampa formattata
            printf("───────────────────────────────────────────────────────────\n");
            printf("CODICE PRESTITO: %d\n", ptr_prestiti[i].codice_prestito);
            printf("LIBRO:           %s\n", titolo);
            printf("UTENTE:          %s\n", nome_utente);
            printf("DATA PRESTITO:   %s\n", ptr_prestiti[i].data_prestito);
            printf("RESTITUZIONE:    %s\n", ptr_prestiti[i].data_restituzione);
            printf("───────────────────────────────────────────────────────────\n\n");

            trovati++; // incremento trovati
        }
    }

    if (!trovati)
    {
        printf("Nessun prestito attivo trovato.\n");
    }
}

void salva_titolo(char *ptr_ISBN, libro *ptr_libri, int conta_libri, char *titolo)
{
    strcpy(titolo, "Libro non trovato!"); // salvo messaggio di errore

    // cerco libro tramite ISBN
    for (int i = 0; i < conta_libri; i++) // scorro componenti occupate
    {
        if (!strcmp(ptr_ISBN, ptr_libri[i].codice_ISBN)) // se l'ho trovato ...
        {
            strcpy(titolo, ptr_libri[i].titolo); // salvo
            return;
        }
    }
}

void salva_nomeUtente(int cod_utente, utente *ptr_utenti, int conta_utenti, char *nome_utente)
{
    strcpy(nome_utente, "Utente non trovato!"); // salvo messaggio di errore

    // cerco utente tramite codice utente
    for (int i = 0; i < conta_utenti; i++) // scorro componenti occupate
    {
        if (cod_utente == ptr_utenti[i].codice_utente) // se l'ho trovato ...
        {
            sprintf(nome_utente, "%s %s", ptr_utenti[i].nome, ptr_utenti[i].cognome); // salvo
            return;
        }
    }
}

// FINE SCELTA 11

// SCELTA 12

// prototipi
int esiste_utente(int cod, utente *ptr_utenti, int conta_utenti);
int stampa_storico_utente(int cod, libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int conta_prestiti);

void visualizza_storico_utente(libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int conta_prestiti)
{
    // chiedo codice utente
    int cod;
    printf("\nInserire codice utente: ");
    scanf("%d", &cod);

    // pulizia buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    if (!esiste_utente(cod, ptr_utenti, conta_utenti)) // se NON ho trovato l'utente ...
    {
        printf("\nL'utente non esiste!\n");
        return;
    }

    // ho trovato l'utente
    printf("\nUtente trovato con successo!\n");

    if (!stampa_storico_utente(cod, ptr_libri, conta_libri, ptr_utenti, conta_utenti, ptr_prestiti, conta_prestiti))
    {
        printf("\nL'utente non ha prestiti registrati a suo nome!\n");
    }
}

int esiste_utente(int cod, utente *ptr_utenti, int conta_utenti)
{
    // cerco utente tramite codice utente (ind attualmente)
    for (int i = 0; i < conta_utenti; i++)
    {
        if (cod == ptr_utenti[i].codice_utente) // se l'ho trovato ...
        {
            return 1; // ho trovato l'utente
        }
    }
    return 0; // NON ho trovato l'utente
}

int stampa_storico_utente(int cod, libro *ptr_libri, int conta_libri, utente *ptr_utenti, int conta_utenti, prestito *ptr_prestiti, int conta_prestiti)
{
    int trovati = 0;
    char titolo[101], nome_utente[102]; // nome(50) , spazio , cognome(50) , terminatore

    printf("\n");
    for (int i = 0; i < conta_prestiti; i++) // scorro prestiti
    {
        if (cod == ptr_prestiti[i].codice_utente) // se il prestito è dell'utente ...
        {
            // salvo titolo e nome_utente corrispondenti
            salva_titolo(ptr_prestiti[i].codice_ISBN_libro, ptr_libri, conta_libri, titolo);
            salva_nomeUtente(ptr_prestiti[i].codice_utente, ptr_utenti, conta_utenti, nome_utente);

            // Stampa formattata in stile Scelta 11
            printf("───────────────────────────────────────────────────────────\n");
            printf("CODICE PRESTITO: %d\n", ptr_prestiti[i].codice_prestito);
            printf("LIBRO:           %s\n", titolo);
            printf("UTENTE:          %s\n", nome_utente);
            printf("DATA PRESTITO:   %s\n", ptr_prestiti[i].data_prestito);
            printf("RESTITUZIONE:    %s\n", ptr_prestiti[i].data_restituzione);

            // Indicatore stato prestito (richiesto dalle specifiche)
            if (ptr_prestiti[i].restituito)
            {
                printf("STATO:          RESTITUITO\n");
            }
            else
            {
                printf("STATO:          IN PRESTITO\n");
            }
            printf("───────────────────────────────────────────────────────────\n\n");

            trovati++; // incremento trovati
        }
    }

    return trovati;
}

// FINE SCELTA 12

// FINE SEZIONE C

// SEZIONE D

// 13 - STATISTICHE GENERALI

// funzione che mi serve per identificare libro più cercato in accordo con lo storico prestisti

libro *libro_più_prestato(int numero_libri, int numero_prestiti, prestito *ptr_prestiti, libro *ptr_libri)
{
    if (numero_libri == 0 || numero_prestiti == 0)
    {
        return NULL; // gestisco i casi in cui gli array sono vuoti
    }
    int indice_libro;          // variabile che traccia indice del libro più prestato
    int numero_volte_prestato; // variabile dove registro il numero di prestiti del libro più prestato
    int temp;                  // variabile temporanea per memorizzare quante volte libro della corrente iterazione è stato prestato

    indice_libro = 0;          // inzializzo indice del + prestato al primo elemento di libri
    numero_volte_prestato = 0; // inizializzo numero di prestiti del + prestato a zero

    for (int i = 0; i < numero_libri; i++)
    {
        temp = 0; // ad ogni iterata riazzero la variabile temporanea

        for (int j = 0; j < numero_prestiti; j++)
        {
            if (strcmp(ptr_libri[i].codice_ISBN, ptr_prestiti[j].codice_ISBN_libro) == 0)
            {
                // se vi è corrispondenza tra libro i-esimo e il libro del prestito j-esimo
                temp += 1;
            }
        }

        if (temp > numero_volte_prestato)
        {
            numero_volte_prestato = temp;
            indice_libro = i;
        }
    }

    return &ptr_libri[indice_libro];
}

void statistiche_generali(int numero_libri, int numero_utenti, int numero_prestiti, prestito *ptr_prestiti, libro *ptr_libri)
{
    printf("Il numero totale di libri nel catalogo è pari a: %d\n", numero_libri);

    int numero_copie = 0; // inizializzo numero di copie a zero

    int prestiti_attivi = 0; // variabile per memorizzare numero prestiti attivi

    for (int i = 0; i < numero_libri; i++)
    {
        numero_copie += ptr_libri[i].numero_copie; // per ogni libro aggiungo alla somma il numero delle copie associato
    }
    printf("Il numero totale di copie disponibili è pari a: %d\n", numero_copie);

    printf("Il numero totale di utenti registrati è pari a: %d\n", numero_utenti);

    printf("Il numero totale di prestiti effettuati è pari a: %d\n", numero_prestiti);

    for (int j = 0; j < numero_prestiti; j++)
    {
        if (ptr_prestiti[j].restituito == 0) // se il libro del j-esimo prestito non è ancora stato restituito allora incremento il contatore
        {
            prestiti_attivi += 1;
        }
    }

    printf("Il numero di prestiti attivi è pari a: %d\n", prestiti_attivi);

    libro *libro_prestato = libro_più_prestato(numero_libri, numero_prestiti, ptr_prestiti, ptr_libri);

    if (libro_prestato == NULL)
    {
        printf("Non vi sono libri prestati nel sistema!\n");
    }
    else
    {
        printf("Il libro più prestato è: %s di %s\n", libro_prestato->titolo, libro_prestato->autore);
    }
}

// 14 - Libri per genere

char *stringa_maiuscolo(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = toupper(str[i]);
    }

    return str;
}

void conteggio_libri(libro *ptr_libri, int numero_libri)
{
    int contatori[6] = {0}; // 1) classico 2) fantasy 3) giallo 4) fantascienza 5) romanzo 6) saggio // inizializzato a zero ogni elemento
    char *generi[] = {"CLASSICO", "FANTASY", "GIALLO", "FANTASCIENZA", "ROMANZO", "SAGGIO"};

    if (numero_libri == 0)
    {
        printf("Non vi sono libri registrati!\n");
        return;
    }

    // primo ciclo per scorrere sui libri presenti
    for (int i = 0; i < numero_libri; i++)
    {
        // secondo ciclo per confrontare genere libro corrente con i generi ammissibili
        for (int j = 0; j < 6; j++)
        {
            if (strcmp(ptr_libri[i].genere, generi[j]) == 0)
            {
                contatori[j] += 1; // aggiorno il contatore del j-genere dato che c'è corrispondenza tra vettore contatori e generi
                break;             // una volta trovato il genere posso uscire dal ciclo più interno (assumo che ogni libro appartenga ad un solo genere)
            }
        }
    }

    // stampo i risultati
    for (int i = 0; i < 6; i++)
    {
        printf("Numero di libri del genere %s: %d\n", generi[i], contatori[i]);
        printf("\n");
    }
}

// 15 - Visualizza 5 libri più prestati

int n_volte_libro_prestato(libro *ptr_libro, prestito *ptr_prestiti, int numero_prestiti)
{
    if (numero_prestiti == 0)
    {
        printf("Non sono stati effettuati prestiti!\n");
        return 0;
    }

    int volte_prestito = 0; // variabile che memorizza numero volte che il libro in input è stato prestato

    for (int i = 0; i < numero_prestiti; i++)
    {
        if (strcmp(ptr_libro->codice_ISBN, ptr_prestiti[i].codice_ISBN_libro) == 0)
        {
            volte_prestito++; // incremento il numero di prestiti di 1 se vi è corrispondenza tra gli ISBN
        }
    }

    return volte_prestito;
}

int indice_max(int *ptr, int numero_elementi)
// funzioni che dato un vettore di interi restituisce l'indice all'interno del vettore dell'elemento di valore maggiore
{
    int pos = 0;      // indice dell'elemento massimo che inizializzo a zero
    int max = ptr[0]; // variabile dove memorizzo valore massimo; la inizializzo al primo elemento

    for (int i = 1; i < numero_elementi; i++)
    {
        if (ptr[i] > max)
        {
            max = ptr[i];
            pos = i; // registra posizione del corrente valore massimo
        }
    }

    return pos;
}

void libri_più_prestati(libro *ptr_libri, int numero_libri, prestito *ptr_prestiti, int numero_prestiti)
{

    if (numero_prestiti == 0 || numero_libri == 0)
    {
        printf("Non vi sono libri registrati o prestiti effettuati!\n");
        return;
    }

    int *num_prestiti_per_libro = (int *)malloc(numero_libri * sizeof(int)); // array in cui all'i-esimo posto corrisponde il numero di voltre che libri[i] è stato prestato in accordo con lo storico

    if (num_prestiti_per_libro == NULL)
    {
        printf("Errore allocazione memoria!\n");
        return;
    }

    // ciclo per memorizzare numero volte che ciascun lbro è stato prestato
    for (int i = 0; i < numero_libri; i++)
    {
        num_prestiti_per_libro[i] = n_volte_libro_prestato((ptr_libri + i), ptr_prestiti, numero_prestiti); // num_prestiti_per_libro[i] = numero di volte che è stato prestato libri[i]
    }

    // dato che voglio sapere i 5 libri più prestati, ciclo da 1 a 5
    int temp;
    for (int j = 0; j < 5; j++)
    {
        temp = indice_max(num_prestiti_per_libro, numero_libri);

        if (num_prestiti_per_libro[temp] == 0) // gestisco caso in cui ho meno di 5 libri prestati
        {
            // non ha senso continuare a stampare se il numero di volte che è stato prestato il (j+1)-esimo libro più richiesto è pari a zero; dunque mi fermo
            printf("Sono stati prestati solo %d in totale!\n", j);
            break; // esce dal ciclo perchè tanto so che se continuassi con il ciclo otterei solo numeri di prestiti pari a zero
        }
        else
        {
            printf("il %d libro più prestato è: %s di %s\n", j + 1, ptr_libri[temp].titolo, ptr_libri[temp].autore);
            printf("prestato: %d volte\n", num_prestiti_per_libro[temp]);
            printf("posizione: è il %d libro registrato\n", temp + 1);
            num_prestiti_per_libro[temp] = -1; // annullo il valore in modo che alla prossima iterazione indice_max restituisca l'indice dell'elemento più prestato tolti quelli delle prime j iterazioni
        }
    }

    free(num_prestiti_per_libro);
}

// aggiungere chiamate ai case nel main

// FINE SEZIONE D