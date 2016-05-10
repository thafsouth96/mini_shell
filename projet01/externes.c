/*--------------------------------------------------------------------------
 * headers à inclure afin de pouvoir utiliser divers appels systèmes
 * -----------------------------------------------------------------------*/
#include <stdio.h>     // pour printf() and co
#include <stdlib.h>    // pour exit()
#include <errno.h>     // pour errno and co
#include <unistd.h>    // pour pipe
#include <sys/types.h> // pour avoir pid_t
#include <signal.h>    // pour sigaction
#include <string.h>    // pour avoir strcmp and co

#include <sys/wait.h>  // pour avoir wait and co

/*--------------------------------------------------------------------------
 * header à inclure pour constantes et types spéciaux
 * -----------------------------------------------------------------------*/
#include "jobs.h"
#include "externes.h"

/*-------------------------------------------------------------------------------
 * Macro pour éviter le warning "unused parameter" dans une version intermédiaire
 * -----------------------------------------------------------------------------*/
#define UNUSED(x) (void)(x)



/*--------------------------------------------------------------------------
 * crée un fils pour exécuter la commande ligne_analysee->commandes[num_comm]
 * enregistre son pid dans job->pids[num_comm]
 * le fils définit ses handlers à différents signaux grâce à sig
 * -----------------------------------------------------------------------*/
static void execute_commande_dans_un_fils(job_t *job,int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig)
{
  // TODO : supprimer les lignes suivantes et compléter la procédure

  pid_t res_fork=1;

  res_fork=fork();
  if (res_fork==0) {
    execvp(ligne_analysee->commandes[num_comm][0], ligne_analysee->commandes[num_comm]);

  }
  else job->pids[num_comm] = res_fork;

  UNUSED(job); UNUSED(num_comm); UNUSED(ligne_analysee); UNUSED(sig);


}
/*--------------------------------------------------------------------------
 * Fait exécuter les commandes de la ligne par des fils
 * -----------------------------------------------------------------------*/
void executer_commandes(job_t *job, ligne_analysee_t *ligne_analysee, struct sigaction *sig)
{
  // recopie de la ligne de commande dans la structure job
  strcpy(job->nom,ligne_analysee->ligne);

  // on lance l'exécution de la commande dans un fils
  execute_commande_dans_un_fils(job,0,ligne_analysee, sig);

  // TODO : à compléter
   waitpid(job->pids[0],NULL,0);

  // on ne se sert plus de la ligne : ménage
  *ligne_analysee->ligne='\0';
}
