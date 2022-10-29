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
	*SckEsc = TCP_CreaSockServidor(IPloc, portTCPloc);
    if(*SckEsc==-1){
        *MisRes = "ERROR: El socket ip no s'ha pogut crear\0";
        return -1;
    }
    else{
        *MisRes = "EXIT: El socket s'ha pogut crear\0";
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
    TCP_TrobaAdrSockLoc(SckEsc, IPser, portTCPser);
    TCP_TrobaAdrSockRem(SckEsc, IPcli, portTCPcli);
	if(TCP_AcceptaConnexio(SckEsc, IPcli, portTCPcli) == -1){
        *MisRes = "ERROR:No s'ha pogut acceptar la conexio entre el Socket local amb IP remota\0";
        return -1;
    }
    else{
        *MisRes = "EXIT:S'ha pogut acceptar la conexio entre el Socket local amb Ip remota\0";
        return SckEsc;
    } 
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
    //crea un buffer de 9999+3+4 chars
    char buffer[10006];
    //llavors utilitza el buffer i l'emplenem amb rebtcp
    if(TCP_Rep(SckCon, buffer, 10006) == -1){
        *MisRes = "ERROR: No s'ha pogut rebre la peticio\0";
        return -1;
    }
    //si el buffer es buit, tanca la conexio
    if(buffer[0] == '\0'){
        *MisRes = "ERROR: La conexio s'ha tancat\0";
        return -3;
    }

    //3 primers chars del buffer i guardarlos a tipusPeticio
    strncpy(TipusPeticio, buffer, 3);

    //si tipusPeticio no és igual a OBT, retorna -2
    if(strcmp(TipusPeticio, "OBT") != 0){
        *MisRes = "ERROR: El tipus de peticio no es correcte\0";
        return -2;
    }

    //llegeix els 3 seguents elements del buffer
    char tamanyfitxer[3];
    strncpy(tamanyfitxer, buffer[3], 3);

    //si el int es 0000, retorna -4
    if(tamanyfitxer == "0000"){
        *MisRes = "ERROR: El tamany del fitxer no es correcte\0";
        return -4;
    }

    //llegeix el seguent char
    char primerchar = buffer[7];

    si no és un /, retorna -4
    llegeix tamanyfitxer-1 bits i els guarda a NomFitx
    
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

    if(TCP_TancaConnexio(SckCon) == -1){
        *MisRes = "ERROR: No s'ha pogut tancar la connexio\0";
        return -1;
    }
    else{
        *MisRes = "EXIT: S'ha pogut tancar la connexio\0";
        return 0;
    }


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
	
}
