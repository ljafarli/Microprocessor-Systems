Arduino Code:

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t digit = 0;
volatile uint8_t paused = 0;

const uint8_t seg[10] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

static inline void show_digit(uint8_t n) {
    uint8_t x = seg[n];
    PORTB = (PORTB & ~0x3F) | (x & 0x3F);      // a-f -> PB0..PB5
    PORTC = (PORTC & ~0x01) | ((x >> 6) & 1); // g   -> PC0
}

ISR(TIMER1_COMPA_vect) {
    if (!paused) {
        digit = (digit + 1) % 10;
        show_digit(digit);
    }
}

ISR(INT0_vect) {
    paused ^= 1;   // pause/resume toggle
}

int main(void) {
    DDRB |= 0x3F;          // PB0..PB5 output
    DDRC |= (1 << PC0);    // PC0 output
    DDRD &= ~(1 << PD2);   // PD2 input
    PORTD |= (1 << PD2);   // internal pull-up for button

    show_digit(digit);     // start from 0

    // Timer1 CTC, 0.5 s exactly
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS12); // CTC, prescaler 256
    OCR1A  = 31249;                      // 16e6 / 256 = 62500 Hz -> 0.5 s

    TIMSK1 = (1 << OCIE1A);              // enable Timer1 compare interrupt

    // External interrupt INT0 on falling edge
    EICRA = (1 << ISC01);
    EIMSK = (1 << INT0);

    sei();

    while (1) { }
}
