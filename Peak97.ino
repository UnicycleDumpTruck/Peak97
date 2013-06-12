/*
This sketch is for the Peak 97 Arduino in charge of the Polulu Power
Switch, the Adafruit LCD, the piezo element, the game timing, and 
the snow cap climber detection.
*/

// include the library code:
#include <LiquidCrystal.h>
#include "pitches.h"
//#include <EEPROM.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// notes in the melody:
int melody[] = {
  NOTE_C5, NOTE_C6, NOTE_A5, NOTE_G5, NOTE_F5, NOTE_G5, NOTE_F5, NOTE_E5, NOTE_D5};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 4, 4, 8,4,8,4,4,4 };
int thisNote = 0;
  
// Stopwatch Variables
long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch
int fractional;                     // variable used to store fractional part of time
int musicOn = 0;

void setup() {
  // Initialize the EEPROM with bogus high score
  //eeprom_write_dword(0, (float)millis());
  pinMode(5, OUTPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  pinMode(3, OUTPUT); // Polulu Power Switch
  // Snow Cap pins
  pinMode(2, INPUT);       // not really necessary, pins default to INPUT anyway Lloyd
  pinMode(4, INPUT);       // not really necessary, pins default to INPUT anyway Mary
  digitalWrite(2, HIGH);   // turn on pullup resistors. Wire climber so that contact shorts snow cap to ground.
  digitalWrite(4, HIGH);   // turn on pullup resistors. Wire climber so that contact shorts snow cap to ground.
  // Print a message to the LCD.
  lcd.print("  -PEAK 97-");
  lcd.setCursor(0,1);
  lcd.print("Jeff Highsmith");
  
  /*for (int thisNote = 0; thisNote < 8; thisNote++) {
    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(5, melody[thisNote],noteDuration);
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(5);
  }*/

  
  //delay(1000);
  //lcd.clear();
  //lcd.print("   Please be");
  //lcd.setCursor(0,1);
  //lcd.print("    gentle.");
  delay(1000);
  // Display Countdown
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("       3");
  tone(5, 262, 200);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("       2");
  tone(5, 262, 200);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("       1");
  tone(5, 262, 200);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("   Climb on!");
  // Start stopwatch
  startTime = millis();
  // Enable climbers
  digitalWrite(6, HIGH);
  //tone(5, 247, 600);
  musicOn = 1;
} // end setup

void loop() {
  elapsedTime =   millis() - startTime;
  if (elapsedTime > 300000) {
    shutdown();
  }
  // Wait for snow cap detection
  if (digitalRead(2) == LOW) {
    elapsedTime =   millis() - startTime; // store elapsed time
    musicOn = 0;
    digitalWrite(6, LOW); // Disable climbers 
    tone(5, 262, 100); // Play Sound
    delay(100);
    tone(5, 1047, 1500);
    // Announce winner
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Lloyd wins!");
    lcd.setCursor(0,1);
    lcd.print((int)(elapsedTime / 1000L));
    lcd.print(".");
    // use modulo operator to get fractional part of time 
    fractional = (int)(elapsedTime % 1000L);
    // pad in leading zeros - wouldn't it be nice if 
    // Arduino language had a flag for this? :)
    if (fractional == 0)
       lcd.print("000");      // add three zero's
    else if (fractional < 10)    // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
       lcd.print("00");       // add two zeros
    else if (fractional < 100)
       lcd.print("0");        // add one zero
    lcd.print(fractional);  // print fractional part of time 
    lcd.print(" sec");
    // Determine if new high score
    
    // If new high score, announce as such and write to EEPROM
    
    // Shut down
    delay(10000);
    shutdown();
    //digitalWrite(3, HIGH);
    //delay(10000);
  }
  if (digitalRead(4) == LOW) {
    elapsedTime =   millis() - startTime; // Stop stopwatch and store elapsed time
    digitalWrite(6, LOW); // Disable climbers 
    musicOn = 0;
    tone(5, 262, 100); // Play Sound
    delay(100);
    tone(5, 1047, 1500);
    // Announce winner
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mary wins!");
    lcd.setCursor(0,1);
    lcd.print((int)(elapsedTime / 1000L));
    lcd.print(".");
    // use modulo operator to get fractional part of time 
    fractional = (int)(elapsedTime % 1000L);
    // pad in leading zeros - wouldn't it be nice if 
    // Arduino language had a flag for this? :)
    if (fractional == 0)
       lcd.print("000");      // add three zero's
    else if (fractional < 10)    // if fractional < 10 the 0 is ignored giving a wrong time, so add the zeros
       lcd.print("00");       // add two zeros
    else if (fractional < 100)
       lcd.print("0");        // add one zero
    lcd.print(fractional);  // print fractional part of time 
    lcd.print(" sec");
    // Determine if new high score
    
    // If new high score, announce as such and write to EEPROM
    
    // Shut down
    delay(10000);
    shutdown();
    //digitalWrite(3, HIGH);
    //delay(10000);
  }
  if (musicOn == 1) {
    int noteDuration = 1000/noteDurations[thisNote];
    tone(5, melody[thisNote],noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(5);
    if (thisNote == 8) {
      thisNote = 0;
    } else {
      thisNote++;
    }
  }
} //end main loop

void shutdown()
{
  digitalWrite(3, HIGH);
  delay(10000);
}
