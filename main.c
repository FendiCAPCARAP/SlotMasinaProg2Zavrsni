#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include "funkcije.h"

int main() {
	srand(time(NULL)); // Inicijalizacija generatora slučajnih brojeva
	Racun racun = { POCETNI_KREDITI, MIN_ULOG, 0 };
	int opcija;

	ucitajStanje(&racun);

	do {
		ispisiIzbornik();
		scanf("%d", &opcija);
		getchar();

		switch (opcija) {
		case 1:
			igra(&racun);
			break;
		case 2:
			dodajKredite(&racun);
			break;
		case 3:
			promjeniUlog(&racun);
			break;
		case 4:
			prikaziStanje(&racun);
			break;
		case 5:
			printf("Izlaz...\n");
			break;
		default:
			printf("Neispravan odabir, pokušajte ponovo.\n");
		}
	} while (opcija != 5);

	spremiStanje(&racun);

	return 0;
}
