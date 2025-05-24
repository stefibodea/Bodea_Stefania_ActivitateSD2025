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

typedef struct NodListaPrincipala NodListaPrincipala;
typedef struct NodListaSecundara NodListaSecundara;
struct NodListaPrincipala {
	Masina info;
	NodListaPrincipala* next;
	NodListaPrincipala* vecini;
};

struct NodListaSecundara {
	NodListaSecundara* next;
	NodListaSecundara* nodInfo;
};

void inserareLP(NodListaPrincipala** cap, Masina masina) {
	NodListaPrincipala* nou = (NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
	nou->next = NULL;
	nou->info = masina;
	nou->vecini = NULL;
	if (*cap) {
		NodListaPrincipala* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}

}
void inserareLS(NodListaSecundara** cap, NodListaPrincipala* nodInfo) {
	NodListaSecundara* nou = (NodListaSecundara*)malloc(sizeof(NodListaSecundara));
	nou->next = *cap;
	nou->nodInfo = nodInfo;
	*cap = nou;
}

NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* cap, int id) {
	while (cap && cap->info.id != id) {
		cap = cap->next;
	}
	return cap;
}

void inserareMuchie(NodListaPrincipala* cap, int idStart, int idStop) {
	NodListaPrincipala* nodStart = cautaNodDupaID(cap, idStart);
	NodListaPrincipala* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop) {
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}


NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodListaPrincipala* graf = NULL;
	if (f) {
		while (!feof(f)) {
			inserareLP(&graf, citireMasinaDinFisier(f));
		}
	}
	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodListaPrincipala* graf) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f))
		{
			int idStart;
			int idStop;
			fscanf(f, "%d %d", &idStart, &idStop);
			inserareMuchie(graf, idStart, idStop);
		}
	}
	fclose(f);
}

void stergereLS(NodListaSecundara** cap) {
	while (*cap) {
		NodListaSecundara* aux = (*cap)->next;
		free(*cap);
		(*cap) = aux;
	}
}

void dezalocareNoduriGraf(NodListaPrincipala** cap) {
	while (*cap) {
		NodListaPrincipala* aux = (*cap)->next;
		stergereLS(&((*cap)->vecini));
		if ((*cap)->info.numeSofer)
		{
			free((*cap)->info.numeSofer);
		}
		if ((*cap)->info.model)
		{
			free((*cap)->info.model);
		}
		free(*cap);
		(*cap) = aux;
	}
}

NodListaPrincipala* citireGrafDinFisiere(const char* numeFisierNoduri, const char* numeFisierMUchii)
{
	NodListaPrincipala* graf = citireNoduriMasiniDinFisier(numeFisierNoduri);
	citireMuchiiDinFisier(numeFisierMUchii, graf);
	return graf;
}

void afisareGraf(NodListaPrincipala* graf) {
	while (graf)
	{
		printf("In nodul %d avem masina:", graf->info.id);
		afisareMasina(graf->info);
		printf("aceasta are ca vecini nodurile urmatoare: ");
		NodListaSecundara* p = graf->vecini;
		while (p)
		{
			printf("%d, ", p->nodInfo->info.id);
			p = p->next;
		}
		printf("\n\n");

		graf = graf->next;
	}
}
int main() {
	NodListaPrincipala* graf = citireGrafDinFisiere("masini.txt", "muchii.txt");
	afisareGraf(graf);
	dezalocareNoduriGraf(&graf);
	return 0;
}