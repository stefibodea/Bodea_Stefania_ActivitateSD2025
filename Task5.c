//1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru.Daca lista
//are mai putine noduri decat index - ul dat, nu se realizeaza stergerea.
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
	nodNou->next = (*cap);
	*cap = nodNou;
}

void adaugaMasiniInListaDupaPret(Nod** cap, Masina m) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = m;
	nodNou->next = NULL;

	if (*cap == NULL || (*cap)->info.pret > m.pret) { //daca lista e goala sau primul nod are pretul mai mare, inseram la inceput
		nodNou->next = *cap;
		(*cap) = nodNou;
	}
	else {//ne oprim inainte primului nod cu pretul mai mare - inseram intre temp si temp->next
		Nod* temp = *cap;
		while (temp->next != NULL && temp->next->info.pret < m.pret) {
			temp = temp->next;
		}
		nodNou->next = temp->next;
		temp->next = nodNou;
	}
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) { //returneaza adresa primului nod din lista
	FILE* f = fopen(numeFisier, "r");
	Nod* cap = NULL;

	if (f) {
		while (!feof(f)) {
			Masina m;
			m = citireMasinaDinFisier(f);
			//adaugaMasinaInLista(&cap, m);
			adaugaMasiniInListaDupaPret(&cap, m);
		}
	}
	fclose(f);
	return cap;
}

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
				printf("Am parcurs toata lista fara sa gasim indexul dat.\n");
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


Masina* masiniCuMaiMultDeUnNumarDeUsiDat(Nod* cap, int* nrMasini, int nrUsiDat) {
	Nod* temp = cap;
	
	*nrMasini = 0;
	while (temp != NULL)  {
		if (temp->info.nrUsi > nrUsiDat) {
			(*nrMasini)++;
		}
		temp = temp->next;
	}

	Masina* vectorMasini = (Masina*)malloc(sizeof(Masina) * (*nrMasini));
	temp = cap;
	int i = 0;
	while (temp != NULL) {
		if (temp->info.nrUsi > nrUsiDat) {
			vectorMasini[i].id = temp->info.id;

			vectorMasini[i].model = (char*)malloc(strlen(temp->info.model) + 1);
			strcpy_s(vectorMasini[i].model, strlen(temp->info.model) + 1, temp->info.model);

			vectorMasini[i].nrUsi = temp->info.nrUsi;

			vectorMasini[i].numeSofer = (char*)malloc(strlen(temp->info.numeSofer) + 1);
			strcpy_s(vectorMasini[i].numeSofer, strlen(temp->info.numeSofer) + 1, temp->info.numeSofer);

			vectorMasini[i].pret = temp->info.pret;
			vectorMasini[i].serie = temp->info.serie;
			
			i++;
		}
		temp = temp->next;
	}
	return vectorMasini;
}

void afisareVectorMasini(Masina* vectorMasini, int nrMasini) {
	if (vectorMasini != NULL) {
		for (int i = 0; i < nrMasini; i++) {
			printf("Masina %d:\n", i + 1);
			printf("id: %d, ", vectorMasini[i].id);
			printf("nr usi: %d, ", vectorMasini[i].nrUsi);
			printf("pret: %.2f, ", vectorMasini[i].pret);
			printf("model: %s, ", vectorMasini[i].model);
			printf("nume sofer: %s, ", vectorMasini[i].numeSofer);
			printf("serie: %c\n\n", vectorMasini[i].serie);
		}
	}
	else {
		printf("Vectorul de masini este gol.\n");
	}
}

void dezalocareVectorMasini(Masina* vectorMasini, int nrMasini) {
	if (vectorMasini != NULL) {
		for (int i = 0; i < nrMasini; i++) {
			free(vectorMasini[i].model);
			free(vectorMasini[i].numeSofer);
		}
		free(vectorMasini);
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

	int nrUsiDat, nrMasini = 0;
	
	printf("Sa se afiseze masinile cu mai mult de ");
	scanf("%d", &nrUsiDat);
	printf(" usi: \n");
	Masina* vectorMasini = masiniCuMaiMultDeUnNumarDeUsiDat(cap, &nrMasini, nrUsiDat);
	afisareVectorMasini(vectorMasini, nrMasini);
	dezalocareVectorMasini(vectorMasini, nrMasini);
	dezalocareListaDeMasini(&cap);
}
