#include "define.h"
#include "fir.h"
#include "iir.h"
#include "mesure.h"
#include "affichage.h"
#include "fichiers.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    char* filename = "data/log1.dat";		
    FILE *file = initFichier(filename);	//on ouvre le fichier, si il n'y en a pas on renvoit un message d'erreur

    //on definit nos variables
    absorp donnees, donnees_apres_filtres_fir, donnees_apres_filtres_iir;
    oxy donnees_apres_mesure;
    
    absorp x_n_1 = {0, 0, 0, 0};
    absorp y_n_1 = {0, 0, 0, 0};
    
    int etat;
    float peak_b_acr = -700, peak_b_acir = -700;
    float peak_h_acr = 700, peak_h_acir = 700;
    float ptp_acr = 0, ptp_acir = 0;
    int index = 0;
    int etat_signal_acr = -1;
    int etat_signal_acir = -1;
    int i1 = -1, i2 = -1, i = 0;
    
    do {
        donnees = lireFichier(file, &etat); // on lit le fichier et met les valeurs dans donnees
        if (etat == EOF) break; //lorsqu'on arrive a la fin du fichier (eof), on casse le while.
        
        donnees_apres_filtres_fir = fir(donnees);		//on envoit les données du fichier dans notre fonction fir, qui nous renvoir ces données filtrées
        donnees_apres_filtres_iir = iir(donnees_apres_filtres_fir, x_n_1, y_n_1);



        if (etat_signal_acr == 0 && donnees_apres_filtres_iir.acr > y_n_1.acr) {
            peak_b_acr = y_n_1.acr;
            etat_signal_acr = 1;
            i2=index;
            i=i2-i1;
        }
        if (etat_signal_acr != 0 && donnees_apres_filtres_iir.acr < y_n_1.acr) {
            peak_h_acr = y_n_1.acr;
            etat_signal_acr = 0;
            i1=index;
            

        }
       
        if (etat_signal_acir == 0 && donnees_apres_filtres_iir.acir > y_n_1.acir) {
            peak_b_acir = y_n_1.acir;
            etat_signal_acir = 1;
        }
        if (etat_signal_acir != 0 && donnees_apres_filtres_iir.acir < y_n_1.acir) {
            peak_h_acir = y_n_1.acir;
            etat_signal_acir = 0;
        }



        ptp_acr = peak_b_acr - peak_h_acr;
        ptp_acir = peak_b_acir - peak_h_acir;
        

        donnees_apres_mesure = mesure(donnees_apres_filtres_iir, ptp_acr, ptp_acir, i);
        affichage(donnees_apres_mesure);



        // Mise à jour des valeurs précédentes pour l'IIR
        x_n_1 = donnees_apres_filtres_fir;
        y_n_1 = donnees_apres_filtres_iir;     


        index++;
    } while (!feof(file));
    
    fclose(file);
    return 0;
}
