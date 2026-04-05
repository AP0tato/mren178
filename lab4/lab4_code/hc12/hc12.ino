#include <SoftwareSerial.h>
SoftwareSerial hc12(10, 11); // RX, TX
#define SET_PIN 2 // HC-12 SET pin
void setup() {
pinMode(SET_PIN, OUTPUT);
digitalWrite(SET_PIN, LOW); // Enter AT mode
Serial.begin(9600);
hc12.begin(9600);
Serial.println("HC-12 AT Command Mode Ready");
}
void loop() {
  if (Serial.available()) {
    hc12.write(Serial.read());
  }
  if (hc12.available()) {
    Serial.write(hc12.read());
  }
}