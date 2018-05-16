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

#define NUM_ELECTRODES 12

using namespace std;

bool volatile keepRunning = true;

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

  while (keepRunning) {
    if (MPR121.touchStatusChanged()) {
      MPR121.updateTouchData();

      for (int i=0; i < NUM_ELECTRODES; i++) {
        if (MPR121.isNewTouch(i)) {
          cout << "electrode " << i << " was just touched" << endl;

	  if (i == 0) {
	    system("clear");
	    system("sudo killall -9 /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/example_bareconductive > /dev/null 2>&1");
	    system("sudo cp -rf /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data/annie_gabriel.xml /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data/ofxpimapper.xml");
	    system("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/example_bareconductive -f & > /dev/null");
	  }
	  else if (i == 1) {
	    system("clear");
	    system("sudo killall -9 /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/example_bareconductive > /dev/null 2>&1");
	    system("sudo cp -rf /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data/jeff_goodboi.xml /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data/ofxpimapper.xml");
	    system("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/example_bareconductive -f & > /dev/null");
	  }
	  else if (i == 2) {
	    system("clear");
	    system("sudo killall -9 /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/example_bareconductive > /dev/null 2>&1");
	    system("sudo cp -rf /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data/karine_cournoyer.xml /home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/data/ofxpimapper.xml");
	    system("/home/pi/openFrameworks/addons/ofxPiMapper/example_bareconductive/bin/example_bareconductive -f & > /dev/null");
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
  }

  // make sure we return gracefully
  return(0);
}
