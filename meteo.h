#pragma once
#include"config.h"

//fonction de gestion des logs
void log_message(const char* type, const char* message);

//fonction de la gestion r�seau 
int reseaux_initialisation();
int reseaux_ecoute();
int reseaux_fin();

//fonction de la base de donn�es
int database_initialisation();
int database_connexion();
int database_etat();
int database_envoie_data(int temps, int temperature, int humilite, int pression, int gaz_resistance);
int database_recup_data(const char* query, int* ligne, int** resultat);
void database_close();

//fonction de traitement des donn�es
void traitement_data(char message[reseaux_port_ecoute], int nb_bytes);
char* get_json_live_temperature();
char* get_json_live_humidite();
char* get_json_live_pression();
char* get_json_live_essentials_data();
void simple_data(json_object* json, double valeur);
void complex_data(json_object* json, int* Point, int nbPoint, int ligne);

//fonction de l'API http
int http_api();