/**
 * geonames.c
 *
 * Affiche les n villes les plus peuplees du monde
 *
 * @author Messan, Ekoue Andy Scott
 */

// Inclusion des bibliotheques
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

// Inclusion des constantes
#define USAGE "\
Usage: %s <NUMBER OF CITIES>\n\
\n\
Display a table of the n most populous cities on stdout. The number of cities must not exceed 5000.\n\
\n\
If no argument is provided, the program prints this help and exit.\n\
\n\
Program parameters :\n\
  NUMBER of cities         Number de cities to print (1..500)\n\
  "

#define FORMAT_TITRE    "%4s   %-20.20s   %-20.20s   %s\n"
#define FORMAT_COLONNES "%4d   %-20.20s   %-20.20s   %10ld\n"
#define CODE_ASCII_0 48
#define CODE_ASCII_9 57
#define NB_VILLES_MAX 5000
#define NB_VILLES_MIN 1
#define INFO_PAYS "countryInfo.txt"
#define INFO_VILLES "cities15000.txt"
#define DELIMITEUR "\t"
#define TAILLE_TABLEAU_VILLES 26268
#define TAILLE_TABLEAU_PAYS 254
#define LONGUEUR_MAX_LIGNE 7300
#define TAILLE_TAB_PARCOURS_PAYS 700
#define TAILLE_TAB_PARCOURS_VILLE 1000
#define LIMITE_TABLEAU_VILLE 20
#define NA "(NULL)"
#define TABULATION '\t'
#define LIMITE_TABLEAU_PAYS 4



// Declaration des types
/**
 * ----------------
 * Types de données
 * ----------------
 */
struct Pays {
    char nom[50]; // Le nom du pays
    char code[3]; // Le code de deux lettres identifiant le pays
};

struct Ville {
    char nom[100];    // Le nom ASCII de la ville
    long population;  // La population
    struct Pays pays; // Le pays dans laquelle la ville existe
};

/**
 * -----------------------
 * Codes d'erreur
 * -----------------------
 */

enum error {
    OK                      = 0, // Pas d'erreur
    ERREUR_NB_ARGS          = 1,
    ERREUR_NB_VILLES        = 2,
    ERREUR_ARGS_TYPE	    = 3
};

//Initialisation des fonctions
void validerNombreArguments(int,char[]) ;
void validerNombreVilles(int) ;
void verifierTypeArgument(char[]) ;
void trierStructureVille(struct Ville [] , int, int );
void afficher(struct Ville [], int ) ;
int trouverTailleTableau(char []);
void viderTab(char *);
void chargerVilles(char [], struct Ville *, struct Pays *);
void chargerPays(char [], struct Pays []);

// Declaration des fonctions

/**
 * Verifie s'il y a le bon nombre d'arguments
 *
 * @param nombreArguments Le nombre d'arguments saisis
 */
void validerNombreArguments(int nombreArguments,char nomArgument[]){
    enum error codeErreur = OK ;
    if(nombreArguments != 2){
        codeErreur = ERREUR_NB_ARGS ;
        fprintf(stderr,"nombre arguments invalide\n");
        printf(USAGE,nomArgument);
        exit(codeErreur);
    }
}

/**
 * Verifie s'il y a le bon nombre de villes (entre 1 et 5000 inclus)
 *
 * @param nombreDeVilles  Le nombre de villes a afficher
 */
void validerNombreVilles(int nombreDeVilles){
    enum error codeErreur = OK ;
    if(nombreDeVilles < NB_VILLES_MIN || nombreDeVilles > NB_VILLES_MAX){
        codeErreur = ERREUR_NB_VILLES ;
        fprintf(stderr,"nombre de ville invalide\n");
        exit(codeErreur) ;
    }
}

/**
 * Verifie si l'argument est un entier
 *
 * @param argument L'argument passe en parametre au main
 */
void verifierTypeArgument(char argument[]){
    enum error codeErreur = OK ;
    char valeur[strlen(argument)] ;
    strcpy(valeur,argument) ;
    int longueurArgument = strlen(valeur) ;
    for(int i = 0 ; i < longueurArgument ; i++){
        if( valeur[i] < CODE_ASCII_0 || valeur[i] > CODE_ASCII_9 ){
            codeErreur = ERREUR_ARGS_TYPE ;
            fprintf(stderr,"type argument invalide\n") ;
            exit(codeErreur) ;
        }
    }
}

