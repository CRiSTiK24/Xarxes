/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer aUEBc.c que implementa la capa d'aplicació de UEB, sobre la     */
/* cap de transport TCP (fent crides a la "nova" interfície de la         */
/* capa TCP o "nova" interfície de sockets TCP), en la part client.       */
/*                                                                        */
/* Autors: Cristian Bezerdic Stoica, Guillem Díaz Cabanas                 */
/* Data: 08/11/2022                                                       */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions externes es cridessin entre elles, faria falta fer   */
/*   un #include del propi fitxer capçalera)                              */

#include "UEBp1-tTCP.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1);
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int UEBc_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa UEB, en la part client.                */

/* Crea un socket TCP "client" en una @IP i #port TCP local qualsevol, a  */
/* través del qual demana una connexió a un S UEB que escolta peticions   */
/* al socket TCP "servidor" d'@IP "IPser" i #portTCP "portTCPser".        */
/* Escriu l'@IP i el #port TCP del socket "client" creat a "IPcli" i      */
/* "portTCPcli", respectivament.                                          */
/* Escriu un missatge que descriu el resultat de la funció a "MisRes".    */
/*                                                                        */
/* "IPser" i "IPcli" són "strings" de C (vector de chars imprimibles      */
/* acabats en '\0') d'una longitud màxima de 16 chars (incloent '\0').    */
/* "MisRes" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  l'identificador del socket TCP connectat si tot va bé;                */
/* -1 si hi ha un error a la interfície de sockets.                       */
int UEBc_DemanaConnexio(const char *IPser, int portTCPser, char *IPcli, 
						int *portTCPcli, char *MisRes)
{
	int socket = TCP_CreaSockClient(IPcli, *portTCPcli);
    int retornada = socket;
    if(socket == -1) 
	{
        retornada = -1;
        char tmp[200] = "Hi ha hagut un error al crear el socket Client \n\0";

        strcpy(MisRes, tmp);
    }
    else 
	{
        int conexioCorrecte = TCP_DemanaConnexio(socket, IPser, portTCPser);
        if(conexioCorrecte == -1) 
		{
            retornada = -1;
            char tmp[200] =
				"Hi ha hagut un error al crear la connexió amb el servidor \n\0";
            strcpy(MisRes,tmp);
            MisRes[199] = '\0';
        }
        else 
		{
            if(TCP_TrobaAdrSockLoc(socket, IPcli, portTCPcli) == -1) 
			{
                char tmp[200] = "ERROR: No s'ha treure les ip i ports del socket\n\0";
                strcpy(MisRes,tmp);
                MisRes[199] = '\0';
                retornada = -1;

            }
            else 
			{
                char tmp[200] = "EXIT: S'ha pogut demanar la conexio entre el "
							 "Socket local amb Ip remota\n\0";
                strcpy(MisRes,tmp);
                MisRes[199] = '\0';
            }

        }
    }
    return retornada;
}

/* Obté el fitxer de nom "NomFitx" del S UEB a través de la connexió TCP  */
/* d'identificador "SckCon". Escriu els bytes del fitxer a "Fitx" i la    */
/* longitud del fitxer en bytes a "LongFitx".                             */
/* Escriu un missatge que descriu el resultat de la funció a "MisRes".    */
/*                                                                        */
/* "NomFitx" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud <= 10000 chars (incloent '\0').                   */
/* "Fitx" és un vector de chars (bytes) qualsevol (recordeu que en C,     */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/* "MisRes" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si el fitxer existeix al servidor;                                  */
/*  1 la petició és ERRònia (p.e., el fitxer no existeix);                */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio, etc.); */
/* -3 si l'altra part tanca la connexió.                                  */
int UEBc_ObteFitxer(int SckCon, const char *NomFitx, char *Fitx, int *LongFitx,
					char *MisRes)
{
    int retornada = 0;
    int llargadaPath = strlen(NomFitx);
    if(llargadaPath > 10000 || llargadaPath <= 0) 
	{
        retornada = -2;
        char tmp[200] = "ERROR: El fitxer és més gran de 10000 o més petit de 0\n\0";
        strcpy(MisRes,tmp);
        MisRes[199] = '\0';
    }
    else 
	{
        if(ConstiEnvMis(SckCon, "OBT\0", NomFitx, llargadaPath) == -1) /// error aqui
		{
            retornada = -1;
            char tmp[200] = "ERROR: No s'ha pogut construir i enviar el missatge\n\0";
            strcpy(MisRes,tmp);
            MisRes[199] = '\0';
        }
        else 
		{
            char tipus[4];
            if(RepiDesconstMis(SckCon, tipus, Fitx, LongFitx) == -1) 
			{
                retornada = -1;
                char tmp[200] = "ERROR: No s'ha pogut rebre i desconstruir el missatge\n\0";
                strcpy(MisRes,tmp);
                MisRes[199] = '\0';
            }
            else{
                printf("Fitxer: %s\n", Fitx);
            }
        }
    }
    return retornada;
}

