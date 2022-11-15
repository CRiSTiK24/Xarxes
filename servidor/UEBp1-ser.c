/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer ser.c que implementa la interfície aplicació-administrador      */
/* d'un servidor de l'aplicació de UEB, sobre la capa d'aplicació de      */
/* (la part servidora de) UEB (fent crides a la interfície de la part     */
/* servidora de la capa UEB).                                             */

/*                                                                        */
/* Autors: Cristian Bezerdic Stoica, Guillem Díaz Cabanas                 */
/* Data: 08/11/2022                                                       */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "UEBp1v3-aUEBs.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
 /* Declaració de variables, p.e., int n;                                */
    int socket;
    int port = 45456; // despres farem que es llegeixi de fitxer
    char missatgeError[200];
    char IPser[16];
    int portTCPser;
    char IPcli[16];
    int portTCPcli;
    char tipusPeticio[4];
    char nomFitx[10000];


	/* Expressions, estructures de control, crides a funcions, etc.       */
    if(UEBs_IniciaServ(&socket, port, &missatgeError) == -1) 
	{
        printf(missatgeError);
    }
    else 
	{

        printf("La ip del servidor és %s i el port %d",IPser,portTCPser);
        if(UEBs_AcceptaConnexio(socket, IPser, &portTCPser, IPcli, &portTCPcli, &missatgeError) == -1) 
		{
            printf(missatgeError);
        }
        else 
		{
            if(UEBs_ServeixPeticio(socket, tipusPeticio, nomFitx, &missatgeError) < 0) 
			{
                printf(missatgeError);
            }
            
        }
        if(UEBs_TancaConnexio(socket, &missatgeError) == -1) 
		{
            printf(missatgeError);
        }
    }
    return 0;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */

