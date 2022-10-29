/*
La funció inet_addr(), donada una @IP en format decimal de vector de caràcters acabat en '\0' en
el format d’ordenació de bytes de la màquina, la converteix a un enter unsigned long en el format
d’ordenació de bytes de la xarxa, mentre que inet_ntoa() ho fa al revés. Les seves declaracions són
les següents:
*/

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
unsigned long inet_addr(const char *adr_decimal); //ip vec decimals acabat en \0, pasa a ser un 
char *inet_ntoa(struct in_addr adr_ulong);


/*
La funció htons(), donat un enter unsigned short en el format d’ordenació de bytes de la màquina,
el converteix a un enter unsigned short en el format d’ordenació de bytes de la xarxa, mentre que
ntohs() ho fa al revés. Les funcions htonl() i ntohl() fan el mateix pel cas d’un enter unsigned
long. Les seves declaracions són les següents:
*/

#include <netinet/in.h>
unsigned short htons(unsigned short local_ushort);
unsigned long htonl(unsigned long local_ulong);
unsigned short ntohs(unsigned short xarxa_ushort);
unsigned long ntohl(unsigned long xarxa_ulong);

//com crear un socket

#include <sys/types.h>
#include <sys/socket.h>
int socket(AF_INET,SOCK_STREAM,0);

//AF_INET <- TCP/IP
//SOCK_STREAM <- TCP ; SOCK_DGRAM <- UDP
//protocol 0

/*
//SERVER POSAR IP I PORT
#include <sys/types.h>
#include <sys/socket.h>
int bind(int s,struct sockaddr *loc_adr,int loc_adrlon); // cal assignar port avans, o es posa un automatic
*/
/*
//CLIENT POSAR IP I PORT
PAG 18

#include <sys/types.h>
#include <sys/socket.h>
int connect(int s,struct sockaddr *rem_adr,int rem_adrlon);

Getter socket adrress: getsockname()
*/