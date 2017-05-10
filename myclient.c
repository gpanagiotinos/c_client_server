#include "myclient.h"

int main(int argc, char **argv){



char command;


// Print Welcome message
printWelcome();


if(argc == 3 && (strcmp(argv[1], "P") == 0) && (strcmp(argv[2], "C") == 0)){

	create_Paccount();/* Dhmiourgh Account gia enan idiokthth parking */
	
}

if(argc == 4 && (strcmp(argv[1], "P") == 0) && (strcmp(argv[2], "A") == 0)){

	print_Paccount(atoi(argv[3]));/* Emfanizei ena account sthn o8onh analoga me to ID pou dinoume */
}

if(argc == 3 && (strcmp(argv[1], "D") == 0) && (strcmp(argv[2], "C")) == 0){

	create_Daccount(); /* Dhmiourgh Account gia enan odhgo */

}











return 0;
};

/* Dimiourgia enos kainourgiou parking */

void create_Paccount(){

printf("\n Enter create Paccount\n");
char choise = 'P'; 
char command = 'C';
int parkingID = -1;
int socket = Initialize_c();
parking *new_parking;
write(socket, &choise, sizeof(choise));/* Apostolh tou choise sto server (P)arking */	
write(socket, &command, sizeof(command));/* Apostolh tou command sto server (C)reate */	
read(socket, &parkingID, sizeof(parkingID));/* Diabasma ths numParking apo to server */
if(parkingID == -1){

printf("Parking Account not create!!!!!\n");

}else{



new_parking = (parking*)malloc(sizeof(parking));


new_parking->parkingID = parkingID; /* Ekxwrhsh tou ekastote id pou mas exei steilei o server mesw ths numParking sto parkingID */
/*Eisagwgh tou onomatos apo ton xrhsth*/
printf("Give your Parking name\n");
int index1 = 0;
	char ch1 = '\0';
	while( (ch1 != '\n') && (index1 < PKNAME_SIZE) ){
		ch1 = getc( stdin );
		new_parking->pk_name[index1] = ch1;
		index1 ++;
	}
	new_parking->pk_name[index1 - 1] = '\0';
printf("Parking name: %s\n", new_parking->pk_name);
/* Eisagwgh ths perigrafhs apo ton xrhsth */
printf("Give your Parking info\n");
int index2 = 0;
	char ch2 = '\0';
	while( (ch2 != '\n') && (index2 < INFO_SIZE) ){
		ch2 = getc( stdin );
		new_parking->info[index2] = ch2;
		index2 ++;
	}
	new_parking->info[index2 - 1] = '\0';
printf("Info: %s\n", new_parking->info);

/* Eisagwgh ths xrewshs ana 2lepto tou parking */
printf("Give your parking price/2min\n");
int pr = 0;
//do{
	scanf("%d", &pr);


//}while(pr == 0);
new_parking->price=pr;
printf("Price: %dE\n", pr);
/* Arxikopoihsh tou pinaka me tis 8esois tou parking */
int i;
for(i =0; i<5; i++){
new_parking->pk_pos[i] = 0;
}
printf(" Available seats: 5 \n");
printf("\nID:%d\n", new_parking->parkingID);

write(socket, new_parking, sizeof(parking));
}

free(new_parking);
close(socket);

}// Telos create_Paccount!!!

/* Sunarthsh pou emfanizei ena parking account */
void print_Paccount(int parkingID){
printf("\n Enter print Paccount\n");
int numParking = -1;
int i;
char choise = 'P'; 
char command = 'A';
parking *myparking;
int socket = Initialize_c();
write(socket, &choise, sizeof(choise));/* Apostolh tou choise sto server (P)arking */	
write(socket, &command, sizeof(command));/* Apostolh tou command sto server (A)ppearance */	
write(socket, &parkingID, sizeof(parkingID));/* Apostolh tou parkingID tou parking to opoio 8eloume na emfanisoume */
read( socket, &numParking, sizeof(int) );
myparking = (parking *) malloc(numParking *  sizeof(parking) );
read(socket, myparking, numParking *  sizeof(parking));



						printf("\n*------------------------------------------------------*\n");
						printf("\n*------------------PARKING ACCOUNT---------------------*\n");
						printf("\n*------------------------------------------------------*\n");
						printf("Parking ID: %d\n", parkingID);								
						printf("Parking name: %s\n", myparking[parkingID].pk_name);
						printf("Parking info: %s\n", myparking[parkingID].info);
						printf("Parking price/2min: %dE\n", myparking[parkingID].price);
						for(i =0; i<5; i++){
						printf("\n%d position have %d Driver\n", 1+i, myparking->pk_pos[i]);
						}
						printf("\n*------------------------------------------------------*\n");
						printf("\n*------------------------------------------------------*\n");	


close(socket);


}// Telos print_Paccount

void create_Daccount(){
printf("\n Enter create Daccount\n");
char choise = 'D'; 
char command = 'C';
int driverID = -1;
int socket = Initialize_c();
driver *new_driver;
write(socket, &choise, sizeof(choise));/* Apostolh tou choise sto server (D)river */	
write(socket, &command, sizeof(command));/* Apostolh tou command sto server (C)reate */	
read(socket, &driverID, sizeof(driverID));/* Diabasma ths numDriver apo to server */
if(driverID == -1){

printf("Driver Account not create!!!!!\n");

}else{
	new_driver = (driver*)malloc(sizeof(driver));


new_driver->driverID = driverID; /* Ekxwrhsh tou ekastote id pou mas exei steilei o server mesw ths numParking sto parkingID */
/*Eisagwgh tou onomatos apo ton xrhsth*/
printf("Give your name\n");
int index = 0;
	char ch = '\0';
	while( (ch != '\n') && (index < DRNAME_SIZE) ){
		ch = getc( stdin );
		new_driver->dr_name[index] = ch;
		index ++;
	}
	new_driver->dr_name[index - 1] = '\0';
printf("Driver name: %s\n", new_driver->dr_name);

new_driver->money = 300;
printf("Your money is: %dE\n ",new_driver->money);
printf("\nID:%d\n", new_driver->driverID);

write(socket, new_driver, sizeof(driver));

}
free(new_driver);
close(socket);

}// Telos create_Daccount



/* Arxikopoihsh tou client socket kai epistrofh enos socket file descriptor */
int Initialize_c(){

/* Arxikopoihsh metablhtwn */
int sockfd;
	struct sockaddr_un servaddr; /* socket address structure */
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);  /* create unix socket descriptor !!!! socket(AF_LOCAL .....) !!!!!!*/
	bzero(&servaddr, sizeof(servaddr)); /* clear address structure */
	servaddr.sun_family = AF_LOCAL;     /* set local address */
	strcpy(servaddr.sun_path, F_PATH);  /* set path address */

	if (connect(sockfd, (SA *) & servaddr, sizeof(servaddr)) < 0) {        /* trying to connect to the server */
		printf("\nDid you start the server? Please run ./server first.\n");
		perror("connect");
		exit(1);
	}
	
	return sockfd;
}// Telos Initialize_c





void printWelcome(){

printf("\n******************************************************************************\n\n");
printf("                |****WELCOME TO SYSTEM PARKING AND DRIVER****|           ");
printf("\n\n******************************************************************************\n");




}




