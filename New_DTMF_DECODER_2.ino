// Jardina 9000 DTMF decoder made for The Snow Plow Show / Phone Losers of America
// March 12th, 2025: Removed the stupid amount repeating "if" statements / cleaned up the code. Also unknownly made the binary input make logical sense too.

float X; float Y; float Z; byte C; bool R = 1; // Random global variables 
char TouchTones[16] = {'D', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '*', '#', 'A', 'B', 'C'};

#include <LiquidCrystal.h> // Uses the Hitachi HD44780 display with an RGB backlight. This uses the Adafruit library.
LiquidCrystal lcd(12, 11, 10, 9, 8, 7); // (RS, E, D4, D5, D6, D7)

byte CACTUS[] = {B00100, B10100, B10101, B11101, B00111, B00100, B00100, B00100};

void setup() {
  lcd.createChar(0, CACTUS);
  lcd.begin(16, 2);  // Sets the LCD's number of columns and rows:
  lcd.print("PLA ");
  lcd.write(byte(0)); // This is the Cactus symbol
  lcd.print(" APCS ");
  lcd.write(byte(0));
  lcd.print(" PCN");
  lcd.setCursor(0, 1); // set the cursor to column 0, line 1
  lcd.blink();
  for (byte B = 13; B < 18; B++) { pinMode(B, INPUT); } // These are the input pins from the DTMF decoder
}

void loop() {
  backlight();              // Changes the color of the backlight
  if (input() > 15) {       // Checks to see if a tone is being received.
    byte DTMF = input();    // Adds & sets a new variable to compare after delay.
    delay(25);              // Just a brief delay cause random audio can trigger the DTMF decoder chip.
    if (DTMF == input()) {  // Compares the current tone to the one before delay & if same...
      lcd.print(TouchTones[DTMF - 16]); // Prints the tone to the LCD from array.
      screen();             // Advances the cursor
} } }

byte input() {
  byte Tone;
  for (byte B = 0; B <= 4; B++) {             // This use the MT8870 IC / module to decode the tones and outputs it in binary
    bitWrite(Tone, B, digitalRead(17 - B)); } // This loop reads and advances all five inputs to a single byte
  return Tone;
}

void screen() {
  C += 1;
  if (C == 16) {C = 0; R = !R;}
  lcd.setCursor(C, R);
  while (input() > 15) {Y = 0; X = millis() + 20025; backlight();} 
}

void backlight() { // This changes the color of the display from Green to Blue, and changes to Red when a Tone is detected
  if (X > millis() + 10025) {
    analogWrite(6, 255);                      // Red
    analogWrite(5, 0);                        // Green
    analogWrite(3, 0);                        // Blue
  }
  else if (X > millis() + 25 && Z < millis()) {
    analogWrite(6, abs(cos(Y * .05) * 255));  // Red
    analogWrite(5, 0);                        // Green
    analogWrite(3, abs(sin(Y * .05) * 255));  // Blue
    Y += .0625;
    Z = millis() + 18;
  }
  else if (X < millis()) {
    analogWrite(6, 0);                        // Red
    analogWrite(5, abs(cos(Y * .05) * 255));  // Green
    analogWrite(3, abs(sin(Y * .05) * 255));  // Blue
    Y += .0625;
    X = millis() + 25;
} }
