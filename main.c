#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX 40

// Couleurs ANSI
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define MAGENTA "\033[35m"
#define WHITE   "\033[37m"

struct joueur {
    int id;
    char full_name[40];
    int numeroMaillot;
    char poste[20];
    int age;
    int buts;
    char dateInscription[20];
    char statut[20]; 
};

// Prototypes 
void ajouter_joueur(struct joueur joueurs[], int *nbjoueur, int *next_id);
void ajouter_plusieurs_joueurs(struct joueur joueurs[], int *nbjoueur, int *next_id);
void afficher_les_joueurs(struct joueur joueurs[], int nbjoueur);
void rechercher_joueur(struct joueur joueurs[], int nbjoueur);
void modifier_joueur(struct joueur joueurs[], int nbjoueur);
void supprimer_joueur(struct joueur joueurs[], int *nbjoueur);
void statistiques(struct joueur joueurs[], int nbjoueur);
void sauvegarder_fichier(struct joueur joueurs[], int nbjoueur);
void charger_fichier(struct joueur joueurs[], int *nbjoueur, int *next_id);

// my own functions
int my_strcmp(const char s1[], const char s2[]) {
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i]) return s1[i] - s2[i];
        i++;
    }
    return s1[i] - s2[i];
}

//verifier star 
void verifier_stars(struct joueur joueurs[], int *nbjoueur) {
    int i = 0;
    while (i < *nbjoueur) {
        if (joueurs[i].buts >= 10) {
            printf(YELLOW ">>> %s est une STAR de l'équipe avec %d buts !\n" RESET,
                   joueurs[i].full_name, joueurs[i].buts);
        }
        i++;
    }
}

