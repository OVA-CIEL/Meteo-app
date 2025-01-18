#include"config.h"
#include"meteo.h"

//initalisation des variables globales
int meteo_data_time = 0;
int meteo_data_temperature = 0;
int meteo_data_humidite = 0;
int meteo_data_pression = 0;
int meteo_data_gaz = 0;

void test()
{
	while (true)
	{
		reseaux_initialisation();
		reseaux_ecoute();
		printf("test\n");
		reseaux_fin();
		this_thread::sleep_for(chrono::milliseconds(10));
	}
}

int main()
{
	log_message("INFO","Démarrage du programme");
	
	database_initialisation();
	database_connexion();

	int* resultat = NULL;
	int ligne = 0;
	database_recup_data("SELECT temperature FROM data", &ligne , &resultat);

	for (int i = 0; i < ligne; i++)
	{
		printf("%d valll\n", resultat[i]);
	}

	http_api();

	thread t1(test);

	t1.join();

	
}