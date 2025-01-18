#pragma once
#define _CRT_SECURE_NO_WARNINGS

//bibliothèques de base
#include<stdio.h>
#include<time.h>
#include<stdarg.h>
#include<thread>
#include<string.h>
#include<math.h>
using namespace std;

//bibliothèques pour la base de données
#include<mysql/mysql.h>

//bibliothèques pour la communication réseau

#ifdef _WIN32 //windows
	#include<winsock2.h>
	#include<WS2tcpip.h>
	#pragma comment(lib, "ws2_32.lib")
#else //linux
	#include<sys/socket.h>
	#include<netinet/in.h>
#endif

//bibliothèques pour l'API http
#include<microhttpd.h>

//bibliothèques pour le traitement des données
#include<json-c/json.h>

//base de données
#define database_ip "localhost"
#define database_port 3306
#define database_user "root"
#define database_password "y4uRFwRm95pM73"
#define database_name "meteo"

//Connection réseaux avec l'ESP32
#define reseaux_port_ecoute 17544
#define reseaux_buffer_taille 1024
#define reseaux_taille_queue 1

//API http
#define http_port 17545

//Gestion des logs
#define log_fichier "log.txt"
#define log_taille_max 2048

//variable extenne 
extern int meteo_data_time;
extern int meteo_data_temperature;
extern int meteo_data_humidite;
extern int meteo_data_pression;
extern int meteo_data_gaz;
