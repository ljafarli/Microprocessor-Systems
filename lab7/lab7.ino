Master Arduino Code:

#include <Arduino.h>

void spiMasterInit() {
  DDRB |= (1 << DDB2) | (1 << DDB3) | (1 << DDB5);
  DDRB &= ~(1 << DDB4);
  PORTB |= (1 << PORTB2);

  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

void spiSend(uint8_t data) {
  PORTB &= ~(1 << PORTB2);
  delayMicroseconds(20);

  SPDR = data;
  while (!(SPSR & (1 << SPIF)));

  delayMicroseconds(20);
  PORTB |= (1 << PORTB2);
}

void setup() {
  spiMasterInit();
}

void loop() {
  uint8_t values[] = {85, 170, 255};

  for (uint8_t i = 0; i < 3; i++) {
    spiSend(values[i]);
    delay(1000);
  }
}

Slave Arduino Code:

#include <Arduino.h>

void spiSlaveInit() {
  DDRB &= ~((1 << DDB2) | (1 << DDB3) | (1 << DDB5)); // SS, MOSI, SCK input
  DDRB |= (1 << DDB4);                                // MISO output
  SPCR = (1 << SPE);                                  // SPI enable, Slave
}

uint8_t spiReceive() {
  while (!(SPSR & (1 << SPIF))); // byte gələnə qədər gözlə
  return SPDR;
}

void setup() {
  Serial.begin(9600);
  spiSlaveInit();
}

void loop() {
  uint8_t received = spiReceive();
  Serial.println(received);
}
