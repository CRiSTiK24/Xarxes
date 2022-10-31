/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer aUEBc.c que implementa la capa d'aplicació de UEB, sobre la     */
/* cap de transport TCP (fent crides a la "nova" interfície de la         */
/* capa TCP o "nova" interfície de sockets TCP), en la part client.       */
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
int UEBc_DemanaConnexio(const char *IPser, int portTCPser, char *IPcli, int *portTCPcli, char *MisRes)
{
    int retornada = 0;
	int socket = TCP_CreaSockClient("0.0.0.0", "0");
    if(socket==-1){
        retornada = -1;
        *MisRes = "Hi ha hagut un error al crear el socket Client \0";
    }
    else{
        int conexioCorrecte = TCP_DemanaConnexio(socket, IPser, portTCPser);
        if(connexioCorrecte == -1){
            retornada = -1;
            *MisRes = "Hi ha hagut un error al crear la connexió amb el servidor \0";
        }
        else{
            if(TCP_TrobaAdrSockLoc(SckEsc,IPcli,portTCPcli)==-1 || TCP_TrobaAdrSockRem(SckEsc,IPser,portTCPser) ==-1){
                *MisRes = "ERROR:No s'ha treure les ip i ports del socket\0";
                retornada = -1;

            }
            else{
                *MisRes = "EXIT:S'ha pogut demanar la conexio entre el Socket local amb Ip remota\0";
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
int UEBc_ObteFitxer(int SckCon, const char *NomFitx, char *Fitx, int *LongFitx, char *MisRes)
{
    int retornada = 0;
    int llargadaPath = strlen(NomFitx);
    if(llargadaPath >10000 || llargadaPath <= 0){
        retornada = -2;
        *MisRes = "ERROR: El fitxer és més gran de 10000 o més petit de 0\0";
    }
    else{
        if(ConstiEnvMis(SckCon, "OBT\0", NomFitx, llargadaPath) == -1){
            retornada = -1;
            *MisRes = "ERROR: No s'ha pogut construir i enviar el missatge\0";
        }
        else{
            char* tipus = (char*)malloc(4*sizeof(char));
            if(RepiDesconstMis(SckCon, tipus, Fitx, LongFitx)==-1){
                retornada = -1;
                *MisRes = "ERROR: No s'ha pogut rebre i desconstruir el missatge\0";
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
	if(TCP_TancaSock(SckCon)==-1){
        retornada = -1;
        *MisRes = "ERROR: No s'ha pogut tancar el fitxer\0";
    }
    else{
        *MisRes = "EXIT: S'ha pogut tancar el fitxer\0";
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
/*  0 si tot va bé;                                                       */
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
        if(tipus!="COR"||tipus!="ERR"){
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
