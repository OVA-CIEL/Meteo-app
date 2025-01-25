#include"config.h"
#include"meteo.h"

static MHD_Result reponce_request(void* cls, struct MHD_Connection* connection, const char* url, const char* method, const char* version, const char* upload_data, size_t* upload_data_size, void** con_cls) {
    
	// V�rifier la m�thode HTTP

	if (strcmp(method, "GET") == 0) {

		if (strcmp(url, "/data-loc") == 0)
		{

			message_json message;

			int timestampMin = 0;
			int timestampMax = 0;
			int nbPoint = 1;
			char val[21];
			char* json_response = NULL;

			const char* ParamTimestampMin = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "timestampMin");
			const char* ParamTimestampMax = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "timestampMax");
			const char* ParamNbPoint = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "nbPoint");
			const char* ParamVal = MHD_lookup_connection_value(connection, MHD_GET_ARGUMENT_KIND, "val");

			bool trace_graf = true;

			if (ParamTimestampMin != NULL) {
				timestampMin = atoi(ParamTimestampMin);
			}
			else 
			{
				timestampMin = -1;
				message.inserte_brut_live();
				trace_graf = false;
			}

			if (ParamTimestampMax != NULL) {
				timestampMax = atoi(ParamTimestampMax);
			}
			else 
			{
				timestampMax = -1;
				message.inserte_brut_live();
				trace_graf = false;
			}

			if (ParamNbPoint != NULL) 
			{
				nbPoint = atoi(ParamNbPoint);
			}
			else 
			{
				nbPoint = 1;
				message.inserte_brut_live();
				trace_graf = false;
			}

			if (ParamVal != NULL && trace_graf == true)
			{
				strcpy(val, ParamVal);
				message.inserte_graf(timestampMin, timestampMax, nbPoint, val);
			}
			else
			{
				message.inserte_brut_live();
			}
			// G�n�rer les donn�es en JSON

			json_response = message.get_message_string();
			
			struct MHD_Response* response = MHD_create_response_from_buffer(strlen(json_response), (void*)json_response, MHD_RESPMEM_MUST_FREE);
			MHD_add_response_header(response, "Content-Type", "application/json");
			MHD_add_response_header(response, "Access-Control-Allow-Origin", "*"); // Autoriser les requetes cross-origin

			// Envoyer la r�ponse
			int ret = MHD_queue_response(connection, MHD_HTTP_OK, response);

			MHD_destroy_response(response);

			return MHD_YES;
		}
		
		return MHD_NO;
	}
	return MHD_NO;
}

int http_api()
{
	//variable d'initialisation
	struct MHD_Daemon* daemon;

	// D�marrer le serveur HTTP
	daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, http_port, NULL, NULL, &reponce_request, NULL, MHD_OPTION_END);
	if (daemon == NULL)
	{
		log_message("ERREUR", "Impossible de d�marrer le serveur HTTP");
		return(-1);
	}

	log_message("INFO", "Le serveur HTTP est d�marr�");

	return(1);
}
