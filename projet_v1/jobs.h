#ifndef JOBS_H
#define JOBS_H

/*--------------------------------------------------------------------------
 * header où sont définies des constantes
 * -----------------------------------------------------------------------*/
#define NB_MAX_JOBS 1      // nb max de lignes que l'on peut exécuter concuremment
#define AUCUN_JOB -2       // valeur pour un numéro de job pour signifier "aucun job défini"
#define NON_UTILISE -2     // valeur pour un pid pour signifier que la variable n'est pas utilisée
#define TERMINE 0          // valeur pour un pid pour signifier que le processus est terminé
                           // -1 n'est pas utilisée car signifie n'importe quel pid pour waitpid()

/*--------------------------------------------------------------------------
 * header à inclure pour les constantes
 * -----------------------------------------------------------------------*/
#include "analyse_ligne.h"

/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser certains types
 * -----------------------------------------------------------------------*/
#include <unistd.h>    // pour pid_t
#include <sys/types.h> // pour pid_t

/*--------------------------------------------------------------------------
 * type structuré décrivant un job
 * -----------------------------------------------------------------------*/
typedef struct job_t
{
  char   nom[NB_MAX_CAR];             // ligne de commandes exécutée par le job
  pid_t  pids[NB_MAX_COMMANDES+1];    // pids des fils exécutant chaque commande
                                      // pids[p]==NON_UTILISE si le job n'a pas de pème fils
                                      // pids[p]==TERMINE si le pème fils s'est terminé
                                      // le job est en cours d'exécution si pids[0]!=NON_UTILISE
} job_t;

/*--------------------------------------------------------------------------
 * type structuré décrivant un ensemble de jobs
 * -----------------------------------------------------------------------*/
typedef struct job_set_t
{
  job_t  jobs[NB_MAX_JOBS];
  int    job_fg; //numéro du job en avant-plan, qu'il faut donc attendre; =AUCUN_JOB s'il n'y en a pas
                 // ne sera utile qu'à partir de la version avec gestion de SIGCHLD
} job_set_t;

/*--------------------------------------------------------------------------
 * Fonction qui initialise les structures de contrôle des jobs
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
void initialiser_jobs(job_set_t *mes_jobs);


/*--------------------------------------------------------------------------
 * Action sur un job qui est supposé exister
 * Utilisée dans internes.c
 * -----------------------------------------------------------------------*/
void action_job(int j, job_t job,int sig,const char*text);


/*--------------------------------------------------------------------------
 * Prépare un nouveau job
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
job_t *preparer_nouveau_job(int isfg, char *ligne, job_set_t *mes_jobs);

#endif
