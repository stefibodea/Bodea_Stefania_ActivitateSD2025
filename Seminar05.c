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

struct Nod
{
	Masina info;
	struct Nod* next;
	struct Nod* prev;
};
typedef struct Nod Nod;

struct ListaDubla
{
	Nod* inceput;
	Nod* final;
};
typedef struct ListaDubla Lista;

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

void afisareListaMasiniDeLaInceput(Lista lista) {
	Nod* p = lista.inceput;
	while (p)
	{
		afisareMasina(p->info);
		p = p->next;
	}
}

void afisareListaMasiniDeLaFinal(Lista lista) {
	Nod* p = lista.final;
	while (p)
	{
		afisareMasina(p->info);
		p = p->prev;
	}
}

void adaugaMasinaInLista(Lista* lista, Masina masinaNoua) {
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

void adaugaLaInceputInLista(Lista* lista, Masina masinaNoua) {
	Nod* nodNou = (Nod*)malloc(sizeof(Nod));
	nodNou->info = masinaNoua;
	nodNou->next = NULL;
	nodNou->prev = lista->inceput;

	if (lista->inceput)
		lista->inceput->prev = nodNou;
	else
		lista->final = nodNou;

	lista->inceput = nodNou;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	Lista lista;
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

void dezalocareLDMasini(Lista* lista) {
	while (lista->inceput)
	{
		if (lista->inceput->info.numeSofer)
			free(lista->inceput->info.numeSofer);

		if (lista->inceput->info.model)
			free(lista->inceput->info.model);

		Nod* p = lista->inceput;
		lista->inceput = p->next;
		free(p);
	}
	lista->final = NULL;
}

float calculeazaPretMediu(Lista lista) {
	float suma = 0;
	int contor = 0;
	Nod* temp = lista.inceput;
	while (temp)
	{
		suma += temp->info.pret;
		contor++;
		temp = temp->next;
	}

	return suma / contor;
}

void stergeMasinaDupaID(Lista* lista, int id) {
	Nod* p = lista->inceput;
	while (p && p->info.id != id)
	{
		p = p->next;

		if (p->prev)
		{
			p->prev->next = p->next;
			if (p->next)
				p->next->prev = p->prev;
			else
				lista->final = p->prev;
		}
		else
		{
			lista->inceput = p->next;
			if (p->next)
				p->next->prev = NULL;
			else
				lista->final = NULL;
		}

		free(p->info.model);
		free(p->info.numeSofer);
		free(p);
	}

}

char* getNumeSoferMasinaScumpa(Lista lista) {
	Nod* p = lista.inceput;
	Nod* pretMaximMasina = p;
	if (lista.inceput)
	{
		while (p)
		{
			if (p->info.pret > pretMaximMasina->info.pret)
				pretMaximMasina = p;
			p = p->next;
		}

		char* numeSofer = (char*)malloc(strlen(pretMaximMasina->info.numeSofer) + 1);
		strcpy_s(numeSofer, strlen(pretMaximMasina->info.numeSofer) + 1, pretMaximMasina->info.numeSofer);

		return numeSofer;
	}
}

int main() {
	Lista lista;
	lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasiniDeLaInceput(lista);

	printf_s("\n\n");

	afisareListaMasiniDeLaFinal(lista);

	printf_s("\n\n");

	float medie = calculeazaPretMediu(lista);
	printf_s("%2f", medie);

	stergeMasinaDupaID(&lista, 1);

	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf_s("%s", numeSofer);
	free(numeSofer);

	dezalocareLDMasini(&lista);
	return 0;
}