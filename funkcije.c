#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"

#define MAX_ULOG 5000
#define MAX_DOBICI 5

SimbolDobitak dobitci[] = {
    {'A', 50, 200, 1000},
    {'B', 40, 150, 800},
    {'C', 30, 100, 600},
    {'D', 20, 75, 400},
    {'E', 10, 50, 200}
};

static int zadnjiDobici[MAX_DOBICI] = {0}; // Polje za pohranu zadnjih dobitaka
static int brojDobitaka = 0;

void ispisiIzbornik() {
    printf("\nIzbornik:\n");
    printf("1. Igraj\n");
    printf("2. Dodaj kredite\n");
    printf("3. Promjeni ulog\n");
    printf("4. Prikaži stanje računa\n");
    printf("5. Spremi zadnjih 5 dobitaka u datoteku i sortiraj\n");
    printf("6. Obriši datoteku s dobitcima\n");
    printf("7. Izlaz\n");
    printf("Odaberite opciju: ");
}

// Quick Sort algoritam
void quickSort(int* array, int low, int high) {
    if (low < high) {
        int pivotIndex = partition(array, low, high);
        quickSort(array, low, pivotIndex - 1);
        quickSort(array, pivotIndex + 1, high);
    }
}

int partition(int* array, int low, int high) {
    int pivot = array[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (array[j] > pivot) { // Sortiranje u opadajućem redoslijedu
            i++;
            int temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        }
    }

    int temp = array[i + 1];
    array[i + 1] = array[high];
    array[high] = temp;

    return i + 1;
}

void spremiSortiraneDobitke() {
    if (brojDobitaka > 0) {
        quickSort(zadnjiDobici, 0, brojDobitaka - 1);

        FILE* datoteka = fopen("dobici.txt", "w");
        if (datoteka) {
            for (int i = 0; i < brojDobitaka; i++) {
                fprintf(datoteka, "Dobitak %d: %d\n", i + 1, zadnjiDobici[i]);
            }
            fclose(datoteka);
            printf("Dobici su spremljeni u datoteku i sortirani.\n");
        } else {
            perror("Pogreška pri otvaranju datoteke");
        }
    } else {
        printf("Nema dobitaka za spremiti.\n");
    }
}

void obrisiDatotekuDobitaka() {
    if (remove("dobici.txt") == 0) {
        printf("Datoteka dobitaka je uspješno obrisana.\n");
    } else {
        perror("Pogreška pri brisanju datoteke");
    }
}

void igra(Racun* racun) {
    if (racun->stanjeRacuna < racun->ulog) {
        printf("Nemate dovoljno kredita za igru.\n");
        return;
    }

    static const char* simboli[] = { "A", "B", "C", "D", "E" };
    char matrica[REDCI][STUPCI];
    for (int i = 0; i < REDCI; i++) {
        for (int j = 0; j < STUPCI; j++) {
            matrica[i][j] = simboli[rand() % 5][0];
        }
    }

    printf("\nRezultat igre:\n");
    for (int i = 0; i < REDCI; i++) {
        for (int j = 0; j < STUPCI; j++) {
            printf("%c ", matrica[i][j]);
        }
        printf("\n");
    }

    int dobitak = izracunajDobitak(matrica);
    racun->stanjeRacuna += dobitak - racun->ulog;

    if (dobitak > racun->najveciDobitak) {
        racun->najveciDobitak = dobitak;
    }

    // Dodavanje dobitka u polje zadnjih dobitaka
    if (brojDobitaka < MAX_DOBICI) {
        zadnjiDobici[brojDobitaka++] = dobitak;
    } else {
        for (int i = 1; i < MAX_DOBICI; i++) {
            zadnjiDobici[i - 1] = zadnjiDobici[i];
        }
        zadnjiDobici[MAX_DOBICI - 1] = dobitak;
    }

    printf("Vaš dobitak je: %d\n", dobitak);
    printf("Trenutno stanje računa: %d\n", racun->stanjeRacuna);
}

void dodajKredite(Racun* racun) {
    int iznos;
    char unos[100]; // buffer za unos

    while (1) {
        printf("Unesite iznos za dodavanje kredita: ");
        if (fgets(unos, sizeof(unos), stdin) != NULL) {
            if (sscanf(unos, "%d", &iznos) == 1) {
                if (iznos > 0) {
                    racun->stanjeRacuna += iznos;
                    printf("Novi saldo računa: %d\n", racun->stanjeRacuna);
                    break;
                } else {
                    printf("Iznos mora biti pozitivan.\n");
                }
            } else {
                printf("Neispravan unos, molimo unesite broj.\n");
            }
        } else {
            printf("Greška pri čitanju unosa.\n");
        }
    }
}

void promjeniUlog(Racun* racun) {
    int noviUlog;
    char unos[100]; // buffer za unos

    while (1) {
        printf("Unesite novi ulog: ");
        if (fgets(unos, sizeof(unos), stdin) != NULL) {
            if (sscanf(unos, "%d", &noviUlog) == 1) {
                if (noviUlog >= MIN_ULOG && noviUlog <= MAX_ULOG) {
                    racun->ulog = noviUlog;
                    printf("Novi ulog postavljen na: %d\n", racun->ulog);
                    break;
                } else {
                    printf("Ulog mora biti između %d i %d\n", MIN_ULOG, MAX_ULOG);
                }
            } else {
                printf("Neispravan unos, molimo unesite broj.\n");
            }
        } else {
            printf("Greška pri čitanju unosa.\n");
        }
    }
}

void prikaziStanje(const Racun* racun) {
    printf("Trenutno stanje računa: %d\n", racun->stanjeRacuna);
    printf("Trenutni ulog: %d\n", racun->ulog);
    printf("Najveći dobitak: %d\n", racun->najveciDobitak);
}

int nadiDobitak(char simbol, int brojUzastopnih) {
    for (int i = 0; i < sizeof(dobitci) / sizeof(dobitci[0]); i++) {
        if (dobitci[i].simbol == simbol) {
            switch (brojUzastopnih) {
            case 3: return dobitci[i].dobitakZa3;
            case 4: return dobitci[i].dobitakZa4;
            case 5: return dobitci[i].dobitakZa5;
            default: return 0;
            }
        }
    }
    return 0;
}

int izracunajDobitak(char matrica[REDCI][STUPCI]) {
    int dobitak = 0;

    // Provjera redaka
    for (int i = 0; i < REDCI; i++) {
        int brojUzastopnih = 1;
        for (int j = 1; j < STUPCI; j++) {
            if (matrica[i][j] == matrica[i][j - 1]) {
                brojUzastopnih++;
            } else {
                break;
            }
        }
        dobitak += nadiDobitak(matrica[i][0], brojUzastopnih);
    }

    return dobitak;
}

void ucitajStanje(Racun* racun) {
    FILE* file = fopen("stanje.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d %d %d", &racun->stanjeRacuna, &racun->ulog, &racun->najveciDobitak);
        fclose(file);
    }
}

void spremiStanje(const Racun* racun) {
    FILE* file = fopen("stanje.txt", "w");
    if (file != NULL) {
        fprintf(file, "Stanje računa: %d\nUlog: %d\nNajveci dobitak: %d\n", racun->stanjeRacuna, racun->ulog, racun->najveciDobitak);
        fclose(file);
    }
}
    
