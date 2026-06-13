// RX code
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led1 A0
#define led2 A1

int buttonState = 0;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00002";

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  radio.startListening();
  while (!radio.available());
  radio.read(&buttonState, sizeof(buttonState));
  Serial.println(buttonState);

  if (buttonState == 1) {
    digitalWrite(led1, LOW);
  }
  else  if (buttonState == 0) {
    digitalWrite(led1, HIGH);
  }
  else if (buttonState == 3) {
    digitalWrite(led2, LOW);
  }
  else  if (buttonState == 2) {
    digitalWrite(led2, HIGH);
  }
}
