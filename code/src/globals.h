int8_t camera_valeurs[128];
int pos_min_servo; //845
int pos_max_servo; //1417
int pos_milieu_servo; //1126
float objectif_vitesse;
float moteur_derniere_erreur;
float moteur_integrale;
uint8_t moteur_compteur;
int8_t controle_derniere_erreur;
int16_t controle_integrale;
uint8_t controle_derniere_position;
int8_t main_fin_boucle;
float main_timer_period;
uint8_t	Capteurs_resultat;
uint8_t CONTROLE_KP;
uint8_t CONTROLE_KD;
uint8_t CONTROLE_KI;
uint16_t compteur_acquisitions_invalides;