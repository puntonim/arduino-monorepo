/*
 * MULTIPLE PUSH BUTTONS
 * =====================
 * Connect 3 buttons, detect the push of any button and turn ON the builtin LED.
 * Using Arduino's integrated pull-up resistors on digital pins.
 *
 * ALTERNATIVE w/ 1 analog PIN only: the same thing can be done with an Analog Input,
 *  connecting the resistors in series and each interrupted by a button. 
 *  The Analog Input would measure the drop of tension and thus deduce how many
 *  resistors were connected, thus what button was pressed. Handy because it 
 *  uses only 1 PIN vs the 3 PINs of the version in this sketch with Digital Inputs.
 *  https://youtu.be/Y23vMfynUJ0?si=sErJHSPfWTwCD8Bg
 *
 * Docs:
 *  - https://docs.arduino.cc/built-in-examples/basics/DigitalReadSerial/
 *  - https://docs.arduino.cc/built-in-examples/digital/Button/
 */

const unsigned short BUTTON_A_PIN = 2;
const unsigned short BUTTON_B_PIN = 3;
const unsigned short BUTTON_C_PIN = 4;

// Note: never use the pin 13 as it is used by the LED_BUILTIN.

void setup() {
  Serial.begin(57600);
  Serial.println("START");

  // Options: INPUT, OUTPUT, or INPUT_PULLUP.
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);
  // Declare the integrated LED on pin 13 as output;
  pinMode(LED_BUILTIN, OUTPUT); // Pin 13.
}

void loop() {
  bool isButtonAPressed = !digitalRead(BUTTON_A_PIN);
  bool isButtonBPressed = !digitalRead(BUTTON_B_PIN);
  bool isButtonCPressed = !digitalRead(BUTTON_C_PIN);

  char symbol = 'X';
  if (isButtonAPressed) symbol = 'A';
  else if (isButtonBPressed) symbol = 'B';
  else if (isButtonCPressed) symbol = 'C';
  Serial.println(symbol);
  
  // Turn ON the LED if any button is pressed.
  digitalWrite(LED_BUILTIN, isButtonAPressed || isButtonBPressed || isButtonCPressed);
  delay(100);
}
