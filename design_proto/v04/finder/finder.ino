#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);

//for battery variables
const byte address[6]="00001";
char batt[3] = "000";

//for testing RF
boolean button_state = 0;
int led_pin = 3; //set led pin
int buzzer_pin = 4; //set buzzer pin

//for testing potentiometer
int pot_led = 2; //set potentiometer led pin
int pot_pin = A0; //set potentiometer analog pin
int pot_val = 0; //set potentiometer value

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("Receiver Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(1,address);
    radio.enableAckPayload();
    radio.writeAckPayload(1, &batt, sizeof(batt));
    radio.startListening();
    pinMode(pot_led, OUTPUT);
    pinMode(led_pin, OUTPUT);
}

void loop() {
    if ( radio.available() ) {
      char text[32] = "";                 //Saving the incoming data
      radio.read(&text, sizeof(text));    //Reading the data
      radio.read(&button_state, sizeof(button_state));    //Reading the data
      if(button_state == HIGH){
        tone(buzzer_pin, 1000);
        digitalWrite(led_pin, HIGH);
        //Serial.println(text);
      }
      else  {
        noTone(buzzer_pin);
        digitalWrite(led_pin, LOW);
        //Serial.println(text);
      }
      pot_val = analogRead(pot_pin); //read potentiometer value
      analogWrite(pot_led, pot_val);
      //Serial.println(pot_val); //print potentiometer value
      battery();
      radio.writeAckPayload(1, &batt, sizeof(batt)); //send message 
    }
}

void battery() {
  //compute battery percentage
  float maxbatt = 1023;
  float batt_80 = maxbatt*0.8;
  float batt_60 = maxbatt*0.6;
  float batt_40 = maxbatt*0.4;
  float batt_20 = maxbatt*0.2;
  float batt_5 = maxbatt*0.05;
  
  //initialize leds for battery
  //char led[6] = "000000";
  int led[6] = {0,0,0,0,0,0};
  char combination[6] = "";
  char level[3] = "";
  
  //checks battery level and turns on led based on level
  if (pot_val >= batt_5){
    led[0] = 1;
    if (pot_val >= batt_20){
      led[1] = 1;
      if (pot_val >= batt_40){
        led[2] = 1;
        if (pot_val >= batt_60){
          led[3] = 1;
          if (pot_val >= batt_80){
            led[4] = 1;
            if (pot_val >= maxbatt){
              led[5] = 1; 
            }
          }
        }
      }
    }
  }

  //converts int array led to string
  sprintf(combination, "%u%u%u%u%u%u", led[0],led[1],led[2],led[3],led[4],led[5]);
  //Serial.print("6-input Combination: ");
  //Serial.println(combination);

  //basically reversing input of decoder (from truth table)
  if (strcmp(combination, "000000") == 0){level[0] = 0; level[1] = 0, level[2] = 0;}
  if (strcmp(combination, "100000") == 0){level[0] = 0; level[1] = 0, level[2] = 1;}
  if (strcmp(combination, "110000") == 0){level[0] = 0; level[1] = 1, level[2] = 0;}
  if (strcmp(combination, "111000") == 0){level[0] = 0; level[1] = 1, level[2] = 1;}
  if (strcmp(combination, "111100") == 0){level[0] = 1; level[1] = 0, level[2] = 0;}
  if (strcmp(combination, "111110") == 0){level[0] = 1; level[1] = 0, level[2] = 1;}
  if (strcmp(combination, "111111") == 0){level[0] = 1; level[1] = 1, level[2] = 0;}


  //converts 3-input int array to string for rf
  sprintf(batt, "%u%u%u", level[0],level[1],level[2]);
  Serial.print("3-input Combination: ");
  Serial.println(batt);  
}
