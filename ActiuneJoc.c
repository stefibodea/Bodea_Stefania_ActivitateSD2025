#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Definiți structura ActiuneJoc, care conține următoarele câmpuri:
 numele acțiunii (char*), nivel dificultate (int), durată în secunde (int), categorie (char*: „atac”, „apărare”, „specială”).*/
struct ActiuneJoc {
	char* numeActiune;
	int nivelDificultate;
	int durata;
	char* categorie;
};
typedef struct ActiuneJoc Actiune;

/*Implementați o stivă care stochează acțiunile efectuate de un jucător (LIFO).*/
struct Nod {
	Actiune info;
	struct Nod* next;
};
typedef struct Nod Nod;

Actiune citirActiuneJocDinFisier(FILE* file) {
	char buffer[100], sep[3] = ",\n";
	char* aux;
	Actiune a;
	fgets(buffer, 100, file);

	aux = strtok(buffer, sep);
	a.numeActiune = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.numeActiune, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	a.nivelDificultate = atoi(aux);

	aux = strtok(NULL, sep);
	a.durata = atoi(aux);

	aux = strtok(NULL, sep);
	a.categorie = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a.categorie, strlen(aux) + 1, aux);

	return a;
}

/*Implementați funcția de adăugare (push) în stivă.*/
void pushStack(Nod** stiva, Actiune a) {
	Nod* nod = (Nod*)malloc(sizeof(Nod));
	nod->info = a;
	if ((*stiva)) {
		nod->next = (*stiva);
	}
	else {
		nod->next = NULL;
	}
	*stiva = nod;
}

Nod* citireStackActiuniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* stiva = NULL;
	while (!feof(file)) {
		Actiune a = citirActiuneJocDinFisier(file);
		pushStack(&stiva, a);
	}
	fclose(file);
	return stiva;
}

/*Implementați funcția de extragere (pop) a ultimei acțiuni și afișarea ei.*/
Actiune popStack(Nod** stiva) {
	if (*stiva) {
		Actiune a = (*stiva)->info;
		Nod* temp = *stiva;
		*stiva = (*stiva)->next;
		free(temp);
		return a;
	}
	Actiune a = { NULL,-1,-1,NULL };
	return a;
}

void afisareActiune(Actiune a) {
	printf("nume actiune: %s\n", a.numeActiune);
	printf("nivel dificultate: %d\n", a.nivelDificultate);
	printf("durata: %d\n", a.durata);
	printf("categorie: %s\n\n", a.categorie);
}

/*Implementați o funcție care afișează toate acțiunile din stivă fără a le elimina.*/
void afisareStackDeActiuni(Nod* stiva) {
	Nod* temp = stiva;
	while (temp) {
		afisareActiune(temp->info);
		temp = temp->next;
	}
}

/*Implementați o funcție care returnează acțiunea cu cea mai mare durată (deep copy).*/
Actiune actiuneCuDurataMaxima(Nod* stiva) {
	Actiune a = { NULL, -1, -1, NULL };

	while (stiva) {
		if (stiva->info.durata > a.durata) {
			
			if (a.numeActiune) free(a.numeActiune);
			if (a.categorie) free(a.categorie);

			a.numeActiune = (char*)malloc(strlen(stiva->info.numeActiune) + 1);
			strcpy_s(a.numeActiune, strlen(stiva->info.numeActiune) + 1, stiva->info.numeActiune);

			a.nivelDificultate = stiva->info.nivelDificultate;
			a.durata = stiva->info.durata;

			a.categorie = (char*)malloc(strlen(stiva->info.categorie) + 1);
			strcpy_s(a.categorie, strlen(stiva->info.categorie) + 1, stiva->info.categorie);
		}
		stiva = stiva->next;
	}
	return a;
}

/*Eliberați întreaga memorie.*/
void dezalocareStackDeActiuni(Nod** stiva) {
	while (*stiva) {
		Actiune a = popStack(stiva);
		free(a.categorie);
		free(a.numeActiune);
	}
}
int main() {
	Nod* stiva = citireStackActiuniDinFisier("actiuni.txt");

	printf("Toate actiunile citite din fisier\n");
	afisareStackDeActiuni(stiva);

	printf("Actiunea cu durata maxima\n");
	Actiune max = actiuneCuDurataMaxima(stiva);
	afisareActiune(max);
	free(max.numeActiune);
	free(max.categorie);

	printf("Ultima actiune (pop)\n");
	Actiune ultima = popStack(&stiva);
	afisareActiune(ultima);
	free(ultima.numeActiune);
	free(ultima.categorie);

	printf("Stiva dupa pop\n");
	afisareStackDeActiuni(stiva);

	dezalocareStackDeActiuni(&stiva);

	return 0;
}
