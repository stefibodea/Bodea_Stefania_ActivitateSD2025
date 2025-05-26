#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Definiți structura PiesaTeatru, care conține următoarele câmpuri:
 titlu (char*), durată în minute (int), anul premierei (int), popularitate (float – scor între 0 și 10).*/
struct PiesaTeatru {
	char* titlu;
	int durata;
	int anPremiera;
	float popularitate;
};
typedef struct PiesaTeatru PiesaTeatru;

struct Nod {
	PiesaTeatru info;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

PiesaTeatru citirePiesaTeatru(FILE* file) {
	char buffer[100], sep[3] = ",\n";
	char* aux;
	PiesaTeatru piesa;
	fgets(buffer, 100, file);

	aux = strtok(buffer, sep);
	piesa.titlu = (char*)malloc(strlen(aux) + 1);
	strcpy_s(piesa.titlu, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	piesa.durata = atoi(aux);

	aux = strtok(NULL, sep);
	piesa.anPremiera = atoi(aux);

	aux = strtok(NULL, sep);
	piesa.popularitate = atof(aux);
	return piesa;
}

void afisarePiesaTeatru(PiesaTeatru piesa) {
	printf("titlu: %s\n", piesa.titlu);
	printf("durata: %d\n", piesa.durata);
	printf("an premiera: %d\n", piesa.anPremiera);
	printf("popularitate: %.2f\n\n", piesa.popularitate);
}

/*Implementați o funcție de inserare într-un arbore binar de căutare, sortând piesele alfabetic după titlu.*/
void adaugaPiesaTeatruInArbore(PiesaTeatru piesa, Nod** radacina) {
	if (!(*radacina)) {
		Nod* nod = (Nod*)malloc(sizeof(Nod));
		nod->dreapta = NULL;
		nod->stanga = NULL;
		nod->info = piesa;
		*radacina = nod;
	}
	else {
		if (strcmp(piesa.titlu, (*radacina)->info.titlu) < 0) {
			adaugaPiesaTeatruInArbore(piesa, &((*radacina)->stanga));
		}
		else if (strcmp(piesa.titlu, (*radacina)->info.titlu) > 0) {
			adaugaPiesaTeatruInArbore(piesa, &((*radacina)->dreapta));
		}
	}
}

Nod* citireArborePieseTeatruDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		PiesaTeatru piesa = citirePiesaTeatru(file);
		adaugaPiesaTeatruInArbore(piesa, &radacina);
	}
	fclose(file);
	return radacina;
}

/*Implementați o funcție de parcurgere în inordine și afișare a pieselor (titlu, durată, an, popularitate).*/
void afisareArborePieseTeatruInOrdine(Nod* radacina) {
	if (radacina) {
		afisareArborePieseTeatruInOrdine(radacina->stanga);
		afisarePiesaTeatru(radacina->info);
		afisareArborePieseTeatruInOrdine(radacina->dreapta);
	}
}

/*Implementați o funcție care returnează piesa cu cea mai mare popularitate, folosind deep copy.*/
float calculeazaPopularitateMaxima(Nod* radacina) {
	if (radacina == NULL) {
		return 0.0f;
	}

	float popularitateStanga = calculeazaPopularitateMaxima(radacina->stanga);
	float popularitateDreapta = calculeazaPopularitateMaxima(radacina->dreapta);

	float popularitateMaxima = radacina->info.popularitate;
	if (popularitateStanga > popularitateMaxima) {
		popularitateMaxima = popularitateStanga;
	}
	if (popularitateDreapta > popularitateMaxima) {
		popularitateMaxima = popularitateDreapta;
	}
	return popularitateMaxima;
}

PiesaTeatru afiseazaPiesaCuPopularitateMaxima(Nod* radacina, float popularitateMaxima) {
	if (radacina) {
		if (radacina->info.popularitate == popularitateMaxima) {
			PiesaTeatru piesa;
			piesa.titlu = (char*)malloc(strlen(radacina->info.titlu) + 1);
			strcpy(piesa.titlu, radacina->info.titlu);
			piesa.durata = radacina->info.durata;
			piesa.anPremiera = radacina->info.anPremiera;
			piesa.popularitate = radacina->info.popularitate;
			return piesa;
		}

		PiesaTeatru piesaSubarboreStang = afiseazaPiesaCuPopularitateMaxima(radacina->stanga, popularitateMaxima);
		PiesaTeatru piesaSubarboreDrept = afiseazaPiesaCuPopularitateMaxima(radacina->dreapta, popularitateMaxima);

		if (piesaSubarboreStang.titlu) {
			return piesaSubarboreStang;
		}
		else return piesaSubarboreDrept;
	}
}

/*Implementați o funcție care caută o piesă după titlu și o returnează (deep copy).*/

PiesaTeatru cautaPiesaDupaTitlu(Nod* radacina, const char* titluCautat) {
	if (radacina) {
		if (strcmp(titluCautat, radacina->info.titlu) == 0) {
			PiesaTeatru copiePiesa;
			copiePiesa.titlu = (char*)malloc(strlen(radacina->info.titlu) + 1);
			strcpy_s(copiePiesa.titlu, strlen(radacina->info.titlu) + 1, radacina->info.titlu);
			copiePiesa.anPremiera = radacina->info.anPremiera;
			copiePiesa.durata = radacina->info.durata;
			copiePiesa.popularitate = radacina->info.popularitate;
			return copiePiesa;
		}
		else if (strcmp(titluCautat, radacina->info.titlu) < 0) {
			return cautaPiesaDupaTitlu(radacina->stanga, titluCautat);
		}
		else return cautaPiesaDupaTitlu(radacina->dreapta, titluCautat);
	}
}

/*Eliberați toată memoria.*/
void dezalocareArborePieseTeatru(Nod** radacina) {
	if (*radacina) {
		Nod* subarboreStanga = (*radacina)->stanga;
		Nod* subarboreDreapta = (*radacina)->dreapta;

		dezalocareArborePieseTeatru(&subarboreStanga);
		dezalocareArborePieseTeatru(&subarboreDreapta);

		free((*radacina)->info.titlu);
		free(*radacina);
		*radacina = NULL;
	}
}


int main() {
	Nod* radacina = citireArborePieseTeatruDinFisier("piese.txt");
	
	printf("Piese in ordine alfabetica \n");
	afisareArborePieseTeatruInOrdine(radacina);

	float popMax = calculeazaPopularitateMaxima(radacina);
	printf("Popularitate maxima: %.2f\n\n", popMax);

	PiesaTeatru piesaMax = afiseazaPiesaCuPopularitateMaxima(radacina, popMax);
	if (piesaMax.titlu != NULL) {
		printf("Piesa cu popularitate maxima:\n");
		afisarePiesaTeatru(piesaMax);
		free(piesaMax.titlu);
	}

	const char* titluCautat = "Ivan Turbinca";
	PiesaTeatru piesaCautata = cautaPiesaDupaTitlu(radacina, titluCautat);
	if (piesaCautata.titlu) {
		printf("\nPiesa cautata: %s\n", titluCautat);
		afisarePiesaTeatru(piesaCautata);
		free(piesaCautata.titlu);
	}
	else {
		printf("\nPiesa \"%s\" nu a fost gasita.", titluCautat);
	}
	dezalocareArborePieseTeatru(&radacina);
	return 0;
}
