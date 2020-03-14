/*
 * laser_cat_ams7.cpp
 *
 * Created: 3/7/2020 5:49:34 PM
 * Author : Equinox
 */ 
#define ARDUINO_ARCH_AVR
#define F_CPU 16000000UL


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <stdbool.h>
#include <math.h>
#include <util/delay.h>
//#include "Servo-master/src/Servo.h"


unsigned long counter = 0;

//Start off with 0 seconds left on wait
volatile unsigned long activate = 0;
volatile bool active = false;
volatile bool move_servo = false;

bool dir = false;


uint8_t position = 95;


ISR(TIMER0_COMPA_vect){
	counter ++; 
 	if(counter >= 10000){
		if(active){// ---- Active Operation
			PORTB^=(1<<5); // LED
			move_servo = true;
		
			//Manage Runtime. Activate will decrement until 0.
			if(activate > 0) {
				activate --;
			} else { 
				activate = 7200; //2 hour between operation
				active = false;	
				PORTB ^= (1<<4);
			}

		} else {// ---- Inactive Operation
			 if(activate > 0) { 
				activate --;
			 } else {
				activate = 60;//1 minute operation
				active = true;
				PORTB ^= (1<<4);
			 }
		}

		counter= 0;
	}
}
void init_servo() {
	//Set the output
	DDRB |= (1<<2); // set OC1B as output
	TCCR1A |= (1<< COM1B1); //Output compare on OC1B
	TCCR1A |= (1<< WGM10) | (1<< WGM11);
	TCCR1B |= (1<< WGM12) | (1<< WGM13);; // use fast PWM mode
	TCCR1B |= (1<<CS12); // turn the timer on, use prescaler 256
	
	// set the frequency of pulses to 50Hz; (1/50) = 20ms; 
	//(1/(16,000,000/256))*1000 = 0.016ms per pulse. 
	//OCR1A = (20 ms/.016 ms/clck_pulse) = 1250 clck_pulse; 
	OCR1A = 1250;
	
	// Center the Servo; at 1.5ms ; 94 * .016 =1.504
	OCR1B = position; 
	
	//LS-006 Servo : 
	//MAX Travel = 165 degrees
	//.8ms min = 50 (4% Duty Cycle) 
	// 2ms Max = 125 (10% Duty Cycle); 
	// (2  - .8) / 165 = .45 degrees per DC unit
}
void init_timer(){ 
	TCNT0 = 0; //Set Counter to 0;
	DDRB |= (1<<5); 
	TCCR0A |= (1<<WGM01); //CTC Mode
	TCCR0B |= (1<<CS01);// | 8 prescaler
	TIMSK0|= (1<<OCIE0A); // Interrupt Service Routine COMPA
	
	//Each Overflow is work .1ms : ((1/(16,000,000clk/8 prescale)) * 1000 us to ms) * 200 Overflows =.1ms
	OCR0A = 199;
}
int main(void)
{

	sei();
	init_servo();
	init_timer(); 
	
	//Laser pin
	DDRB |= (1<<4); 
	
	uint8_t nextpos;
	while (true) 
    {	
		if(move_servo){
			if(dir){
				nextpos = rand()%20 + 75;
				dir = !dir;
			} else {
				nextpos = rand()%20 + 95;
				dir = !dir;
			} 
			while(position != nextpos){
				int delta= position - nextpos; 
				if(delta > 0) {
					position = position -1; 
				} else {
					position = position + 1;
				}
				_delay_ms(10);
				OCR1B = position;
			}
			move_servo = false;
		}
	}
}