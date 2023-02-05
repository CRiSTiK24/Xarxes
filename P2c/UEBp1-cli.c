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

#include "UEBp1-aUEBc.h"
#include "UEBp2-aDNSc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h> 

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

int desferURIm1(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx);
/* int FuncioInterna(arg1, arg2...);                                      */


int main(int argc,char *argv[])
{
	/* Declaració de variables, p.e., int n;                              */
    int socket;
    char missatgeError[200];
    char IPser[16];
    char IPcli[16] = "10.100.100.102\0";
    int portTCPcli = 0;
    char fitxer[10000];
    int longFitx = 0;

	/* Expressions, estructures de control, crides a funcions, etc.       */
    char uri[100];
	char esquema[100], nom_host[100], nomFitx[10000];
	int portTCPser;
	int n;
	 
	/* Es demana un URI */
	printf("Entra la URL del que vols obtenir amb format protocol://nomDNS:port/nomFitxer\n");
    printf("per exemple, pueb://PC-a:45456/llocUEB/primera.html: ");
	int escanejat = scanf("%s", uri);
	
    while(scanf != 0){ //finalitzem quan el usuari no posi res al scanf
        /* Es desfà l'URI, mètode 1 */ 
        n = desferURIm1(uri, esquema, nom_host, &portTCPser, nomFitx);
        if(DNSc_ResolDNSaIP(nom_host, IPser, missatgeError)!=0){
            printf("%s\n",missatgeError);
        }
        if(strcmp(esquema,"pueb")!=0){
            printf("L'esquema no és correcte, cal que sigui pueb\n");
        }
        printf("Esquema: %s\n",esquema);
        printf("NomHost: %s\n",nom_host);
        printf("IPservidor: %s\n",IPser);
        printf("PORTservidor: %d\n",portTCPser);
        printf("NomFitxer: %s\n",nomFitx);

        char path[2000];
        int llargadaPath = strlen(getcwd(NULL, 0));
        memcpy(path, getcwd(NULL, 0), llargadaPath);
        memcpy(path + llargadaPath, nomFitx, strlen(nomFitx));
        path[llargadaPath+ strlen(nomFitx)] = '\0';

        int fitxerSortida = open(path, O_CREAT| O_WRONLY | O_TRUNC);
        if(fitxerSortida < 0){
            printf("Error al crear fitxer sortida amb: %s\n",path);
        }
        else{
            char exit[100] = "Exit al crear fitxer on escriuriem el rebut\n\0";
        }

        portTCPcli = 0; //Perque es faci servir un port aleatori, ja que si no, només va el primer cop ja que es guarda el port anterior i intenta usar-lo un altre cop, quan ja esta en us

        socket = UEBc_DemanaConnexio(IPser, portTCPser, IPcli, &portTCPcli, missatgeError);

        int conexioActiva = 0;
        if(socket == -1)
        {
            printf("%s\n",missatgeError);
        }
        else 
        {
            conexioActiva = 1;
            int estatusFitxer = UEBc_ObteFitxer(socket,nomFitx,fitxer,&longFitx,missatgeError);
            if(estatusFitxer < 0 || estatusFitxer == 1)
            {
                printf("%s\n",missatgeError);
            }
            else{
                    write(fitxerSortida, fitxer, longFitx);
                    printf("Fitxer rebut exitosament\n"); 
                    close(fitxerSortida);
                }
        }
        printf("Esperant 10 sec: \n");
        sleep(10);
        printf("Tancant conexió: \n");
        if(UEBc_TancaConnexio(socket,missatgeError) == -1){
                printf("%s\n",missatgeError);
        }

        printf("\n");
        printf("Entra la URL del que vols obtenir amb format protocol://nomDNS:port/nomFitxer\n");
        printf("per exemple, pueb://PC-a:45456/llocUEB/primera.html: ");
	    int escanejat = scanf("%s", uri);
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


/* Desfà l'URI "uri" en les seves parts: l'esquema (protocol) "esq", el   */
/* nom DNS (o l'@IP), el "nom_host", el número de port "port" i el nom    */
/* del fitxer "nom_fitxer".                                               */
/*                                                                        */
/* L'URI ha de tenir la forma "esq://nom_host:port/nom_fitxer" o bé       */
/* sense el número de port "esq://nom_host/nom_fitxer", i llavors port    */
/* s'emplena amb el valor 0 (la resta de casos no es contemplen).         */
/*                                                                        */
/* "uri", "esq", "nom_host" i "nom_fitxer" són "strings" de C (vector de  */
/* chars imprimibles acabat en '\0') d'una longitud suficient.            */
/*                                                                        */
/* Retorna:                                                               */
/*  el nombre de parts de l'URI que s'han assignat (4 si l'URI tenia      */
/*  número de port o 3 si no en tenia.                                    */
int desferURIm1(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx)
{
	int nassignats;
	char port_str[100];
	 
	strcpy(esq, "");
	strcpy(nom_host, "");
	*port = 0;
	strcpy(nom_fitx, "");

	nassignats = sscanf(uri, "%[^:]://%[^:]:%[^/]%s", esq, nom_host, port_str, nom_fitx);
	 
	/*
	printf("nassignats %d\n",nassignats);
	printf("esq %s\n", esq);
	printf("nom_host %s\n", nom_host);
	printf("port_str %s\n", port_str);
	printf("nom_fitx %s\n", nom_fitx);
	*/
	 
	/* URIs amb #port, p.e., esq://host:port/fitx, 4 valors assignats */
	if(nassignats == 4)
	{
		*port = atoi(port_str);
		return nassignats;
	}  
	  
	/* URIs sense #port, p.e., esq://host/fitx, 2 valors assignats,  */
	/* i llavors es fa port = 0.                                     */
	if(nassignats == 2)
	{
		*port = 0;
		nassignats = sscanf(uri, "%[^:]://%[^/]%s", esq, nom_host, nom_fitx);
		return nassignats;
	}

	return nassignats;
}