/**
 * Permet de trier les villes par ordre croisssant
 *
 * @param lesVilles
 * @param plusPetitePositionTableau
 * @param plusGrandePositionTableau
 */
void trierStructureVille(struct Ville lesVilles[], int plusPetitePositionTableau, int plusGrandePositionTableau) {
    int i, j ;
    long population ;
    struct Ville uneVille ;

    i = plusPetitePositionTableau ;
    j = plusGrandePositionTableau ;

    int taille = (i + j) / 2 ;

    population = lesVilles[taille].population ;

    do {
        while((lesVilles[i].population < population) && (i < plusGrandePositionTableau) ) i++;
        while((lesVilles[j].population > population) && (j > plusPetitePositionTableau) ) j--;
        if(i <= j) {
            uneVille = lesVilles[i];
            lesVilles[i] = lesVilles[j];
            lesVilles[j] = uneVille;
            i++;
            j--;
        }
    } while(i <= j);

    if(plusPetitePositionTableau < j) trierStructureVille(lesVilles, plusPetitePositionTableau, j);
    if(i < plusGrandePositionTableau) trierStructureVille(lesVilles, i, plusGrandePositionTableau);

}

/**
 * Permet de creer des structures pays a partir des donnees du fichier en parametre
 * et de les ajouter a un tableau de pays
 *
 * @param cheminFichierALire
 * @param pays
 * @return un pointeur de Pays
 */
void chargerPays(char cheminFichierALire[], struct Pays pays[]){
    FILE *leFichier ;
    char uneLigneDuFichier[TAILLE_TAB_PARCOURS_PAYS] ;
    char *provisoir ;
    struct Pays unPays ;

    leFichier = fopen(cheminFichierALire, "r") ;

    if(leFichier == NULL){
        printf("Erreur %s\n",strerror(errno)) ;
        exit(1) ;
    }

    int j = 0 ;
    while(fgets(uneLigneDuFichier,TAILLE_TAB_PARCOURS_PAYS,leFichier) != NULL){
        if(uneLigneDuFichier[0] != '#')
        {
            int i = 0 ;
            provisoir = strtok(uneLigneDuFichier,DELIMITEUR) ;
            while (provisoir != 0 && i <= LIMITE_TABLEAU_PAYS) {
                if(i == 0){
                    strcpy(unPays.code,provisoir) ;
                }
                if(i == 4){
                    strcpy(unPays.nom,provisoir) ;
                }
                provisoir = strtok(0, DELIMITEUR);
                i++ ;
            }
            pays[j] = unPays ;
            j++;
        }
    }

    if(fclose(leFichier) == EOF){
        printf("Erreur lors de la fermeture du fichier.\n") ;
        exit(1) ;
    }
}

/**
 * Permet de vider un tableau
 *
 * @param tabAVider
 */
void viderTab(char *tabAVider){
    for(int i = 0 ; i < TAILLE_TAB_PARCOURS_VILLE ; i++) {
        if(tabAVider[i] != 0){
            tabAVider[i] = '\0' ;
        }
    }
}

/**
 *Permet de charger les donnees du fichier en parametre et de creer des villes
 * dans un tableeau de villes
 *
 * @param cheminFichierALire
 * @param villes
 * @param pays
 */
