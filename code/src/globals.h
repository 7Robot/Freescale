int8_t camera_valeurs[128];
int pos_min_servo; //845
int pos_max_servo; //1417
int pos_milieu_servo; //1126
float objectif_vitesse;
float moteur_derniere_erreur;
float moteur_integrale;
uint16_t moteur_compteur;
int16_t controle_derniere_erreur;
float controle_integrale;
uint8_t controle_derniere_position;
int8_t main_fin_boucle;
float main_timer_period;
uint8_t	Capteurs_resultat;
uint16_t compteur_acquisitions_invalides;
//pepino
uint8_t i_camera_interupt;
uint16_t camera_valeurs_brutes1[128];
uint16_t camera_valeurs_brutes2[128];