// Date automatique
void date_aujourdhui(char buffer[]) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(buffer, "%02d/%02d/%04d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

//  ajouter joueur 
void ajouter_joueur(struct joueur joueurs[], int *nbjoueur, int *next_id) {
    if (*nbjoueur >= MAX) {
        printf(RED "La liste est pleine.\n" RESET);
        return;
    }
    int existe;
    int i = 0;
    int valid;
    int choix;
    int jour, mois, annee;
    int joursParMois[] = {0,31,28,31,30,31,30,31,31,30,31,30,31}; 
    
    // Validation du nom - CORRIGÉ
    do {
        valid = 1; // Assume valid initially
        printf(CYAN "Nom complet : " RESET);
        scanf(" %[^\n]", joueurs[*nbjoueur].full_name);
        i = 0;
        while(joueurs[*nbjoueur].full_name[i] != '\0') {
            if (joueurs[*nbjoueur].full_name[i] >= '0' && joueurs[*nbjoueur].full_name[i] <= '9') {
                printf(RED "Erreur: Le nom ne doit pas contenir de chiffres.\n" RESET);
                valid = 0;
                break;
            }
            i++;
        }
    } while(valid == 0);

    // Validation de l'âge
    do {
        printf(CYAN "Âge (l'âge doit être entre 15 et 50): " RESET);
        scanf("%d", &joueurs[*nbjoueur].age);
        if(joueurs[*nbjoueur].age < 15 || joueurs[*nbjoueur].age > 50) {
            printf(RED "Erreur: l'âge doit être entre 15 et 50. Réessayez : " RESET);
        }
    } while(joueurs[*nbjoueur].age < 15 || joueurs[*nbjoueur].age > 50);

    // Validation du numéro de maillot
    do {
        existe = 0;  // reset flag
        printf(CYAN "Numéro de maillot : " RESET);
        scanf("%d", &joueurs[*nbjoueur].numeroMaillot);
        for (i = 0; i < *nbjoueur; i++) {
            if(joueurs[i].numeroMaillot == joueurs[*nbjoueur].numeroMaillot) {
                existe = 1;
                printf(RED "Erreur : ce numéro de maillot est déjà pris par %s. Réessayez.\n" RESET, joueurs[i].full_name);
                break;
            }
        }
    } while(existe == 1);

    // Choix du poste
    do {
        printf(CYAN "Choisissez le poste :\n" RESET);
        printf("1. Gardien\n");
        printf("2. Défenseur\n");
        printf("3. Milieu\n");
        printf("4. Attaquant\n");
        printf("Votre choix : ");
        scanf("%d", &choix);
        switch(choix) {
            case 1: strcpy(joueurs[*nbjoueur].poste, "gardien"); break;
            case 2: strcpy(joueurs[*nbjoueur].poste, "defenseur"); break;
            case 3: strcpy(joueurs[*nbjoueur].poste, "milieu"); break;
            case 4: strcpy(joueurs[*nbjoueur].poste, "attaquant"); break;
            default:
                printf(RED "Erreur : choix invalide. Réessayez.\n" RESET);
                choix = 0;
        }
    } while(choix < 1 || choix > 4);

    // Choix du statut
    do {
        printf(CYAN "Choisissez le statut :\n" RESET);
        printf("1. Titulaire\n");
        printf("2. Remplaçant\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: strcpy(joueurs[*nbjoueur].statut, "titulaire"); break;
            case 2: strcpy(joueurs[*nbjoueur].statut, "remplacant"); break;
            default:
                printf(RED "Erreur : choix invalide. Réessayez.\n" RESET);
                choix = 0;
        }
    } while (choix < 1 || choix > 2);

    // Nombre de buts
    printf(CYAN "Nombre de buts : " RESET);
    scanf("%d", &joueurs[*nbjoueur].buts);

    // Validation de la date d'inscription
    do {
        printf(CYAN "Date d'inscription (jj/mm/aaaa) : " RESET);
        scanf("%d/%d/%d", &jour, &mois, &annee);
        
        int bissextile = 0;
        if((annee % 4 == 0 && annee % 100 != 0) || (annee % 400 == 0)) {
            bissextile = 1;
        }
        if(bissextile) {
            joursParMois[2] = 29;
        } else {
            joursParMois[2] = 28;
        }
        if(annee < 1999 || annee > 2005 || mois < 1 || mois > 12 || jour < 1 || jour > joursParMois[mois]) {
            printf(RED "Erreur : date invalide. L'année doit être entre 1999 et 2005.\n" RESET);
            annee = 0; 
        }
    } while(annee < 1999 || annee > 2005 || mois < 1 || mois > 12 || jour < 1 || jour > joursParMois[mois]);

    sprintf(joueurs[*nbjoueur].dateInscription, "%02d/%02d/%04d", jour, mois, annee);
    joueurs[*nbjoueur].id = (*next_id)++;
    (*nbjoueur)++;
    
    printf(GREEN ">>> Joueur ajouté avec succès !\n" RESET);
}

//  ajouter plusieurs joueurs
void ajouter_plusieurs_joueurs(struct joueur joueurs[], int *nbjoueur, int *next_id) {
    int c;
    int i = 0;
    printf("Combien de joueurs voulez-vous ajouter ? ");
    scanf("%d", &c);

    while(i < c) {
        printf(YELLOW "\n**** Joueur %d ****\n" RESET, i + 1);
        ajouter_joueur(joueurs, nbjoueur, next_id);
        i++;
    }
}

// afficher les joueurs - CORRIGÉ
void afficher_les_joueurs(struct joueur joueurs[], int nbjoueur) {
    if (nbjoueur == 0) {
        printf(RED "Aucun joueur enregistré.\n" RESET);
        return;
    }

    int choix;
    printf(MAGENTA "\n**** Affichage des Joueurs ****\n" RESET);
    printf(CYAN "1. Trier par Nom (ordre alphabétique)\n" RESET);
    printf(CYAN "2. Trier par Âge (croissant)\n" RESET);
    printf(CYAN "3. Afficher par Poste\n" RESET);
    printf(CYAN "4. Afficher par Statut\n" RESET);
    printf(CYAN "5. Afficher par tranche d'âge\n" RESET);
    printf(CYAN "0. Retour\n" RESET);
    printf("Votre choix : ");
    scanf("%d", &choix);

    if (choix == 0) return;

    switch (choix) {
        case 1: {
            // Tri par nom
            int i = 0, j = 0;
            while(i < nbjoueur - 1) {
                j = i + 1;
                while(j < nbjoueur) {
                    if(my_strcmp(joueurs[i].full_name, joueurs[j].full_name) > 0) {
                        struct joueur tmp = joueurs[i];
                        joueurs[i] = joueurs[j];
                        joueurs[j] = tmp;
                    }
                    j++;
                }
                i++;
            }
            break;
        }
        case 2: {
            // Tri par âge
            int i = 0, j = 0;
            while(i < nbjoueur - 1) {
                j = i + 1;
                while(j < nbjoueur) {
                    if (joueurs[i].age > joueurs[j].age) {
                        struct joueur tmp = joueurs[i];
                        joueurs[i] = joueurs[j];
                        joueurs[j] = tmp;
                    }
                    j++;
                }
                i++;
            }
            break;
        }
        case 3: {
            char posteRecherche[40];
            printf("Entrer le poste (gardien/defenseur/milieu/attaquant) : ");
            scanf(" %[^\n]", posteRecherche);

            printf(GREEN "\n--- Joueurs au poste de %s ---\n" RESET, posteRecherche);
            int trouve = 0;
            int i = 0;
            while(i < nbjoueur) {
                if (my_strcmp(joueurs[i].poste, posteRecherche) == 0) {
                    printf("[%d] %s | %d ans | %s | %d buts | Maillot %d | %s | Inscrit le %s\n",
                           joueurs[i].id, joueurs[i].full_name, joueurs[i].age,
                           joueurs[i].poste, joueurs[i].buts, joueurs[i].numeroMaillot,
                           joueurs[i].statut, joueurs[i].dateInscription);
                    trouve = 1;
                }
                i++;
            }
            if (!trouve) {
                printf(RED "Aucun joueur trouvé pour ce poste.\n" RESET);
            }
            return; 
        }
        case 4: {
            char statutRecherche[20];
            printf("Entrer le statut (titulaire/remplacant) : ");
            scanf(" %[^\n]", statutRecherche);
            int i = 0, trouve = 0;
            printf(GREEN "\n--- Joueurs avec le statut %s ---\n" RESET, statutRecherche);
            while (i < nbjoueur) {
                if (my_strcmp(joueurs[i].statut, statutRecherche) == 0) {
                    printf("[%d] %s | %d ans | %s | %d buts | Maillot %d | %s | Inscrit le %s\n",
                           joueurs[i].id, joueurs[i].full_name, joueurs[i].age,
                           joueurs[i].poste, joueurs[i].buts, joueurs[i].numeroMaillot,
                           joueurs[i].statut, joueurs[i].dateInscription);
                    trouve = 1;
                }
                i++;
            }
            if (!trouve) {
                printf(RED "Aucun joueur trouvé avec ce statut.\n" RESET);
            }
            return;
        }
        case 5: {
            int minAge, maxAge;
            printf("Entrer la tranche d'âge (min max) : ");
            scanf("%d %d", &minAge, &maxAge);
            int i = 0, trouve = 0;
            printf(GREEN "\n--- Joueurs entre %d et %d ans ---\n" RESET, minAge, maxAge);
            while (i < nbjoueur) {
                if (joueurs[i].age >= minAge && joueurs[i].age <= maxAge) {
                    printf("[%d] %s | %d ans | %s | %d buts | Maillot %d | %s | Inscrit le %s\n",
                           joueurs[i].id, joueurs[i].full_name, joueurs[i].age,
                           joueurs[i].poste, joueurs[i].buts, joueurs[i].numeroMaillot,
                           joueurs[i].statut, joueurs[i].dateInscription);
                    trouve = 1;
                }
                i++;
            }
            if (!trouve) {
                printf(RED "Aucun joueur trouvé dans cette tranche d'âge.\n" RESET);
            }
            return;
        }
        default:
            printf(RED "Choix invalide.\n" RESET);
            return;
    }
    
    // Affichage final après tri (seulement pour les cas 1 et 2)
    if (choix == 1 || choix == 2) {
        printf(GREEN "\n--- Liste des Joueurs ---\n" RESET);
        for (int i = 0; i < nbjoueur; i++) {
            printf("[%d] %s | %d ans | %s | %d buts | Maillot %d | %s | Inscrit le %s\n",
                   joueurs[i].id, joueurs[i].full_name, joueurs[i].age,
                   joueurs[i].poste, joueurs[i].buts, joueurs[i].numeroMaillot,
                   joueurs[i].statut, joueurs[i].dateInscription);
        }
    }
}

// rechercher joueur
void rechercher_joueur(struct joueur joueurs[], int nbjoueur) {
    int choix;
    printf(MAGENTA "\n**** Recherche ****\n" RESET);
    printf("1. Rechercher par ID\n");
    printf("2. Rechercher par Nom\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    if (choix == 1) {
        int id;
        printf("Entrer l'ID : ");
        scanf("%d", &id);
        for (int i = 0; i < nbjoueur; i++) {
            if (joueurs[i].id == id) {
                printf(GREEN "Joueur trouvé : %s (%d buts)\n" RESET, joueurs[i].full_name, joueurs[i].buts);
                return;
            }
        }
    } else if (choix == 2) {
        char nom[40];
        printf("Entrer le nom : ");
        scanf(" %[^\n]", nom);
        for (int i = 0; i < nbjoueur; i++) {
            if (my_strcmp(joueurs[i].full_name, nom) == 0) {
                printf(GREEN "Joueur trouvé : %s (%d buts)\n" RESET, joueurs[i].full_name, joueurs[i].buts);
                return;
            }
        }
    } else {
        printf(RED "Choix invalide.\n" RESET);
        return;
    }
    printf(RED "Aucun joueur trouvé.\n" RESET);
}

// modifier joueur
void modifier_joueur(struct joueur joueurs[], int nbjoueur) {
    int id;
    printf("Entrer l'ID du joueur à modifier : ");
    scanf("%d", &id);

    for (int i = 0; i < nbjoueur; i++) {
        if (joueurs[i].id == id) {
            printf("Nouveau poste : ");
            scanf(" %[^\n]", joueurs[i].poste);
            printf("Nouvel âge : ");
            scanf("%d", &joueurs[i].age);
            printf("Nouveau nombre de buts : ");
            scanf("%d", &joueurs[i].buts);
            printf(GREEN "Joueur modifié avec succès.\n" RESET);
            return;
        }
    }
    printf(RED "Joueur introuvable.\n" RESET);
}

// supprimer joueur
void supprimer_joueur(struct joueur joueurs[], int *nbjoueur) {
    int id;
    printf("Entrer l'ID du joueur à supprimer : ");
    scanf("%d", &id);

    for (int i = 0; i < *nbjoueur; i++) {
        if (joueurs[i].id == id) {
            for (int k = i; k < *nbjoueur - 1; k++) {
                joueurs[k] = joueurs[k + 1];
            }
            (*nbjoueur)--;
            printf(GREEN "Joueur supprimé avec succès.\n" RESET);
            return;
        }
    }
    printf(RED "Joueur introuvable.\n" RESET);
}

// statistiques
void statistiques(struct joueur joueurs[], int nbjoueur) {
    if (nbjoueur == 0) {
        printf(RED "Aucun joueur enregistré.\n" RESET);
        return;
    }

    int choix;
    printf(MAGENTA "\n**** Statistiques ****\n" RESET);
    printf("1. Nombre total de joueurs\n");
    printf("2. Âge moyen des joueurs\n");
    printf("3. Meilleur buteur\n");
    printf("4. Joueur le plus jeune et le plus âgé\n");
    printf("0. Retour\n");
    printf("Votre choix : ");
    scanf("%d", &choix);

    switch (choix) {
        case 1:
            printf("Nombre total : %d joueurs\n", nbjoueur);
            break;
        case 2: {
            int somme = 0;
            for (int i = 0; i < nbjoueur; i++) somme += joueurs[i].age;
            printf("Âge moyen : %.2f ans\n", (float)somme / nbjoueur);
            break;
        }
        case 3: {
            int max = joueurs[0].buts, idx = 0;
            for (int i = 1; i < nbjoueur; i++) {
                if (joueurs[i].buts > max) { max = joueurs[i].buts; idx = i; }
            }
            printf("Meilleur buteur : %s (%d buts)\n", joueurs[idx].full_name, joueurs[idx].buts);
            break;
        }
        case 4: {
            int min = joueurs[0].age, max = joueurs[0].age;
            int idxMin = 0, idxMax = 0;
            for (int i = 1; i < nbjoueur; i++) {
                if (joueurs[i].age < min) { min = joueurs[i].age; idxMin = i; }
                if (joueurs[i].age > max) { max = joueurs[i].age; idxMax = i; }
            }
            printf("Le plus jeune : %s (%d ans)\n", joueurs[idxMin].full_name, joueurs[idxMin].age);
            printf("Le plus âgé   : %s (%d ans)\n", joueurs[idxMax].full_name, joueurs[idxMax].age);
            break;
        }
        case 0:
            return;
        default:
            printf(RED "Choix invalide.\n" RESET);
    }
}

// main function 
int main() {
    struct joueur joueurs[MAX];
    int nbjoueur = 0;
    int next_id = 1;
    int choix;

    do {
        printf(BLUE "\n******** MENU PRINCIPAL ********\n" RESET);
        printf("1. Ajouter un joueur\n");
        printf("2. Ajouter plusieurs joueurs\n");
        printf("3. Afficher tous les joueurs\n");
        printf("4. Modifier un joueur\n");
        printf("5. Supprimer un joueur\n");
        printf("6. Rechercher un joueur\n");
        printf("7. Statistiques\n");
        printf("0. Quitter\n");
        printf("*******************************\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1: 
                ajouter_joueur(joueurs, &nbjoueur, &next_id); 
                break;
            case 2: 
                ajouter_plusieurs_joueurs(joueurs, &nbjoueur, &next_id); 
                break;
            case 3: 
                afficher_les_joueurs(joueurs, nbjoueur); 
                break;
            case 4: 
                modifier_joueur(joueurs, nbjoueur); 
                break;
            case 5: 
                supprimer_joueur(joueurs, &nbjoueur); 
                break;
            case 6: 
                rechercher_joueur(joueurs, nbjoueur); 
                break;
            case 7: 
                statistiques(joueurs, nbjoueur); 
                break;
            case 0: 
                printf(RED "Au revoir.\n" RESET); 
                break;
            default: 
                printf(RED "Choix invalide.\n" RESET);
        }
    } while (choix != 0);

    return 0;
}