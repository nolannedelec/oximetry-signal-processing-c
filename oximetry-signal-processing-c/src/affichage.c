#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "define.h"


//si un fichier verrou existe

void affichage(oxy myOxy) {
    // Vérifier si le fichier de verrou existe
    if (access(".verrouData", F_OK) != -1) {    //si un fichier '.verrouData' existe alors on ne peut pas modifier le fichier 'data.txt'
    	printf("fichier verrouillé.\n");		//message d'erreur dans le terminal pour savoir.
        return;									//on quitte la fonction
    }

    //création du fichier verrou.
    FILE *verrou = fopen(".verrouData", "w");	//on crée un fichier verrou
    if (!verrou) {
        perror("erreur création du verrou");	//au cas ou le fichier verrou ne se crée pas, on informe dans le terminal
        return;
    }
    fclose(verrou);

    // Ouverture de data.txt pour écriture
    FILE *file = fopen("data.txt", "w");
    if (!file) {
        perror("erreur ouverture du fichier data.txt");
        remove(".verrouData");  // Supprimer le verrou en cas d'erreur
        return;
    }

    // Écriture des données

    fprintf(file,"%d\n", myOxy.spo2);
    fprintf(file, "%d", myOxy.pouls);
    fclose(file);

    // Suppression du fichier de verrou après écriture
    remove(".verrouData");
}