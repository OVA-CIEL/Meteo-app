#include"config.h"
#include"meteo.h"

void traitement_data(char message[reseaux_port_ecoute], int nb_bytes)
{
    float temperature, humidite, pression, gaz;
    log_message("INFO", "Traitement des données");
    //traitement des données
    if (nb_bytes > 10 && nb_bytes < 100)
    {
        int result = sscanf(message, "T:%f/H:%f/P:%f/G:%f",
            &temperature, &humidite, &pression, &gaz);

        if (result != 4) {
            log_message("ERREUR", "Format de message incorrect");
            return;
        }

        if (temperature > -30 && temperature < 80 &&
            humidite > 0 && humidite < 100 &&
            pression > 30000 && pression < 110000 &&
            gaz > 0 && gaz < 1000000)
        {
            int temps = time(NULL);
            meteo_data_time = temps;
            meteo_data_temperature = (int)(temperature * 100);
            meteo_data_humidite = (int)(humidite * 100);
            meteo_data_pression = (int)pression;
            meteo_data_gaz = (int)gaz;

            if (database_etat() == -1)
            {
                log_message("ERREUR", "Impossible de se connecter à la base de données");
                return;
            }
            else
            {
                database_envoie_data(meteo_data_time, meteo_data_temperature, meteo_data_humidite, meteo_data_pression, meteo_data_gaz);
            }

            log_message("INFO", "Données traitées");
        }
        else
        {
            log_message("ERREUR", "Données interpréter incorrectes");
        }
    }
    else
    {
        log_message("WARNING", "Données reçues incorrectes");
        return;
    }
}

void complex_data(json_object* json, double* Point, int nbPoint, int ligne, double* result_moyenne, double* PointMin, double* PointMax)
{
    double ratio = (double)ligne / nbPoint;

    if (ratio >= 1)
    {
        double moyenne = 0;
        double block = ratio;
        
        for (int i = 0; i < nbPoint; i++)
        {
            int block_start = (int)(i * block);
            int block_end = (int)((i + 1) * block);
            double block_moyenne = 0;
            int block_nombre_interne = 0;
            double resultat = 0;

            if (block_end > ligne)
            {
                block_end = ligne;
            }
            for (int j = block_start; j < block_end; j++)
            {
                block_moyenne = block_moyenne + Point[j];
                moyenne = moyenne + Point[j];
                block_nombre_interne++;
                if (Point[j] > *PointMax)
                {
                    *PointMax = Point[j];
                }
                if (Point[j] < *PointMin)
                {
                    *PointMin = Point[j];
                }
            }

            resultat = block_moyenne / block_nombre_interne;
            json_object_array_add(json, json_object_new_double(resultat));
        }

        *result_moyenne = moyenne / ligne;

    }
    else if (ratio < 1 && ratio != 0)
    {
        double step = (double)(ligne - 1) / (nbPoint - 1);
        double moyenne = 0;

        for (int i = 0; i < nbPoint; i++) {

            // Calculer la position interpolée
            double position = i * step;
            int index1 = (int)position;           
            int index2 = index1 + 1;             
            double fraction = position - index1;

            if (index2 >= ligne) index2 = nbPoint - 1;

            // Calcul de la valeur interpolée
            double interpolated_value = Point[index1] + (Point[index2] - Point[index1]) * fraction;

            json_object_array_add(json, json_object_new_double(interpolated_value));

            for (int i = 0; i < ligne; i++)
            {
                moyenne = moyenne + Point[i];
                if (Point[i] > *PointMax)
                {
                    *PointMax = Point[i];
                }
                if (Point[i] < *PointMin)
                {
                    *PointMin = Point[i];
                }
            }

            *result_moyenne = moyenne / ligne;
        }

    }
    else if (ratio == 0)
    {
        json_object_array_add(json, json_object_new_double(0));
    }
}

char* erreur_data()
{
    // Créer un objet JSON
    json_object* json = json_object_new_object();

    // Ajouter les données
    json_object_object_add(json, "erreur", json_object_new_boolean(true));

    // Convertir l'objet JSON en chaîne de caractères
    const char* json_str = json_object_to_json_string(json);
    char* json_response = (char*)malloc(strlen(json_str) + 1);
    if (json_response != NULL)
    {
        strcpy(json_response, json_str);
    }

    // Libérer la mémoire
    json_object_put(json);

    return json_response;
}