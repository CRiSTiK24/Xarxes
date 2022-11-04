/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer cli.c que implementa el client de UEB sobre la capa de          */
/* transport TCP (fent crides a la "nova" interfície de la capa de        */
/* transport o "nova" interfície de sockets).                             */
/*                                                                        */
/* Autors: Cristian Bezerdic Stoica, Guillem Díaz Cabanas                 */
/* Data: 08/11/2022                                                       */
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
	/* Declaració de variables, p.e., int n;                              */
 
	int SckConv;
	int bytes_llegits, bytes_escrits;
	char buffer[200];
	char iprem[16], iploc[16];
	int portrem, portloc;

	/* Expressions, estructures de control, crides a funcions, etc.       */
	
	/* Creem el socket local 											  */
	if((SckConv = TCP_CreaSockClient("0.0.0.0", 0)) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	/* Demanem connexió amb el servidor 								  */
	if(TCP_DemanaConnexio(SckConv, iprem, portrem) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	/* Un cop fet connect() es diu que el socket scon està "connectat" al */ 
	/* socket remot. Com que és un socket TCP això també vol dir que s'ha */
	/* establert una connexió TCP.     									  */
	
	/* Obtenim la informació del socket local							  */
	if(TCP_TrobaAdrSockLoc(SckConv, iploc, portloc == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	/* S'envia pel socket connectat scon el que es llegeix del teclat     */
	if(TCP_Rep(SckConv, buffer, sizeof(buffer)) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	if(TCP_Envia(SckConv, buffer, bytes_llegits) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	/* Es tanca el socket scon, que com que és un socket TCP, també vol   */ 
	/* dir que es tanca la connexió TCP establerta.						  */
	if(TCP_TancaSock(SckConv) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
	}
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */
