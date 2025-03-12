#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

void afisareMasina(Masina masina) {
	//afiseaza toate atributele unei masini

	printf("ID: %d \n", masina.id);
	printf("Numar usi: %d \n", masina.nrUsi);
	printf("Pret: %f \n", masina.pret);
	printf("Model: %s \n", masina.model);
	printf("Nume sofer: %s \n", masina.numeSofer);
	printf("Serie: %c \n \n", masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	//afiseaza toate elemente de tip masina din vector
	//prin apelarea functiei afisareMasina()

	for (int i = 0; i < nrMasini; i++) 
	{
		afisareMasina(masini[i]);
		printf("\n");
	}
}

void adaugaMasinaInVector(Masina** masini, int* nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;

	Masina* vectorNou = (Masina*)malloc(sizeof(Masina) * ((*nrMasini) + 1)); //prioritizare deferentiere, fara paranteze trece la adresa urmatoare
	for (int i = 0; i < (*nrMasini); i++)
		vectorNou[i] = (*masini)[i];

	vectorNou[(*nrMasini)] = masinaNoua;
	free(*masini);

	(*masini) = vectorNou;
	(*nrMasini)++;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	Masina m;
	char buffer[100];
	char delimitator[4] = ",;\n";
	fgets(buffer, 100, file);
	char* aux;
	aux = strtok(buffer, delimitator);
	m.id = atoi(aux);
	m.nrUsi = atoi(strtok(NULL, delimitator));
	m.pret = atof(strtok(NULL, delimitator));

	aux = strtok(NULL, delimitator);
	m.model = malloc(strlen(aux) + 1);
	strcpy_s(m.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, delimitator);
	m.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m.numeSofer, strlen(aux) + 1, aux);

	aux = strtok(NULL, delimitator);
	m.serie = aux[0];

	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* f = fopen(numeFisier, "r");
	Masina* masini = NULL;
	while (!feof(f)) //feof e un flag care se seteaza cand ajungem la finalul fisierului
	{
		Masina m = citireMasinaFisier(f);
		adaugaMasinaInVector(&masini, nrMasiniCitite, m);
	}

	fclose(f);
	return masini;
}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini

	for (int i = 0; i < (*nrMasini); i++)
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
	free(*vector);
	(*vector) = NULL;
	(*nrMasini) = 0;
}

int main() {

	Masina* masini = NULL;
	int nrMasini = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nrMasini);
	afisareVectorMasini(masini, nrMasini);
	dezalocareVectorMasini(&masini, &nrMasini);

	return 0;
}