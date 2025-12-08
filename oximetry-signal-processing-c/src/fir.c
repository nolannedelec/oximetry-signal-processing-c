#include "fir.h"
#include "fichiers.h"
#include <stdio.h>


//variables globales, pour alloué une seule fois la mémoire et rester accessible tout au long du programme
//(eviter une réallocation de mémoire à chaque appel de la fonction)
#define ordre_filtre 51           //ordre fu filtre
const float coef_fir[51]={        // coefficients k du filtre FIR listés dans le tableau 3 du cours
    1.4774946e-019,         // obtenus via une fenêtre de Hamming
    1.6465231e-004,         // permettent de filtrer es composantes ACr et ACir en conservatn DCr et DCir
    3.8503956e-004,
    7.0848037e-004,
    1.1840522e-003,
    1.8598621e-003,
    2.7802151e-003,
    3.9828263e-003,
    5.4962252e-003,
    7.3374938e-003,
    9.5104679e-003,
    1.2004510e-002,
    1.4793934e-002,
    1.7838135e-002,
    2.1082435e-002,
    2.4459630e-002,
    2.7892178e-002,
    3.1294938e-002,
    3.4578348e-002,
    3.7651889e-002,
    4.0427695e-002,
    4.2824111e-002,
    4.4769071e-002,
    4.6203098e-002,
    4.7081811e-002,
    4.7377805e-002,
    4.7081811e-002,
    4.6203098e-002,
    4.4769071e-002,
    4.2824111e-002,
    4.0427695e-002,
    3.7651889e-002,
    3.4578348e-002,
    3.1294938e-002,
    2.7892178e-002,
    2.4459630e-002,
    2.1082435e-002,
    1.7838135e-002,
    1.4793934e-002,
    1.2004510e-002,
    9.5104679e-003,
    7.3374938e-003,
    5.4962252e-003,
    3.9828263e-003,
    2.7802151e-003,
    1.8598621e-003,
    1.1840522e-003,
    7.0848037e-004,
    3.8503956e-004,
    1.6465231e-004,
    1.4774946e-019
};

//fonction FIR, elle ne fonctionne que pour un quatuor de valeure donnée c'est pourquoi il faut un while dans firtest() pour lire chacune des lignes
absorp fir(absorp input) {

    // buffer circulaire 

    //on utilise des static pour conserver les valeurs lors des différents appels de fonctions
    static float acrBuffer[ordre_filtre] = {0}; // le buffer sert a stocker les 51 dernieres valeurs de ACr pour faire le calcul de y[n]
    static float acirBuffer[ordre_filtre] = {0}; // de même mais pour ACir
    static int index = 0;  //represente l'endroit ou la derniere valeure a été stockée.
    //a chaque nouvel échantillon d'entrée, l'index index indique où insérer la nouvelle valeur dans le buffer
    //l'accès aux valeurs passées dans le buffer se fait via une simple opération de décalage modulo (buffer circulaire), 
    //au lieu d'un décalage manuel (buffer classique)
    

    absorp myAbsorp = {0, 0, 0, 0}; //initialisation de notre sortie (acr, dcr, acir, dcir)
    
    //ajouter les nouvelles valeur aux buffers circulaires
    acrBuffer[index] = input.acr;  
    acirBuffer[index] = input.acir;
    
    //initialiser nos valeurs filtrés
    float acr_filtre = 0.0f;
    float acir_filtre = 0.0f;
    
    //     
    for (int k = 0; k < ordre_filtre; k++) {                  // k représente l'indice des coefficients h du calcul
        int idx = (index - k + ordre_filtre) % ordre_filtre;  //vu qu'on veut acceder aux valeurs passés du buffer on recule de i positions.
                                            //on veut s'assurer d'être dqns les bornes de ordre_filtre donc on l'ajjoute et on fait un modulo.

        //calcul des y[n]
        acr_filtre += coef_fir[k] * acrBuffer[idx];
        acir_filtre += coef_fir[k] * acirBuffer[idx];
    }
    
    // Mettre à jour l'index du buffer circulaire
    index = (index + 1) % ordre_filtre;
    
    // Remplir la structure de sortie
    myAbsorp.acr = acr_filtre;      //ACr calculé
    myAbsorp.dcr = input.dcr;       //DCr non changé
    myAbsorp.acir = acir_filtre;    //ACir calculé
    myAbsorp.dcir = input.dcir;     //DCir non changé
    
    return myAbsorp;        //On renvoit notre sortie
}

// Fonction firTest pour lire un fichier et appliquer le filtre FIR
absorp firTest(char* filename) {

    FILE* file = initFichier(filename); //on ouvre le fichier, si il n'y en a pas on renvoit un message d'erreur

    absorp donnees_fichier, myAbsorp;   //initialisation de l'entrée et de la sortie de la fonction fir
    
    do {
        int etat;   //etat sert a savoir a quel ligne on est dans le fichier, et donc SURTOUT si on est a la fin du fichier
        donnees_fichier = lireFichier(file, &etat); // on lit le fichier et met les valeurs dans donnees_fichier
        if(etat==EOF){  //lorsqu'on arrive a la fin du fichier (eof), on casse le while.
            break;
        }

        myAbsorp = fir(donnees_fichier);        //on envoit les données du fichier dans notre fonction fir, qui nous renvoir ces données filtrées
    } while (!feof(file));
    
    fclose(file);
    return myAbsorp;        //on renvoit notre sortie
}


