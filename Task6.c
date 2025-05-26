#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla {
	Nod* inceput;
	Nod* final;
};
typedef struct ListaDubla ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = (char*)malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = (char*)malloc(strlen(aux) + 1);
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

void adaugaMasinaInLista(ListaDubla* lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->final;

	if (lista->final != NULL)
		lista->final->next = nodNou;
	else
		lista->inceput = nodNou;
	lista->final = nodNou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.final = NULL;
	lista.inceput = NULL;
	while (!feof(f))
	{
		Masina m;
		m = citireMasinaDinFisier(f);
		adaugaMasinaInLista(&lista, m);
	}
	fclose(f);
	return lista;
}

/*1. Implementati o functie care sterge un nod de pe o pozitie data ca parametru. Daca lista
are mai putine noduri decat index-ul dat, nu se realizeaza stergerea;*/

void stergeNodDePePozitieData(ListaDubla** lista, int index) {
	if ((*lista)->inceput == NULL || index < 0) {
		return; 
	}
	Nod* nou = (*lista)->inceput;
	for (int i = 0; i < index && nou != NULL; i++) {
		nou = nou->next;
	}
	if (nou == NULL) {
		return;
	}
	if (nou->prev != NULL) {
		nou->prev->next = nou->next;
	} else {
		(*lista)->inceput = nou->next; // Nodul este capul listei
	}
	if (nou->next != NULL) {
		nou->next->prev = nou->prev;
	} else {
		(*lista)->final = nou->prev; // Nodul este coada listei
	}
	free(nou);
}

/*2. Implementati o functie care sa insereze elementele in cadrul listei dublu inlantuite astfel
incat acestea a fie sortate crescator dupa un camp la alegerea voastra;*/

void insereazaSortat(ListaDubla** lista, int valoare) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info.pret = valoare;
	nou->next = NULL;
	nou->prev = NULL;
	if ((*lista)->inceput == NULL) { // Lista este goala
		(*lista)->inceput = nou;
		(*lista)->final = nou;
		return;
	}
	Nod* p = (*lista)->inceput;
	while (p != NULL && p->info.pret < valoare) {
		p = p->next;
	}
	if (p == (*lista)->inceput) { // Inserare la inceput
		nou->next = (*lista)->inceput;
		(*lista)->inceput->prev = nou;
		(*lista)->inceput = nou;
	} else if (p == NULL) { // Inserare la sfarsit
		nou->prev = (*lista)->final;
		(*lista)->final->next = nou;
		(*lista)->final = nou;
	} else { // Inserare in mijloc
		nou->prev = p->prev;
		nou->next = p;
		p->prev->next = nou;
		p->prev = nou;
	}
}

/*3. Realizati parcurgerile pentru lista dublu inlantuita, astfel incat sa afiseze elementele
sortate crescator dar si sortate descrescator;*/

void afisareMasiniCrescatorLD(ListaDubla* lista) {
	Nod* nou = lista->inceput;
	while (nou != NULL) {
		afisareMasina(nou->info);
		nou = nou->next;
	}
	printf("\n");
}

void afisareMasiniDescrescatorLD(ListaDubla* lista) {
	Nod* nou = lista->final;
	while (nou != NULL) {
		afisareMasina(nou->info);
		nou = nou->prev;
	}
	printf("\n");
}

/*4. Implementati o functie care salveaza intr-o lista simplu inlantuita toate obiectele care
indeplinesc o conditie stabilita de voi. Realizati deep copy, astfel incat elementele din
listasimplu inlantuita sa fie diferite de cele din lista dublu inlantuita. Observati diferenta
dintre utilizarea de lista inlantuita si vector (task-ul precedent);*/

struct ListaSimpla {
	Masina info;
	struct ListaSimpla* next;
};
typedef struct ListaSimpla LS;

LS* creeazaListaSimpla(ListaDubla* listaDubla, float pret) {
	Nod* nodNouLD = listaDubla->inceput;
	LS* inceputLS = NULL;
	LS* finalLS = NULL;
	

	while(nodNouLD != NULL) {
		if (nodNouLD->info.pret < pret) {

			LS* nodNouLS = (LS*)malloc(sizeof(LS));

			nodNouLS->info.id = nodNouLD->info.id;
			nodNouLS->info.nrUsi = nodNouLD->info.nrUsi;
			nodNouLS->info.pret = nodNouLD->info.pret;

			nodNouLS->info.model = (char*)malloc(strlen(nodNouLD->info.model) + 1);
			strcpy_s(nodNouLS->info.model, strlen(nodNouLD->info.model) + 1, nodNouLD->info.model);

			nodNouLS->info.numeSofer = (char*)malloc(strlen(nodNouLD->info.numeSofer) + 1);
			strcpy_s(nodNouLS->info.numeSofer, strlen(nodNouLD->info.numeSofer) + 1, nodNouLD->info.numeSofer);

			nodNouLS->info.serie = nodNouLD->info.serie;

			nodNouLS->next = NULL;

			if (inceputLS == NULL) {
				inceputLS = nodNouLS;
				finalLS = nodNouLS;
			}
			else {
				finalLS->next = nodNouLS;
				finalLS = nodNouLS;
			}
		}
		nodNouLD = nodNouLD->next;
	}
	return inceputLS;
}

void afisareListaSimpla(LS* lista) {
	while (lista != NULL) {
		afisareMasina(lista->info);
		lista = lista->next;
	}
}
void dezalocareListaSimpla(LS* lista) {
	while (lista != NULL) {
		LS* temp = lista;
		lista = lista->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
}

/*5. Implementati o functie care primeste lista dublu inlantuita si doua pozitii. In cadrul
functiei trebuie sa interschimbati elementele de pe cele doua pozitii din lista primita.
Interschimbati informatiile utile din cele doua noduri.*/

void interschimbaElemente(ListaDubla* lista, int index1, int index2) {
	if (lista->inceput == NULL || index1 < 0 || index2 < 0) {
		return; 
	}
	Nod* nod1 = lista->inceput;
	Nod* nod2 = lista->inceput;
	for (int i = 0; i < index1 && nod1 != NULL; i++) {
		nod1 = nod1->next;
	}
	for (int i = 0; i < index2 && nod2 != NULL; i++) {
		nod2 = nod2->next;
	}
	if (nod1 == NULL || nod2 == NULL) {
		return; 
	}
	Masina m = nod1->info;
	nod1->info = nod2->info;
	nod2->info = m;
}
void dezalocareListaDubla(ListaDubla* lista) {
	Nod* p = lista->inceput;
	while (p != NULL) {
		Nod* temp = p;
		p = p->next;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	lista->inceput = NULL;
	lista->final = NULL;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");

	printf("Parcurgere Crescatoare:\n");
	afisareMasiniCrescatorLD(&lista);

	printf("Parcurgere Descrescatoare:\n");
	afisareMasiniDescrescatorLD(&lista);

	interschimbaElemente(&lista, 0, 1);
	printf("Dupa interschimbarea pozitiilor 0 si 1\n");
	afisareMasiniCrescatorLD(&lista);

	float prag = 5000.0f;
	LS* listaSimpla = creeazaListaSimpla(&lista, prag);
	printf("Masini cu pret < %.2f\n", prag);
	afisareListaSimpla(listaSimpla);

	stergeNodDePePozitieData(&lista, 2);
	printf("Dupa stergerea de pe pozitia 2\n");
	afisareMasiniCrescatorLD(&lista);

	dezalocareListaDubla(&lista);
	dezalocareListaSimpla(listaSimpla);

	return 0;
}
