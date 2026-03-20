#include <Arduino.h>

uint8_t result;
uint8_t sreg_copy;


void setup() {
  Serial.begin(9600);
  while (!Serial);

  uint8_t a = 20;
  uint8_t b = 30;

  asm volatile (
    "ldi r16, %[valA]     \n\t"
    "ldi r17, %[valB]     \n\t"
    "sub r16, r17         \n\t"
 
    "in  r18, __SREG__    \n\t"
    "sts %[res], r16      \n\t"
    "sts %[sreg], r18     \n\t"
    

    : // End of assembly instructions section

    : [valA] "M" (a),
      [valB] "M" (b),
      [res]  "m" (result),
      [sreg] "m" (sreg_copy)
    : "r16", "r17", "r18"
  );

  Serial.println("=== CPU Register Subtraction ===");
  Serial.print("R16 (Result): ");
  Serial.println(result);

  Serial.print("SREG: 0b");
  Serial.println(sreg_copy, BIN);

  Serial.print("Zero Flag (Z): ");
  Serial.println((sreg_copy & (1 << 1)) ? "SET" : "CLEARED");

  Serial.print("Carry Flag (C): ");
  Serial.println((sreg_copy & (1 << 0)) ? "SET" : "CLEARED");
}

void loop() {
}
