#include "iivx_leo.h"
#include "Encoder.h"
iivxReport_t report;
#include "Keyboard.h"

#define REPORT_DELAY 2000
// Number of microseconds between HID reports
// 2000 = 500hz

// Options, change only these values
int old_macroButtonState = HIGH;
int old_startButtonState = HIGH;
int old_state = 0;
int old_Button1State = HIGH;
int old_Button2State = HIGH;
uint8_t buttonCount = 7;
uint8_t macropin = 16;
uint8_t lightMode = 0;
// 0 = reactive lighting, 1 = HID lighting
uint8_t buttonPins[] = { 4, 5, 6, 7, 8, 9, 10};
uint8_t ledPins[] = {15, 15, 15, 15, 15, 15, 15, 15, 15};
/* current pin layout
    pins 6 to 12 = LED 1 to 7 //{12, 11, 10, 9, 8, 7, 6}
    pins 13, A0 to A5 = Button input 1 to 7
   
    pins 5 = system pin
      connect system pin to ground with a small button
        press together with other buttons to change lighting scheme
      system button + button 1 = reactive lighting
      system button + button 3 = HID lighting
*/

#define ENCODER_SENSITIVITY (double) 0.375
// encoder sensitivity = number of positions per rotation times 4 (24*4) / number of positions for HID report (256)
Encoder encL(2, 3), encR(1, 0);
/*
   connect encoders
   VOL-L to pins 0 and 1
   VOL-R to pins 2 and 3
*/

void lights(uint8_t lightDesc) {
  for (int i = 0; i < buttonCount; i++) {
    if ((lightDesc >> i) & 1) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void setup() {
//Serial.begin(9600);
// while (!Serial) ;
  
  delay(1000);
  // Setup I/O for pins
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
    pinMode(macropin, INPUT_PULLUP);
  //  pinMode(sysPin,INPUT_PULLUP);
  //setup interrupts


// if (digitalRead(9) == HIGH && (digitalRead(10) == HIGH)) {
 // Serial.print("Both pins are high");
// }
}
void loop() {
  // Read buttons
  for (int i = 0; i < buttonCount; i++) {
    if (digitalRead(buttonPins[i]) != HIGH) {
      report.buttons |= (uint16_t)1 << i;
    } else {
      report.buttons &= ~((uint16_t)1 << i);
    }
  }
  // Read Encoders
  report.xAxis = (uint8_t)((int32_t)(encL.read() / ENCODER_SENSITIVITY) % 256);
  report.yAxis = (uint8_t)((int32_t)(encR.read() / ENCODER_SENSITIVITY) % 256);
  // Light LEDs
  if (lightMode == 0) {
    lights(report.buttons);
  } else {
    lights(iivx_led);
  }

/* 
   ------- KEYBOARD ASCII CODES -------
  Top row keys: (use these if you don't have a numpad on your pc)
  48 - 0
  49 - 1
  50 - 2
  51 - 3
  52 - 4
  53 - 5
  54 - 6
  55 - 7
  56 - 8
  57 - 9
  45 - - (minus)(default service button)
  61 - = (equals)(default test button)
  8 - backspace
  
  
  220 NumPad /
  221 NumPad *
  222 NumPad -
  223 NumPad +
  224 NumPad ENTER
  225 NumPad 1 and End
  226 NumPad 2 and Down Arrow
  227 NumPad 3 and PageDn
  228 NumPad 4 and Left Arrow
  229 NumPad 5
  230 NumPad 6 and Right Arrow
  231 NumPad 7 and Home
  232 NumPad 8 and Up Arrow
  233 NumPad 9 and PageUp
  234 NumPad 0 and Insert
  235 NumPad . and Delete
  */

int macroButtonState = digitalRead(macropin);
int startButtonState = digitalRead(4);
int Button1State = digitalRead(5);
int Button2State = digitalRead(6);

// Press Start + Macro to send numpad_6
if (macroButtonState != old_macroButtonState) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(macropin) == LOW) { //if macro button is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(54); //top row 6
      delay(30);
      Keyboard.release(54);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }

//Commented out some shortcuts, can't tell if causing input lag or not... maybe it's just my terrible sdvx skills
/*  // Press Start + Btn1 to send Numpad 9
if (Button1State != old_Button1State) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(5) == LOW) { //if Btn1 is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(233); //numpad_9
      delay(30);
      Keyboard.release(233);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }

    // Press Start + Btn2 to send Numpad 3
if (Button2State != old_Button2State) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(6) == LOW) { //if Btn6 is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(227); //numpad_3
      delay(30);
      Keyboard.release(227);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }
*/
// ****************Change your PIN here using the ascii table above**************************

if (digitalRead(4) == HIGH) {
 if (macroButtonState != old_macroButtonState) {  //compare the button states (from StateChangeDetection Example)
   if (digitalRead(macropin) == LOW) {  //if macro button is pressed
   Keyboard.begin();
   Keyboard.press(8); //backspace (triggers card scan)
   delay(300);
   Keyboard.release(8);
   delay(30);
   Keyboard.press(49);//toprow 1
   delay(30);
   Keyboard.release(49);
   delay(30);
   Keyboard.press(50); //toprow 2
   delay(30);
   Keyboard.release(50);
   delay(30);
   Keyboard.press(51); //toprow 3
   delay(30);
   Keyboard.release(51);
   delay(30);
   Keyboard.press(52); //toprow 4
   delay(30);
   Keyboard.release(52);
   Keyboard.end();// end keyboard
   } else {
  Keyboard.end();
      }
  }
}

//Press button 1-7 at the same time to send Numpad_Divide, which can be mapped to the Test button. 
int state=0;

 if (digitalRead(4) == LOW) {
 state = state + 1;
 }
 if (digitalRead(5) == LOW) {
 state = state + 1;
 }
 if (digitalRead(6) == LOW) {
 state = state + 1;
 }
 if (digitalRead(7) == LOW) {
 state = state + 1;
 }
 if (digitalRead(8) == LOW) {
 state = state + 1;
 }
 if (digitalRead(9) == LOW) {
 state = state + 1;
 }
 if (digitalRead(10) == LOW) {
 state = state + 1;
 }
if (state != old_state) {
 if (state == 7) {
  Keyboard.begin();
  Keyboard.press(61); //Top row equals
  delay(40);
  Keyboard.release(61);
  Keyboard.end();
 } else {
  Keyboard.end();
 }
}


old_macroButtonState = macroButtonState; // save the current state as the last state, for next time through the loop
old_startButtonState = startButtonState; // save the current state as the last state, for next time through the loop
old_state = state;
old_Button1State = Button1State;
old_Button2State = Button2State;

    
  
    
  // Detect lighting changes
  // if(digitalRead(sysPin)!=HIGH){
  //   if(report.buttons == 1){
  //     lightMode = 0;
  //  } else if (report.buttons == 4){
  //    lightMode = 1;
  //  } else if (report.buttons == 16){
  //    report.buttons = (uint16_t)1 << (buttonCount);
  //   } else if (report.buttons == 64){
  //     report.buttons = (uint16_t)1 << (buttonCount+1);
  //   }
  //  }
  // Send report and delay
  iivx.setState(&report);
  delayMicroseconds(REPORT_DELAY);
}
