/*
 * keypad_lib.h
 *
 * Created: 17-Sep-16 7:31:45 AM
 *  Author: Serag
 */ 

#ifndef F_CPU

#define F_CPU 16000000UL

#endif

#include <avr/io.h>

#define clr_bit(reg,pos) reg&=~(1<<pos)		//used to clear bit x on register x to 0


void key_init(void);						//initialize keypad

void key_init(void)
{
	DDRC = 0x0F;			//assign rows bits PA0-PA3 as outputs and columns bits PA4-PA7 as inputs
}

unsigned char key_scan(void);				//scan pressed key and return it

unsigned char key_scan(void)
{
	unsigned char cnt, key;
	
	
	
	for ( cnt = 0 ; cnt < 4 ; ++cnt )
	{
		PORTC = 0XFF;				//first level check rows one by one
		
		if ( cnt == 0 )
			clr_bit(PORTC,0);		//first row check
			
		if ( cnt == 1 )
			clr_bit(PORTC,1);		//second row check
		
		if ( cnt == 2 )
			clr_bit(PORTC,2);		//third row check
		
		if ( cnt == 3 )
			clr_bit(PORTC,3);		//fourth row check
		
		key = PINC & 0xF0;			//check if any key is pressed
		
		if ( key != 0xF0 )			//if any key pressed go to the second level
		{
			while ( ( PINC & 0xF0 ) != 0xF0 );
			break;
		}
	}
	
	if ( cnt == 4 )					//end of first level scan loop
		return 0;
		
	switch ( cnt )					//scan columns
	{
		case 0:						//case 0 for first row
			
			switch ( key )			
				{
					case 0xE0:		//Check if row 1 col 1 --> ( 7 )
					return 0x07;
					break;
					
					case 0xD0:		//Check if row 1 col 2 --> ( 8 )
					return 0x08;
					break;
					
					case 0xB0:		//Check if row 1 col 3 --> ( 9 )
					return 0x09;
					break;
					
					case 0x70:		//Check if row 1 col 4 --> ( A )
					return 0x0A;
					break;
				}
		break;
		
		case 1:						//case 1 for second row
		
			switch ( key )
				{
					case 0xE0:		//Check if row 2 col 1 --> ( 4 )
					return 0x04;
					break;
			
					case 0xD0:		//Check if row 2 col 2 --> ( 5 )
					return 0x05;
					break;
			
					case 0xB0:		//Check if row 2 col 3 --> ( 6 )
					return 0x06;
					break;
			
					case 0x70:		//Check if row 2 col 4 --> ( B )
					return 0x0B;
					break;
				}
		break;
		
		case 2:						//case 2 for third row
		
			switch ( key )
				{
					case 0xE0:		//Check if row 3 col 1 --> ( 1 )
					return 0x01;
					break;
			
					case 0xD0:		//Check if row 3 col 2 --> ( 2 )
					return 0x02;
					break;
			
					case 0xB0:		//Check if row 3 col 3 --> ( 3 )
					return 0x03;
					break;
			
					case 0x70:		//Check if row 3 col 4 --> ( C )
					return 0x0C;
					break;
				}
		break;
		
		case 3:						//case 3 for fourth row
		
			switch ( key )
			{
					case 0xE0:		//Check if row 4 col 1 --> ( F )
					return 0x0F;
					break;
					
					case 0xD0:		//Check if row 4 col 2 --> ( 0 )
					return '0';
					break;
					
					case 0xB0:		//Check if row 4 col 3 --> ( E )
					return 0x0E;
					break;
					
					case 0x70:		//Check if row 4 col 4 --> ( D )
					return 0x0D;
					break;
			}
		break;
	}
	
	return 0;
}
