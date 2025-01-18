#include"config.h"
#include"meteo.h"

//initialisation variable
MYSQL* connecteur = NULL;

int database_initialisation()
{
	//initialisation de la base de donn�es
	connecteur = mysql_init(NULL);
	if (connecteur == NULL)
	{
		log_message("ERREUR", "Impossible d'initialiser la base de donn�es");
		return(-1);
	}
	return(1);
}

int database_connexion()
{
	//connexion � la base de donn�es
	if (mysql_real_connect(connecteur, database_ip, database_user, database_password, database_name, database_port, NULL, 0) == NULL)
	{
		log_message("ERREUR", "Impossible de se connecter � la base de donn�es");
		return(-1);
	}
	return(1);
}

int database_etat()
{
	//v�rification de l'�tat de la base de donn�es
	if (mysql_ping(connecteur) != 0)
	{
		log_message("ERREUR", "Perte de connexion avec la base de donn�es");
		return(-1);
	}
	return(1);
}

int database_envoie_data(int temps, int temperature, int humilite, int pression, int gaz_resistance)
{
	//envoie des donn�es � la base de donn�es
	char requete[1024];
	snprintf(requete, sizeof(requete), "INSERT INTO data VALUES (%d,%d,%d,%d,%d)", temps, temperature, humilite, pression, gaz_resistance);
	
	if (mysql_query(connecteur, requete) != 0)
	{
		log_message("ERREUR", "Impossible d'envoyer les donn�es � la base de donn�es");
		return(-1);
	}
	return(1);
}

int database_recup_data(const char *query, int* ligne, int** resultat)
{
	if (mysql_query(connecteur, query) != 0)
	{
		log_message("ERREUR", "Impossible de r�cup�rer les donn�es de la base de donn�es");
		return(-1);
	}

	MYSQL_RES* resultat_requete = mysql_store_result(connecteur);

	if (resultat_requete == NULL)
	{
		log_message("ERREUR", "Impossible de stocker les r�sultats de la requ�te");
		return(-1);
	}

	*ligne = mysql_num_rows(resultat_requete);

	*resultat = new int[*ligne];

	MYSQL_ROW ligne_resultat;

	for (int i = 0; i < *ligne; i++)
	{
		ligne_resultat = mysql_fetch_row(resultat_requete);
		(*resultat)[i] = atoi(ligne_resultat[0]);
	}

	mysql_free_result(resultat_requete);

	return(1);
}

void database_close()
{
	//fermeture de la base de donn�es
	mysql_close(connecteur);
}


