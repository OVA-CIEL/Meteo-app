#include"config.h"
#include"meteo.h"

//initalisation des variables globales
int meteo_data_time = 0;
int meteo_data_temperature = 0;
int meteo_data_humidite = 0;
int meteo_data_pression = 0;
int meteo_data_gaz = 0;

bool boucle_ecoute = true;

void reseaux()
{
	while (reseaux_initialisation() == -1)
	{
		this_thread::sleep_for(chrono::milliseconds(100));
	}
	while (boucle_ecoute)
	{
		int result = reseaux_ecoute();
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}

int main()
{
	log_message("INFO","Démarrage du programme");
	
	database_initialisation();
	database_connexion();

	http_api();

	thread t1(reseaux);

	t1.join();

	reseaux_fin();

	
}