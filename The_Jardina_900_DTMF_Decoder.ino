// The Jardina 900 DTMF Decoder made for The Snow Plow Show / Phone Losers of America
// Made by Ryan Jardina / KF5EKB ~ Last update: The 17th of October 2025
// Code is under the GNU GPL v2

// User parameters --- If you don't know how to code these are the only things that need to be changed ------------------------------------------------

  // Use the POT or a set number for the Backlight brightness
    uint8_t BacklightRef = 0;                 // 0 = POT or 1-100 = % 

  // Does the Backlight have more than one color? (16-pins for single color & 18-pins for multi)
    bool MultiColorBacklight = 0;             // 0 = false, 1 = true

  // If the Backlight is multi color what is the order?
    uint16_t BacklightColorOrder = 0x653;     // 0x653 == RGB, each digit is the pin number

  // Does multicolor look weird than flip this bit. Should fade inbetween Green & Blue then turn Red on a Tone
  // May also be need for single color backlight too if the blacklight is on when it should be off & vice versa
  // just flip this bit if it's acting weird
    bool InvertBacklight = 1;                 // 0 = false, 1 = true

// End of User parameters ---------------------------------------------------------------------------------------------------------------------------

float X; float Y; float Z;                    // Variables for backlight timing
uint8_t Column; bool R = 1;                   // To keep track of the Cursor

uint8_t ButtonPins[] = {1, 0, 2, 4};          // Pins for Buttons (Back, Down, Up, Enter)
uint8_t DecoderPins[] = {17, 16, 15, 14, 13}; // Pins for Decoder (Q4, Q3, Q2, Q1, STD)

char TouchTones[] = {'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C'}; // Output for the tone, their in binary order

#include <LiquidCrystal.h>                    // Library for display
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);       // Creates object for display using those pin numbers (RS, E, D4, D5, D6, D7)
uint8_t CACTUS[] = {B00100, B10100, B10101, B11101, B00111, B00100, B00100, B00100}; // Cactus symbol

bool InputIndicator = 0;                      // Inducates if the button(s) were pressed
long TimeSincePressed = 0;                    // Time for when the buttons were first pressed to help with debouncing & combos
uint8_t ButtonInputted = 0;                   // Store which buttons were pressed

#include <EEPROM.h>                           // Library for long term momery for power cycling
bool BacklightOn;                             // Bit to toggle backlight

void setup(void) {
  lcd.createChar(0, CACTUS);                  // Places the Cactus symbol in the LCD memory
  lcd.begin(16, 2);                           // Sets the LCD's number of columns & rows:
  lcd.print("TheSnowPlowShow");               // Displayed message
  lcd.write(byte(0));                         // Display Cactus symbol
  lcd.setCursor(0, 1);                        // set the cursor to column 0, line 1
  lcd.blink();                                // Blinks the cursor
  for (uint8_t i = 0; i < sizeof(DecoderPins); i++) { pinMode(DecoderPins[i], INPUT); }      // Initiate pins for the DTMF decoder
  for (uint8_t i = 0; i < sizeof(ButtonPins); i++) { pinMode(ButtonPins[i], INPUT_PULLUP); } // Initiate pins for Buttons
  // These are options on the board that aren't used in code BUT there here if needed for future things!
  // pinMode(A4, OUTPUT); pinMode(A5, OUTPUT);   // I2C pins or in this case set has digital outputs
  // analogRead(A6) is on board also not being used but for more future things
  BacklightOn = EEPROM.read(0); }             // Value of the backlight from last power on

void loop(void) {
  if (Decoder() > 15) {                            // Checks to see if a tone is being received.
    uint8_t DTMF = Decoder();                      // Adds & sets a new variable to compare after delay.
    delay(25);                                     // Just a brief delay cause random audio can trigger the DTMF decoder chip.
    if (DTMF == Decoder()) {                       // Compares the current tone to the one before delay & if same...
      lcd.print(TouchTones[DTMF - 16]);            // Prints the tone to the LCD from array.
      if (Column++ == 16) { Column = 0; R = !R; }  // Advises the cursor in memory
      lcd.setCursor(Column, R);                    // Update cursor on display
      while (Decoder() == DTMF) { Y = 0; X = millis() + 60000; Backlight(); } // Waits for the tone to stop playing
      DTMF = 0; } }                                // Sets tone back to zero
  Backlight();                                     // Updates the backlight
  UserInput(); }                                   // Checks to see if any buttons are being pressed & acts on them

