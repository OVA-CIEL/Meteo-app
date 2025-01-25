#pragma once
#include"config.h"

//fonction de gestion des logs
void log_message(const char* type, const char* message);

//fonction de la gestion réseau 
int reseaux_initialisation();
int reseaux_ecoute();
int reseaux_fin();

//fonction de la base de données
int database_initialisation();
int database_connexion();
int database_etat();
int database_envoie_data(int temps, int temperature, int humilite, int pression, int gaz_resistance);
int database_recup_data(const char* query, int* ligne, double** resultat);
void database_close();

//fonction de traitement des données
void traitement_data(char message[reseaux_port_ecoute], int nb_bytes);
/*char* get_json_data_loc(int timestampMin, int timestampMax, int nbPoint, char val[5]);
void simple_data(json_object* json, double valeur);*/
void complex_data(json_object* json, double* Point, int nbPoint, int ligne, double* moyenne, double* PointMin, double* PointMax);
char* erreur_data();

//fonction de l'API http
int http_api();

//class message json

class message_json
{
private:
	json_object* json;
	json_object* json_base_table;
	bool brut_live;
	bool all_live;
	bool future;
	bool erreur;

	

public:
	message_json();
	~message_json();
	char* get_message_string();
	void inserte_graf(int timestampMin, int timestampMax, int nbPoint, char val[21]);
	void inserte_brut_live();
	void inserte_all_live();
	void inserte_future();
	void inserte_erreur();
};