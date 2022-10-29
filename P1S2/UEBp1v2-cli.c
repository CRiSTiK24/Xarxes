/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer cli.c que implementa el client de UEB sobre la capa de          */
/* transport TCP (fent crides a la "nova" interfície de la capa de        */
/* transport o "nova" interfície de sockets).                             */
/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "UEBp1v2-tTCP.h"

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
int bytes_llegits, bytes_escrits;
char buffer[200];
char iprem[16];
int portrem;
/*CANVI, passar la adreça i port per teclat*/
printf("Entra la ip del server remot (menys de 15 caracters)");
scanf("%s",iprem);
printf("Entra el port del server remot (int)");
scanf("%d", &portrem);
TCP_CreaSockClient(iprem,portrem);
/*FI CANVI*/
//strcpy(iprem,"10.0.0.23");
//portrem = 3000; 

/* Un cop fet connect() es diu que el socket scon està "connectat" al socket remot. */
/* Com que és un socket TCP això també vol dir que s'ha establert una connexió TCP. */
/* S'envia pel socket connectat scon el que es llegeix del teclat */
if((bytes_llegits=read(0,buffer,sizeof(buffer)))==-1)
{
perror("Error en read");
 close(scon);
 exit(-1);
}
if((bytes_escrits=write(scon,buffer,bytes_llegits))==-1)
{
 perror("Error en write");
 close(scon);
 exit(-1);
}
/* Es tanca el socket scon, que com que és un socket TCP, també vol dir que es tanca la */
/* connexió TCP establerta. */
close(scon);
return(0);
 /* Declaració de variables, p.e., int n;                                 */

 /* Expressions, estructures de control, crides a funcions, etc.          */

}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */
