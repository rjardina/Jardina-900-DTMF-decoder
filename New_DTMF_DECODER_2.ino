byte D = 25; // Delay in milliseconds
byte RGB[3];
float X;
float Y;
float Z;
byte C;
byte R;
byte Q;
byte Tone;

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 10, 9, 8, 7); // rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;

byte CACTUS[] = {
  B00100,
  B10100,
  B10101,
  B11101,
  B00111,
  B00100,
  B00100,
  B00100  };

void setup() {
  lcd.createChar(0, CACTUS);
  lcd.begin(16, 2);  // set up the LCD's number of columns and rows:
  lcd.print("APCS ");
  lcd.write(byte(0));
  lcd.print(" PLA ");
  lcd.write(byte(0));
  lcd.print(" PCN");
  lcd.setCursor(0, 1);  // set the cursor to column 0, line 1
  lcd.blink();
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);
  pinMode(16, INPUT);
  pinMode(17, INPUT);
  R = 1;
}

void loop() {
  backlight();
  input();
  if (Tone > 15) {
    if (Tone == 16) {delay(D); input(); if (Tone == 16) {lcd.print("D"); screen();}}
    if (Tone == 17) {delay(D); input(); if (Tone == 17) {lcd.print("8"); screen();}}
    if (Tone == 18) {delay(D); input(); if (Tone == 18) {lcd.print("4"); screen();}}
    if (Tone == 19) {delay(D); input(); if (Tone == 19) {lcd.print("#"); screen();}}
    if (Tone == 20) {delay(D); input(); if (Tone == 20) {lcd.print("2"); screen();}}
    if (Tone == 21) {delay(D); input(); if (Tone == 21) {lcd.print("0"); screen();}}
    if (Tone == 22) {delay(D); input(); if (Tone == 22) {lcd.print("6"); screen();}}
    if (Tone == 23) {delay(D); input(); if (Tone == 23) {lcd.print("B"); screen();}}
    if (Tone == 24) {delay(D); input(); if (Tone == 24) {lcd.print("1"); screen();}}
    if (Tone == 25) {delay(D); input(); if (Tone == 25) {lcd.print("9"); screen();}}
    if (Tone == 26) {delay(D); input(); if (Tone == 26) {lcd.print("5"); screen();}}
    if (Tone == 27) {delay(D); input(); if (Tone == 27) {lcd.print("A"); screen();}}
    if (Tone == 28) {delay(D); input(); if (Tone == 28) {lcd.print("3"); screen();}}
    if (Tone == 29) {delay(D); input(); if (Tone == 29) {lcd.print("*"); screen();}}
    if (Tone == 30) {delay(D); input(); if (Tone == 30) {lcd.print("7"); screen();}}
    if (Tone == 31) {delay(D); input(); if (Tone == 31) {lcd.print("C"); screen();}}
  }
}

void input() {
  bitWrite(Tone, 0, digitalRead(14));
  bitWrite(Tone, 1, digitalRead(15));
  bitWrite(Tone, 2, digitalRead(16));
  bitWrite(Tone, 3, digitalRead(17));
  bitWrite(Tone, 4, digitalRead(13));
}

void screen(){
  C += 1;
  if (C == 16) {C = 0; R = !R;}
  lcd.setCursor(C, R);
  while (Tone > 15) {input(); Y = 0; X = millis() + 20025; backlight();} 
}

void backlight() {
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
  }
}
