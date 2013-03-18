#include "MPC5604B_M27V.h"
//#include "camera.h"
#include "milieu_ligne.h"
#include "controle.h"
#include "moteur.h"
#include "delay.h"
#include "globals.h"
#include "extern_globals.h"

int main(void) {

  init();
    
  // moteurs
   moteur_derniere_erreur = 0;
   moteur_integrale = 0;

  /* Loop forever */
  for (;;) {
  	//delay(5000);
    interruptionCamera();
    interruptionControle();
  }
}



