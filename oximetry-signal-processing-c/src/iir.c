#include <stdio.h>
#include <stdlib.h>
#include "iir.h"
#include "fichiers.h"

const float alpha=0.992;

absorp iir(absorp x, absorp x_n_1, absorp y_n_1){ // cette fonction s'occupe du calcule de façon récursive et retourn myabsorb

    absorp myAbsorp; //Le résulatat que l'on veut
    // je fais la fonction filtre y(n) = x(n) - x(n-1) + alpha*y(n-1)
    myAbsorp.acr = x.acr - x_n_1.acr + alpha*y_n_1.acr; //traitemant des données sur les variations PP de la lumière rouge
    myAbsorp.acir = x.acir - x_n_1.acir + alpha*y_n_1.acir; //traitemant des données sur les variations PP de la lumière infra rouge
    myAbsorp.dcir = x.dcir;
    myAbsorp.dcr = x.dcr;


    return myAbsorp;

}

absorp iirTest(char* filename){ // cette fonction lit le fichier et boucle le calcule de la fonction calcule et retourne myabsorb
    int etat=0;
    FILE*fichier=initFichier(filename); // j'initialise le fichier avec la fonction initfichier dans le fichier.c
    absorp x; // Les mesures entrée actuelle
    absorp x_n_1={0, 0, 0, 0}; // les mesures précédentes
    absorp y_n_1={0, 0, 0, 0}; // les mesures de sortie précédentes
    absorp myAbsorp = {0, 0, 0, 0};  // Résultat actuel

    x=lireFichier(fichier, &etat);  // cette fonction lit le fichier ********

    while (etat!=EOF)
    {

        myAbsorp = iir(x, x_n_1, y_n_1); // on fait les calcule ici
        x_n_1=x;
        y_n_1=myAbsorp;   // on met à jour le x[n-1] et y[n-1] en prenant les valeurs précédente

        // Lire la prochaine ligne
        x = lireFichier(fichier, &etat); // on prend la nouvelle valeur de x
    }

    return myAbsorp; // on retourne la valeur final de absorp
}