uint8_t Decoder(void) {
  uint8_t Tone = 0;                                // Place holder for which tone is being played
  for (uint8_t i = 0; i < sizeof(DecoderPins); i++) { bitWrite(Tone, i, digitalRead(DecoderPins[i])); } // Loops through all the pins to see if a tone is being played & which one 
  return Tone; }                                   // Returns which tone is being play

void UserInput(void) {
  if (Buttons() || InputIndicator) {                                // Are the buttons being pressed or were they
    if (!InputIndicator) {                                          // Were the button pressed in the recent pass
      TimeSincePressed = millis() + 125;                            // Bbuttons weren't being pressed set current time plus an 8th of a second variable
      InputIndicator = 1; }                                         // Set buttons indicator to true
    if (ButtonInputted < Buttons()) { ButtonInputted = Buttons(); } // This is to allow botton combos + debounce 
    if (!Buttons() && TimeSincePressed < millis()) {                // Buttons are now open again & at least an 8th of a second has passed since first press
      switch (ButtonInputted) {                                     // Goto case for which buttons where inputted
        default: case 0: break;                                     // Do nothing
        case 1 ... 15:                                              // Pretty much only one thing to do with the buttons
          BacklightOn = !BacklightOn;                               // Backlight on than turn off or vice versa
          EEPROM.write(0, BacklightOn);                             // Place value into long term momery
          break; }                                                  // Breaks out of the switch/case statement
      InputIndicator = 0;                                           // Set button indicator back to false
      ButtonInputted = 0; } } }                                     // Set ButtonInputted back to 0

uint8_t Buttons(void) {
  uint8_t Pressed = 0;                                              // Place holder for buttons
  for (uint8_t i = 0; i < sizeof(ButtonPins); i++) { bitWrite(Pressed, i, !digitalRead(ButtonPins[i])); } // Cycles through all the buttons
  return Pressed; }                                                 // Returns which buttons are pressed 

void Backlight(void) {
  if (BacklightOn || X > millis()) {                                // Option for Backlight always on or only if a tone was just played
    float Brightness = map(analogRead(A7), 0, 1024, 0, 100) * .01;  // Reads POT input & scales it to 0-1
    if (BacklightRef) { Brightness = 100 - BacklightRef * .01; }    // POT not selected then use preset %
    if (MultiColorBacklight) {                                      // Is the backlight multicolored
      if (X > millis() + 10000) {                                   // Turn the screen Red
        analogWrite(BacklightColorOrder >> 8 & 0xF, (InvertBacklight ? 255 : 0) - 255 * Brightness);                     // Red
        analogWrite(BacklightColorOrder >> 4 & 0xF, (InvertBacklight ? 255 : 0));                                        // Green
        analogWrite(BacklightColorOrder      & 0xF, (InvertBacklight ? 255 : 0)); }                                      // Blue
      else if (X > millis() + 25 && Z < millis()) {                 // Fade from Red to Blue
        analogWrite(BacklightColorOrder >> 8 & 0xF, (InvertBacklight ? 255 : 0) - abs(cos(Y * .05) * 255) * Brightness); // Red
        analogWrite(BacklightColorOrder >> 4 & 0xF, (InvertBacklight ? 255 : 0));                                        // Green
        analogWrite(BacklightColorOrder      & 0xF, (InvertBacklight ? 255 : 0) - abs(sin(Y * .05) * 255) * Brightness); // Blue
        Y += .0625;                                                 // Add a fruction to  Y also I don't remember
        Z = millis() + 18; }                                        // Add millis + 18 =  Z    "         "
      else if (X < millis()) {                                      // Fades from Blue to Green & back again
        analogWrite(BacklightColorOrder >> 8 & 0xF, (InvertBacklight ? 255 : 0));                                        // Red
        analogWrite(BacklightColorOrder >> 4 & 0xF, (InvertBacklight ? 255 : 0) - abs(cos(Y * .05) * 255) * Brightness); // Green
        analogWrite(BacklightColorOrder      & 0xF, (InvertBacklight ? 255 : 0) - abs(sin(Y * .05) * 255) * Brightness); // Blue
        Y += .0625;                                                 // Add a fruction to  Y also I don't remember
        X = millis() + 25; } }                                      // Add millis & 18 to X    "         "
    else { analogWrite(6, 255 * Brightness - (InvertBacklight ? 255 : 0)); } } // Backlight isn't multicolored
  else {                                                            // Turns off Backlight
    analogWrite(6, InvertBacklight ? 255 : 0);                      // Turn off Red
    analogWrite(5, InvertBacklight ? 255 : 0);                      // Turn off Green
    analogWrite(3, InvertBacklight ? 255 : 0); } }                  // Turn off Blue