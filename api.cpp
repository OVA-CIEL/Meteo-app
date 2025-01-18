#include"config.h"
#include"meteo.h"

static MHD_Result reponce_request(void* cls, struct MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls) {
    
	// Vérifier la méthode HTTP

	if (strcmp(method, "GET") == 0) {

		if (strcmp(url, "/data-loc") == 0)
		{
			int timestampMin = 0;
			int timestampMax = 0;
			int nbPoint = 1;
			char val[5] = "ffff";

			const char* ParamTimestampMin = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "timestampMin");
			const char* ParamTimestampMax = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "timestampMax");
			const char* ParamNbPoint = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "nbPoint");
			const char* ParamVal = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "val");

			if (ParamTimestampMin != NULL) {
				timestampMin = atoi(ParamTimestampMin);
			}
			else if (atoi(ParamTimestampMax) == -1) {
				timestampMin = -1;
			}
			if (ParamTimestampMax != NULL) {
				timestampMax = atoi(ParamTimestampMax);
			}
			else {
				timestampMax = -1;
			}
			if (ParamNbPoint != NULL) {
				nbPoint = atoi(ParamNbPoint);
			}
			else {
				nbPoint = 1;
			}
			if (ParamVal != NULL) {
				if (strcmp(ParamVal, "temp") == 0) {
					strcpy(val, "temp");
				}
				else if (strcmp(ParamVal, "humi") == 0) {
					strcpy(val, "humi");
				}
				else if (strcmp(ParamVal, "pres") == 0) {
					strcpy(val, "pres");
				}
				else {
					strcpy(val, "ffff");
				}
			}
			else {
				strcpy(val, "ffff");
			}

			// Générer les données en JSON



			return MHD_YES;
		}
		
		
	}


	
	/*if (strcmp(method, "GET") == 0) {

        if (strcmp(url, "/live-temperature") == 0)
        {
			// Générer les données en JSON
			char* json_response = get_json_live_temperature();

			struct MHD_Response* response = MHD_create_response_from_buffer(strlen(json_response), (void*)json_response, MHD_RESPMEM_MUST_FREE);
			MHD_add_response_header(response, "Content-Type", "application/json");
			MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Autoriser les requetes cross-origin

			// Envoyer la réponse
			int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

			MHD_destroy_response(response);
            return MHD_YES;
		}
		else if (strcmp(url, "/live-humidity") == 0)
		{
			// Générer les données en JSON
			char* json_response = get_json_live_humidite();

			struct MHD_Response* response = MHD_create_response_from_buffer(strlen(json_response), (void*)json_response, MHD_RESPMEM_MUST_FREE);
			MHD_add_response_header(response, "Content-Type", "application/json");
			MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Autoriser les requetes cross-origin

			// Envoyer la réponse
			int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

			MHD_destroy_response(response);
			return MHD_YES;
		}
		else if (strcmp(url, "/live-pressure") == 0)
		{
			// Générer les données en JSON
			char* json_response = get_json_live_pression();
				
			struct MHD_Response* response = MHD_create_response_from_buffer(strlen(json_response), (void*)json_response, MHD_RESPMEM_MUST_FREE);
			MHD_add_response_header(response, "Content-Type", "application/json");
			MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Autoriser les requetes cross-origin

			// Envoyer la réponse
			int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

			MHD_destroy_response(response);
			return MHD_YES;
		}
		else if (strcmp(url, "/live-essentials-data") == 0)
		{
			// Générer les données en JSON
			char* json_response = get_json_live_essentials_data();

			struct MHD_Response* response = MHD_create_response_from_buffer(strlen(json_response), (void*)json_response, MHD_RESPMEM_MUST_FREE);
			MHD_add_response_header(response, "Content-Type", "application/json");
			MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Autoriser les requetes cross-origin

			// Envoyer la réponse
			int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

			MHD_destroy_response(response);
			return MHD_YES;
		}
		else
		{
			return MHD_NO;
		}
	}
	else
	{
		return MHD_NO;
	}*/
}

int http_api()
{
	//variable d'initialisation
	struct MHD_Daemon* daemon;

	// Démarrer le serveur HTTP
	daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, http_port, NULL, NULL, &reponce_request, NULL, MHD_OPTION_END);
	if (daemon == NULL)
	{
		log_message("ERREUR", "Impossible de démarrer le serveur HTTP");
		return(-1);
	}

	log_message("INFO", "Le serveur HTTP est démarré");

	return(1);
}
