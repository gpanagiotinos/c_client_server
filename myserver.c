#include "myserver.h"


/*Listes account sto server*/
/*Account ston server gia odhgous*/
extern driver sDriver[MAX_DRIVER];

/*Account ston server gia parking*/

extern parking sParking[MAX_PARKING];

/*Deikths sthn koinh mnhmh driver*/
extern driver *sDriver_ptr;

/*Deikths sthn koinh mnhmh gia parking*/

extern parking *sParking_ptr;




/*Ari8mos driver accounts sto server*/
extern int *numDriver;

/*Ari8mos parking accounts sto server*/
extern int *numParking;

/*Semaphore*/

extern sem_t* semaph;

/*Metablhtes pou 8a apo8hkeusoume  tis dieu8hnshs ths koinhs mnhmhs gia driver*/
extern int shm_Driver_id, shm_numDriver_id, error_dr;


/*Metablhtes pou 8a apo8hkeusoume  tis dieu8hnshs ths koinhs mnhmhs gia driver*/
extern int shm_Parking_id, shm_numParking_id, error_pk;

/* MAIN */
int main(){

/*Metablhtes*/
/*sockfd to socket pou dimiourgoume, cest: connection establised edw epistrefete h timh ths accept*/
int sockfd,cest;

/*To mege8os tou socket pou ginete h accept*/
socklen_t sin_size;




/*Arxikopoihsh ton ID twn odhgwn kai twn parking*/
int driverID = 0, parkingID = 0, i;

/*Socket address*/
	struct sockaddr sockaddr;/*Domh tou socket tou client*/
	struct sockaddr_un servaddr;/*Domh tou socket tou server*/

 /* Metablhth pou 8a apo8hkeuh thn epilogh tou xrhsth analoga an einai odhgos(Driver(D) ) h Idiokthths Parking(Parking(P)) */

char choise, command;



initialize_s();/* Arxikopoihsh tou server */


/*Dhmiourgia tou socket sockfd sto server kai elegxos an egine swsta to anoigma tou socket*/
if ((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {	
		perror("socket");
		exit(1);
	}


unlink(F_PATH); /*Diagrafh arxeiou pou periexi to socket !!!!!! Rwta ton xristo gia thn F_PATH*/
bzero(&servaddr, sizeof(servaddr));/*Arxikopoihsh me mhden thn dieu8hnsh tou socket tou server */
servaddr.sun_family = AF_LOCAL;/*Dieu8hnsh tou socket tou server*/
strcpy(servaddr.sun_path, F_PATH);/*Ekxorhsh tou onomatos tou arxeiou ws onoma sto socket tou server wste na sunxronistoune*/


if (bind(sockfd, (SA *) & servaddr, sizeof(servaddr)) == -1) {	/* Desmeush tou socket apo to server */
		perror("bind");
		exit(1);
	}

if (listen(sockfd, 1) == -1) {	/* To socket tou server bainei se katastash listen wste na boresei na "akousei" aithshs apo tous client */
		perror("listen");
		exit(1);
	}


signal(SIGCHLD, sigcatch); /*Pianei tis zombie child diergasies kai ths diagrafoi*/
signal(SIGQUIT, catch_int);/* Pianei to signal pou prokalei to Ctrl+C */
//signal(SIGALRM, catch_alarm);/* Singal gia ta 5sec ananewshs tou server */









for(;;){
	
       
	
	
	 //alarm(5);/* Alarm signal ka8e 5sec */
	
	sin_size = sizeof(sockaddr);
	cest = accept(sockfd, (SA *)&sockaddr, &sin_size); /*Dhmiourgia neas sundeshs metaksh client kai server*/
	
	/*Elegxos gia tuxon sfalma sthn accept*/
	if (cest == -1) {
			
			perror("accept");
			continue;
		}
	
	/* Dhmiourgia mia child diergasias gia diaxeirhsh tou client */
	if(fork() != 0){
		
			
		close (sockfd); /* Kleisimo tou socket.Child den to need */
		
		read(cest, &choise, sizeof(choise));/* Diabasma ths epiloghs tou client (D/P) */
		
			 /*--------------------------------------------------*/
			/*          Connect to the shared memory            */
		       /*--------------------------------------------------*/
		      /*-----------------Parking--------------------------*/
			numParking = shmat(shm_numParking_id, NULL, 0);
	
			if (numParking == (int *) -1){
				fprintf(stderr, "Could not attach to shared memory!\n");
				exit(1);
			}
	
			sParking_ptr = shmat(shm_Parking_id, NULL, 0);	
			if (sParking_ptr == (parking *) NULL){
				fprintf(stderr, "Could not attach to shared memory!\n");
				exit(1);
			} 
			/*---------------------------------------------------*/
			/*----------------Driver-----------------------------*/
			numDriver = shmat(shm_numDriver_id, NULL, 0);
	
			if (numDriver == (int *) -1){
				fprintf(stderr, "Could not attach to shared memory!\n");
				exit(1);
			}
	
			sDriver_ptr = shmat(shm_Driver_id, NULL, 0);	
			if (sDriver_ptr == (driver *) NULL){
				fprintf(stderr, "Could not attach to shared memory!\n");
				exit(1);
			} 

			
			/*---------------------------------------------------*/
			/*---------------------------------------------------*/
			
	
		printf("\nChoise: %c", choise);
		switch(choise)
		{
			case 'P': case 'p':
				printf("\nEnter p\n");
				
				read(cest, &command, sizeof(command)); /* Diabasma epiloghs to ti 8a kanei ston ekastote client (C)reate/(A)pearance */
				switch(command)
				{	
					case'C': case 'c':	/* Dimiourgia kanourgiou parking account */
						printf("\n Enter c\n");
						write(cest, numParking, sizeof(numParking));
						

						sem_wait(sem_pk);
					read( cest, &sParking_ptr[(*numParking)], sizeof(parking) );
					(*numParking) = (*numParking) + 1;
					
						sem_post(sem_pk);				
					break;
				
					case 'A': case 'a':     /* Emfanish enos account parking */
						printf("\n Enter a\n");
						read(cest, &parkingID, sizeof(parkingID));					
						write(cest, numParking, sizeof(int));						
						write(cest, sParking_ptr,(*numParking) * sizeof(parking));
						
						printf("\n*------------------------------------------------------*\n");
						printf("\n*------------------PARKING ACCOUNT---------------------*\n");
						printf("\n*------------------------------------------------------*\n");
						printf("Parking ID: %d\n", parkingID);								
						printf("Parking name: %s\n", sParking_ptr[parkingID].pk_name);
						printf("Parking info: %s\n", sParking_ptr[parkingID].info);
						printf("Parking price/2min: %d\n", sParking_ptr[parkingID].price);
						for(i =0; i<5; i++){
						printf("\n%d position have %d Driver\n", 1+i, sParking_ptr[parkingID].pk_pos[i]);
						}
						printf("\n*------------------------------------------------------*\n");
						printf("\n*------------------------------------------------------*\n");		
					break;
				}

					




							
			break;
			case 'D': case 'd':
				printf("\nEnter d\n");
				read(cest, &command, sizeof(command)); /* Diabasma epiloghs to ti 8a kanei ston ekastote client (C)reate/(A)pearance */
				switch(command)
				{	
					case'C': case 'c':	/* Dimiourgia kanourgiou driver account */
						printf("\n Enter c\n");
						write(cest, numDriver, sizeof(numDriver));
						
						sem_wait(sem_dr);
					read( cest, &sDriver_ptr[(*numDriver)], sizeof(driver) );
					(*numDriver) = (*numDriver) + 1;
					
						sem_post(sem_dr);
					break;					
				}


	
		}//end of switch

			  /*--------------------------------------------------*/
			 /*		Detach Shared Memory Segment	     */
			/*--------------------------------------------------*/
		       /*--------------------------------------------------*/
		      /*-----------------Parking--------------------------*/
			error_pk = shmdt(numParking);
			if (error_pk == -1){
				fprintf(stderr, "Child server %d could not detach from the shared memory (numParking).", getpid());
				fflush(stdout);
			}
		
			error_pk = shmdt(sParking_ptr);
			if (error_pk == -1){
				fprintf(stderr, "Child server %d could not detach from the shared memory (sParking_ptr).", getpid());
				fflush(stdout);
			}
			/*---------------------------------------------------*/
			/*----------------Driver-----------------------------*/
			error_dr = shmdt(numDriver);
			if (error_dr == -1){
				fprintf(stderr, "Child server %d could not detach from the shared memory (numParking).", getpid());
				fflush(stdout);
			}
		
			error_dr = shmdt(sDriver_ptr);
			if (error_dr == -1){
				fprintf(stderr, "Child server %d could not detach from the shared memory (sParking_ptr).", getpid());
				fflush(stdout);
			}			

			/*----------------------------------------------------*/
			/*----------------------------------------------------*/
		

		
		
	close(cest);
	sem_close(semaph);
	exit(0);

		
	}//end of fork





 }//end of for(;;)

return 0;
}// Telos main




/* SIGCATCH : Diaxeirizete to singal SIGCHLD kanontas eksodo apo to server, diagrafh tou arxeio stderr kai kleinei kai diagrafh to shmaforo */
void sigcatch()
{
	fprintf(stderr, "\nSERVER: Eksodo apo to Server....");
	if(fprintf(stderr,"\n***\tKleisimo tou Server.") ){
		printf("\nSERVER: Diagrafh tou stderr .");
	}
	fflush(stdout);

	//Kleisimo shmaforou.
	sem_close(semaph);
	// Diagrafh shmaforou
	sem_unlink(SEM_NAME);
	exit(0);
}//Telos sigcatch.

/* Diaxeirhsh touy signal SIGALARM */
void catch_alarm(int sig_num)
{




printf("\n 5sec \n");




alarm(5);





}//Telos catch_alarm.

void catch_int(int sig_num){







signal(SIGQUIT, catch_int);


alarm(0);

 printf("\n*****************************");
 printf("\n******Server Exit!!!!!*******");
 printf("\n*****************************\n");    
fflush(stdout);


exit(0);


}//Telos catch_int.




/*!!!!!!!!!!!!SHARED MEMORY!!!!!!!!!!!!!!!!!*/
/******DRIVER******/

/*Desmeush koinhs mnhmhs gia twn ari8mo twn driver*/
void init_sharednumDriver(int *shm_numDriver_id){

int *shm_dr;
/*Desmeush ths koinhs mnhmhs kai ekxwrhsh ths tautothtas ths*/
*shm_numDriver_id = shmget(SHM_NUMDR_KEY, SHM_NUMDR_SIZE, 0600 | IPC_CREAT);

/*Elegxos gia tuxon sfalma sthn prospa8eia desmeushs koinhs mnhmhs*/
if((*shm_numDriver_id)<0)
{
  fprintf(stderr, "Server: Den boresa na dimiourghsw koinh mnhmh gia numDriver!\n");
  exit(1);
}

/*Sundesh ths koinhs mnhmhs me to shm_dr kai elegxos gia sfalma*/
if((shm_dr = shmat((*shm_numDriver_id),NULL,0)) == (int *) -1)
{
  perror("shmat");
  exit(1);
}
/*Antistixoish ths numDriver me thn shm_dr*/
numDriver = (int *)shm_dr;

}// Telos ths sunarthshs init_sharednumDriver !!!!!!!


/*Desmeush koinhs mnhmh gia ta account twn driver*/
void init_sharedDriver(int *shm_driver_id)
{
  driver *shm_dr;
 /*Desmeush ths koinhs mnhmhs kai ekxwrhsh ths tautothtas ths*/
  *shm_driver_id = shmget(SHM_DR_KEY, SHM_DR_SIZE, 0600 | IPC_CREAT);

/*Elegxos gia tuxon sfalma sthn prospa8eia desmeushs koinhs mnhmhs*/
if((*shm_driver_id) < 0)
{
  fprintf(stderr, "Server: Den boresa na dimiourghsw koinh mnhmh gia driver!\n");
  exit(1);
}

/*Sundesh ths koinhs mnhmhs me to shm_dr gia na ekxwroume ta account kai elegxos gia sfalma*/
if((shm_dr = (driver *)shmat((*shm_driver_id), NULL, 0)) == (driver *) -1)
{
  perror("shmat");
  exit(1);
}

  sDriver_ptr = shm_dr;

}// Telos ths init_sharedDriver !!!!!!!!!!!!


/******PARKING**********/

/*Desmeush koinhs mnhmhs gia twn ari8mo twn parking*/
void init_sharednumParking(int *shm_numParking_id){

int *shm_pk;
/*Desmeush ths koinhs mnhmhs kai ekxwrhsh ths tautothtas ths*/
*shm_numParking_id = shmget(SHM_NUMPK_KEY, SHM_NUMPK_SIZE, 0600 | IPC_CREAT);

/*Elegxos gia tuxon sfalma sthn prospa8eia desmeushs koinhs mnhmhs*/
if((*shm_numParking_id)<0)
{
  fprintf(stderr, "Server: Den boresa na dimiourghsw koinh mnhmh gia numParking!\n");
  exit(1);
}

/*Sundesh ths koinhs mnhmhs me to shm_pk kai elegxos gia sfalma*/
if((shm_pk = shmat((*shm_numParking_id),NULL,0)) == (int *) -1)
{
  perror("shmat");
  exit(1);
}
/*Antistixoish ths numParking me thn shm_pk*/
numParking = (int *)shm_pk;

}/* Telos ths sunarthshs init_sharednumParking !!!!!!! */


/*Desmeush koinhs mnhmh gia ta account twn parking*/
void init_sharedParking(int *shm_Parking_id)
{
  parking *shm_pk;
 /*Desmeush ths koinhs mnhmhs kai ekxwrhsh ths tautothtas ths*/
  *shm_Parking_id = shmget(SHM_PK_KEY, SHM_PK_SIZE, 0600 | IPC_CREAT);

/*Elegxos gia tuxon sfalma sthn prospa8eia desmeushs koinhs mnhmhs*/
if((*shm_Parking_id) < 0)
{
  fprintf(stderr, "Server: Den boresa na dimiourghsw koinh mnhmh gia Parking!\n");
  exit(1);
}

/*Sundesh ths koinhs mnhmhs me to shm_pk gia na ekxwroume ta account kai elegxos gia sfalma*/
if((shm_pk = (parking *)shmat((*shm_Parking_id), NULL, 0)) == (parking *) -1)
{
  perror("shmat");
  exit(1);
}

  sParking_ptr = shm_pk;

}/* Telos ths init_sharedParking !!!!!!!!!!!!*/



/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! INITIALIZE SERVER !!!!!!!!!!!!!!!!!!!!!!!!!!*/
void initialize_s(){


/*SERVER!!!!!!!!!!!!!*/

/*Orismo tou deiktei sDriver_ptr na deixnei sthn prwth 8esh tou pinaka sDriver pou periexonte ola ta accounts twn odhgwn */
sDriver_ptr = sDriver;

/*Orismo tou deiktei sParking_ptr na deixnei sthn prwth 8esh tou pinaka sParking pou periexonte ola ta accounts twn parkoin */
sParking_ptr = sParking;

/*Dimiourgia koinhs gia twn ari8mo twn ekastote accounts*/
/*Dimiourgia koinh mnhmhs gia twn ari8mo twn odhgwn.*/
init_sharednumDriver(&shm_numDriver_id);

/*Dimiourgia koinh mnhmhs gia twn ari8mo twn parkoin.*/
init_sharednumParking(&shm_numParking_id);


/*Dimiourgia koinhs gia ta ekastote account*/
/*Dimiourgia koinhs mnhmhs gia ta account twn odhgwn*/
init_sharedDriver(&shm_Driver_id);

/*Dimiourgia koinhs mnhmhs gia ta account twn parkoin*/
init_sharedParking(&shm_Parking_id);




/*Epanarxikopoihsh tou numDriver*/
*numDriver = 0;

/*Epanarxikopoihsh tou numDriver*/
*numParking = 0;


/*SEMAPHORES!!!!!!!!!!*/


sem_pk = sem_open(SEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);

if (sem_pk == SEM_FAILED){
fprintf(stderr, "\nServer: Den boresa na dhmiourghsw shmaforo\n");
exit(1);
}

/*Kanei koinoxrhsto to shmaforo sta childs kai ksekinaei me ena*/
sem_init(sem_pk, 1, 1);

sem_dr = sem_open(SEM_NAME, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);

if (sem_dr == SEM_FAILED){
fprintf(stderr, "\nServer: Den boresa na dhmiourghsw shmaforo\n");
exit(1);
}

/*Kanei koinoxrhsto to shmaforo sta childs kai ksekinaei me ena*/
sem_init(sem_dr, 1, 1);

}// Telos ths initialize_s !!!!!!!!

























