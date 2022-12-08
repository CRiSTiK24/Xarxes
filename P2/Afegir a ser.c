/* Afegiu-ho a la zona del fitxer que toqui, a la dels #define, a la de   */
/* les funcions externes, a la de les funcions internes, etc.             */

#define NOMBRECONNSMAX		2

int AfegeixSck(int Sck, int *LlistaSck, int LongLlistaSck);
int TreuSck(int Sck, int *LlistaSck, int LongLlistaSck);

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
		if (LlistSck[i] == -1) 
		{ 
			pos = i; 
			break; 
		} 
	}
	if (pos != -1) LlistSck[pos] = Sck; 
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
		if (LlistSck[i] == Sck)
		{
			pos = i;
			break;
		}
	}
	if (pos != Sck) LlistSck[pos] = -1; 
	else retornada = -1;
	return retornada;
}
