#include"config.h"
#include"meteo.h"

//initialisation variable
MYSQL* connecteur = NULL;

int database_initialisation()
{
	//initialisation de la base de données
	connecteur = mysql_init(NULL);
	if (connecteur == NULL)
	{
		log_message("ERREUR", "Impossible d'initialiser la base de données");
		return(-1);
	}
	return(1);
}

int database_connexion()
{
	//connexion à la base de données
	if (mysql_real_connect(connecteur, database_ip, database_user, database_password, database_name, database_port, NULL, 0) == NULL)
	{
		log_message("ERREUR", "Impossible de se connecter à la base de données");
		return(-1);
	}
	return(1);
}

int database_etat()
{
	//vérification de l'état de la base de données
	if (mysql_ping(connecteur) != 0)
	{
		log_message("ERREUR", "Perte de connexion avec la base de données");
		return(-1);
	}
	return(1);
}

int database_envoie_data(int temps, int temperature, int humilite, int pression, int gaz_resistance)
{
	//envoie des données à la base de données
	char requete[1024];
	snprintf(requete, sizeof(requete), "INSERT INTO data VALUES (%d,%d,%d,%d,%d)", temps, temperature, humilite, pression, gaz_resistance);
	
	if (mysql_query(connecteur, requete) != 0)
	{
		log_message("ERREUR", "Impossible d'envoyer les données à la base de données");
		return(-1);
	}
	return(1);
}

int database_recup_data(const char *query, int* ligne, double** resultat)
{
	if (mysql_query(connecteur, query) != 0)
	{
		log_message("ERREUR", "Impossible de récupérer les données de la base de données");
		printf("%s", mysql_error(connecteur));
		return(-1);
	}

	MYSQL_RES* resultat_requete = mysql_store_result(connecteur);

	if (resultat_requete == NULL)
	{
		log_message("ERREUR", "Impossible de stocker les résultats de la requête");
		return(-1);
	}

	*ligne = mysql_num_rows(resultat_requete);

	*resultat = new double[*ligne];

	MYSQL_ROW ligne_resultat;

	for (int i = 0; i < *ligne; i++)
	{
		ligne_resultat = mysql_fetch_row(resultat_requete);
		(*resultat)[i] = atof(ligne_resultat[0]);
	}

	mysql_free_result(resultat_requete);

	return(1);
}

void database_close()
{
	//fermeture de la base de données
	mysql_close(connecteur);
}


