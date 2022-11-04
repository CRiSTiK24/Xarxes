/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer ser.c que implementa el servidor de UEB sobre la capa de        */
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
	
	int SckEsc, SckConv;
	int bytes_llegits, bytes_escrits;
	char buffer[200];
	socklen_t long_adrrem;
	struct sockaddr_in adrloc, adrrem;
	char iprem[16], iploc[16];
	int portrem, portloc;
	
	/* Expressions, estructures de control, crides a funcions, etc.       */
	
	/* Creem el socket del servidor 									  */		
	if((SckEsc = TCP_CreaSockServidor("0.0.0.0", 0)) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	/* Acceptem la petició de connect									  */
	if((SckConv = TCP_AcceptaConnexio(SckEsc, iprem, portrem)) == -1)
	{
		return(-1);
	} 
	
	/* Ara adrrem conté l'adreça del socket remot (@IP i #port TCP).      */
	
	/* Obtenim la informació del socket local							  */
	if(TCP_TrobaAdrSockLoc(SckConv, iploc, portloc == -1)
	{
		printf("%s\n", TCP_ObteMissError());
		exit(-1);
	}
	
	/* S'escriu a pantalla el que arriba pel socket connectat SckConv     */
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
	/* dir que es tanca la connexió TCP establerta, i es tanca el socket  */
	/* d'escolta sesc.                        							  */
	if(TCP_TancaSock(SckConv) == -1)
	{
		printf("%s\n", TCP_ObteMissError());
	}
	
	if(TCP_TancaSock(SckEsc) == -1)
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
