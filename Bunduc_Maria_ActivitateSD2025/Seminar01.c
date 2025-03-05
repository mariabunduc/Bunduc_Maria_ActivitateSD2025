//#include<stdio.h>
//#include<stdlib.h>
//
//struct Masina
//{
//	int id;
//	int anFabricatie;
//	char* sofer;
//	float kmParcursi;
//	char initialaProducator;
//	//1 octet = 8 bytes - de la -255 la 255
//	//unsigned de la 0 la 512
//};
//
//struct Masina initializare(int id, int anFabricatie, const char* sofer, float kmParcursi, char initialaProducator)
//{
//	struct Masina m;
//	m.id = id;
//	m.anFabricatie = anFabricatie;
//	m.sofer = (char*)malloc(strlen(sofer) + 1); //cast explicit
//	//malloc e functie si returneaza void*
//	//malloc primeste dimensiune masurata in octeti
//	//void* este template 
//	//cast implicit daca nu pun (char*)
//	strcpy_s(m.sofer, strlen(sofer) + 1, sofer);
//	//strcpy_s se duce maxim, nu merge mai departe ca srcpy (copiaza peste alte informatii)
//	m.kmParcursi = kmParcursi;
//	m.initialaProducator = initialaProducator;
//	return m;
//}
//
//void afisare(struct Masina m)
//{
//	//int %d
//	//char* %s
//	// char %c
//	//float %f
//	printf("ID: %d \n", m.id);
//	printf("An fabricatie: %d \n", m.anFabricatie);
//	printf("Sofer: %s \n", m.sofer);
//	printf("Kilometri parcursi: %f \n", m.kmParcursi);
//	printf("Initiala producator: %c \n", m.initialaProducator);
//}
//
//void modifica_sofer(struct Masina* m, const char* soferNou)
////MEMORY LEAK 
////desen heap si stiva
//{
//	if (strlen(soferNou) > 2)
//	{
//		if (m->sofer != NULL)
//		{
//			free(m->sofer);
//		}
//		m->sofer = (char*)malloc(strlen(soferNou) + 1);
//		strcpy_s(m->sofer, strlen(soferNou) + 1, soferNou);
//	}
//}
//
//void dezalocare(struct Masina* m)
//{
//	if (m->sofer != NULL)
//	{
//		free(m->sofer);
//		m->sofer = NULL;
//	}
//}
//
//int main() {
//	struct Masina masina;
//	masina = initializare(1, 2004, "Maria", 2432.56, 'W');
//	//metoda primeste pointer la obiectul pentru care apelam si se memoreaza in pointerul this
//	//'W' - ia codul ascii
//	//"W" - sir de caractere
//
//	//afisare(masina);
//
//	modifica_sofer(&masina, "MariaB");
//	afisare(masina);
//
//	dezalocare(&masina);
//	
//	return 0;
//}