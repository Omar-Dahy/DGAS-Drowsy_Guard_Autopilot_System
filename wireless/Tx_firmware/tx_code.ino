// TX code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define buttonPin1 3
#define buttonPin2 4
#define buttonPin3 5
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00002";

int lastSent1 = -1;
int lastSent2 = -1;
int lastSent3 = -1;

void setup() {
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);
  pinMode(buttonPin3, INPUT_PULLUP);
  
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  int value1 = digitalRead(buttonPin1);
  int value2 = digitalRead(buttonPin2);
  int value3 = digitalRead(buttonPin3);
  
  int dataToSend1 = (value1 == LOW) ? 5 : 0;
  int dataToSend2 = (value2 == LOW) ? 10 : 0;
  int dataToSend3 = (value3 == LOW) ? 15 : 0;

  if (dataToSend1 != lastSent1) {
    radio.write(&dataToSend1, sizeof(dataToSend1));
    Serial.print("Sent: ");
    Serial.println(dataToSend1);
    lastSent1 = dataToSend1;
  }

  if (dataToSend2 != lastSent2) {
    radio.write(&dataToSend2, sizeof(dataToSend2));
    Serial.print("Sent: ");
    Serial.println(dataToSend2);
    lastSent2 = dataToSend2;
  }

  if (dataToSend3 != lastSent3) {
    radio.write(&dataToSend3, sizeof(dataToSend3));
    Serial.print("Sent: ");
    Serial.println(dataToSend3);
    lastSent3 = dataToSend3;
  }

  delay(100);  // reduce transmission frequency
}
