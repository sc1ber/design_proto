#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); 

const byte address[6]="00001";

int led1[3];
byte len = 3;

int button_pin = 2;
int led_1 = 8;
int led_2 = 7;
int led_3 = 6;
boolean button_state=0;

void setup() {
    Serial.begin(9600);
    Serial.println("Transmitter Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.enableAckPayload();
    radio.setRetries(3,5); // delay, count
    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
    pinMode(led_3, OUTPUT);
}

void loop() {
  button_state = digitalRead(button_pin);
  radio.openWritingPipe(address);
  if(button_state == HIGH)  {
    digitalWrite(led_1, HIGH);
    digitalWrite(led_2, HIGH);
    digitalWrite(led_3, HIGH);
    const char text[] = "Your Button State is HIGH";
    radio.write(&text, sizeof(text));
  }
  else  {
    digitalWrite(led_1, LOW);
    digitalWrite(led_2, LOW);
    digitalWrite(led_3, LOW);
    const char text[] = "Your Button State is LOW";
    radio.write(&text, sizeof(text));
  } 
  if ( radio.isAckPayloadAvailable() ) {
      radio.read(led1,len);
      Serial.print("Acknowledge received: ");
      Serial.println(led1);
      if(led1 == 101){
        digitalWrite(led_1,HIGH);
      }
  }
  radio.write(&button_state, sizeof(button_state));
}
