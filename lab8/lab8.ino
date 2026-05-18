Arduino Master Code:

//master code
#include <Wire.h>

#define SLAVE_ADDR  8 //The slave Arduino will have address 8
#define BUTTON_PIN 2
#define LED_PIN 9

void setup() {

  Wire.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);  //default state is high, when pressed, conneects to gnd and becomes low
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  byte buttonState;  //variable called buttonstate, stores values 0-255, stores whether the button is pressed or not

  if (digitalRead(BUTTON_PIN) == LOW) { //button pressed
    buttonState = 1;
  }
  else {
    buttonState = 0;
  }

  // Send to slave
  Wire.beginTransmission(SLAVE_ADDR);  //starts communication with address 8
  Wire.write(buttonState);  //sends the value of buttonstate to slave
  Wire.endTransmission();

  // Request slave button state
  Wire.requestFrom(SLAVE_ADDR, 1);  //asks slave its button state

  if (Wire.available()) { //checks whether data has arrived from the slave

    byte received = Wire.read();   //read the received data

    if (received == 1) {  //if the button state 1, turn on
      digitalWrite(LED_PIN, HIGH);
    }
    else {
      digitalWrite(LED_PIN, LOW);
    }
  }

  delay(50);  //prevents communication from happenind too fast
}

Arduino Slave Code:

//slave code
#include <Wire.h>

#define SLAVE_ADDR 8  //The slave Arduino will have address 8
#define BUTTON_PIN 2
#define LED_PIN 9

volatile byte receivedData = 0;  //cvariable called receiveData, stores values 0-255, stores data received from master

void setup() {
  Wire.begin(SLAVE_ADDR);  //Starts I2C communication as a SLAVE

  Wire.onReceive(receiveEvent);  //Registers a function called receiveEvent, automatically runs whenever the master sends data
  Wire.onRequest(requestEvent);  //Registers a function called requestEvent, automatically runs whenever the master requests data from the slave

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  if (receivedData == 1) {  //master buton is pressed
    digitalWrite(LED_PIN, HIGH);
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
}

void receiveEvent(int howMany) {  //how many tells how many bytes were received, automatically runs when slave receives data from master

  while (Wire.available()) {  //checks whether unread data is arrived in the I2C
    receivedData = Wire.read();  //reads data and stores in receiveData
  }
}

void requestEvent() {  //automatically runs when the master requests data from the slave

  byte buttonState;  //variable to store slave button state.

  if (digitalRead(BUTTON_PIN) == LOW) {
    buttonState = 1;
  }
  else {
    buttonState = 0;
  }

  Wire.write(buttonState);
}
