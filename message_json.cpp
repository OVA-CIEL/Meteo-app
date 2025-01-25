#include"config.h"
#include"meteo.h"

message_json::message_json()
{
	this->json = json_object_new_object();
	this->json_base_table = json_object_new_array();
	this->all_live = false;
	this->brut_live = false;
	this->future = false;
	this->erreur = false;
}

message_json::~message_json()
{
	json_object_put(this->json);
}

char* message_json::get_message_string()
{
	json_object_object_add(json, "meteo", json_base_table);
	const char* json_str = json_object_to_json_string(json);
	size_t json_str_len = strlen(json_str) + 1;

	char* json_response = (char*)malloc(json_str_len);
	if (json_response != NULL)
	{
		strncpy(json_response, json_str, json_str_len);
		json_response[json_str_len - 1] = '\0';
	}

	return json_response;
}

void message_json::inserte_graf(int timestampMin, int timestampMax, int nbPoint, char val[21])
{
	if (strcmp(val, "temperature") == 0 || strcmp(val, "humidite") == 0 || strcmp(val, "pression") == 0 || strcmp(val, "gaz") == 0)
	{ 
		json_object* json_table = json_object_new_object();
		json_object* json_data_table = json_object_new_array();

		json_object_object_add(json_table, "data_type", json_object_new_string("garf"));
		json_object_object_add(json_table, "data_type_valeur", json_object_new_string(val));

		double* resultat = NULL;
		int ligne = 0;

		if (strcmp(val, "gaz") == 0)
		{
			char query[250];
			snprintf(query, sizeof(query), "SELECT %s FROM data WHERE temps BETWEEN %d AND %d", val, timestampMin, timestampMax);
			database_recup_data(query, &ligne, &resultat);
		}
		else
		{
			char query[250];
			snprintf(query, sizeof(query), "SELECT %s / 100 FROM data WHERE temps BETWEEN %d AND %d", val, timestampMin, timestampMax);
			database_recup_data(query, &ligne, &resultat);
		}

		double* moyenne = NULL;
		double* pointMin = NULL;
		double* pointMax = NULL;

		moyenne = new double;
		pointMin = new double;
		pointMax = new double;

		*moyenne = 0;
		*pointMin = 0;
		*pointMax = 0;
		
		complex_data(json_data_table, resultat, nbPoint, ligne, moyenne, pointMin, pointMax);

		json_object_object_add(json_table, "data_timestampMin", json_object_new_int(timestampMin));
		json_object_object_add(json_table, "data_timestampMax", json_object_new_int(timestampMax));
		json_object_object_add(json_table, "data_moyenne", json_object_new_double(*moyenne));
		json_object_object_add(json_table, "data_max", json_object_new_double(*pointMin));
		json_object_object_add(json_table, "data_min", json_object_new_double(*pointMax));

		json_object_object_add(json_table, "data_list_point", json_data_table);

		json_object_array_add(json_base_table, json_table);

		delete moyenne;
		delete pointMin;
		delete pointMax;
	}
}

void message_json::inserte_brut_live()
{
	if (brut_live == false && erreur == false)
	{
		json_object* json_table = json_object_new_object();
		json_object* json_data_table = json_object_new_object();

		json_object_object_add(json_table, "data_type", json_object_new_string("data_brut_live"));
		json_object_object_add(json_table, "data_time", json_object_new_int(meteo_data_time));

		json_object_object_add(json_data_table, "temperature", json_object_new_double(meteo_data_temperature));
		json_object_object_add(json_data_table, "humidite", json_object_new_double(meteo_data_humidite));
		json_object_object_add(json_data_table, "pression", json_object_new_double(meteo_data_pression));
		json_object_object_add(json_data_table, "gaz_resistance", json_object_new_int(meteo_data_gaz));

		json_object_object_add(json_table, "data", json_data_table);

		json_object_array_add(json_base_table, json_table);

		brut_live = true;
	}
}

void message_json::inserte_all_live()
{
	if (all_live == false && erreur == false)
	{
		json_object* json_table = json_object_new_object();
		json_object* json_data_table = json_object_new_object();

		float data_temperature = (float)meteo_data_temperature / 100;
		float data_humidite = (float)meteo_data_humidite / 100;
		float data_pression = (float)meteo_data_pression;
		int data_gaz = meteo_data_gaz;
		double data_point_rosee = -1;
		double data_humidex = -1;
		if (data_temperature > 0 && data_temperature < 60)
		{
			const double a = 17.27;
			const double b = 237.7;

			double alpha = (a * data_temperature) / (b + data_temperature) + log(data_humidite / 100);

			data_point_rosee = (b * alpha) / (a - alpha);

			if (data_point_rosee >= 60 && data_point_rosee <= 0)
			{
				data_point_rosee = -1;
			}
			else
			{
				double e = 6.11 * pow(10, (7.5 * data_point_rosee) / (b + data_point_rosee));
				data_humidex = data_temperature + (5 / 9) * (e - 10);
			}
		}

		double data_altitude = 555.53;

		const double T0 = 288.15; // Température standard au niveau de la mer (en Kelvin)
		const double L = 0.0065;  // Gradient thermique (K/m)
		const double g = 9.80665; // Accélération gravitationnelle (m/s²)
		const double R = 8.3144598; // Constante universelle des gaz parfaits (J/(mol·K))
		const double M = 0.0289644; // Masse molaire de l'air (kg/mol)

		double data_pression_niveau_mer = data_pression * pow(1 - (L * data_altitude / T0), -g * M / (R * L));

		double data_IAQ = 100 * (data_gaz / 10000);

		json_object_object_add(json_table, "data_type", json_object_new_string("data_all_live"));
		json_object_object_add(json_table, "data_time", json_object_new_int(meteo_data_time));

		json_object_object_add(json_data_table, "temperature", json_object_new_double(data_temperature));
		json_object_object_add(json_data_table, "humidite", json_object_new_double(data_humidite));
		json_object_object_add(json_data_table, "pression", json_object_new_double(data_pression));
		json_object_object_add(json_data_table, "pression_niveau_mer", json_object_new_double(data_point_rosee));
		json_object_object_add(json_data_table, "gaz", json_object_new_int(data_gaz));
		json_object_object_add(json_data_table, "data_point_rosee", json_object_new_double(data_point_rosee));
		json_object_object_add(json_data_table, "data_humidex", json_object_new_double(data_humidex));
		json_object_object_add(json_data_table, "IAQ", json_object_new_double(data_IAQ));
		json_object_object_add(json_data_table, "altitude", json_object_new_double(data_altitude));

		json_object_object_add(json_table, "data", json_data_table);

		json_object_array_add(json_base_table, json_table);

		all_live = true;
	}
}

void message_json::inserte_future()
{
}

void message_json::inserte_erreur()
{
}
