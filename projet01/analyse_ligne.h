#ifndef ANALYSE_LIGNE_H
#define ANALYSE_LIGNE_H

/*--------------------------------------------------------------------------
 * header où sont définies des constantes
 * -----------------------------------------------------------------------*/
#define NB_MAX_COMMANDES 1 // nb max de commandes sur une ligne
#define NB_MAX_MOTS 30     // nb max de mots pour une commande
#define NB_MAX_CAR 4096    // nb max de caractères dans une ligne

/*--------------------------------------------------------------------------
 * type structuré contenant les fruits de l'analyse d'une ligne de commandes
 * -----------------------------------------------------------------------*/
typedef struct ligne_analysee_t
{
  int    nb_fils;			           // nb de fils à créer pour la ligne reçue
  char*  commandes[NB_MAX_COMMANDES][NB_MAX_MOTS]; // contient les mots des différentes commandes
  char   ligne[NB_MAX_CAR];			   // contient la ligne d'entrée
} ligne_analysee_t;

/*--------------------------------------------------------------------------
 * Lecture de la ligne de commande
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
void lit_ligne(ligne_analysee_t *ligne_analysee);

/*--------------------------------------------------------------------------
 * Fonction qui extrait chaque commande
 * de la ligne de commande. Renvoie le nombre de commandes lancées.
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
int extrait_commandes(ligne_analysee_t *ligne_analysee);
#endif
