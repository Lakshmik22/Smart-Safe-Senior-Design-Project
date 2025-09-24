#include <Adafruit_Fingerprint.h>   //https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
#include <HardwareSerial.h>
#include <Keypad.h>

#define ROWS  4 // four rows
#define COLS  4 // three columns

#define PIN_RED    2 // GPIO2
#define PIN_GREEN  4 // GPIO4
#define PIN_BLUE   5 // GPIO5

char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

uint8_t colPins[COLS] = {13, 12, 14, 27}; // row pins
uint8_t rowPins[ROWS] = {26, 25, 33, 32}; // column pins

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

const String password = "1234"; // change your password here
String input_password;

const int buzzerPin = 15; // the buzzer pin

int attempts=0;

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

void setup() {
  Serial.begin(9600);
  Serial2.begin(57600);

  input_password.reserve(32); // maximum input characters is 33, change if needed
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  pinMode(buzzerPin, OUTPUT); // Set as output
  finger.begin(57600);
 
 if (finger.verifyPassword()) 
 {
   Serial.println("Fingerprint Sensor Connected");
   delay(3000); 
  // display.clearDisplay();
 }
 
 else  
 {
   Serial.println("Unable to find Sensor");
   delay(3000);
   Serial.println("Check Connections");
 }
}


void loop() {
  //KEYPAD STUFF STARTS HERE
  char key = keypad.getKey();
  if (key) {
    Serial.println(key);
    if (key == '*') {
      input_password = ""; // clear input password
    } 
    else if (key == '#') {
      if (password == input_password) {
        Serial.println("The password is correct, ACCESS GRANTED!");
        analogWrite(PIN_RED,   0);
       analogWrite(PIN_GREEN, 250);
       analogWrite(PIN_BLUE,  0);
       delay(1000); // keep the color 1 second
       analogWrite(PIN_RED,   0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
      }
      else if(password!= input_password) {
        Serial.println("The password is incorrect, ACCESS DENIED!");
        analogWrite(PIN_RED,   250);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        delay(1000); // keep the color 1 second
        analogWrite(PIN_RED,   0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        attempts++;
      }

      if(attempts==3){
        Serial.println("LOCKDOWN MODE ACTIVATED!");
        digitalWrite(buzzerPin, HIGH); // Set to HIGH to make the buzzer sound
        delay(1000); // Wait for 3 milliseconds

        analogWrite(PIN_RED,   250);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        delay(500); // keep the color 1 second
        analogWrite(PIN_RED,   0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        delay(500); // keep the color 1 second
        analogWrite(PIN_RED,   250);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        delay(500); // keep the color 1 second
        analogWrite(PIN_RED,   0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        delay(500); // keep the color 1 second
        analogWrite(PIN_RED,   250);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);
        delay(500); // keep the color 1 second
        analogWrite(PIN_RED,   0);
        analogWrite(PIN_GREEN, 0);
        analogWrite(PIN_BLUE,  0);

        digitalWrite(buzzerPin, LOW); // LOW to turn off the buzzer

        attempts=0;
      }

      input_password = ""; // clear input password
    } else {
      input_password += key; // append new character to input password string
    }
  }
  //KEYPAD STUFF ENDS HERE

  //FINGERPRINT SCANNER STARTS HERE
  static bool printonce=false;
  unsigned int p = finger.getImage();
   if (p == FINGERPRINT_NOFINGER && !printonce ) {
    Serial.println("Waiting For Valid Finger");
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  0);
    printonce= true;
   }
   p = finger.image2Tz();
   if (p != FINGERPRINT_OK && !printonce){
    Serial.println("Messy Image Try Again");
    printonce= true;
   }
   p = finger.fingerFastSearch();
   if (p == FINGERPRINT_OK) {
    Serial.println("Access granted");
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 250);
    analogWrite(PIN_BLUE,  0);
    delay(1000); // keep the color 1 second
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  0);
   }
   else if (p == FINGERPRINT_NOMATCH)  {
   Serial.println("Not Valid Finger");
   //delay(3000);
   analogWrite(PIN_RED,   250);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  0);
    delay(1000); // keep the color 1 second
    analogWrite(PIN_RED,   0);
    analogWrite(PIN_GREEN, 0);
    analogWrite(PIN_BLUE,  0);
 }
 //FINGERPRINT SCANNER STUFF ENDS HERE
}
