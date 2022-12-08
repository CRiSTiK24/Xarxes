/* Afegiu-ho a la zona del fitxer que toqui, a la dels #define, a la de   */
/* les funcions externes, a la de les funcions internes, etc.             */

/* Examina simultàniament durant "Temps" (en [ms]) els sockets (poden ser */
/* TCP, UDP i teclat -stdin-) amb identificadors en la llista “LlistaSck” */
/* (de longitud “LongLlistaSck” sockets) per saber si hi ha arribat       */
/* alguna cosa per ser llegida. Si Temps és -1, s'espera indefinidament   */
/* fins que arribi alguna cosa.                                           */
/*                                                                        */
/* "LlistaSck" és un vector d'int d'una longitud d'almenys LongLlistaSck. */
/*                                                                        */
/* Retorna:                                                               */
/*  l'identificador del socket a través del qual ha arribat alguna cosa;  */
/*  -1 si hi ha error;                                                    */
/*  -2 si passa "Temps" sense que arribi res.                             */
int TCP_HaArribatAlgunaCosaEnTemps(const int *LlistaSck, int LongLlistaSck, int Temps)
{
	struct timeval
	{
		long tv_sec; /* segons 											  */
l		long tv_usec; /* microsegons 									  */
	};
	struct timeval temps;
	temps.tv_sec = Temps / 100;
	temps.tv_usec = 0;
	fd_set conjunt; /* conjunt de descriptors de fitxer de lectura 		  */
	int descmax = -1; /* número de descriptor de fitxer major 			  */
	FD_ZERO(&conjunt); /* esborrem el contingut de la llista 			  */
	int i = 0;
	for(i = 0; i < LongLlistaSck; i++) 
	{
		/* afegim ("marquem") els elements de LlistaSck al conjunt 		  */
		FD_SET(LlistaSck[i], &conjunt); 
		if(LlistaSck[i] > descmax) descmax = LlistaSck[i]+1;
	}
	if(Temps == -1) 
	{
		if(select(descmax+1, &conjunt, NULL, NULL, NULL) == -1)
		{
			return -1;
		}
	}
	else 
	{
		if(select(descmax+1, &conjunt, NULL, NULL, temps) == -1) 
		{
			return -2;
		}
	}
	i = 0;
	bool trobat = false;
	while(i <= LongLlistaSck && !trobat) 
	{
		if(FD_ISSET(LlistaSck[i], &conjunt)) trobat = true;
		else i++;
	}
	if(trobat) return LlistaSck[i];
	else return -1;
}
