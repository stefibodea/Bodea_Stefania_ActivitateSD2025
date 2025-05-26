#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
struct Masina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct Masina Masina;

struct Nod {
	struct Nod* stanga;
	struct Nod* dreapta;
	Masina info;
};
typedef struct Nod Nod;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);

	char* aux;
	Masina m;

	aux = strtok(buffer, sep);
	m.id = atoi(aux);

	aux = strtok(NULL, sep);
	m.nrUsi = atoi(aux);

	aux = strtok(NULL, sep);
	m.pret = atof(aux);

	aux = strtok(NULL, sep);
	m.model = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy_s(m.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.numeSofer = (char*)malloc((strlen(aux) + 1) * sizeof(char));
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m.serie = *aux;
	return m;
}

void afisareMasina(Masina masina) {
	printf("id: %d\n", masina.id);
	printf("nr usi: %d\n", masina.nrUsi);
	printf("pret: %.2f\n", masina.pret);
	printf("model: %s\n", masina.model);
	printf("nume sofer: %s\n", masina.numeSofer);
	printf("serie: %c\n\n", masina.serie);
}

void adaugaMasinaInArbore(Masina masina, Nod** radacina) {
	if ((*radacina) == NULL) {
		Nod* nodNou = (Nod*)malloc(sizeof(Nod));
		nodNou->dreapta = NULL;
		nodNou->info = masina;
		nodNou->stanga = NULL;
		(*radacina) = nodNou;
	}
	else {
		if (masina.id < (*radacina)->info.id) {
			adaugaMasinaInArbore(masina, &((*radacina)->stanga));
		}
		else if (masina.id > (*radacina)->info.id) {
			adaugaMasinaInArbore(masina, &((*radacina)->dreapta));
		}
	}
}

Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file;
	file = fopen(numeFisier, "r");

	Nod* radacina = NULL;

	while (!feof(file)) {
		Masina masina;
		masina = citireMasinaDinFisier(file);
		adaugaMasinaInArbore(masina, &radacina);
	}
	fclose(file);
	return radacina;
}

void afisareArboreDeMasiniInOrdine(Nod* radacina) { //inordine = Stanga, Radacina, Dreapta
	if (radacina != NULL) {
		afisareArboreDeMasiniInOrdine(radacina->stanga);
		afisareMasina(radacina->info);
		afisareArboreDeMasiniInOrdine(radacina->dreapta);
	}
}

void afisareArboreDeMasiniPreOrdine(Nod* radacina) { //preordine = Radacina, Stanga, Dreapta 
	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisareArboreDeMasiniPreOrdine(radacina->stanga);
		afisareArboreDeMasiniPreOrdine(radacina->dreapta);
	}
}

void afisareArboreDeMasiniPostOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareArboreDeMasiniPostOrdine(radacina->stanga);
		afisareArboreDeMasiniPostOrdine(radacina->dreapta);
		afisareMasina(radacina->info);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	if ((*radacina) != NULL) {
		Nod* subarboreStanga = (*radacina)->stanga;
		Nod* subarboreDreapta = (*radacina)->dreapta;

		dezalocareArboreDeMasini(&subarboreStanga);
		dezalocareArboreDeMasini(&subarboreDreapta);

		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		(*radacina) = NULL;
	}
}

/*Sa se stearga un nod pentru care se primeste id-ul de cautare. 
Dupa stergere arborele trebuie sa isi pastreze proprietatea de Arbore Binar de Cautare.*/

Nod* stergeRadacina(Nod** radacina) {
	Nod* aux = (*radacina);

	if (aux->stanga != NULL) { 
		(*radacina) = aux->stanga;
		if (aux->dreapta != NULL) {
			Nod* temp = aux->stanga;
			while (temp->dreapta) {
				temp = temp->dreapta;
			}
			temp->dreapta = aux->dreapta;
		}
	}
	else {//daca stanga nu exista
		if (aux->dreapta != NULL) { //si are doar dreapta
			(*radacina) = aux->dreapta;
		}
		else { //nu are nici stanga si nici dreapta
			(*radacina) = NULL;
		}
	}
	free(aux->info.model);
	free(aux->info.numeSofer);
	free(aux);
	aux = NULL;
	return (*radacina);
}

