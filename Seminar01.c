//#include<stdio.h>
//
//int main() {
//	printf("Salutare!\nIntroduceti un nr intreg: ");
//	int variabila = 0;
//	scanf_s("%d", &variabila);
//	printf("Ai introdus: %d", variabila);
//	//citirea unei variabile reale.
//
//	return 0;
//}

#include<stdio.h>
#include<stdlib.h>

struct Masina {
	int id;
	int anFabricatie ;
	char* sofer;
	float kilometriParcursi ;
	char initialaProducator ;
};

struct Masina initializare(int id, int anFabricatie, const char* sofer, float kilometriParcursi, char initialaProducator) {
	struct Masina m;
	m.id = id;
	m.anFabricatie = anFabricatie;
	m.sofer = (char*)malloc(strlen(sofer) + 1);
	strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
	m.kilometriParcursi = kilometriParcursi;
	m.initialaProducator = initialaProducator;
	return m;
}

void afisare(struct Masina m) {
	printf("Id: %d\n", m.id);
	printf("An fabricatie: %d\n", m.anFabricatie);
	printf("Sofer: %s\n", m.sofer);
	printf("Km parcursi: %d\n", m.kilometriParcursi);
	printf("Initiala producatorului: %c\n", m.initialaProducator);
}

void modifica_Sofer(struct Masina* m, const char* nouSofer) {
	if (strlen(nouSofer) > 2) {
		if (m->sofer != NULL) {
			free(m->sofer);
		}
	}
	m->sofer = (char*)malloc(strlen(nouSofer) + 1);
	strcpy_s(m->sofer, strlen(nouSofer) + 1, nouSofer);
}

void dezalocare(struct Masina* m) {
	//dezalocare campuri alocate dinamic
	if (m->sofer != NULL) {
		free(m->sofer);
		m->sofer = NULL;
	}
}

int main() {
	struct Masina masina;
	masina = initializare(1, 2004, "stefy", 2000, 'P');//"P"
	afisare(masina);
	modifica_Sofer(&masina, "denys");
	afisare(masina);
	dezalocare(&masina);
	return 0;
}