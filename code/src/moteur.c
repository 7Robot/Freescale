#include "moteur.h"
#include "constantes.h"
#include "liaison_serie.h"
#include "extern_globals.h"
#include "Config_PIT.h"

void Compteur_Moteur(void)
{
        moteur_compteur++;
        
        EMIOS_0.CH[11].CSR.B.FLAG = 0x1;
}

void Asserv_Vitesse(void)
{
        float erreur;
        float derivee;
        int32_t commande;
    
    	erreur = objectif_vitesse - moteur_compteur;
                
        derivee = erreur - moteur_derniere_erreur;
        moteur_integrale +=  erreur;
        commande = MOTEUR_KP * erreur + MOTEUR_KD * derivee + MOTEUR_KI * moteur_integrale;

    moteur_derniere_erreur = erreur;

    // objectif_vitesse
    commande = commande < 0 ? 0 : commande;
    //Moteurs en série:
    //EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + 800;// Moteurs en serie
    //Moteurs en parallèle : 
    EMIOS_0.CH[6].CBDR.R = EMIOS_0.CH[6].CADR.R + commande;//HBridge gauche
    EMIOS_0.CH[7].CBDR.R = EMIOS_0.CH[7].CADR.R + commande;//HBridge Droit

        moteur_compteur = 0;

        // on indique que l'interruption est finie
        PIT_ClearFlag(1);

}