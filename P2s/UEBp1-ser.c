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

#define NOMBRECONNSMAX		2
#define TRUE 1
#define FALSE 0

int AfegeixSck(int Sck, int *LlistaSck, int LongLlistaSck);
int TreuSck(int Sck, int *LlistaSck, int LongLlistaSck);

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
 /* Declaració de variables, p.e., int n;                                */
    char path[10000]; 
    char arrelUEB[10000];
    int continuarServidor = TRUE;

   struct PaquetUEB paquet;
   paquet.IPser[16] = "10.100.100.101\0";
   paquet.portTCPser = 45456;
   paquet.LongLlistaSck = 10;
   paquet.arrelUEB = 0;



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

    int arrelDiferent = UEBs_ConfiguracioServer(paquet);
    if(arrelDiferent!=0){
        paquet.arrelUEB = NULL;
        //printf("Error al llegir la configuració\n\0");
        printf("%s\n",paquet.missatgeError);
    }
    else{
        char exit[100] = "Exit al cambiar la arrel del servidor a partir del fitxer de configuració\n\0";
        write(fitxerLog,exit,strlen(exit));
    }

    int InicialitzadorSockets = 0;
    while(InicialitzadorSockets<paquet.LongLlistaSck){
        paquet.LlistaSck[InicialitzadorSockets] = -1;
        InicialitzadorSockets++;
    }
    

	/* Expressions, estructures de control, crides a funcions, etc.       */
    if(UEBs_IniciaServ(paquet) == -1)
	{
        printf("%s\n",paquet.missatgeError);
        write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
    }
    else 
	{
        AfegeixSck(paquet.SckEsc, paquet.LlistaSck,paquet.LongLlistaSck);
        write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
        AfegeixSck(0, paquet.LlistaSck,paquet.LongLlistaSck); //afegim el teclat a la llista
        write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
        printf("La ip del servidor és %s i el port %d\n",paquet.IPser,paquet.portTCPser);

        do{
            printf("LISTA DE SOCKETS: \n\0");

            int LlistarSockets = 0;
            while(LlistarSockets<paquet.LongLlistaSck){
                printf("LlistaSck[%d] = %d\n\0", LlistarSockets, paquet.LlistaSck[LlistarSockets]);
                LlistarSockets++;
            }
            int haArribatAlgo = UEBs_HaArribatAlgunaCosa(paquet);
            printf("Ha arribat alguna cosa: %d\n\0", haArribatAlgo);
            if(haArribatAlgo == -1){
                printf("Error: Error al Ha Arribat Alguna Cosa\n\0");
                printf("%s\n",paquet.missatgeError);
            }
            else if(haArribatAlgo==-2){
                printf("No ha arribat res \n\0");
            }
            else{
                int trobat = 0;
                int i = 0;
                while(!trobat && i<paquet.LongLlistaSck){
                    if(paquet.LlistaSck[i]==haArribatAlgo){
                        trobat = 1;
                    }
                    else{
                        i++;
                    }
                }
                printf("L'hem trobat o no? %d\n\0", trobat);
                printf("socket: %d\n\0", paquet.SckEsc);
                if(paquet.SckEsc == haArribatAlgo){
                    printf("Ha arribat una nova connexió\n\0");
                    int socketAccepta = socketAccepta = UEBs_AcceptaConnexio(paquet);
                    printf("Socket ser:%d",socketAccepta);
                    if(socketAccepta == -1){
                        write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
                        printf("Error al acceptar connexió\n\0");
                        printf("%s\n",paquet.missatgeError);
                    }
                    printf("La ip del client és %s i el port %d\n",paquet.IPcli,paquet.portTCPcli);
                    printf("El socket de la connexió és %d\n",socketAccepta);
                    AfegeixSck(socketAccepta, paquet.LlistaSck ,paquet.LongLlistaSck);
                }
                else if(trobat){
                    if(haArribatAlgo==0){ //Toca llegir de teclat
                        char text[100];
                        
                        int escanejat = scanf("%s", text);
                        if(strcmp(text,"exit")==0){
                            continuarServidor = FALSE;
                            char tmp[200] = "S'ha escrit exit per teclat\n\0";
                            strcpy(paquet.missatgeError,tmp);
                            paquet.missatgeError[199] = '\0';
                            write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
                            printf("%s\n",paquet.missatgeError);
                            int i = 0;
                            while(i<paquet.LongLlistaSck){
                                if(paquet.LlistaSck[i]!=-1){
                                    UEBs_TancaConnexio(paquet);
                                    write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
                                }
                                i++;
                            }
                        }
                        else{
                            char tmp[200] = "No s'ha escrit exit per teclat\n\0";
                            strcpy(paquet.missatgeError,tmp);
                            paquet.missatgeError[199] = '\0';
                            write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
                            printf("%s\n",paquet.missatgeError);
                        }
                    }
                    else{
                        int retorn = UEBs_ServeixPeticio(paquet);
                    write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
                    if(retorn == -3){
                        printf("Error al servir petició -3\n\0");
                    }
                    else if(retorn == -2){
                        printf("Error al servir petició -2\n\0");
                    }
                    else if(retorn == -4){
                        printf("Error al servir petició -4\n\0");
                    }
                    else if(retorn == -1){
                        printf("Error al servir petició -1\n\0");
                        
                    }
                    if(retorn <0){ //si hi ha hagut error a serveix peticio, doncs traiem socket
                        printf("%s\n",paquet.missatgeError);
                        TreuSck(haArribatAlgo, paquet.LlistaSck ,paquet.LongLlistaSck);
                        if(UEBs_TancaConnexio(paquet) == -1)
                        {
                            write(fitxerLog,paquet.missatgeError,strlen(paquet.missatgeError));
                            printf("%s\n",paquet.missatgeError);
                        }
                    }
                    }
                }
                else{
                    printf("No ha arribat ni socket ni algo de la llista, que ha pasat?\n\0");
                }
            }
        }while(continuarServidor);
        UEBs_TancaConnexio(paquet);
        printf("%s\n",paquet.missatgeError);
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

/* Donada la llista d'identificadors de sockets “LlistaSck” (de longitud  */
/* “LongLlistaSck” sockets), hi busca una posició "lliure" (una amb un    */
/* contingut igual a -1) i hi escriu l'identificador de socket "Sck".     */
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha error.                                                     */
int AfegeixSck(int Sck, int *LlistaSck, int LongLlistaSck)
{
	int i; 
	int pos = -1; 
	int retornada = 0;
	for (i = 0; i < LongLlistaSck; i++)
	{
		if (LlistaSck[i] == -1) 
		{ 
			pos = i; 
			break; 
		} 
	}
	if (pos != -1) LlistaSck[pos] = Sck; 
	else retornada = -1;
	return retornada;
}

/* Donada la llista d'identificadors de sockets “LlistaSck” (de longitud  */
/* “LongLlistaSck” sockets), hi busca la posició on hi ha l'identificador */
/* de socket "Sck" i la marca com "lliure" (hi escriu un contingut igual  */
/* a -1).                                                                 */ 
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha error.                                                     */
int TreuSck(int Sck, int *LlistaSck, int LongLlistaSck)
{
	int i;
	int pos = Sck;
	int retornada = 0;
	for (i = 0; i < LongLlistaSck; i++) 
	{
		if (LlistaSck[i] == Sck)
		{
			pos = i;
			break;
		}
	}
	if (pos != Sck) LlistaSck[pos] = -1; 
	else retornada = -1;
	return retornada;
}
