#ifndef _SERVER_H		/*Avoid conflict with other .h file */
#define _SERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <semaphore.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <malloc.h>

#include "mytypes.h"



#define MAX_DRIVER	1000	/* Max number of driver */
#define MAX_PARKING	1000	/* Max number of Parking */
/*Allakse onoma sto semaforo !!!!!!!!!!!!**************/
#define SEM_NAME	"mutex_park"	/* Semaphore name */
#define SEM_NAME	"mutex_dr"
/*Ta key gia tous odhgous*/
#define SHM_NUMDR_KEY	2501
#define SHM_NUMDR_SIZE	sizeof(int)
#define SHM_DR_KEY	54321
#define SHM_DR_SIZE	MAX_DRIVER * sizeof(driver)

/*Ta key gia ta parking*/
#define SHM_NUMPK_KEY	2510
#define SHM_NUMPK_SIZE	sizeof(int)
#define SHM_PK_KEY	54321
#define SHM_PK_SIZE	MAX_PARKING * sizeof(parking)

/*Listes account sto server*/
/*Account ston server gia odhgous*/
 driver sDriver[MAX_DRIVER];

/*Account ston server gia parking*/

 parking sParking[MAX_PARKING];

/*Deikths sthn koinh mnhmh driver*/
 driver *sDriver_ptr;

/*Deikths sthn koinh mnhmh gia parking*/

 parking *sParking_ptr;




/*Ari8mos driver accounts sto server*/
 int *numDriver;

/*Ari8mos parking accounts sto server*/
 int *numParking;

/*Semaphore*/

 sem_t* sem_dr;
sem_t* sem_pk;

/*Metablhtes pou 8a apo8hkeusoume  tis dieu8hnshs ths koinhs mnhmhs gia driver*/
  int shm_Driver_id, shm_numDriver_id, error_dr;


/*Metablhtes pou 8a apo8hkeusoume  tis dieu8hnshs ths koinhs mnhmhs gia driver*/
 int shm_Parking_id, shm_numParking_id, error_pk;

/*Orismos ths sigcatch pou diaxeirizete ena singal*/
void sigcatch();

/* Orismos ths catch_alarm  pou diaxeirizete ena singal */
void catch_alarm(int sig_num);

/* Orismos ths catch_int  pou diaxeirizete ena singal */
void catch_int(int sig_num);

/*Orismos ths sunarthshs Initialize_s()  pou kanei arxikopoihsh tou server */
void initialize_s();

/*Orismos ths sunarthshs pou kanei desmeush koinhs mnhmhs gia twn ari8mo twn driver*/
void init_sharednumDriver(int *shm_numDriver_id);

/*Orismos ths sunarthshs pou kanei desmeush koinhs mnhmh gia ta account twn driver*/
void init_sharedDriver(int *shm_driver_id);

/*Orismos ths sunarthshs pou kanei desmeush koinhs mnhmhs gia twn ari8mo twn parking*/
void init_sharednumParking(int *shm_numParking_id);

/*Orismos ths sunarthshs pou kanei desmeush koinhs mnhmh gia ta account twn parking*/
void init_sharedParking(int *shm_Parking_id);




#endif