void chargerVilles(char cheminFichierALire[], struct Ville villes[], struct Pays *pays){
    FILE *leFichier ;
    char uneLigneDuFichier[LONGUEUR_MAX_LIGNE] ;
    char partieDeLaLigne[TAILLE_TAB_PARCOURS_VILLE] = "";
    char uneVilleString[LIMITE_TABLEAU_VILLE][TAILLE_TAB_PARCOURS_VILLE] ;
    struct Ville uneVille ;
    int curseur = 0 ;

    leFichier = fopen(cheminFichierALire, "r") ;

    if(leFichier == NULL){
        printf("Erreur %s\n",strerror(errno)) ;
        exit(1) ;
    }
    while (fgets(uneLigneDuFichier, LONGUEUR_MAX_LIGNE, leFichier) != NULL ) {
        int j = 0;
        int i = 0;
        int v = 0;
        while (j < LIMITE_TABLEAU_VILLE) {
            while (uneLigneDuFichier[i] != '\0') {
                v = 0 ;
                while (uneLigneDuFichier[i] != TABULATION && i <= LONGUEUR_MAX_LIGNE) {
                    partieDeLaLigne[v] = uneLigneDuFichier[i];
                    i++;
                    v++;
                }
                strcpy(uneVilleString[j], partieDeLaLigne);
                j++;
                if (uneLigneDuFichier[i + 1] == TABULATION) {
                    strcpy(uneVilleString[j], NA);
                    i++;
                    j++;
                }
                i++;
                viderTab((char *)&partieDeLaLigne);
            }
        }

        for (int valeur = 0; valeur < 15; valeur++) {
            if (valeur == 2) {
                strcpy(uneVille.nom, uneVilleString[valeur]);
            } else if (valeur == 8) {
                int c = 0;
                int valide = -1;
                while ((c < TAILLE_TABLEAU_PAYS) && (valide == -1)) {
                    if (strncmp(pays[c].code, uneVilleString[valeur], 2) == 0) {
                        strcpy(uneVille.pays.nom, pays[c].nom);
                        strcpy(uneVille.pays.code, pays[c].code);
                        valide = 0;
                    }
                    c++;
                }
            } else if (valeur == 14) {
                uneVille.population = atoi(uneVilleString[valeur]);
            }
        }
        villes[curseur] = uneVille;
        curseur++;
    }

    if(fclose(leFichier) == EOF){
        printf("Erreur lors de la fermeture du fichier.\n") ;
        exit(1) ;
    }

}


/**
 * Permet de trouver la taille adequate pour tableaux (dans ce cadre de pays et de villes)
 * a partir du fichier entre en parametre
 *
 * @param cheminFichierALire Le fichier a manipuler
 * @return         la taille du tableau
 */
int trouverTailleTableau(char cheminFichierALire[]){
    int tailleTab = 0 ;
    FILE *leFichierALire ;
    char uneLigneDuFichier[TAILLE_TAB_PARCOURS_VILLE] ;


    leFichierALire = fopen(cheminFichierALire, "r") ;

    if(leFichierALire == NULL){
        printf("Erreur %s\n",strerror(errno)) ;
        exit(1) ;
    }

    while(fgets(uneLigneDuFichier,TAILLE_TAB_PARCOURS_VILLE,leFichierALire) != NULL){
        if(uneLigneDuFichier[0] != '#'){
            tailleTab++ ;
        }

    }

    if(fclose(leFichierALire) == EOF){
        printf("Erreur lors de la fermeture du fichier.\n") ;
        exit(1) ;
    }

    return tailleTab ;
}


/**
 * Permet d'afficher le nombre de villes demandes
 *
 * @param villes
 * @param nbVilles
 */
void afficher(struct Ville villes[], int nbVilles){
    int tailleFinale =  TAILLE_TABLEAU_VILLES - 3 ;
    int v = 1 ;
    printf(FORMAT_TITRE,"Rang","Nom","Pays","Population") ;
    printf(FORMAT_TITRE,"----","---","----","----------") ;
    for(int j = tailleFinale ; j > (tailleFinale - nbVilles) ; j--){
        printf(FORMAT_COLONNES, v, villes[j].nom, villes[j].pays.nom, villes[j].population);
        v++;
    }
}

// Fonction main
int main(int argc, char *argv[]) {
    int nombreArgument = argc ;
    char *nomExecutable = argv[0] ;
    char *valeurNbVilles = argv[1] ;
    int nbVilles = 0 ;
    int tailleFichierVilles = 0 ;
    int tailleFichierPays = 0 ;

    validerNombreArguments(nombreArgument,nomExecutable) ;
    verifierTypeArgument(valeurNbVilles) ;

    nbVilles = atoi(argv[1]) ;
    validerNombreVilles(nbVilles) ;

    tailleFichierPays = trouverTailleTableau(INFO_PAYS) ;
    struct Pays pays[tailleFichierPays];

    tailleFichierVilles = trouverTailleTableau(INFO_VILLES) ;
    struct Ville villes[tailleFichierVilles];

    chargerPays(INFO_PAYS,(struct Pays *)&pays) ;

    chargerVilles(INFO_VILLES, (struct Ville *)&villes, pays);

    trierStructureVille((struct Ville *)&villes, 0, TAILLE_TABLEAU_VILLES) ;

    afficher(villes, nbVilles) ;

    return 0;
}
