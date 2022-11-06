/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer aUEB.c que implementa la capa d'aplicació de UEB, sobre la      */
/* cap de transport TCP (fent crides a la "nova" interfície de la         */
/* capa TCP o "nova" interfície de sockets TCP), en la part servidora.    */
/*                                                                        */
/* Autors:                                                                */
/* Data:                                                                  */
/*                                                                        */
/**************************************************************************/

/* Inclusió de llibreries, p.e. #include <sys/types.h> o #include "meu.h" */
/*  (si les funcions externes es cridessin entre elles, faria falta fer   */
/*   un #include del propi fitxer capçalera)                              */

#include "UEBp1v3-tTCP.h"
#include <unistd.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/stat.h"
#include "sys/types.h"
#include <fcntl.h> 


/* Definició de constants, p.e.,                                          */

/* #define XYZ       1500                                                 */

/* Declaració de funcions INTERNES que es fan servir en aquest fitxer     */
/* (les  definicions d'aquestes funcions es troben més avall) per així    */
/* fer-les conegudes des d'aquí fins al final d'aquest fitxer, p.e.,      */

/* int FuncioInterna(arg1, arg2...);                                      */

int ConstiEnvMis(int SckCon, const char *tipus, const char *info1, int long1);
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1);

/* Definició de funcions EXTERNES, és a dir, d'aquelles que es cridaran   */
/* des d'altres fitxers, p.e., int UEBs_FuncioExterna(arg1, arg2...) { }  */
/* En termes de capes de l'aplicació, aquest conjunt de funcions externes */
/* formen la interfície de la capa UEB, en la part servidora.             */

/* Inicia el S UEB: crea un nou socket TCP "servidor" a una @IP local     */
/* qualsevol i al #port TCP “portTCPser”. Escriu l'identificador del      */
/* socket creat a "SckEsc".                                               */
/* Escriu un missatge que descriu el resultat de la funció a "MisRes".    */
/*                                                                        */
/* "MisRes" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si tot va bé;                                                       */
/* -1 si hi ha un error en la interfície de sockets.                      */
int UEBs_IniciaServ(int *SckEsc, int portTCPser, char *MisRes)
{
    const char *IPloc = "0.0.0.0\0";
    if(portTCPser==0)portTCPser=3000; // llegir port de fitxer en el main
	*SckEsc = TCP_CreaSockServidor(IPloc, portTCPser);
    if(*SckEsc==-1){
        char * tmp = "ERROR: El socket ip no s'ha pogut crear\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
        return -1;
    }
    else{
        char * tmp = "EXIT: El socket s'ha pogut crear\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
        return *SckEsc;
    }
}

/* Accepta una connexió d'un C UEB que arriba a través del socket TCP     */
/* "servidor" d'identificador "SckEsc". Escriu l'@IP i el #port TCP del   */
/* socket TCP "client" a "IPcli" i "portTCPcli", respectivament, i l'@IP  */
/* i el #port TCP del socket TCP "servidor" a "IPser" i "portTCPser",     */
/* respectivament.                                                        */
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
int UEBs_AcceptaConnexio(int SckEsc, char *IPser, int *portTCPser, char *IPcli, int *portTCPcli, char *MisRes)
{
    int retornada;
    if(TCP_TrobaAdrSockLoc(SckEsc,IPser,portTCPser)==-1 || TCP_TrobaAdrSockRem(SckEsc,IPcli,portTCPcli) ==-1){
        char * tmp = "ERROR:No s'ha treure les ip i ports del socket\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
        retornada = -1;

    }
    else{
        if(TCP_AcceptaConnexio(SckEsc, IPcli, portTCPcli) == -1){
            char * tmp = "ERROR:No s'ha pogut acceptar la conexio entre el Socket local amb IP remota\0";
            strncpy(MisRes, tmp, strlen(tmp));
            MisRes[sizeof MisRes - 1] = '\0';
            retornada = -1;
        }
        else{
            char * tmp = "EXIT:S'ha pogut acceptar la conexio entre el Socket local amb Ip remota\0";
            strncpy(MisRes, tmp, strlen(tmp));
            MisRes[sizeof MisRes - 1] = '\0';
        }
    }
    return retornada;
}

