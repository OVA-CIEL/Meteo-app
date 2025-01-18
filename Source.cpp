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

	http_api();

	thread t1(test);

	t1.join();

	
}