Nod* stergeNodDupaID(Nod** radacina, int idCautat) {
	if (*radacina == NULL) return NULL;

	if ((*radacina)->info.id == idCautat) {
		(*radacina) = stergeRadacina(&(*radacina));
	}
	else {
		if (idCautat < (*radacina)->info.id) {
			(*radacina)->stanga = stergeNodDupaID(&((*radacina)->stanga), idCautat);
		}
		else {
			(*radacina)->dreapta = stergeNodDupaID(&((*radacina)->dreapta), idCautat);
		}
	}
	return *radacina;
}

/*Sa se implementeze o functie care pentru o radacina primita ca parametru, returneaza
radacina subarborelui stang sau drept care are inaltimea mai mare.*/
int calculeazaInaltimeArbore(Nod* arbore) {
	if (arbore == NULL) {
		return 0;
	}
	int st = calculeazaInaltimeArbore(arbore->stanga);
	int dr = calculeazaInaltimeArbore(arbore->dreapta);
	return 1 + (st > dr ? st : dr);
}

Nod* subarboreMaiMare(Nod* radacina) {
	if (radacina == NULL || (radacina->stanga == NULL && radacina->dreapta == NULL)) {
		return NULL;
	}

	int inaltimeStanga = calculeazaInaltimeArbore(radacina->stanga);
	int inaltimeDreapta = calculeazaInaltimeArbore(radacina->dreapta);
	if (inaltimeStanga >= inaltimeDreapta) {
		return radacina->stanga;
	}
	else return radacina->dreapta;
}

/*Sa se realizeze o functie care pentru un nod radacina primit, determina numarul de noduri
aflate in subordine.*/
int nrNoduriTotal(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	int nrNoduriStanga = nrNoduriTotal(radacina->stanga);
	int nrNoduriDreapta = nrNoduriTotal(radacina->dreapta);
	return 1 + nrNoduriDreapta + nrNoduriStanga;
}

int nrNoduriSubordine(Nod* radacina) {
	if (radacina == NULL) {
		return 0;
	}
	return (nrNoduriTotal(radacina) - 1);
}

/*Sa se realizeze o functie care pentru un nod radacina primit ca parametru, returneaza
radacina subarborelui stang sau drept care are mai multe noduri in subordine.*/
Nod* subarboreMaiMulteNoduri(Nod* radacina) {
	if (radacina == NULL || (radacina->stanga == NULL && radacina->dreapta == NULL)) {
		return NULL;
	}
	if (nrNoduriSubordine(radacina->stanga) >= nrNoduriSubordine(radacina->dreapta)) {
		return radacina->stanga;
	}
	else return radacina->dreapta;
}
int main() {
	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");

	printf("AFISARE ARBORE INORDINE\n\n");
	afisareArboreDeMasiniInOrdine(arbore);

	int idCautat = 10;
	stergeNodDupaID(&arbore, idCautat);
	printf("AFISARE ARBORE DUPA STERGERE NOD CU id = %d\n\n", idCautat);
	afisareArboreDeMasiniInOrdine(arbore);

	Nod* nod = subarboreMaiMare(arbore);
	printf("Subarborele cu inaltime mai mare incepe cu id = %d\n\n", nod->info.id);

	printf("Nr noduri subordine = %d\n\n", nrNoduriSubordine(arbore));

	Nod* nod2 = subarboreMaiMulteNoduri(arbore);
	printf("Subarborele cu mai multe noduri este cel care incepe cu id = %d\n\n", nod2->info.id);

	dezalocareArboreDeMasini(&arbore);
}
