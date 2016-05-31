#ifndef EXTERNES_H
#define EXTERNES_H

#include <signal.h>    // pour struct sigaction

/*--------------------------------------------------------------------------
 * header à inclure pour constantes et types spéciaux
 * -----------------------------------------------------------------------*/
#include "jobs.h"


/*--------------------------------------------------------------------------
 * Fait exécuter les commandes de la ligne par des fils
 * Utilisée dans mini_shell.c
 * -----------------------------------------------------------------------*/
void from_stdin_to_pipe(job_t *job, int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig);
void from_pipe_to_pipe(job_t *job, int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig);
void from_pipe_to_stdout(job_t *job, int num_comm, ligne_analysee_t *ligne_analysee, struct sigaction *sig);

void executer_commandes(job_t *job, ligne_analysee_t *ligne_analysee, struct sigaction *sig);

#endif
