/*
 * laser_cat_ams7.cpp
 *
 * Created: 3/7/2020 5:49:34 PM
 * Author : Equinox
 */ 
#define ARDUINO_ARCH_AVR
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>
#include "Servo-master/src/Servo.h"

int main(void)
{
	Servo new_servo =Servo();
	new_servo.attach(9);
	
	//Set the output
	DDRB |= (1<<5) ;// PB5 is output
	PORTB|= (1<<5); //Turn on LED	
	
		
	//Timer1 
	TCCR1B = 0x0A;
	TCNT1 =0;
	OCR1A = 
	
    /* Replace with your application code */
    while (1) 
    {

		
		
		
    }
}