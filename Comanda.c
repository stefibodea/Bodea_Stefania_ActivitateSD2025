#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda {
	unsigned int id;
	char* dataLansare;
	char* dataLivrare;
	char* numeClient;
	float pret;
};
typedef struct Comanda Comanda;

struct Nod {
	Comanda info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

Comanda citireComandaDinFisier(FILE* file) {
	char buffer[100], sep[3] = ",\n";
	char* aux;
	Comanda c;
	fgets(buffer, 100, file);

	aux = strtok(buffer, sep);
	c.id = atoi(aux);

	aux = strtok(NULL, sep);
	c.dataLansare = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.dataLansare, strlen(aux) + 1, aux);
	
	aux = strtok(NULL, sep);
	c.dataLivrare = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.dataLivrare, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.numeClient = (char*)malloc(strlen(aux) + 1);
	strcpy_s(c.numeClient, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	c.pret = atof(aux);
	return c;
}

void afisareComanda(Comanda c) {
	printf("id: %d\n", c.id);
	printf("data lansare: %s\n", c.dataLansare);
	printf("data livrare: %s\n", c.dataLivrare);
	printf("nume client: %s\n", c.numeClient);
	printf("pret: %.2f\n\n", c.pret);
}

void adaugaComenziInArbore(Comanda c, Nod** radacina) {
	if (!(*radacina)) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->info = c;
		nod->stanga = NULL;
		nod->dreapta = NULL;
		*radacina = nod;
	}
	else {
		if (c.id <= (*radacina)->info.id) {
			adaugaComenziInArbore(c, &((*radacina)->stanga) );
		}
		else if (c.id > (*radacina)->info.id) {
			adaugaComenziInArbore(c, &((*radacina)->dreapta));
		}
	}
}

Nod* citireArboreComenziDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Comanda c = citireComandaDinFisier(file);
		adaugaComenziInArbore(c, &radacina);
	}
	fclose(file);
	return radacina;
}

void afisareArboreComenziPostordine(Nod* radacina) {
	if (radacina) {
		afisareArboreComenziPostordine(radacina->stanga);
		afisareArboreComenziPostordine(radacina->dreapta);
		afisareComanda(radacina->info);
	}
}

void dezalocareArboreComenzi(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreComenzi(&((*radacina)->stanga));
		dezalocareArboreComenzi(&((*radacina)->dreapta));

		free((*radacina)->info.dataLansare);
		free((*radacina)->info.dataLivrare);
		free((*radacina)->info.numeClient);
		free(*radacina);
		*radacina = NULL;
	}
	
}

float cautaPretNodDupaId(Nod* radacina, unsigned int idCautat) {
	if (radacina) {
		if (idCautat == radacina->info.id) {
			return radacina->info.pret;
		}
		else {
			if (idCautat < radacina->info.id) {
				return cautaPretNodDupaId(radacina->stanga, idCautat);
			}
			else if (idCautat > radacina->info.id) {
				return cautaPretNodDupaId(radacina->dreapta, idCautat);
			}
		}
	}
	else return 0.0f;
}

float calculeazaValoareMedie3ComenziDinArbore(Nod* radacina, unsigned int id1, unsigned int id2, unsigned int id3) {
	Comanda c1, c2, c3;
	c1.pret = cautaPretNodDupaId(radacina, id1);
	c2.pret = cautaPretNodDupaId(radacina, id2);
	c3.pret = cautaPretNodDupaId(radacina, id3);

	return (c1.pret + c2.pret + c3.pret) / 3.0f;
}

int getNumarComenziDupaNume(Nod* arbore, const char* nume) {
	if (arbore) {
		if (strcmp(arbore->info.numeClient, nume) == 0) {
			return 1 + getNumarComenziDupaNume(arbore->stanga, nume) + getNumarComenziDupaNume(arbore->dreapta, nume);
		}
		else {
			return getNumarComenziDupaNume(arbore->stanga, nume) + getNumarComenziDupaNume(arbore->dreapta, nume);
		}
	}
	return 0;
}

void adaugaComenziInVector(Nod* arbore, const char* nume, Comanda* vector, int* index) {
	if (arbore) {
		if (strcmp(arbore->info.numeClient, nume) == 0) {
			vector[*index] = arbore->info;
			(*index)++;
		}
		adaugaComenziInVector(arbore->stanga, nume, vector, index);
		adaugaComenziInVector(arbore->dreapta, nume, vector, index);
	}
}

Comanda* vectorComenziDupaNume(Nod* arbore, const char* nume, int* dimensiuneVector) {
	int numarComenzi = getNumarComenziDupaNume(arbore, nume);
	*dimensiuneVector = numarComenzi;

	if (numarComenzi == 0) return NULL;

	Comanda* vector = (Comanda*)malloc(sizeof(Comanda) * numarComenzi);
	int index = 0;
	adaugaComenziInVector(arbore, nume, vector, &index);
	return vector;
}


int main() {
	Nod* radacina = citireArboreComenziDinFisier("comenzi.txt");

	printf("AFISARE ABC POSTORDINE: \n");
	afisareArboreComenziPostordine(radacina);

	float medie = calculeazaValoareMedie3ComenziDinArbore(radacina, 1, 2, 3);
	printf("Media valorilor comenzilor cu id-urile 1, 2, 3 este : %.2f\n", medie);

	int nrComenzi = 0;
	Comanda* vector = vectorComenziDupaNume(radacina, "Bodea Stefania", &nrComenzi);
	printf("\nComenzile comandate de Bodea Stefania:\n");
	for (int i = 0; i < nrComenzi; i++) {
		afisareComanda(vector[i]);
	}
	free(vector);


	dezalocareArboreComenzi(&radacina);
}