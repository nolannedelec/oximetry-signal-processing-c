#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mesure.h"
#include "fichiers.h"


oxy mesure(absorp myAbsorp,float ptp_acr, float ptp_acir, int i ){ // on fait le calcule de R puis de la valeur de spo2
    oxy myOxy;                                              // avec spo2 = 110 - 25*(R/(R+1))
    float R, spo2_float;
    int bpm = 0;
    R = (ptp_acr/myAbsorp.dcr)/(ptp_acir/myAbsorp.dcir);      // calcule de R
    if (R<=1){
        spo2_float=-25*R+110;  // on integre la valeur du spo2 au spo2 de myOxy qui est calculer à l'aide de l'équation à la droite que l'on nous a fournie
        int spo2_int = (int)round(spo2_float);
        myOxy.spo2=spo2_int;
    }
    else if (R>1){
        spo2_float=-35*R+120;  // on integre la valeur du spo2 au spo2 de myOxy
        int spo2_int = (int)round(spo2_float);
        myOxy.spo2=spo2_int;
    }

    bpm = 60 / (((i) * 4) / 1000.0);
    // Correction du facteur 2,on fait *4 car *2 pour les ms et *2 car nous prenons un min et un max
    myOxy.pouls=bpm;

    return myOxy;    // on retourne myOxy
}


oxy mesureTest(char* filename){
    oxy myOxy;
    absorp myAbsorp;
    int etat=0;
    FILE*fichier=initFichier(filename); // j'initialise le fichier avec la fonction initfichier dans le fichier.c
    float peak_b_acr =  -700, peak_h_acr =  700, peak_b_acir =  -700, peak_h_acir =  700; // on initialise à de petite valeur peak_h pour qu'il soit bien actualisé et inversement pour peak_b
    float acr_p = 0, acir_p= 0;  // sert à stocker les valeur en mémoir pour réutiliser et actualiser les peak to peak
    float ptp_acr = 0, ptp_acir = 0; // les valeurs peak to peak

   //valeur en mémoir pour le bpm

    int i1_acr = 0, i2_acr = 0, i1_acir=0, i2_acir=0 ,i_acr=0, i_acir=0,i=0;
    int index = 0; // Compteur de temps (chaque itération = 2ms) va prendre toute les itérations
    int etat_signal_acr = -1; // -1: inconnu, 0: en descente, 1: en montée
    int etat_signal_acir = -1; // -1: inconnu, 0: en descente, 1: en montée


    // Lire la première ligne du fichier
    myAbsorp = lireFichier(fichier, &etat);




    while (etat !=EOF) // ce while permet de calculer les valeurs des pique haut et bas sans se préocuper de valeurs entre.
    // Elles calcule aussi le temps entre un pique haut et bas.
    {
        // Détection du pic bas (i1) - Minimum local
        // si l'état du signal est à 0 alors la courbe diminue donc une condition de validé.
        // si myAbsorbe c'est à dire la valeur actuelle de acr est supérieur à la valeur acr_p, la valeur précédente alors cela veut diire que nous remontons donc peak bas
        if (etat_signal_acr == 0 && myAbsorp.acr > acr_p) {
            peak_b_acr = acr_p;
            i2_acr = index;
            etat_signal_acr = 1;
            // Calcul du BPM si un cycle complet est détecté
            i_acr = i2_acr-i1_acr;
        }
        // Détection du pic haut (i2) - Maximum après i1
        if (etat_signal_acr != 0 && myAbsorp.acr < acr_p) {
            peak_h_acr = acr_p;
            i1_acr = index;
            etat_signal_acr = 0;
        }
       
        if (etat_signal_acir == 0 && myAbsorp.acir > acir_p) {
            peak_b_acir = acir_p;
            i2_acir=index;
            etat_signal_acir = 1;
            i_acir = i2_acir-i1_acir;

        }
        if (etat_signal_acir != 0 && myAbsorp.acir < acir_p) {
            peak_h_acir = acir_p;
            i1_acir=index;
            etat_signal_acir = 0;
        }
       
        ptp_acr = fabs(peak_h_acr-peak_b_acr);    // valeur absolue pour obtenir le peak to peak
        ptp_acir = fabs(peak_h_acir-peak_b_acir);

        i = (i_acr+i_acir)/2; //moyenne des deux périodes acr et acir

        myOxy = mesure(myAbsorp,ptp_acr, ptp_acir,i );

        index++; // Chaque itération = 2ms

        acr_p = myAbsorp.acr;  // on réactualise les valeurs de acr_a et acir_a qui sont les valeur actuelle de myAbsorp
        acir_p = myAbsorp.acir; // cela nous permet d'avoir des valeurs de ptp qui suivent les variations
        // Lire la prochaine ligne

        myAbsorp = lireFichier(fichier, &etat);
    }
   
    // Debugging affichage

    return myOxy;

}