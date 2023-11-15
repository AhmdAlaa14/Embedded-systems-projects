#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
typedef unsigned char uint8;
uint8 sec1=0 , sec2=0 , min1=0 , min2=0 , hour1=0 , hour2=0 ;
void reset(void);
// ISR for interrupts

ISR(TIMER1_COMPA_vect){
	sec1++;
	if(sec1==10){
		sec1=0;
		sec2++;
	}
	if(sec2==6){
		sec2=0;
		min1++;
	}
	if(min1==10){
		min1=0;
		min2++;
	}
	if(min2==6){
		min2=0;
		hour1++;
	}
	if(hour1==10){
		hour1=0;
		hour2++;
	}
	if(hour2==2&&hour1==4){
		reset();
	}
}

ISR(INT0_vect){
	reset();
	TCNT1 = 0;
}

ISR(INT1_vect){
	TCCR1B&=0xf8;
}

ISR(INT2_vect){
	TCCR1B|=(1<<CS12)|(1<<CS10);
}

// timer initiative function
void timer1init(void){
	TCCR1A=(1<<FOC1A);
	TCCR1B|=(1<<WGM12);
	TCCR1B|=(1<<CS12)|(1<<CS10);
	/* WGM12=1 to enable compare mode and cs12,cs10 to set prescaler to 1024*/
	TCNT1 = 0; // to make the timer start counting from 0;
	OCR1A=1000;
	TIMSK |= (1<<OCIE1A); // To enable interrupt for compare mode
}
// functions
void reset(void){
	sec1=0;
	sec2=0;
	min1=0;
	min2=0;
	hour1=0;
	hour2=0;
}

int main(void){

	//configuring i/o pins
	DDRD&=0xf3; // to make pin 3 and 4 in port d input pins
	DDRC|=0x0f; // to make first 4 pins in port c output pins
	DDRA|=0X3F; // to make first 6 pins of port a output pins
	DDRB&=~(1<<PB2); // to make pin 3 in make port b input pin

	PORTC=0; // to initialize output pin as zero
	PORTD|=(1<<PD2); // to activate internal pull up for int0
	PORTB|=(1<<PB2); // to activate internal pull up for int2

	SREG|=(1<<7); // to enable global interrupt
	GICR|=(1<<INT1)|(1<<INT0)|(1<<INT2); // TO ENABLE INTERRUPTS 0 , 1 AND 2
	MCUCR|=(1<<ISC01)|(1<<ISC11)|(1<<ISC10);
	MCUCR&=~(1<<ISC00);
	// ISC00 0 and ISC01 1 to trigger int0 with falling edge
	// ISC11 1 and ISC10 1 to trigger int1 with rising edge
	MCUCSR&=~(1<<ISC2); // ISC2 0 to trigger int2 with falling edge

	timer1init(); // to initialize the timer

	/* program */
	while(1){


		// displaying the 6 7-segment in order
		// displaying the first seconds digit
		PORTA|=(1<<PD0);
		PORTC=sec1;
		_delay_ms(2);
		PORTA=0;
		// displaying the second seconds digit
		PORTA|=(1<<PD1);
		PORTC=sec2;
		_delay_ms(2);
		PORTA=0;
		// displaying the first minutes digit
		PORTA|=(1<<PD2);
		PORTC=min1;
		_delay_ms(2);
		PORTA=0;
		// displaying the second minutes digit
		PORTA|=(1<<PD3);
		PORTC=min2;
		_delay_ms(2);
		PORTA=0;
		// displaying the first hours digit
		PORTA|=(1<<PD4);
		PORTC=hour1;
		_delay_ms(2);
		PORTA=0;
		// displaying the second hours digit
		PORTA|=(1<<PD5);
		PORTC=hour2;
		_delay_ms(2);
		PORTA=0;

	}
}
