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
	printf("Serie: %c", masina.serie);
}


typedef struct NodLP NodLP;
typedef struct NodLS NodLS;

struct NodLP {
	Masina info;
	NodLP* next;
	NodLS* vecini;
};

struct NodLS {
	NodLS* next;
	NodLP* nodInfo;
};


void inserareLP(NodLP** cap, Masina masina)
{
	NodLP* nou = (NodLP*)malloc(sizeof(NodLP));
	nou->next = NULL;
	nou->info = masina;
	nou->vecini = NULL;

	if ((*cap))
	{
		NodLP* p = *cap;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareLS(NodLS** cap, NodLP* nodInfo)
{
	NodLS* nou = (NodLS*)malloc(sizeof(NodLS));
	nou->next = *cap;
	nou->nodInfo = nodInfo;
	*cap = nou;
}


NodLP* cautaNodDupaID(NodLP* cap, int id) {
	while (cap && cap->info.id != id)
	{
		cap = cap->next;
	}
	return cap;
}


void inserareMuchie(NodLP* cap, int idStart, int idStop) {
	NodLP* nodStart = cautaNodDupaID(cap, idStart);
	NodLP* nodStop = cautaNodDupaID(cap, idStop);
	if (nodStart && nodStop)
	{
		inserareLS(&(nodStart->vecini), nodStop);
		inserareLS(&(nodStop->vecini), nodStart);
	}
}


NodLP* citireNoduriMasiniDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodLP* graf = NULL;

	if (f)
	{
		while (!feof(f))
		{
			inserareLP(&graf, citireMasinaDinFisier(f));
		}
	}

	fclose(f);
	return graf;
}

void citireMuchiiDinFisier(const char* numeFisier, NodLP* graf) {
	FILE* f = fopen(numeFisier, "r");

	if (f)
	{
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

void stergereLS(NodLS** cap)
{
	while (*cap)
	{
		NodLS* aux = (*cap)->next;
		free(*cap);
		*cap = aux;
	}
}

void dezalocareNoduriGraf(NodLP** cap) {
	while (*cap)
	{
		NodLP* aux = (*cap)->next;
		stergereLS(&((*cap)->vecini));
		if ((*cap)->info.numeSofer)
			free((*cap)->info.numeSofer);
		if ((*cap)->info.model)
			free((*cap)->info.model);
		free(*cap);
		*cap = aux;
	}
}

NodLP* citireGrafDinFisier(const char* numeFisierNoduri, const char* numeFisierMuchii)
{
	NodLP* graf = citireNoduriMasiniDinFisier(numeFisierNoduri);
	citireMuchiiDinFisier(numeFisierMuchii, graf);
	return graf;
}

void afisareGraf(NodLP* graf) {
	while (graf)
	{
		printf("In nodul %d avem masina:", graf->info.id);
		afisareMasina(graf->info);
		printf("aceasta are ca vecini nodurile urmatoare: ");
		NodLS* p = graf->vecini;
		while (p)
		{
			printf("%d, ", p->nodInfo->info.id);
			p = p->next;
		}
		printf("\n\n");
		graf = graf->next;
	}
}

//Parcurgere in adancime

//creare structura Stiva
//push si pop pentru stiva
struct NodStiva
{
	int id;
	NodStiva* next;
};
typedef struct NodStiva NodStiva;

void push(NodStiva** cap, int id)
{
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->next = (*cap);
	nou->id = id;
	*cap = nou;
}

int pop(NodStiva** cap)
{
	if (*cap)
	{
		NodStiva* aux = cap;
		(*cap) = (*cap)->next;
		int id = aux->id;
		free(aux);
		return id;
	}
	else return -1;
}

int calculeazaNrNoduriGraf(NodLP* graf) 
{
	int ct;
	while (graf->next)
	{
		ct++;
		graf = graf->next;
	}
	return ct;
}

void afisareGrafInAdancime(void* listaPrincipala, int idPlecare) {
	/*
	1. Este luata o STIVA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in STIVA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in STIVA
	4.1 Extragem un nod din STIVA
	4.2 Procesam nodul extras din STIVA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in STIVA
	4.4 reluam procesul
	*/
	int size = calculeazaNrNoduriGraf(listaPrincipala);
	int* vizitate = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		vizitate[i] = 0;
	NodStiva* stiva = NULL;

	vizitate[idPlecare - 1] = 1;
	push(&stiva, idPlecare);

	while (stiva)
	{
		int id = pop(stiva);
		NodLP* NodCautat = cautaNodDupaID(listaPrincipala, id);
		afisareMasina(NodCautat->info);
		NodLS* nodVecini = NodCautat->vecini;

		while (nodVecini)
		{
			if (vizitate[nodVecini->nodInfo->info.id - 1] == 0)
			{
				vizitate[nodVecini->nodInfo->info.id - 1] = 1;
				push(&stiva, nodVecini->nodInfo->info.id);
				//deplasare
			}
		}
	}
}

typedef struct NodStiva NodCoada;

void enqueue(NodCoada** cap, int id)
{
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->next = NULL;
	nou->id = id;
	if (*cap)
	{
		NodCoada* p = *cap;
		while (p->next)
			p = p->next;
		p->next = nou;
	}
	else *cap = nou;
}

int dequeue(NodCoada** cap)
{
	return pop(cap);
}

//Parcurgere in latime

//creare structura coada
//enqueue si dequeue pentru coada

void afisareGrafInLatime(NodLP* listaPrincipala, int idPlecare) {
	/*
	1. Este luata o COADA si un vector de vizitate
	2. Marcam nodul de plecare ca fiind vizitat si il adaugam in COADA noastra
	3. Pornim parcurgerea care va rula cat timp avem noduri in COADA
	4.1 Extragem un nod din stiva
	4.2 Procesam nodul extras din COADA - il afisam
	4.3. identificam toti vecinii nodului extras care nu sunt vizitati si ii marcam ca vizitati, adugandu-i in COADA
	4.4 reluam procesul
	*/
	int size = calculeazaNrNoduriGraf(listaPrincipala);
	int* vizitate = (int*)malloc(size * sizeof(int));
	for (int i = 0; i < size; i++)
		vizitate[i] = 0;
	NodStiva* coada = NULL;

	vizitate[idPlecare - 1] = 1;
	enqueue(&coada, idPlecare);

	while (coada)
	{
		int id = dequeue(coada);
		NodLP* NodCautat = cautaNodDupaID(listaPrincipala, id);
		afisareMasina(NodCautat->info);
		NodLS* nodVecini = NodCautat->vecini;

		while (nodVecini)
		{
			if (vizitate[nodVecini->nodInfo->info.id - 1] == 0)
			{
				vizitate[nodVecini->nodInfo->info.id - 1] = 1;
				enqueue(&coada, nodVecini->nodInfo->info.id);
				//deplasare
			}
		}
	}
}


int main() {

	NodLP* graf = citireNoduriMasiniDinFisier("masini.txt", "muschii.txt");
	afisareGraf(graf);
	afisareGrafInAdancime(graf, 1);

	dezalocareNoduriGraf(&graf);

	printf("\n\n");

	afisareGrafInLungimme(graf, 1);



	return 0;
}