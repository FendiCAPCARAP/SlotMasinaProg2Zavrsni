#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include <time.h>

#define REDCI 3
#define STUPCI 5
#define POCETNI_KREDITI 1000
#define MIN_ULOG 10

typedef struct {
    int stanjeRacuna;
    int ulog;
    int najveciDobitak;
} Racun;

typedef struct {
    char simbol;
    int dobitakZa3;
    int dobitakZa4;
    int dobitakZa5;
} SimbolDobitak;

void ispisiIzbornik();
void igra(Racun* racun);
void dodajKredite(Racun* racun);
void promjeniUlog(Racun* racun);
void prikaziStanje(const Racun* racun);
int izracunajDobitak(char matrica[REDCI][STUPCI]);
void ucitajStanje(Racun* racun);
void spremiStanje(const Racun* racun);
void spremiSortiraneDobitke();
void obrisiDatotekuDobitaka();
void quickSort(int* array, int low, int high);
int partition(int* array, int low, int high);

#endif // FUNKCIJE_H
