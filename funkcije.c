#define _CRT_SECURE_NO_WARNINGS	

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funkcije.h"

#define MAX_ULOG 5000

SimbolDobitak dobitci[] = {
	{'A', 50, 200, 1000},
	{'B', 40, 150, 800},
	{'C', 30, 100, 600},
	{'D', 20, 75, 400},
	{'E', 10, 50, 200}
};

void ispisiIzbornik() {
	printf("\nIzbornik:\n");
	printf("1. Igraj\n");
	printf("2. Dodaj kredite\n");
	printf("3. Promjeni ulog\n");
	printf("4. Prikaži stanje računa\n");
	printf("5. Izlaz\n");
	printf("Odaberite opciju: ");
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

	printf("Vaš dobitak je: %d\n", dobitak);
	printf("Trenutno stanje računa: %d\n", racun->stanjeRacuna);
}

void dodajKredite(Racun* racun) {
	int iznos;
	char unos[100]; // buffer za unos

	while (1) {
		printf("Unesite iznos za dodavanje kredita: ");
		if (fgets(unos, sizeof(unos), stdin) != NULL) {
			// Pokušaj pretvorbe unosa u cijeli broj
			
			if (sscanf(unos, "%d", &iznos) == 1) {
				// Provjera je li iznos pozitivan
				
				if (iznos > 0) {
					racun->stanjeRacuna += iznos;
					printf("Novi saldo računa: %d\n", racun->stanjeRacuna);
					break;
				}
				else {
					printf("Iznos mora biti pozitivan.\n");
				}
			}
			else {
				printf("Neispravan unos, molimo unesite broj.\n");
			}
		}
		else {
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
			// Pokušaj pretvorbe unosa u cijeli broj
			if (sscanf(unos, "%d", &noviUlog) == 1) {
				// Provjera je li unos u dozvoljenom rasponu
				if (noviUlog >= MIN_ULOG && noviUlog <= MAX_ULOG) {
					racun->ulog = noviUlog;
					printf("Novi ulog postavljen na: %d\n", racun->ulog);
					break;
				}
				else {
					printf("Ulog mora biti između %d i %d\n", MIN_ULOG, MAX_ULOG);
				}
			}
			else {
				printf("Neispravan unos, molimo unesite broj.\n");
			}
		}
		else {
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

	for (int i = 0; i < REDCI; i++) {
		int brojUzastopnih = 1;
		int dobitakZaLiniju = 0;
		int udvostruci = 1;  // Flag za udvostručenje dobitka
		char trenutniSimbol = matrica[i][0];

		for (int j = 1; j < STUPCI; j++) {
			if (matrica[i][j] == trenutniSimbol || matrica[i][j] == 'X') {
				brojUzastopnih++;
				if (matrica[i][j] == 'X') {
					udvostruci = 2;
				}
			}
			else {
				if (brojUzastopnih >= 3) {
					dobitakZaLiniju = nadiDobitak(trenutniSimbol, brojUzastopnih) * udvostruci;
					dobitak += dobitakZaLiniju;
				}
				// Resetiraj za sljedeću sekvencu
				brojUzastopnih = 1;
				udvostruci = 1;
				trenutniSimbol = matrica[i][j];
			}
		}
		if (brojUzastopnih >= 3) {
			dobitakZaLiniju = nadiDobitak(trenutniSimbol, brojUzastopnih) * udvostruci;
			dobitak += dobitakZaLiniju;
		}
	}

	return dobitak;
}

void ucitajStanje(Racun* racun) {
	FILE* datoteka = fopen("slot.txt", "r");
	if (datoteka) {
		fscanf(datoteka, "%d %d %d", &racun->stanjeRacuna, &racun->ulog, &racun->najveciDobitak);
		fclose(datoteka);
	}
	else {
		printf("Nema postojeće datoteke stanja, započinjemo s početnim postavkama.\n");
	}
}

void spremiStanje(const Racun* racun) {
	FILE* datoteka = fopen("slot.txt", "w");
	if (datoteka) {
		fprintf(datoteka, "Stanje računa: %d\nUlog: %d\nNajveći dobitak: %d", racun->stanjeRacuna, racun->ulog, racun->najveciDobitak);
		fclose(datoteka);
	}
	else {
		perror("Pogreška pri spremanju stanja");
	}
}
