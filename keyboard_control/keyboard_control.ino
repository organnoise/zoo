#define NUM_BUTTONS 2 // IMPORTANT! define the number of buttons


//key commands
char leftKey = KEY_LEFT_ARROW;
char rightKey = KEY_RIGHT_ARROW;

/*Modifiers list 

KEY_LEFT_CTRL 0x80  128
KEY_LEFT_SHIFT  0x81  129
KEY_LEFT_ALT  0x82  130
KEY_LEFT_GUI  0x83  131
KEY_RIGHT_CTRL  0x84  132
KEY_RIGHT_SHIFT 0x85  133
KEY_RIGHT_ALT 0x86  134
KEY_RIGHT_GUI 0x87  135
KEY_UP_ARROW  0xDA  218
KEY_DOWN_ARROW  0xD9  217
KEY_LEFT_ARROW  0xD8  216
KEY_RIGHT_ARROW 0xD7  215
KEY_BACKSPACE 0xB2  178
KEY_TAB 0xB3  179
KEY_RETURN  0xB0  176
KEY_ESC 0xB1  177
KEY_INSERT  0xD1  209
KEY_DELETE  0xD4  212
KEY_PAGE_UP 0xD3  211
KEY_PAGE_DOWN 0xD6  214
KEY_HOME  0xD2  210
KEY_END 0xD5  213
KEY_CAPS_LOCK 0xC1  193
KEY_F1  0xC2  194
KEY_F2  0xC3  195
KEY_F3  0xC4  196
KEY_F4  0xC5  197
KEY_F5  0xC6  198
KEY_F6  0xC7  199
KEY_F7  0xC8  200
KEY_F8  0xC9  201
KEY_F9  0xCA  202
KEY_F10 0xCB  203
KEY_F11 0xCC  204
KEY_F12 0xCD  205

*/
const int buttonPin[] = {4, 5};//Pins for each button input

// Variables will change:
int buttonState[NUM_BUTTONS];             // the current reading from the input pin
int lastButtonState[NUM_BUTTONS];   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
int reading[NUM_BUTTONS];
long onTime[NUM_BUTTONS];
long hold_Timer[NUM_BUTTONS];
long lastDebounceTime[NUM_BUTTONS];  // the last time the output pin was toggled

long debounceDelay = 50;    // the debounce time; increase if the output flickers
long holdTime = 250; // time interval between serial msgs while being held. So the comp doesn't get buried in msgs

void setup() {
  // make pin 2 an input and turn on the
  // pullup resistor so it goes high unless
  // connected to ground:
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPin[i], INPUT);
    buttonState[i] = HIGH;
    lastButtonState[i] = LOW;
    lastDebounceTime[i] = onTime[i] = 0;
  }
  
  Serial.begin(9600);
  Keyboard.begin();
}

void loop() {

  //Add subsequent function calls for each button
  updateButton(0, leftKey);
  updateButton(1, rightKey);
}


//This checks the button, if it is pressed it sends the specified message.
//There is also the "holdCheck" function which allows the message to be continued to be sent 
//at an interal until released. 

//_i = which button from buttonPin[], _s is the keystroke to be sent 
void updateButton(int _i, char _s) {
  // read the state of the switch into a local variable:
  reading[_i] = digitalRead(buttonPin[_i]);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading[_i] != lastButtonState[_i]) {
    // reset the debouncing timer
    lastDebounceTime[_i] = millis();
    onTime[_i] = millis();
  }

  holdCheck(_i, _s);

  if ((millis() - lastDebounceTime[_i]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading[_i] != buttonState[_i]) {
      buttonState[_i] = reading[_i];

      // only toggle the LED if the new button state is HIGH
      if (buttonState[_i] == HIGH) {
        //Serial.println(_s);
        //Send message
        Keyboard.write(_s);
      }
    }
  }
  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState[_i] = reading[_i];
}

//Check to see if button is being held
void holdCheck(int _i, char _s) {
  if (reading[_i] == HIGH && lastButtonState[_i] == HIGH) {
    if ((millis() - onTime[_i]) > holdTime) {

      //Hold timer so that it isn't frantically printing
      if (millis() - hold_Timer[_i] > holdTime * 0.5) {
        //Serial.println(_s + " HOLD");
        //Send message
        Keyboard.write(_s);
        hold_Timer[_i] = millis();
      }
    }
  }
}
