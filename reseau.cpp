#include"config.h"
#include"meteo.h"

//initalisation des variables
int server_socket, client_socket;
struct sockaddr_in server_addr, client_addr;
char buffer[reseaux_buffer_taille];
socklen_t addr_len_client = sizeof(client_addr);
int addr_len_server = sizeof(server_addr);

//initialisation de la connection r�seaux

int reseaux_initialisation()
{
	//initialisation de la connection r�seaux
	log_message("INFO", "Initialisation de la connection r�seaux");
	#ifdef _WIN32 //windows
		WSADATA wsaData;
		if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		{
			log_message("ERREUR", "Impossible d'initialiser la connection r�seaux");
			return(-1);
		}
	#else //linux
	#endif

	//cr�ation de le socket serveur
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1)
	{
		log_message("ERREUR", "Impossible de cr�er le socket serveur");
		return(-1);
	}

	//configuration de l'adresse du serveur
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(reseaux_port_ecoute);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	//liaison du socket serveur � l'adresse du serveur
	int bind_result = bind(server_socket, (struct sockaddr*)&server_addr, addr_len_server);
	if (bind_result == -1)
	{
		log_message("ERREUR", "Impossible de lier le socket serveur � l'adresse du serveur");
		return(-1);
	}

	log_message("INFO", "Le serveur est initialis�");
	return(1);
}

int reseaux_ecoute()
{
	log_message("INFO", "Mise en �coute du serveur");

	//mise en �coute du serveur
	int listen_result = listen(server_socket, reseaux_taille_queue);
	if (listen_result == -1)
	{
		log_message("ERREUR", "Impossible de mettre en �coute le serveur");
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
	//unsigned long timeout = 50000;
	//setsockopt(client_socket, SOL_SOCKET, SO_RCVTIMEO,&timeout, sizeof(timeout));

	//boucle de r�ception des donn�es
	while (true)
	{
		memset(buffer, 0, reseaux_buffer_taille); //r�initialisation du buffer

		//r�ception des donn�es
		int recv_result = recv(client_socket, buffer, reseaux_buffer_taille, 0);
		if (recv_result == -1)
		{
			log_message("ERREUR", "Impossible de recevoir les donn�es du client");
			return(-1);
		}
		else if (recv_result == 0)
		{
			log_message("INFO", "Fin de la connection avec le client");
			return(0);
		}
		else
		{
			//affichage des donn�es re�ues
			log_message("INFO", buffer);
			
			//traitement des donn�es
			traitement_data(buffer, recv_result);
		}
	}
	return(1);
}

int reseaux_fin()
{
	//fermeture de la connection r�seaux
	log_message("INFO", "Fermeture de la connection r�seaux");

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