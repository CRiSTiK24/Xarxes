/* Afegiu-ho a la zona del fitxer que toqui, a la dels #define, a la de   */
/* les funcions externes, a la de les funcions internes, etc.             */

/* Examina simultàniament i sense límit de temps (una espera indefinida)  */
/* els sockets (poden ser TCP, UDP i  teclat -stdin-) amb identificadors  */
/* en la llista “LlistaSck” (de longitud “LongLlistaSck” sockets) per     */
/* saber si hi ha arribat alguna cosa per ser llegida, excepte aquells    */
/* que tinguin identificadors igual a -1.                                 */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*  l'identificador del socket a través del qual ha arribat alguna cosa;  */
/*  -1 si hi ha error.                                                    */
int UEBs_HaArribatAlgunaCosa(const int *LlistaSck, int LongLlistaSck, char *TextRes)
{
	if (TCP_HaArribatAlgunaCosaEnTemps(LlistaSck, LongLlistaSck, -1) == -1)
	{
		char tmp[200] = " \0";
        strncpy(TextRes, tmp, strlen(tmp));
        TextRes[sizeof TextRes - 1] = '\0';
	}
	char tmp[200] = " \0";
    strncpy(TextRes, tmp, strlen(tmp));
    TextRes[sizeof TextRes - 1] = '\0';
	return TCP_HaArribatAlgunaCosaEnTemps(LlistaSck, LongLlistaSck, -1);
}

/* Tanca la connexió TCP d'identificador "SckCon".                        */
/* Escriu un text que descriu el resultat de la funció a "TextRes".       */
/*                                                                        */
/* "TextRes" és un "string" de C (vector de chars imprimibles acabat en   */
/* '\0') d'una longitud màxima de 200 chars (incloent '\0').              */
/*                                                                        */
/* Retorna:                                                               */
/*   0 si tot va bé;                                                      */
/*  -1 si hi ha un error a la interfície de sockets.                      */
int UEBs_TancaConnexio(int SckCon, char *TextRes)
{
	int retornada = 0;
    if(TCP_TancaSock(SckCon) == -1) 
	{
		retornada = -1;
        char tmp[200] = "ERROR: No s'ha pogut tancar el fitxer\0";
        strncpy(TextRes, tmp, strlen(tmp));
        TextRes[sizeof TextRes - 1] = '\0';
    }
    else 
	{
        char tmp[200] = "EXIT: S'ha pogut tancar el fitxer\0";
        strncpy(TextRes, tmp, strlen(tmp));
        TextRes[sizeof TextRes - 1] = '\0';
    }
    return retornada;
}
