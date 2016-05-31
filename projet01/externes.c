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


void from_stdin_to_pipe(job_t *job, int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig){

  if (pipe(job->mes_tubes[0])==-1)
  {perror("Echec création tube"); exit(errno);}

  pid_t res_fork=1;
  res_fork=fork();
  if (res_fork==0) {

    sig->sa_handler=SIG_DFL;
    sigaction(SIGINT,sig,NULL);

    close(job->mes_tubes[0][0]); //On ne lit pas dans le tube

    dup2(STDOUT_FILENO, job->mes_tubes[0][1]); //il écrit son resultat dans le tube
    close(job->mes_tubes[0][1]);

    printf("S2P : Le contenu de mon execvp : %s\n", ligne_analysee->commandes[num_comm][0]);
    execvp(ligne_analysee->commandes[num_comm][0], ligne_analysee->commandes[num_comm]);

  }
  else
    job->pids[num_comm] = res_fork;

}

void from_pipe_to_pipe(job_t *job, int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig){
  if (pipe(job->mes_tubes[num_comm])==-1)
  {perror("Echec création tube"); exit(errno);}

  pid_t res_fork=1;
  res_fork=fork();


  if(res_fork != 0){

    job->pids[num_comm] = res_fork;
    //le père peut fermer ses accès au tube precedent
    close(job->mes_tubes[num_comm-1][0]);
    close(job->mes_tubes[num_comm-1][1]);

  }
  else {

    sig->sa_handler=SIG_DFL;
    sigaction(SIGINT,sig,NULL);

    close(job->mes_tubes[num_comm-1][1]);     //on ne lit plus dans le tube précédent
    dup2(job->mes_tubes[num_comm-1][0],STDIN_FILENO);
    close(job->mes_tubes[num_comm-1][0]); //descripteur dupliqué, on en a plus besoin

    dup2(job->mes_tubes[num_comm][1],STDOUT_FILENO); // il écrit son résultat dans son tube
    close(job->mes_tubes[num_comm][0]);
    close(job->mes_tubes[num_comm][1]);

    printf("P2P : Le contenu de mon execvp : %s\n", ligne_analysee->commandes[num_comm][0]);
    execvp(ligne_analysee->commandes[num_comm][0], ligne_analysee->commandes[num_comm]);

  }


}

void from_pipe_to_stdout(job_t *job, int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig){
  //if (pipe(job->mes_tubes[ligne_analysee->nb_fils-1]==-1))
  //{perror("Echec création tube"); exit(errno);}
  pid_t res_fork=1;
  res_fork=fork();

  //le père peut fermer ses accès au tube precedent
  if(res_fork != 0 ){
    close(job->mes_tubes[num_comm-1][0]);
    close(job->mes_tubes[num_comm-1][1]);
    job->pids[num_comm] = res_fork;
  }
  else {

    close(job->mes_tubes[num_comm-1][1]);
    dup2(job->mes_tubes[num_comm-1][0],STDIN_FILENO); // il lit ses entrées depuis le tube précedent

    close(job->mes_tubes[num_comm-1][0]); //Le descripteur a été dupliqué, on en a plus besoin
    //dup2(job->mes_tubes[ligne_analysee->nb_fils-1][1],STDOUT_FILENO);

    sig->sa_handler=SIG_DFL;
    sigaction(SIGINT,sig,NULL);

    printf("P2S : Le contenu de mon execvp : %s\n", ligne_analysee->commandes[num_comm][0]);
    execvp(ligne_analysee->commandes[num_comm][0], ligne_analysee->commandes[num_comm]);

  }

}


/*--------------------------------------------------------------------------
 * crée un fils pour exécuter la commande ligne_analysee->commandes[num_comm]
 * enregistre son pid dans job->pids[num_comm]
 * le fils définit ses handlers à différents signaux grâce à sig
 * -----------------------------------------------------------------------*/
static void execute_commande_dans_un_fils(job_t *job,int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig)
{
  // TODO : supprimer les lignes suivantes et compléter la procédure

  if (num_comm==0 && ligne_analysee->nb_fils>1){
    from_stdin_to_pipe(job, num_comm, ligne_analysee, sig);
  }
  else if (num_comm<(ligne_analysee->nb_fils-1) && ligne_analysee->nb_fils>1) {
    from_pipe_to_pipe(job, num_comm, ligne_analysee, sig);
  }
  else if (ligne_analysee->nb_fils>1){
    from_pipe_to_stdout(job, num_comm, ligne_analysee, sig);
  }
  else {
    pid_t res_fork=1;
    res_fork=fork();
    if (res_fork==0) {

      sig->sa_handler=SIG_DFL;
      sigaction(SIGINT,sig,NULL);
      printf("Le contenu de mon execvp : %s\n", ligne_analysee->commandes[num_comm][0]);
      execvp(ligne_analysee->commandes[num_comm][0], ligne_analysee->commandes[num_comm]);

    }
    else job->pids[num_comm] = res_fork;
  }

  UNUSED(job); UNUSED(num_comm); UNUSED(ligne_analysee); UNUSED(sig);


}
/*--------------------------------------------------------------------------
 * Fait exécuter les commandes de la ligne par des fils
 * -----------------------------------------------------------------------*/
void executer_commandes(job_t *job, ligne_analysee_t *ligne_analysee, struct sigaction *sig)
{
  pid_t resw=-1;

  // recopie de la ligne de commande dans la structure job
  strcpy(job->nom,ligne_analysee->ligne);

  // on lance l'exécution de chaque commande dans un fils
  for(int i=0; i<=ligne_analysee->nb_fils-1; i++){
    printf("j'ai %i fils\n J'éxecute la commande %i\n\n", ligne_analysee->nb_fils, i);
    execute_commande_dans_un_fils(job,i,ligne_analysee, sig);
  }


  // TODO : à compléter
  for(int j=0; j<ligne_analysee->nb_fils; j++){
    resw=waitpid(job->pids[j],NULL,0);
    if (resw==-1)
      {printf("%d :",job->pids[j]); perror("Echec waitpid"); exit(errno);}
  }

  // on ne se sert plus de la ligne : ménage
  *ligne_analysee->ligne='\0';
}
