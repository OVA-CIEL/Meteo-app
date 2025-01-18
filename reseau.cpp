#include"config.h"
#include"meteo.h"

//initalisation des variables
int server_socket, client_socket;
struct sockaddr_in server_addr, client_addr;
char buffer[reseaux_buffer_taille];
socklen_t addr_len_client = sizeof(client_addr);
int addr_len_server = sizeof(server_addr);

//initialisation de la connection réseaux

int reseaux_initialisation()
{
	//initialisation de la connection réseaux
	log_message("INFO", "Initialisation de la connection réseaux");
	#ifdef _WIN32 //windows
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			log_message("ERREUR", "Impossible d'initialiser la connection réseaux");
			return(-1);
		}
	#else //linux
	#endif

	//création de le socket serveur
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		log_message("ERREUR", "Impossible de créer le socket serveur");
		return(-1);
	}

	//configuration de l'adresse du serveur
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(reseaux_port_ecoute);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	//liaison du socket serveur à l'adresse du serveur
	int bind_result = bind(server_socket, (struct sockaddr*)&server_addr, addr_len_server);
	if (bind_result == -1)
	{
		log_message("ERREUR", "Impossible de lier le socket serveur à l'adresse du serveur");
		return(-1);
	}

	log_message("INFO", "Le serveur est initialisé");
	return(1);
}

int reseaux_ecoute()
{
	log_message("INFO", "Mise en écoute du serveur");

	//mise en écoute du serveur
	int listen_result = listen(server_socket, reseaux_taille_queue);
	if (listen_result == -1)
	{
		log_message("ERREUR", "Impossible de mettre en écoute le serveur");
		return(-1);
	}

	//attente de la connection d'un client
	client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_len_client);
	if (client_socket == -1)
	{
		log_message("ERREUR", "Impossible d'accepter la connection du client");
		return(-1);
	}

	//timeout de la connection
	#ifdef _WIN32 //windows
	
	#else //linux
		unsigned long timeout = 50000;
		int result = setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
		printf(result);
	#endif

	//boucle de réception des données
	while (true)
	{
		memset(buffer, 0, reseaux_buffer_taille); //réinitialisation du buffer

		//réception des données
		int recv_result = recv(client_socket, buffer, reseaux_buffer_taille, 0);
		if (recv_result == -1)
		{
			log_message("ERREUR", "Impossible de recevoir les données du client");
			return(-1);
		}
		else if (recv_result == 0)
		{
			log_message("INFO", "Fin de la connection avec le client");
			return(0);
		}
		else
		{
			//affichage des données reçues
			log_message("INFO", buffer);
			
			//traitement des données
			traitement_data(buffer, recv_result);
		}
	}
	return(1);
}

int reseaux_fin()
{
	//fermeture de la connection réseaux
	log_message("INFO", "Fermeture de la connection réseaux");

	#ifdef _WIN32 //windows
		WSACleanup();
		closesocket(server_socket);
		closesocket(client_socket);
		return(1);
	#else //linux
		close(server_socket);
		close(client_socket);	
		return(1);
	#endif
}