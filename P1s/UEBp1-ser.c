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
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 

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
    char arrelUEB[10000];
    char path[10000]; 
    int retorn;
    
    memcpy(path,getcwd(NULL, 0),strlen(getcwd(NULL, 0)));
    memcpy(path+strlen(getcwd(NULL, 0)),"/ser.log",8);
    path[strlen(getcwd(NULL, 0))+8] = '\0';

    int fitxerLog = open(path, O_CREAT| O_WRONLY | O_TRUNC);
    if(fitxerLog < 0){
        printf("Error al crear fitxer log amb: %s\n",path);
    }
    else{
        char exit[100] = "Exit al crear fitxer log\n\0";
        write(fitxerLog,exit,strlen(exit));
    }

    int arrelDiferent = UEBs_ConfiguracioServer(arrelUEB, missatgeError);
    if(arrelDiferent!=0){
        //printf("Error al llegir la configuració\n\0");
        printf("%s\n",missatgeError);
    }
    else{
        char exit[100] = "Exit al cambiar la arrel del servidor a partir del fitxer de configuració\n\0";
        write(fitxerLog,exit,strlen(exit));
    }
	/* Expressions, estructures de control, crides a funcions, etc.       */
    if(UEBs_IniciaServ(&socket, portTCPser, missatgeError) == -1)
	{
        printf("%s\n",missatgeError);
        write(fitxerLog,missatgeError,strlen(missatgeError));
    }
    else 
	{
        write(fitxerLog,missatgeError,strlen(missatgeError));
        printf("La ip del servidor és %s i el port %d\n",IPser,portTCPser);
        do{
        if((socketConnexio = UEBs_AcceptaConnexio(socket, IPser, &portTCPser, IPcli, &portTCPcli, missatgeError)) == -1)
		{
        write(fitxerLog,missatgeError,strlen(missatgeError));
            printf("Error al acceptar connexió\n");
            printf("%s\n",missatgeError);
        }
        else 
		{
        write(fitxerLog,missatgeError,strlen(missatgeError));
                do{
                    retorn = UEBs_ServeixPeticio(socketConnexio, tipusPeticio, nomFitx, missatgeError);
                    write(fitxerLog,missatgeError,strlen(missatgeError));
                    if(retorn == -3)
                    {
                        printf("%s\n",missatgeError);
                        if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                        {
                            write(fitxerLog,missatgeError,strlen(missatgeError));
                            printf("%s\n",missatgeError);
                        }
                    }
                    else if(retorn == -2)
                    {
                        printf("%s\n",missatgeError);
                        if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                        {
                            write(fitxerLog,missatgeError,strlen(missatgeError));
                            printf("%s\n",missatgeError);
                        }
                    }
                    else if(retorn == -4)
                    {
                        printf("%s\n",missatgeError);
                        if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                        {
                            write(fitxerLog,missatgeError,strlen(missatgeError));
                            printf("%s\n",missatgeError);
                        }
                    }
                    else if(retorn == -1)
                    {
                        write(fitxerLog,missatgeError,strlen(missatgeError));
                        printf("%s\n",missatgeError);
                        if(UEBs_TancaConnexio(socketConnexio, missatgeError) == -1)
                        {
                            write(fitxerLog,missatgeError,strlen(missatgeError));
                            printf("%s\n",missatgeError);
                        }
                    }
                }while(retorn >= 0);
            } 
        }while(retorn != -3);
        UEBs_TancaConnexio(socket, missatgeError);
        write(fitxerLog,missatgeError,strlen(missatgeError));
    }
    close(fitxerLog);
    
    return 0;
}

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */
