#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); 

const byte address[6]="00001";

char batt[3] = "000";
int pot_val = 0;

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
    radio.openWritingPipe(address);
    pinMode(led_1, OUTPUT);
    pinMode(led_2, OUTPUT);
    pinMode(led_3, OUTPUT);
}

void loop() {
  button_state = digitalRead(button_pin);
  radio.write(&button_state, sizeof(button_state));
  if ( radio.isAckPayloadAvailable() ) {
      radio.read(&pot_val,sizeof(pot_val));
      Serial.print(" Battery: ");
      //for (int i = 0; i < sizeof(batt); i++) Serial.print(batt[i],HEX);
      Serial.print(pot_val);
      battery();
      ledbatt();
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
  Serial.print(" 6-input: ");
  Serial.println(combination);

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
  Serial.print("3-input: ");
  Serial.print(batt);  
}

void ledbatt(){
  //light up leds according to battery level
  if(strcmp(batt, "000") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,LOW);
  }
  if(strcmp(batt, "001") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,HIGH);
  }
  if(strcmp(batt, "010") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,HIGH);
    digitalWrite(led_3,LOW);
  }
  if(strcmp(batt, "011") == 0){
    digitalWrite(led_1,LOW);
    digitalWrite(led_2,HIGH);
    digitalWrite(led_3,HIGH);
  }
  if(strcmp(batt, "100") == 0){
    digitalWrite(led_1,HIGH);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,LOW);
  }
  if(strcmp(batt, "101") == 0){
    digitalWrite(led_1,HIGH);
    digitalWrite(led_2,LOW);
    digitalWrite(led_3,HIGH);
  }
  if(strcmp(batt, "110") == 0){
    digitalWrite(led_1,HIGH);
    digitalWrite(led_2,HIGH);
    digitalWrite(led_3,LOW);
  }
}
