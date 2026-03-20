#include <avr/io.h>//gives access to AVR registers
#include <avr/interrupt.h>

volatile uint8_t countdown = 0;//how many milliseconds are left

ISR(INT0_vect) {
    PORTB |= (1 << 5); //LED ON NOW
    countdown = 10; //Start a 10 ms timer 
}

ISR(TIMER1_COMPA_vect) {
    if (countdown > 0) {
        countdown--;//Reduce it by 1 every 1ms         
        if (countdown == 0) {
            PORTB &= ~(1 << 5);        
        }
    }
}

int main(void) {

    DDRB |= (1 << 5); //PB5 is OUTPUT  
    PORTB &= ~(1 << 5);//PB5 starts OFF

    DDRD  &= ~(1 << 2);  //INPUT
    PORTD |=  (1 << 2);  //Turn ON internal pull-up resistor
   
   //External Interrupt Control Register A
    EICRA |=  (1 << ISC01); //Turn that bit ON. 
    EICRA &= ~(1 << ISC00);  //This clears bit ISC00 = 0
    //falling edge

    //Enables the INT0 interrupt.
    EIMSK |=  (1 << INT0);  //External Interrupt Mask Register


//Timer/Counter1 Control Register A
    TCCR1A = 0; //Clear everything     

    //CTC mode                    
    TCCR1B = (1 << WGM12);  //Waveform Generation Mode 

    //Output Compare Register       
    OCR1A  = 249;  //Timer counts    

    //Timer Interrupt Mask Register 
    //Output Compare Interrupt Enable A      
    TIMSK1 = (1 << OCIE1A);              

    TCCR1B |= (1 << CS11) | (1 << CS10);
    


    sei();//Without this, no interrupt will work.
    while (1) { }

    return 0;
}
