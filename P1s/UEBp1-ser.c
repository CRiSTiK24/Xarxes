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

#include "UEBp1-aUEBs.h"
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
    int socketConnexio;
    char missatgeError[200];
    char IPser[16] = "10.100.100.101\0";
    int portTCPser = 45456;
    char IPcli[16];
    int portTCPcli;
    char tipusPeticio[4];
    char nomFitx[10000];


	/* Expressions, estructures de control, crides a funcions, etc.       */
    if(UEBs_IniciaServ(&socket, portTCPser, missatgeError) == -1)
	{
        printf("%s\n",missatgeError);
    }
    else 
	{

        printf("La ip del servidor és %s i el port %d\n",IPser,portTCPser);
        if((socketConnexio = UEBs_AcceptaConnexio(socket, IPser, &portTCPser, IPcli, &portTCPcli, missatgeError)) == -1)
		{
            printf("Error al acceptar connexió\n\0");
            printf("%s\n",missatgeError);
        }
        else 
		{
            int retorn;
            do{
                retorn = UEBs_ServeixPeticio(socketConnexio, tipusPeticio, nomFitx, missatgeError);
                if(retorn == -3)
                {
                    printf("Error al servir petició -3\n\0");
                    printf("%s\n",missatgeError);
                    if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                    {
                        printf("%s\n",missatgeError);
                    }
                }
                else if(retorn == -2)
                {
                    printf("Error al servir petició -2\n\0");
                    printf("%s\n",missatgeError);
                    if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                    {
                        printf("%s\n",missatgeError);
                    }
                }
                else if(retorn == -4)
                {
                    printf("Error al servir petició -4\n\0");
                    printf("%s\n",missatgeError);
                    if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                    {
                        printf("%s\n",missatgeError);
                    }
                }
                else if(retorn == -1)
                {
                    printf("Error al servir petició -1\n\0");
                    printf("%s\n",missatgeError);
                    if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                    {
                        printf("%s\n",missatgeError);
                    }
                }
            }
            while(retorn>0);
            
        }
        UEBs_TancaConnexio(socket, missatgeError);
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
