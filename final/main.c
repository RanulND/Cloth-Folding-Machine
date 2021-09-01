#ifndef F_CPU
#define F_CPU 16000000UL // 16 MHz clock speed
#endif

#define D4 eS_PORTA4
#define D5 eS_PORTA5
#define D6 eS_PORTA6
#define D7 eS_PORTA7
#define RS eS_PORTD6
#define EN eS_PORTD7

#define valve_1 PB3
#define valve_2 PB5

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include  "lcd.h"
#include  "ultrasonic.h"
#include "keypad.h"

uint8_t sensor_working=0;
uint8_t rising_edge=0;
uint32_t timer_counter=0;
uint32_t distance;
uint8_t distance_str[10];

void lcddisplay(void);


int main(void)
{
	//DDRC = 0b11111111; //making PC0,PC1 and PC2 as output pins.
	DDRA =0xFF;// set LCD Data port as output
	
	DDRD =0xFF;
	
	DDRB = 0xFF;  // Making Port B as output
	PORTB = PORTB | (1<<valve_1); //Turn ON the valve 1
	PORTB = PORTB | (1<<valve_2); //Turn ON the valve 2
	
	int j,x,y;
	int reset1 = 0, reset2 = 0, reset3 = 0;
	unsigned char t[4];
	
	key_init();
	Lcd4_Init();
	ultrasonic_init();
	sei();
	
	
	ultra_triger();
	_delay_ms(15);
	
	
	
	while(1)
	{
		for(x=0;x<3;x++){
			for(j=0;j<20;j++){
				t[x]=key_scan();
				
				if(t[x]>0x00){
					break;
				}
				_delay_ms(10);
			}
			if(t[x]==0 || t[x]>3){
				break;
			}
		}
		
		for(y=0;y<3;y++){
			
			if(t[y]==0x01){
				_delay_ms(300);
				Lcd4_Set_Cursor(1,3);
				Lcd4_Write_String("Activating");
				Lcd4_Set_Cursor(2,1);
				Lcd4_Write_String("Folding Process");
				_delay_ms(500);
				Lcd4_Clear();
				Lcd4_Write_String("Input  Cloth");
				Lcd4_Set_Cursor(2,0);
				Lcd4_Write_String("Type: ");
				
				reset1 = 1;
				reset2 = 0;
				reset3 = 0;
				
				
			}
			
			else if(t[y]==0x02 && reset1==1 && reset3==0){
				Lcd4_Set_Cursor(2,7);
				Lcd4_Write_String("T-Shirt");
				_delay_ms(200);
				reset2 = 1;
				reset1 = 0;
				
				PORTD = PORTD | (1<<5);
				for(int i=0;i<1;i++){
					PORTD = PORTD | (1<<3);
					_delay_ms(5000);
					PORTD = PORTD & (~(1<<3));
					_delay_ms(100);
					PORTD = PORTD | (1<<4);
					_delay_ms(5000);
					PORTD = PORTD & (~(1<<4));
					
				}
				PORTD = PORTD & (~(1<<5));
				_delay_ms(3000);
				
				//starts motor
				PORTB = PORTB | (1<<0);
				PORTB = PORTB & (~(1<<1));
				
				
				/*//Rotate Motor1 to 0 degree
				PORTC = PORTC | (1<<0);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<0));
				_delay_ms(2000);

				//Rotate Motor1 to 180 degree
				PORTC = PORTC | (1<<0);
				_delay_us(2000);
				PORTC = PORTC &(~(1<<0));
				_delay_ms(2000);

				//Rotate Motor1 to 0 degree
				PORTC = PORTC | (1<<0);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<0));
				_delay_ms(2000);


				
				//Rotate Motor2 to 0 degree
				PORTC = PORTC | (1<<1);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<1));
				_delay_ms(2000);

				//Rotate Motor2 to 180 degree
				PORTC = PORTC | (1<<1);
				_delay_us(2000);
				PORTC = PORTC &(~(1<<1));
				_delay_ms(2000);

				//Rotate Motor2 to 0 degree
				PORTC = PORTC | (1<<1);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<1));
				_delay_ms(2000);

				
				//Rotate Motor3 to 0 degree
				PORTC = PORTC | (1<<2);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<2));
				_delay_ms(2000);

				//Rotate Motor3 to 180 degree
				PORTC = PORTC | (1<<2);
				_delay_us(2000);
				PORTC = PORTC &(~(1<<2));
				_delay_ms(2000);

				//Rotate Motor3 to 0 degree
				PORTC = PORTC | (1<<2);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<2));
				_delay_ms(2000);
				*/
				//handling pneumatic cylinders
				PORTB = PORTB & (~(1<<valve_1)); //Turn OFF the Relay 1
				PORTB = PORTB & (~(1<<valve_2)); //Turn OFF the Relay 2
				_delay_ms(2000);
				PORTB = PORTB | (1<<valve_1); //Turn ON the Relay 1
				PORTB = PORTB | (1<<valve_2); //Turn ON the Relay 2
				
				_delay_ms(9000);
				
				//Stops Motor
				PORTB = PORTB & (~(1<<0));
				PORTB = PORTB & (~(1<<1));
				
			}
			
			else if(t[y]==0x03 && reset1==1 && reset2==0){
				Lcd4_Set_Cursor(2,7);
				Lcd4_Write_String("Trouser");
				_delay_ms(200);
				reset3 = 1;
				reset1 = 0;
				
				PORTD = PORTD | (1<<5);        // transmit status of pin PD1 on to pin PD3 to drive MOTOR.
				for(int i=0;i<2;i++){
					PORTD = PORTD | (1<<3);
					_delay_ms(5000);
					PORTD = PORTD & (~(1<<3));
					_delay_ms(100);
					PORTD = PORTD | (1<<4);
					_delay_ms(5000);
					PORTD = PORTD & (~(1<<4));
					_delay_ms(100);
				}
				
				PORTD = PORTD & (~(1<<5));
				_delay_ms(3000);
				
				//starts motor
				PORTB = PORTB | (1<<0);
				PORTB = PORTB & (~(1<<1));
				/*
				//Rotate Motor1 to 0 degree
				PORTC = PORTC | (1<<0);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<0));
				_delay_ms(2000);

				//Rotate Motor1 to 180 degree
				PORTC = PORTC | (1<<0);
				_delay_us(2000);
				PORTC = PORTC &(~(1<<0));
				_delay_ms(2000);

				//Rotate Motor1 to 0 degree
				PORTC = PORTC | (1<<0);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<0));
				_delay_ms(2000);
				
				//Rotate Motor3 to 0 degree
				PORTC = PORTC | (1<<2);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<2));
				_delay_ms(2000);

				//Rotate Motor3 to 180 degree
				PORTC = PORTC | (1<<2);
				_delay_us(2000);
				PORTC = PORTC &(~(1<<2));
				_delay_ms(2000);

				//Rotate Motor3 to 0 degree
				PORTC = PORTC | (1<<2);
				_delay_us(1000);
				PORTC = PORTC &(~(1<<2));
				_delay_ms(2000);
				*/
				//handling pneumatic cylinders
				PORTB = PORTB & (~(1<<valve_1)); //Turn OFF the Relay 1
				PORTB = PORTB & (~(1<<valve_2)); //Turn OFF the Relay 2
				_delay_ms(2000);
				PORTB = PORTB | (1<<valve_1); //Turn ON the Relay 1
				PORTB = PORTB | (1<<valve_2); //Turn ON the Relay 2
				
				_delay_ms(9000);
				
				//Stops Motor
				PORTB = PORTB & (~(1<<0));
				PORTB = PORTB & (~(1<<1));
				
			}
			
			
			
			
		}
		
		
	}
}

