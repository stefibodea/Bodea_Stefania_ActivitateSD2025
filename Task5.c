//2. Implementati o functie care sa insereze elementele in cadrul listei simplu inlantuite astfel
//incat acestea a fie sortate crescator dupa un camp la alegerea voastra.
//3. Implementati o functie care salveaza intr - un vector toate obiectele care indeplinesc o
//conditie stabilita de voi.Realizati deep copy, astfel incat elementele din vector sa fie
//diferentiate de cele din lista.Aveti grija la alocarea de spatiu, deoarece trebuie sa stabiliti
//pentru cate elemente veti aloca spatiu.
//4. Implementati o functie care primeste lista si doua pozitii.In cadrul functiei trebuie sa
//interschimbati elementele de pe cele doua pozitii din lista primita.

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
	struct Nod* next;
};
typedef struct Nod Nod;

//1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru.Daca lista
//are mai putine noduri decat index - ul dat, nu se realizeaza stergerea.
void stergereNodDupaPozitie(int pozitie, Nod** cap) {

	if (*cap == NULL) {
		printf("Lista este goala.\n");
	}
	else {
		Nod* temp;
		Nod* nodDeSters;
		temp = *cap;
		if (pozitie == 0) { //prima pozitie
			(*cap) = (*cap)->next;
			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
		}
		else { //la mijloc
			for (int i = 0; temp != NULL && i < pozitie - 1; i++) {
				temp = temp->next;
			}
			if (temp->next == NULL) { //pozitia data > nr de noduri din lista
				printf("Am parcurs toata lista fara sa gasim indexul dat.");
			}
			else { 
				nodDeSters = temp->next;
				temp->next = nodDeSters->next;
				free(nodDeSters->info.model);
				free(nodDeSters->info.numeSofer);
				free(nodDeSters);
			}	
		}
	}
}

Masina citireMasinaDinFisier(FILE* file) {
	Masina m;
	char buffer[100];
	char separator[3] = ",\n";

	char* p;
	fgets(buffer, 100, file);//citeste prima linie din file si o pune in buffer

	p = strtok(buffer, separator);
	m.id = atoi(p);

	p = strtok(NULL, separator);
	m.nrUsi = atoi(p);

	p = strtok(NULL, separator);
	m.pret = atof(p);

	p = strtok(NULL, separator);
	m.model = (char*)malloc(strlen(p) + 1);
	strcpy_s(m.model, strlen(p) + 1, p);

	p = strtok(NULL, separator);
	m.numeSofer = (char*)malloc(strlen(p) + 1);
	strcpy_s(m.numeSofer, strlen(p) + 1, p);

	p = strtok(NULL, separator);
	m.serie = *p;

	return m;
}

void adaugaMasinaInLista(Nod** cap, Masina m) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));

    nodNou->info = m;
    nodNou->next =  (*cap);
	*cap = nodNou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) { //returneaza adresa primului nod din lista
	FILE* f = fopen(numeFisier, "r");
	Nod* cap = NULL;
	
	if (f) {
		while (!feof(f)) {
			Masina m;
			m = citireMasinaDinFisier(f);
			adaugaMasinaInLista(&cap, m);
		}
	}
	fclose(f);
	return cap;
}

void afisareMasina(Masina m) {
	printf("id: %d\n", m.id);
	printf("nr usi: %d\n", m.nrUsi);
	printf("pret: %.2f\n", m.pret);
	printf("model: %s\n", m.model);
	printf("nume sofer: %s\n", m.numeSofer);
	printf("serie: %c\n\n", m.serie);
}

void afisareListaDeMasini(Nod* cap) {
	while (cap != NULL) {
		afisareMasina(cap->info);
		cap = cap->next;
	}
}

void dezalocareListaDeMasini(Nod** cap) {
	
	while ((*cap) != NULL) {

		Nod* temp = (*cap);

		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);

		(*cap) = (*cap)->next;
	}
}

int main() {
	Nod* cap = citireListaMasiniDinFisier("masini.txt");
	afisareListaDeMasini(cap);

	int pozitie;
	printf("Sterge masina de pe pozitia: ");
	scanf("%d", &pozitie);

	printf("Afisare lista dupa stergere: \n");
	stergereNodDupaPozitie(pozitie, &cap);
	afisareListaDeMasini(cap);

	dezalocareListaDeMasini(&cap);
}
