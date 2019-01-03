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
Encoder encL(0, 1), encR(3, 2);
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
  
  delay(1000);
  // Setup I/O for pins
  for (int i = 0; i < buttonCount; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
    pinMode(macropin, INPUT_PULLUP);
  //  pinMode(sysPin,INPUT_PULLUP);
  //setup interrupts
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
 *  KEYPAD ASCII CODES
  220 Keypad /
  221 Keypad *
  222 Keypad -
  223 Keypad +
  224 Keypad ENTER
  225 Keypad 1 and End
  226 Keypad 2 and Down Arrow
  227 Keypad 3 and PageDn
  228 Keypad 4 and Left Arrow
  229 Keypad 5
  230 Keypad 6 and Right Arrow
  231 Keypad 7 and Home
  232 Keypad 8 and Up Arrow
  233 Keypad 9 and PageUp
  234 Keypad 0 and Insert
  235 Keypad . and Delete
  */

int macroButtonState = digitalRead(macropin);
int startButtonState = digitalRead(4);

// Press Start + Macro to send numpad_6
if (macroButtonState != old_macroButtonState) { //compare the button states (from StateChangeDetection Example)
  if (digitalRead(macropin) == LOW) { //if macro button is pressed
      if (digitalRead(4) == LOW){ // if start button is pressed
      Keyboard.begin();
      Keyboard.press(230); //numpad_6
      delay(40);
      Keyboard.release(230);
      Keyboard.end();
       } else {
        Keyboard.end();
       } 
    }
  }
  
// ****************Change your PIN here using the ascii table above**************************

if (digitalRead(4) == HIGH) {
 if (macroButtonState != old_macroButtonState) {  //compare the button states (from StateChangeDetection Example)
   if (digitalRead(macropin) == LOW) {  //if macro button is pressed
   Keyboard.begin();
   Keyboard.press(223); //numpad + (triggers card scan)
   delay(40);
   Keyboard.release(223);
   delay(40);
   Keyboard.press(226);//numpad 2
   delay(40);
   Keyboard.release(226);
   delay(40);
   Keyboard.press(230); //numpad 6
   delay(40);
   Keyboard.release(230);
   delay(40);
   Keyboard.press(233); //numpad 9
   delay(40);
   Keyboard.release(233);
   delay(40);
   Keyboard.press(228); //numpad 4
   delay(40);
   Keyboard.release(228);
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
  Keyboard.press(220); //Numpad_divide
  delay(40);
  Keyboard.release(220);
  Keyboard.end();
 } else {
  Keyboard.end();
 }
}


old_macroButtonState = macroButtonState; // save the current state as the last state, for next time through the loop
old_startButtonState = startButtonState; // save the current state as the last state, for next time through the loop
old_state = state;

    
  
    
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