void ultrasonic_init(void){

	TRIGER_DDR|=(1<<TRIGER);
	ECHO_DDR&=~(1<<ECHO);
	ECHO_PULLUP|=(1<<ECHO);
	enable_ex_interrupt();
	timer0_init();
	return;
}

void enable_ex_interrupt(void){

	MCUCR |= (1<<ISC10);
	GICR  |= (1<<INT1);

	return;
}

void ultra_triger(void){
	if(!sensor_working){
		TRIGER_PORT|=(1<<TRIGER);
		_delay_us(15);
		TRIGER_PORT&=~(1<<TRIGER);
		sensor_working=1;
	}
	return;
}

ISR(INT1_vect){
	if(sensor_working==1){
		if(rising_edge==0){
			TCNT0=0x00;
			rising_edge=1;
			timer_counter=0;
		}
		else{
			distance=(timer_counter*256+TCNT0)/466;
			Lcd4_Set_Cursor(1,0);
			Lcd4_Clear();
			if(distance>30)
			{
				Lcd4_Set_Cursor(1,1);
				Lcd4_Write_String(" Water Level is");
				Lcd4_Set_Cursor(2,6);
				Lcd4_Write_String("Low");
				_delay_ms(500);
				Lcd4_Clear();
				
			}
			else
			{
				Lcd4_Set_Cursor(1,1);
				Lcd4_Write_String("Water Level is");
				Lcd4_Set_Cursor(2,6);
				Lcd4_Write_String("Good");
				_delay_ms(500);
				Lcd4_Clear();
				
			}
			timer_counter=0;
			rising_edge=0;
		}
	}}

	ISR(TIMER0_OVF_vect){
		timer_counter++;
		if(timer_counter >730){
			TCNT0=0x00;
			sensor_working=0;
			rising_edge=0;
			timer_counter=0;
		}
		return;
	}
	

