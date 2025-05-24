#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

struct Nod {
	Masina info;
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Nod** arbore, Masina masinaNoua) {
	if (!(*arbore)) {
		Nod* nodNou = (Nod*)malloc(sizeof(Nod));
		nodNou->info = masinaNoua;
		nodNou->st = NULL;
		nodNou->dr = NULL;
		(*arbore) = nodNou;
	}
	else {
		if ((*arbore)->info.id > masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->st), masinaNoua);
		}
		else if ((*arbore)->info.id < masinaNoua.id) {
			adaugaMasinaInArbore(&((*arbore)->dr), masinaNoua);
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	while (!feof(f)) {
		Masina m = citireMasinaDinFisier(f);
		adaugaMasinaInArbore(&arbore, m);
	}
	fclose(f);
	return arbore;
}

void afisareInOrdine(Nod* arbore) {
	if (arbore) {
		afisareInOrdine(arbore->st);
		afisareMasina(arbore->info);
		afisareInOrdine(arbore->dr);
	}
}

void afisarePreOrdine(Nod* arbore) {
	if (arbore) {
		afisareMasina(arbore->info);
		afisarePreOrdine(arbore->st);
		afisarePreOrdine(arbore->dr);
	}
}

void afisarePostOrdine(Nod* arbore) {
	if (arbore) {
		afisarePostOrdine(arbore->st);
		afisarePostOrdine(arbore->dr);
		afisareMasina(arbore->info);
	}
}

void dezalocareArboreDeMasini(Nod** arbore) {
	if (*arbore) {
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free(*arbore);
		(*arbore) = NULL;
	}
}

//radacina=6, id=8
Masina getMasinaByID(Nod* arbore, int id) {
	Masina m;
	m.id = -1;
	if (arbore) {
		if (arbore->info.id < id) {
			return getMasinaByID(arbore->dr, id);
		}
		else if (arbore->info.id > id) {
			return getMasinaByID(arbore->st, id);
		}
		else {
			return arbore->info;
		}
	}
	return m;
}

int determinaNumarNoduri(Nod* arbore) {
	if(arbore == NULL) {
		return 0;
	}
	int numarStanga = determinaNumarNoduri(arbore->st);
	int numarDreapta = determinaNumarNoduri(arbore->dr);
	return 1 + numarStanga + numarDreapta;
}

int calculeazaInaltimeArbore(Nod* arbore) {
	if(arbore == NULL) {
		return 0;
	}
	int inaltimeStanga = calculeazaInaltimeArbore(arbore->st);
	int inaltimeDreapta = calculeazaInaltimeArbore(arbore->dr);
	return (inaltimeStanga > inaltimeDreapta ? inaltimeStanga : inaltimeDreapta) + 1;
}

float calculeazaPretTotal(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	float totalStanga = calculeazaPretTotal(arbore->st);
	float totalDreapta = calculeazaPretTotal(arbore->dr);
	return arbore->info.pret + totalStanga + totalDreapta;
}

float calculeazaPretulMasinilorUnuiSofer(Nod* arbore, const char* numeSofer) {
	if(arbore == NULL) {
		return 0;
	}
	if(strcmp(arbore->info.numeSofer, numeSofer) == 0) {
		return arbore->info.pret + calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer) + 
			calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
	}
	return calculeazaPretulMasinilorUnuiSofer(arbore->st, numeSofer) + 
		calculeazaPretulMasinilorUnuiSofer(arbore->dr, numeSofer);
}

int main() {
	Nod* radacina = NULL;
	radacina = citireArboreDeMasiniDinFisier("masini_arbore.txt");
	afisareInOrdine(radacina);
	afisarePostOrdine(radacina);
	int id = 8;

	printf("Masina cautata: \n");
	Masina m = getMasinaByID(radacina, id);
	afisareMasina(m);

	float pret = calculeazaPretTotal(radacina);
	printf("Pretul este: %.2f", pret);

	printf("\nNumarul de noduri este: %d\n", determinaNumarNoduri(radacina));
	printf("Inaltimea arborelui este: %d\n", calculeazaInaltimeArbore(radacina));
	printf("Pretul masinilor soferului Gigel este: %.2f\n", calculeazaPretulMasinilorUnuiSofer(radacina, "Gigel"));

	dezalocareArboreDeMasini(&radacina);
	return 0;
}