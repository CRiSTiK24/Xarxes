/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer cli.c que implementa la interfície aplicació-usuari             */
/* d'un client de l'aplicació de UEB, sobre la capa d'aplicació de        */
/* (la part client de) UEB (fent crides a la interfície de la part        */
/* client de la capa UEB).                                                */
/*                                                                        */
/* Autors: Cristian Bezerdic Stoica, Guillem Díaz Cabanas                 */
/* Data: 08/11/2022                                                       */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <stdio.h> o #include "meu.h"     */

#include "UEBp1v3-aUEBc.h"
#include <stdio.h>
#include <stdlib.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
	/* Declaració de variables, p.e., int n;                              */
    int* socket;
    int port = 3000; // despres farem que es llegeixi de fitxer
    char* missatgeError = malloc(200);
    char *IPser;
    int *portTCPser;
    char *IPcli = "0.0.0.0\0";
    int *portTCPcli;
    char *tiposPeticio;
    char *nomFitx = "/patata\0";
    char *fitxer;
    int *longFitx;

	/* Expressions, estructures de control, crides a funcions, etc.       */
    if(UEBc_DemanaConnexio(IPcli,port, IPser, portTCPser, missatgeError) == -1) 
	{
        printf(missatgeError);
    }
    else 
	{
        if(UEBc_ObteFitxer(*socket,nomFitx,fitxer,longFitx,missatgeError) == -1) 
		{
            printf(missatgeError);
        }
        if(UEBc_TancaConnexio(*socket,missatgeError) == -1) 
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