/* Tanca la connexió TCP d'identificador "SckCon".                        */
/* Escriu un missatge que descriu el resultat de la funció a "MisRes".    */
/*                                                                        */
/* "MisRes" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*   0 si tot va bé;                                                      */
/*  -1 si hi ha un error a la interfície de sockets.                      */
int UEBc_TancaConnexio(int SckCon, char *MisRes)
{
	int retornada = 0;
    if(TCP_TancaSock(SckCon) == -1) 
	{
        retornada = -1;
        char tmp[200] = "ERROR: No s'ha pogut tancar el fitxer\n\0";
        strcpy(MisRes,tmp);
        MisRes[199] = '\0';
    }
    else 
	{
        char tmp[200] = "EXIT: S'ha pogut tancar el fitxer\n\0";
        strcpy(MisRes,tmp);
        MisRes[199] = '\0';
    }
    return retornada;
}

/* Si ho creieu convenient, feu altres funcions EXTERNES                  */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int UEBc_FuncioExterna(arg1, arg2...)
{
	
} */

/* Definició de funcions INTERNES, és a dir, d'aquelles que es faran      */
/* servir només en aquest mateix fitxer. Les seves declaracions es        */
/* troben a l'inici d'aquest fitxer.                                      */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int FuncioInterna(arg1, arg2...)
{
	
} */

/* "Construeix" un missatge de PUEB a partir dels seus camps tipus,       */
/* long1 i info1, escrits, respectivament a "tipus", "long1" i "info1"    */
/* (que té una longitud de "long1" bytes), i l'envia a través del         */
/* socket TCP “connectat” d’identificador “SckCon”.                       */
/*                                                                        */
/* "tipus" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud de 4 chars (incloent '\0').                       */
/* "info1" és un vector de chars (bytes) qualsevol (recordeu que en C,    */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio).       */
int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1)
{
    int retornada = 0;
	char buffer[17+long1];
    memcpy(buffer, tipus, 3);
    char charLong[4];
    sprintf(charLong,"%.4d",long1);
    //charLong[4] = "\0"; //TODO: 222:17 makes integer from pointer without a cast
    memcpy(buffer+3, charLong, 4);
    memcpy(buffer+7, info1, long1);
    if(TCP_Envia(SckCon, buffer, 7 + long1) == -1) 
	{
        retornada = -1;
    }
    return retornada;
}

/* Rep a través del socket TCP “connectat” d’identificador “SckCon” un    */
/* missatge de PUEB i el "desconstrueix", és a dir, obté els seus camps   */
/* tipus, long1 i info1, que escriu, respectivament a "tipus", "long1"    */
/* i "info1" (que té una longitud de "long1" bytes).                      */
/*                                                                        */
/* "tipus" és un "string" de C (vector de chars imprimibles acabat en     */
/* '\0') d'una longitud de 4 chars (incloent '\0').                       */
/* "info1" és un vector de chars (bytes) qualsevol (recordeu que en C,    */
/* un char és un enter de 8 bits) d'una longitud <= 9999 bytes.           */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio);       */
/* -3 si l'altra part tanca la connexió.                                  */
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1)
{
    int retornada = 0;
	char buffer[10006];
    // Llegeix el missatge del socket
	//!TODO mirar si TCP_Rep ha llegit menys de 7 bytes i per tant hi ha perrill de segfault

    int read = TCP_Rep(SckCon, buffer, 10006);

    printf("read: %d\n", read);

    if(read == -1) 
	{
        retornada = -1;
    }
    else if(read == 0){
        retornada = -3;
    }
    else 
	{
		/* Guarda a tipus una substring del buffer del char 0 al 2 		  */
        memcpy(tipus, buffer, 3);
        tipus[3] = '\0';
        if(strcmp(tipus,"COR")!=0 && strcmp(tipus,"ERR")!=0){
            retornada = -2;
        }
        else 
		{
			/* Guarda en una nova string tamanyFitxer una substring del   */
			/* char 3 al 7 del buffer								      */
            char tamanyFitxer[4];
            memcpy(tamanyFitxer, buffer+3, 4);

			/* Converteix tamanyFitxer a un enter 						  */
            *long1 = atoi(tamanyFitxer);
            /* Llegeix els caràcters de tamanyFitxer del buffer i els 	  */
			/* guarda a NomFitx 										  */

            memcpy(info1, buffer+7, *long1);
        }
    }
    return retornada;

}