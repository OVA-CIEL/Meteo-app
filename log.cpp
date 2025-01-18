#include"config.h"
#include"meteo.h"

void log_message(const char* type, const char* message)
{
	//ouverture du fichier
	FILE* fichier = fopen(log_fichier, "a");
	if (fichier == NULL)
	{
		return;
	}

	//récupération de la date
	time_t date = time(NULL);
	struct tm date_info = *localtime(&date);

	//initailisation du message
	char message_final[log_taille_max];
	char message_time[log_taille_max];
	strftime(message_time, log_taille_max, "[%d/%m/%Y %H:%M:%S]", &date_info);
	snprintf(message_final, sizeof(message_final)+5, "%s [%s] %s\n", message_time, type, message);

	//écriture du message
	fprintf(fichier,"%s", message_final);

	//fermeture du fichier
	fclose(fichier);
	
	return;
}