/* Serveix una petició UEB d'un C a través de la connexió TCP             */
/* d'identificador "SckCon". A "TipusPeticio" hi escriu el tipus de       */
/* petició (p.e., OBT) i a "NomFitx" el nom del fitxer de la petició.     */
/* Escriu un missatge que descriu el resultat de la funció a "MisRes".    */
/*                                                                        */
/* "TipusPeticio" és un "string" de C (vector de chars imprimibles acabat */
/* en '\0') d'una longitud de 4 chars (incloent '\0').                    */
/* "NomFitx" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud <= 10000 chars (incloent '\0').                   */
/* "MisRes" és un "string" de C (vector de chars imprimibles acabat en    */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  0 si el fitxer existeix al servidor;                                  */
/*  1 la petició és ERRònia (p.e., el fitxer no existeix);                */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio, etc.); */
/* -3 si l'altra part tanca la connexió;                                  */
/* -4 si hi ha problemes amb el fitxer de la petició (p.e., nomfitxer no  */
/*  comença per /, fitxer no es pot llegir, fitxer massa gran, etc.).     */
int UEBs_ServeixPeticio(int SckCon, char *TipusPeticio, char *NomFitx, char *MisRes)
{
    int retornada = 0;
    int* tamanyFitxer;
    int err = RepiDesconstMis(SckCon, TipusPeticio, NomFitx,tamanyFitxer);
    if(err==-1){
        char * tmp = "ERROR: Interficie socket ha retornat -1\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
        retornada = -1;
    }
    else if(err==-2){
        char * tmp = "ERROR: El tipus de peticio no es correcte o El tamany del fitxer no es correcte\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
        retornada = -2;
    }
    //if NomFitx[0] is not '/', return -4
    else if(NomFitx[0] != '/'){
        char * tmp = "ERROR: El nom del fitxer ha de comensar per \0";
        retornada = -4;
    }
    //create a buffer that adds the currentpath and NomFitx
    else{
        int llargadaPath = strlen(getcwd(NULL,0));
        char* path = malloc(*tamanyFitxer+llargadaPath+1);
        memcpy(path,getcwd(NULL,0), llargadaPath);
        memcpy(path+llargadaPath,NomFitx, *tamanyFitxer);
        memcpy(path+llargadaPath+*tamanyFitxer,'\0',1);//ho convertim en string
        struct stat informacióFitxer;
        if(stat(path, &informacióFitxer) == -1){
            char * tmp = "ERROR: El fitxer no existeix\0";
            strncpy(MisRes, tmp, strlen(tmp));
            MisRes[sizeof MisRes - 1] = '\0';
            retornada =  1;
        }
        else if(informacióFitxer.st_size > 9999){
            char * tmp = "ERROR: El fitxer es massa gran\0";
            strncpy(MisRes, tmp, strlen(tmp));
            MisRes[sizeof MisRes - 1] = '\0';
            retornada = -4;
        }
        else if(informacióFitxer.st_size == 0){
            char * tmp = "ERROR: El fitxer esta buit\0";
            strncpy(MisRes, tmp, strlen(tmp));
            MisRes[sizeof MisRes - 1] = '\0';
            retornada =  -4;
        }
        else{
            int fdArchiu=open(path,O_RDONLY);
            if(fdArchiu==-1){
                retornada = -4;
                char * tmp = "ERROR: No s'ha pogut obrir el fitxer\0";
                strncpy(MisRes, tmp, strlen(tmp));
                MisRes[sizeof MisRes - 1] = '\0';
            }
            char *bufferArchiu = (char*)malloc(9999*sizeof(char));
            if(read(fdArchiu, bufferArchiu,9999)==-1){
                retornada = -4;
                char * tmp = "ERROR: No s'ha pogut llegir el fitxer\0";
                strncpy(MisRes, tmp, strlen(tmp));
                MisRes[sizeof MisRes - 1] = '\0';
            }
            else{
                int enviament = ConstiEnvMis(SckCon, "COR\0", bufferArchiu, informacióFitxer.st_size);
                if(enviament == -1){
                    char * tmp = "ERROR: a la interficie de sockets\0";
                    strncpy(MisRes, tmp, strlen(tmp));
                    MisRes[sizeof MisRes - 1] = '\0';
                    retornada = -1;
                }
                else{
                    char * tmp = "EXIT: S'ha enviat el missatge \0";
                    strncpy(MisRes, tmp, strlen(tmp));
                    MisRes[sizeof MisRes - 1] = '\0';
                }
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
int UEBs_TancaConnexio(int SckCon, char *MisRes)
{
    int retornada = 0;
    if(TCP_TancaSock(SckCon)==-1){
        retornada = -1;
        char * tmp = "ERROR: No s'ha pogut tancar el fitxer\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
    }
    else{
        char * tmp = "EXIT: S'ha pogut tancar el fitxer\0";
        strncpy(MisRes, tmp, strlen(tmp));
        MisRes[sizeof MisRes - 1] = '\0';
    }
    return retornada;
}

/* Si ho creieu convenient, feu altres funcions EXTERNES                  */

/* Descripció de la funció, dels arguments, valors de retorn, etc.        */
/* int UEBs_FuncioExterna(arg1, arg2...)
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
	char *buffer = (char*)malloc((7+long1)*sizeof(char));
    memcpy(buffer,tipus, 3);
    char * longitud;
    sprintf(longitud, "%.4d", long1);
    memcpy(buffer+3,longitud,4);
    memcpy(buffer+7,info1,long1);
    if(TCP_Envia(SckCon, buffer, 7+long1)==-1){
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
/*  0 si tot va bé,                                                       */
/* -1 si hi ha un error a la interfície de sockets;                       */
/* -2 si protocol és incorrecte (longitud camps, tipus de peticio);       */
/* -3 si l'altra part tanca la connexió.                                  */
int RepiDesconstMis(int SckCon, char *tipus, char *info1, int *long1)
{
    int retornada = 0;
	char *buffer = (char*)malloc(1006*sizeof(char));
    //read the message from the socket
    int read = TCP_Rep(SckCon, buffer, 1006);  //TODO mirar si TCP_Rep ha llegit menys de 7 bytes i per tant hi ha perrill de segfault
    //if read is -1, return -1
    if(read == -1){
        int retornada = -1;
    }
    else{
        //save in TiposPeticio the substring of the buffer from 0 to 2
        memcpy(tipus, buffer, 3);
        tipus[3] = '\0';
        if(tipus!="OBT"){
            int retornada = -2;
        }
        else{
            //save in a new string called tamanyFitxer the substring from 3 to 7 of buffer
            char *tamanyFitxer;
            memcpy(tamanyFitxer, buffer+3, 5);
            //convert tamanyFitxer to int
            *long1 = atoi(tamanyFitxer);
            //if tamanyFitxer is 0 or a number, return -2
            for(int i = 0; i<4; i++){
                if(tamanyFitxer[i]<='0' || tamanyFitxer[i]>'9'){
                    int retornada = -2;
                }
            }
            //read tamanyFitxer chars from buffer and save it in NomFitx
            memcpy(info1, buffer+8, *long1);
        }
    }
    return retornada;

}
