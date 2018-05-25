/*******************************************************************************

  Bare Conductive Pi Cap
  ----------------------

  simple-touch.cpp - simple MPR121 touch detection demo with stdout output

  Written for Raspberry Pi.

  Bare Conductive code written by Stefan Dzisiewski-Smith.

  This work is licensed under a MIT license https://opensource.org/licenses/MIT

  Copyright (c) 2016, Bare Conductive

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

 *******************************************************************************/

#include <MPR121.h>
#include <signal.h>
#include <iostream>
#include <stdlib.h>
#include <wiringPi.h>
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>

#define NUM_ELECTRODES 12
#define BUTTON_PIN 1
#define DEBOUNCE_LOCKOUT_MS    10
#define DOUBLEPRESS_TIMEOUT_US 300000
#define LONGPRESS_TIMEOUT_US   750000

using namespace std;

bool volatile keepRunning = true;
bool isCalibrating = false;

// enums and variables for state and timeout action
enum state_t { IDLE, PRESSED, RELEASED };
state_t volatile state = IDLE;

enum action_t { NONE, SINGLE_PRESS, LONG_PRESS };
action_t volatile action = NONE;

bool volatile isrEnabled = true;
bool volatile buttonFlag = false;

// button methods
void singlePress() {
	// single press event handler
	if (isCalibrating == true) {
	    system("sudo killall -9 /home/pi/openFrameworks/addons/ofxPiMapper/example_basic/bin/example_basic > /dev/null 2>&1");
	    system("sudo cp -rf /home/pi/openFrameworks/addons/ofxPiMapper/example_basic/bin/data/ofxpimapper.xml /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/data/presets.xml");
	    system("sudo /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/data/CreatePresets.sh");
	    system("sudo cp -rf /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/data/presetFinal.xml /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/data/ofxpimapper.xml");
	    system("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/example_bareconductiveFinal -f & > /dev/null");
	    isCalibrating = false;
	}
	else {
	    system("sudo killall -9 /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/example_bareconductiveFinal > /dev/null 2>&1");
	    system("/home/pi/openFrameworks/addons/ofxPiMapper/example_basic/bin/example_basic -f & > /dev/null");
	    isCalibrating = true;
	}
}

void doublePress() {
	// double press event handler
}

void longPress() {
	// long press event handler
}

void alarmHandler(int dummy) {
	// time-based part of state machine
	switch (action) {
	case NONE:
		break;
	case SINGLE_PRESS:
		singlePress(); // call the single press event handler
		action = NONE;
		state = IDLE;
		break;
	case LONG_PRESS:
		longPress(); // call the long press event handler
		action = NONE;
		state = IDLE;
		break;
	default:
		break;
	}
}

void buttonIsr(void) {
	// event based part of state machine
	if (isrEnabled) buttonFlag = true; // set the ISR flag, but only if our soft-gate is enabled
}

// this allows us to exit the program via Ctrl+C while still exiting elegantly
void intHandler(int dummy) {
  keepRunning = false;
  exit(0);
}

int main(void) {
  // register our interrupt handler for the Ctrl+C signal
  signal(SIGINT, intHandler);

  // default MPR121 address on the Pi Cap
  if (!MPR121.begin(0x5C)) {
    cout << "error setting up MPR121: ";

    switch (MPR121.getError()) {
      case NO_ERROR:
        cout << "no error" << endl;
        break;
      case ADDRESS_UNKNOWN:
        cout << "incorrect address" << endl;
        break;
      case READBACK_FAIL:
        cout << "readback failure" << endl;
        break;
      case OVERCURRENT_FLAG:
        cout << "overcurrent on REXT pin" << endl;
        break;
      case OUT_OF_RANGE:
        cout << "electrode out of range" << endl;
        break;
      case NOT_INITED:
        cout << "not initialised" << endl;
        break;
      default:
        cout << "unknown error" << endl;
        break;
    }

    exit(1);
  }

  // this is the touch threshold - setting it low makes it more like a proximity trigger
  // default value is 40 for touch
  int touchThreshold = 40;

  // this is the release threshold - must ALWAYS be smaller than the touch threshold
  // default value is 20 for touch
  int releaseThreshold = 20;

  MPR121.setTouchThreshold(touchThreshold);
  MPR121.setReleaseThreshold(releaseThreshold); 

  // register our interrupt handler for the ualarm signal
  signal(SIGALRM, alarmHandler);

  wiringPiSetup();

  // button pin is input, pulled up, linked to a dual-edge interrupt
  pinMode(BUTTON_PIN, INPUT);
  pullUpDnControl(BUTTON_PIN, PUD_UP);
  wiringPiISR(BUTTON_PIN, INT_EDGE_BOTH, buttonIsr);

  system("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/example_bareconductiveFinal -f & > /dev/null");
  //system("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/example_bareconductiveFinal -f");

  while (keepRunning) {
    if (MPR121.touchStatusChanged()) {
      MPR121.updateTouchData();

      for (int i=0; i < NUM_ELECTRODES; i++) {
        if (MPR121.isNewTouch(i)) {
          cout << "electrode " << i << " was just touched" << endl;
	  ofstream out("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductiveFinal/bin/command.txt");
 	  if (!out) {
   	    cout << "Couldn't open file." << endl;
	    exit(1);
	  }
	  else {
	    out << i;
            out.close();
	  }
        }
        else if (MPR121.isNewRelease(i)) {
          cout << "electrode " << i << " was just released" << endl;
        }
      }
    }

    // a little delay so that we don't just sit chewing CPU cycles
    // could implement this with proper interrupts for greater efficiency
    delay(10);

	if (buttonFlag) {
		if (!digitalRead(BUTTON_PIN)) {
			// button just pressed
			switch (state) {
			case IDLE:
				// disable the button ISR, set state to pressed and set long press timeout
				isrEnabled = false;
				state = PRESSED;
				action = LONG_PRESS; // what we'll do if we time out in this state...
				ualarm(LONGPRESS_TIMEOUT_US, 0);
				// delay a bit to avoid erroneous double-presses from switch bounce
				delay(DEBOUNCE_LOCKOUT_MS);
				// re-enable the ISR once we're clear of switch bounce
				isrEnabled = true;
				break;
			case RELEASED:
				// if we get another press when the switch has been released (and before
				// the double-press timeout has occured) we have a double-press
				// so reset the state machine
				action = NONE;
				state = IDLE;
				doublePress(); // call the double press event handler
				break;
			default:
				break;
			}
		}
		else {
			// button just released
			switch (state) {
			case PRESSED:
				// disable the button ISR, set state to released and set double press timeout
				isrEnabled = false;
				action = SINGLE_PRESS; // what we'll do if we timeout in this state
				ualarm(DOUBLEPRESS_TIMEOUT_US, 0);
				// delay a bit to avoid erroneous double-presses from switch bounce
				delay(DEBOUNCE_LOCKOUT_MS);
				state = RELEASED;
				// re-enable the ISR once we're clear of switch bounce
				isrEnabled = true;
				break;
			default:
				break;
			}
		}

		buttonFlag = false;
      }
  }

  // make sure we return gracefully
  return(0);
}
