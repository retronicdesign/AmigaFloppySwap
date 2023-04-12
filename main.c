/*************************************************************************************************************
*    Titre	      : Floppy Swap, selon un pattern de la pin reset du Amiga (double click)
*    Auteur       : Francis Gradel, ing.
*    Fichier      : main.c
*    SW Version   : 1.0
*	 HW Version   : 2.1
*	 Plateforme   : ATTINY13
*    Date début   : 2017.10.05
*    Mise à jours : 2017.10.14
**************************************************************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "eeprom.h"
#include "eepvars.h"
#include "fuses.h"

#define sleep(); asm("sleep");

#define	TIMER_FREQ	1000 			//Hz
#define TIMEOUT	(TIMER_FREQ * 2)	
#define DEBOUNCE_TIME	(TIMEOUT/10)

#define HIGH_FREQ	1600
#define LOW_FREQ	600

volatile unsigned char swap,swapt;
volatile unsigned int t[3];
volatile unsigned int cnt;
volatile unsigned char click;


/*************************************************************************************************************
 	Fonction :		main  
 	Description :	Fonction principale.
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
/** @brief	Fonction principale.
**************************************************************************************************************/
int main (void)
{
	//Définition des pins
	DDRB |= (1<<PB0);	//PB0 (5) sortie speaker (OC0A)
	DDRB &= ~(1<<PB1);	//PB1 (6) entrée reset (INT0) 
	DDRB |= (1<<PB4);	//PB4 (3) sortie swap/no-swap (PB4)

	//PORTB |= (1<<PB1);//pull-up pour test*******************

	//Lecture du eeprom
	EEPROM_read((int)&EEPVars.swap,1,(char*)&swapt);
	swap=swapt;

	//Ajustement de la pin swap selon ce qu'il y a en eeprom
	if(swap)
		PORTB &= ~(1<<PB4);
	else
		PORTB |= (1<<PB4);

	cnt=0;
	click=0;

	// Timer 0 pour mesurer les "clicks" et generer les sons.
	TCCR0A |= ((1<<WGM01)); // CTC 
	TCCR0B |= ((1<<CS01)); // XTAL/8

	OCR0A=((F_CPU)/8/TIMER_FREQ); // Ajustement de la fréquence du timer 0a (mesure du timeout)

	//TIMSK0 |= ((1<<OCIE0A)); // active les interruptions sur le timer 0A

	 // Active les interruptions sur la descente de INT0 (reset du CPU Amiga et CTRL-A-A)
	MCUCR |= (1<<ISC01);
	GIMSK |= (1<<INT0);

	/** Ajuste les paramètres de veille */
	MCUCR |= ((1<<SE)); // IDLE MODE activée sur SLEEP

	sei();

	for(;;)
	{
		/* Endors le microcontrôleur */
		sleep();

		//Si le swap en mémoire EEPROM n'est pas ajusté selon le récent changement, l'ajuster
		if(swapt!=swap)
		{
			cli();

			swapt=swap;
			//Écriture du eeprom
			EEPROM_write((int)&EEPVars.swap,1,(char*)&swapt);
		
			// Mode swap ou pas
			if(swap)
			{
				PORTB &= ~(1<<PB4);	// Sortie swap=0, SEL0 et SEL1 renversés via 74HC153;

				TCCR0A |= ((1<<COM0A0)); // buzz
				for(unsigned char i=((F_CPU)/8/LOW_FREQ);i>=((F_CPU)/8/HIGH_FREQ);i--)
				{
					OCR0A=(i);
					_delay_ms(2);
				}
				TCCR0A &= ~((1<<COM0A0)); // no buzz
				PORTB &= ~(1<<PB0);// Sortie 0v pour empêcher la polarisation du SPK
				OCR0A=((F_CPU)/8/TIMER_FREQ);

			}
			else 
			{
				PORTB |= (1<<PB4);	// Sortie swap=1, SEL0 et SEL1 non renversés via 74HC153;

				TCCR0A |= ((1<<COM0A0)); // buzz
				for(unsigned char i=((F_CPU)/8/HIGH_FREQ);i<=((F_CPU)/8/LOW_FREQ);i++)
				{
					OCR0A=(i);
					_delay_ms(2);
				}
				TCCR0A &= ~((1<<COM0A0)); // no buzz
				PORTB &= ~(1<<PB0);// Sortie 0v pour empêcher la polarisation du SPK
				OCR0A=((F_CPU)/8/TIMER_FREQ);
			}

			click=0;
			cnt=0;
			TIMSK0 &= ~((1<<OCIE0A));//éteint les interruption sur le timer
			TIFR0 |= ((1<<OCIE0A));//effacement du flag interruption du timer
			GIFR |= (1<<INT0);//effacement du flag interruption du reset


			sei();
		}
	}

	return(0);
}

/*************************************************************************************************************
 	Interruption:	SIG_OUTPUT_COMPARE0A
 	Description:	Cette interruption arrive à chaque fois que TCNT0A = OCR0A
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
SIGNAL(SIG_OUTPUT_COMPARE0A)
{
	cnt++;

	// Si nous sommes en état de timeout
	if(cnt>=TIMEOUT)
	{
		cnt=0;
		click=0;
		TIMSK0 &= ~((1<<OCIE0A));//éteint les interruption sur le timer
	}

}
/*************************************************************************************************************
 	Interruption:	INT0_vect
 	Description:	Cette interruption arrive à chaque fois que la pin PB1 passe de 5v à 0v
 	Paramètres: 	aucun
 	Valeur retour:	aucun
**************************************************************************************************************/
SIGNAL(INT0_vect)
{
	click++;

	//Premier click, activer le timeout
	if(click==1)
	{
		cnt=0;
		TIMSK0 |= ((1<<OCIE0A)); //Active le timer
	}
	//Deuxième click (et plus), on swap et fin du calcul de timeout
	else if(click>=2 && cnt>=DEBOUNCE_TIME)
	{
		swap=!swap;

		click=0;
		cnt=0;
	}
}

