/*************************************************************************************************************
*    Titre:         Module d'accès pour eeprom interne
*    Fichier:       eeprom.c
*    Date:          06/02/2007 modifié pour atmega 328 19/08/2011
*************************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include "eeprom.h"

/**************************************************************************************************************
 ** @file eeprom.c
 ** @brief	Module d'accès pour eeprom interne
 **************************************************************************************************************/														

/*************************************************************************************************************
 	Fonction :		EEPROM_write  
 	Description :	Écriture sequentielle sur le EEPROM interne à partir d'un tampon.
 	Paramètres: 	Adresse de départ sur le EEPROM, longueur à écrire, tampon
 	Valeur retour:	Aucune.
**************************************************************************************************************/
/** @brief	Écriture sequentielle sur le EEPROM à partir d'un tampon.
**************************************************************************************************************/
void EEPROM_write(unsigned int adr, unsigned int len, char *buf)
{
	unsigned int i;
	
	/** Attends si une écriture est en cours sur le eeprom */
	while(EECR & (1<<EEPE));
	
	/** Envoie la chaine de caractères en mémoire eeprom */
	for(i=0;i<len;i++)
	{
		/** Place l'adresse et le data à mettre en EEPROM */
		EEAR = adr+i;
		EEDR = buf[i];
		
		cli();
		
		/** Démarre l'écriture */
		EECR |= (1<<EEMPE);
		EECR |= (1<<EEPE);
		
		/** Attends si une écriture est en cours sur le eeprom */
		while(EECR & (1<<EEPE));
		
		sei();
	}
}

/*************************************************************************************************************
 	Fonction :		EEPROM_read  
 	Description :	Lecture sequentielle du EEPROM dans un tampon.
 	Paramètres: 	Adresse interne du EEPROM de départ, longueur à lire, tampon
 	Valeur retour:	Aucune.
**************************************************************************************************************/
/** @brief	Lecture sequentielle du EEPROM dans un tampon.
**************************************************************************************************************/
void EEPROM_read(unsigned int adr, unsigned int len, char *buf)
{
	unsigned int i;
	
	/** Attends si une écriture est en cours sur le eeprom */
	while(EECR & (1<<EEPE));
	
	/** Lis la chaine de caractères en mémoire eeprom */
	for(i=0;i<len;i++)
	{
		/** Place l'adresse à lire en EEPROM */
		EEAR = adr+i;
		
		cli();

		/** Démarre la lecture */
		EECR |= (1<<EERE);
		
		/** Place le caractere dans la chaine */
		buf[i]=EEDR;

		sei();
	}
}
