/*
  Made by Gustavo Silveira, 2016.
  This Sketch reads the Arduino's digital and analog ports and send midi notes and midi control change

  http://www.musiconerd.com
  http://www.bitcontrollers.com

  If you are using for anything that's not personal use don't forget to give credit.

  gustavosilveira@musiconerd.com
*/


// Use if using with ATmega328 - Uno, Mega, Nano...
#include <AnalogSmooth.h> // Smooths the analog input (avarages)
#include <MIDI.h>
MIDI_CREATE_DEFAULT_INSTANCE();



// Use if using with ATmega32U4 - Micro, Pro Micro, Leonardo...
//#include "MIDIUSB.h"

/////////////////////////////////////////////
// buttons
const int NButtons = 3; //*
const int buttonPin[NButtons] = {2, 15, 23}; //* the number of the pushbutton pins in the desired not order
int touchPinState[NButtons] = {0};        // stores the state of the touch pin
int buttonCState[NButtons] = {0};         // stores the button current value
int buttonPState[NButtons] = {0};        // stores the button previous value

int maxVal[NButtons] = {0, 0, 0};

/////////////////////////////////////////////
// debounce
unsigned long lastDebounceTime[NButtons] = {0};  // the last time the output pin was toggled
unsigned long debounceDelay = 5;    //* the debounce time; increase if the output flickers

/////////////////////////////////////////////

byte midiCh = 1; //* MIDI channel to be used
byte note = 36; //* Lowest note to be used
byte notes[NButtons] = {36, 38, 40};
byte cc = 1; //* Lowest MIDI CC to be used

// SMOOTH
AnalogSmooth smoothVal = AnalogSmooth(10);

void setup() {

  Serial.begin(9600); // use if using with ATmega328 (uno, mega, nano...) - 115200 for Hairless MIDI or 31250 for MIDI class compliant

  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
}

void loop() {

  buttons();

}

/////////////////////////////////////////////
// BUTTONS
void buttons() {

  for (int i = 0; i < NButtons; i++) {

    float reading = touchRead(buttonPin[i]);
    touchPinState[i] = smoothVal.smooth(reading);

    //        Serial.print(i);
    //        Serial.print(" ");
    //        Serial.print(touchPinState[i]);
    //        Serial.print("    ");

    if (touchPinState[i] > maxVal[i]) {
      maxVal[i] = touchPinState[i];
    }

    if (touchPinState[i] > maxVal[i] - 400) {
      buttonCState[i] = HIGH;
    } else {
      buttonCState[i] = LOW;
    }

    /*
        // Comment this if you are not using pin 13...
        if (i == pin13index) {
          buttonCState[i] = !buttonCState[i]; //inverts pin 13 because it has a pull down resistor instead of a pull up
        }
        // ...until here
    */
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (buttonPState[i] != buttonCState[i]) {
        lastDebounceTime[i] = millis();

        if (buttonCState[i] == HIGH) {
          // use if using with ATmega328 (uno, mega, nano...)
          //do usbMIDI.sendNoteOn if using with Teensy
          usbMIDI.sendNoteOn(notes[i], random(90, 110), midiCh); // note, velocity, channel
          //          Serial.print(i);
          //          Serial.print(": ");
          //          Serial.print(notes[i]);
          //          Serial.println(" ON");

        }
        else {
          // use if using with ATmega328 (uno, mega, nano...)
          //do usbMIDI.sendNoteOn if using with Teensy
          usbMIDI.sendNoteOn(notes[i], 0, midiCh); // note, velocity, channel
          //          Serial.print(i);
          //          Serial.print(": ");
          //          Serial.print(notes[i]);
          //          Serial.println(" OFF");

        }
        buttonPState[i] = buttonCState[i];
      }
    }
  }
  //Serial.println();
}


