#ifndef _TYPES_H		/* Avoid conflict with other .h file */
#define _TYPES_H


#define SA struct sockaddr
#define F_PATH "./unix.srt"     /* unix socket path desciptor */


/*Sta8eres*/
#define DRNAME_SIZE 40
#define PKNAME_SIZE 15
#define INFO_SIZE 80


/*Structures*/

/*Odhgoi*/
typedef struct driver{
          int driverID;/*Driver ID*/ 
          char dr_name[DRNAME_SIZE];  /*Pinakas me to onoma tou odhgou*/
          int  money; /*Xrhmata ston trapeziko logariasmo*/
          char dr_history[PKNAME_SIZE][4]; /*Pinaka me ta onomata to teleftaiwn 4 parking pou parkare o odhgos*/
          }driver;


/*Idiokthtes parking*/
typedef struct parking{
          int parkingID; /*Parking ID*/
          char pk_name[ PKNAME_SIZE]; /*Pinakas me to onoma tou parking*/
          char info[INFO_SIZE]; /*Pinaka me thn perigrafh tou parking*/
          int price; /*Xrewsh parking*/
          char pk_history[DRNAME_SIZE][10]; /*Pinakas me tous teleftaious 10 odhgous pou parkaran sto parking*/
          int pk_pos[5];
          }parking;
          
          
#endif          
