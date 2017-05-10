#ifndef _CLIENT_H		/*Avoid conflict with other .h file */
#define _CLIENT_H


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>         
#include <sys/un.h>

#include "mytypes.h"

/* Orismos: Dimiourgia neou parking account */
void create_Paccount();
/* Orismos: emfanish enos parking account */
void print_Paccount(int parkingID);

/* Orismos: Dimiourgia neou driver account */
void create_Daccount();

/* Orismos: Emfanish arxikhs selidas */
void printWelcome();

/* Orismos: Arxikopoihsh tou client */
int Initialize_c();
#endif
