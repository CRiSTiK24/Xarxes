/**************************************************************************/
/*                                                                        */
/* P1 - UEB amb sockets TCP/IP - Part I                                   */
/* Fitxer capçalera de aUEBs.c                                            */
/*                                                                        */
/* Autors: Cristian Bezerdic Stoica, Guillem Díaz Cabanas                 */
/* Data: 08/11/2022                                                       */
/*                                                                        */
/**************************************************************************/

/* Declaració de funcions EXTERNES de aUEBs.c, és a dir, d'aquelles       */
/* funcions que es faran servir en un altre fitxer extern a aUEBs.c,      */
/* p.e., int UEBs_FuncioExterna(arg1, arg2...) { }                        */
/* El fitxer extern farà un #include del fitxer aUEBs.h a l'inici, i      */
/* així les funcions seran conegudes en ell.                              */
/* En termes de capes de l'aplicació, aquest conjunt de funcions          */
/* EXTERNES formen la interfície de la capa UEB, en la part servidora.    */
#define MaximSockets 10
typedef struct Paquet{
    int SckEsc;
    int SckCon;

    char IPser[16];
    char IPcli[16];

    int portTCPser;
    int portTCPcli;

    char tipusPeticio[4];
    char longPeticio[5];
    char infoPeticio[9999];
    char NomFitx[10000];

    int LongLlistaSck;
    int LlistaSck[10]; //quan li ho poso amb el define no va

    char arrelUEB[10000];
    

    char missatgeError[200];
} PaquetUEB;

int UEBs_IniciaServ(PaquetUEB paquet);
int UEBs_AcceptaConnexio(PaquetUEB paquet);
int UEBs_ServeixPeticio(PaquetUEB paquet);
int UEBs_TancaConnexio(PaquetUEB paquet);
int UEBs_HaArribatAlgunaCosa(PaquetUEB paquet);
int UEBs_TancaConnexio(PaquetUEB paquet);
int UEBs_ConfiguracioServer(PaquetUEB paquet);
/* int UEBs_FuncioExterna(arg1, arg2...);                                 */
