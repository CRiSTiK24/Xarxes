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
#include <stdio.h>
#include <stdlib.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */
int desferURIm1(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx);
int desferURIm2(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx);
/* int FuncioInterna(arg1, arg2...);                                      */

int main(int argc,char *argv[])
{
	/* Declaració de variables, p.e., int n;                              */
    int socket;
    char missatgeError[200];
    char IPser[16] = "10.100.100.101\0"; // TODO: fer que es llegeixi de teclat
    int portTCPser = 45456;
    char IPcli[16] = "10.100.100.102\0";
    int portTCPcli = 0; // despres farem que es llegeixi de fitxer
    //char tiposPeticio[4] = "OBT\0";
    char nomFitx[10000] = "/llocUEB/primera.html\0"; //TODO: fer que es llegeixi de teclat
    char fitxer[10000];
    int longFitx = 0;

	/* Expressions, estructures de control, crides a funcions, etc.       */
    socket = UEBc_DemanaConnexio(IPser, portTCPser, IPcli, &portTCPcli, missatgeError);

    if(socket == -1)
	{
        printf("%s\n",missatgeError);
    }
    else 
	{
        if(UEBc_ObteFitxer(socket,nomFitx,fitxer,&longFitx,missatgeError) == -1)
		{
            printf("%s\n",missatgeError);
        }
        printf("%s\n",fitxer);
        if(UEBc_TancaConnexio(socket,missatgeError) == -1)
		{
            printf("%s\n",missatgeError);
        }
    }
    return 0;
}


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
int desferURIm2(const char *uri, char *esq, char *nom_host, int *port, char *nom_fitx)
{
	char *urilocal, *hostportlocal;
	char *token;
	int nassignats;
	char port_str[100];
 
	urilocal = (char *) malloc(sizeof(char) * (strlen(uri) + 1));
	strcpy(urilocal, uri);
 
	strcpy(esq, "");
	strcpy(nom_host, "");
	*port = 0;
	strcpy(nom_fitx, "");
	nassignats = 0;
 
	/* La part "esq": fins trobar ":" */
	token = strtok(urilocal, ":");
	if(token == NULL)
	{
		free(urilocal);
		return nassignats;
	}
	strcpy(esq,token);
	nassignats++;
 
	/* La part "nom_host:port": fins trobar "/" */
	/* Es guarda temporalment a hostportlocal   */
	token = strtok(NULL, "/");
	if(token == NULL)
	{
		free(urilocal);
		return nassignats;
	}
	hostportlocal = (char *) malloc(sizeof(char) * (strlen(token) + 1));
	strcpy(hostportlocal,token);
	nassignats++;
  
	/* La part "nom_fitxer": fins al final, p.e., trobar "" */
	/* Cal afegir-li al davant "/"                          */
	token = strtok(NULL, "");
	if(token == NULL)
	{
		free(urilocal);
		free(hostportlocal);
		return nassignats;
	}
	nom_fitx[0] = '/';
	strcpy(nom_fitx+1,token);
	nassignats++;
 
	/* La part de "nom_host:port" o potser "nom_host" sense "port"  */
	/* Cal processar hostportlocal per trobar ":"                   */
	/*  Si no hi és, vol dir que és sense "port", i es fa port = 0  */
	/*  Si hi és, vol dir que és amb "port" i es separa             */
	token = strtok(hostportlocal, ":");
	if(token == NULL)
	{
		strcpy(nom_host, hostportlocal);
		*port = 0;
		free(urilocal);
		free(hostportlocal);
		return nassignats;
	}
	strcpy(nom_host,token);
 
	token = strtok(NULL, "");
	if(token == NULL)
	{
		free(urilocal);
		free(hostportlocal);
		return nassignats;
	}
	strcpy(port_str, token);
	*port = atoi(port_str);
	nassignats++;
 
	/*
	printf("--nassignats %d\n",nassignats);
	printf("--esq %s\n", esq);
	printf("--nom_host %s\n", nom_host);
	printf("--port_str %s\n", port_str);
	printf("--nom_fitx %s\n", nom_fitx);
	*/
  
	free(urilocal);
	free(hostportlocal);

	return nassignats;
}


/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es troben */
/* a l'inici d'aquest fitxer.                                             */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/*int FuncioInterna(arg1, arg2...)
{
	
} */

