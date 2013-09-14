/*
 Counting the pulses from a rotary dial switch. Using a capacitor to ensure 
 accuracy in readings also - 0.1uf - across the pulsing pins. 
 This method was not effective for the latch pins as these idle LOW and are 
 evidently very prone to noise. It was easier to code the sketch to ignore 
 any noise on the latch circuit where 0 pulses had been received. 
 
 Using the LCD Shield for ease of display output, this would not be otherwise 
 required. Using PWM to control the LCD brightness (Pin 10), Analogue Input A0
 to read the buttons on the shield (using voltage division) and pins 4 - 9 for
 the respective inputs. This is initialised as 8, 9, 4, 5, 6, 7 for the LCD 
 that I have; while others may work with the setup contained in the standard
 Arduino LCD Library.
 */

//Includes
#include "LiquidCrystal.h"     //Include the library for the LCD

//Constants
const int brtPin = 10;         //brightness pin for DHT
const int Sw1 = A4;            //Latch switch (normally LOW)
const int Sw2 = A5;            //Pulse switch (normally HIGH)
const int but = A0;            //buttons on the LCD Shield

//Variables
int brtLvl = 50;               //level of brightness as percentage (i.e. 0 to 100)
int rotaryLat = 0;             //reading state of the latch switch to indicate pulses will be received (idle LOW)
int rotaryPul = 0;             //reading state of the pulse switch to count the number of pulses received (idle HIGH, pulse LOW)  
int lastrotaryLat = 0;         //storage for last latch state
int lastrotaryPul = 0;         //storage for last pulse state
int pulse = 0;                 //the number of pulses received
int cur = 0;

//Initialisations
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup() {
  lcd.begin(16, 2);
  lcd.print("Initialising...");

  //setup the LCD Brightness
  pinMode (brtPin, OUTPUT);
  adjBrt (brtLvl);

  //Setup other pins
  pinMode (Sw1, INPUT);
  pinMode (Sw2, INPUT);
  pinMode (but, INPUT);

  delay(1500);
  lcd.clear();
  lcd.setCursor (0, 0);
  lcd.print ("Number to Dial:");
}


void loop() {
  //get the state of the switches
  rotaryLat = digitalRead(Sw1);
  rotaryPul = digitalRead(Sw2);

  //only counting pulses received if the latch is HIGH
  if (rotaryLat == HIGH) {
    //compare the pulse switch state
    if (rotaryPul != lastrotaryPul) {
      //as the pulse switch is idle HIGH, we want to count the
      //number of LOW pulses
      if (rotaryPul == LOW) {
        //do stuff to count the pulses, or give user feedback
        //i.e. pulse an LED or something arbitrary if desired
        pulse++;
        lcd.setCursor (14, 1);
        lcd.print ("  ");
      }
      else {
        //use for error tracking on switch return to HIGH
        //otherwise, this can be removed.
        //in this example, it counts up the pulses as they are received.
        //this can be observed by manually slowing the dial as it spins
        lcd.setCursor (14, 1);
        lcd.print (pulse);
      }
    }
  } 
  //checking for change in the state of the Latch
  else if (rotaryLat != lastrotaryLat) {
    //checking if the latch is off
    if (rotaryLat == LOW) {
      //checking to see that some pulses were received
      if (pulse > 0) {
        //receive 10 digits (max/standard length of most Australian telephone numbers
        //then reset the cursor location to position 0, row 1 and clear the bottom line
        //of the display - except for the last two spaces as this would interfere with the
        //display above
        if (cur >= 10) {
          cur = 0;
          lcd.setCursor (cur, 1);
          lcd.print("              ");
        }
        //do stuff with this information
        //such as print the numbers! 
        lcd.setCursor (cur, 1);
        if (pulse >= 10) pulse = 0;
        lcd.print (pulse);
        lcd.print ("    ");
        //reset the pulse counter after each dial is complete
        pulse = 0;
        cur++;
      }
    }
  }
  //saving the states for next loop
  lastrotaryLat = rotaryLat;
  lastrotaryPul = rotaryPul;
}

void adjBrt (int b) {
  int mb = map (b, 0, 100, 0, 255);
  analogWrite (brtPin, mb);
}
