//
// DTMF Decoder by Ryan Jardina
// The main reason for making this is due not being able to find a program I like.
// Biggest issue with most is they would either double up on a single digit OR not
// be able to decode two same consecutive double digit i.e. 11,33,77 etcetc. 
//

int Q1; int Q2; int Q3; int Q4;
int ROW = 0; int COL = 1;

#include <LiquidCrystal.h> // LCD library
LiquidCrystal lcd(2, 3, 4, 5, 6, 7); // (RS, E, D4, D5, D6, D7)

byte customChar[8] = {0b00100, 0b10100, 0b10101, 0b11101, 0b00111, 0b00100, 0b00100, 0b00100}; //Custom CACTUS symbol

void setup()
{
  lcd.createChar(0, customChar);
  lcd.begin(16, 2); // set up the LCD's number of columns and rows
  lcd.cursor(); // displays cursor
  lcd.blink(); // blinks cursor
  lcd.write((uint8_t)8); // CACTUS symbol
  lcd.print(" CACTUS ");
  lcd.write((uint8_t)8); // CACTUS symbol
  lcd.print(" PLA ");
  lcd.write((uint8_t)8); // CACTUS symbol
  lcd.setCursor(ROW, COL); // Sets Cursor
  pinMode(14, INPUT); // Q1 to A0/D14
  pinMode(15, INPUT); // Q2 to A1/D15
  pinMode(16, INPUT); // Q3 to A2/D16
  pinMode(17, INPUT); // Q4 to A3/D17
  pinMode(18, INPUT); // STD to A4/D18
}

void loop()
{
  while(digitalRead(18) != 1) { } // waits for tone to start, which is on STD pin
  
  Q1 = digitalRead(14);
  Q2 = digitalRead(15); // This reads the 4 inputs from the 
  Q3 = digitalRead(16); // decoder and then sets the Q? values
  Q4 = digitalRead(17);
  
  if (Q4 == 0 && Q3 == 0 && Q2 == 0 && Q1 == 0) lcd.print("D"); 
  if (Q4 == 0 && Q3 == 0 && Q2 == 0 && Q1 == 1) lcd.print("1");
  if (Q4 == 0 && Q3 == 0 && Q2 == 1 && Q1 == 0) lcd.print("2");
  if (Q4 == 0 && Q3 == 0 && Q2 == 1 && Q1 == 1) lcd.print("3");
  if (Q4 == 0 && Q3 == 1 && Q2 == 0 && Q1 == 0) lcd.print("4");
  if (Q4 == 0 && Q3 == 1 && Q2 == 0 && Q1 == 1) lcd.print("5");
  if (Q4 == 0 && Q3 == 1 && Q2 == 1 && Q1 == 0) lcd.print("6");
  if (Q4 == 0 && Q3 == 1 && Q2 == 1 && Q1 == 1) lcd.print("7");
  if (Q4 == 1 && Q3 == 0 && Q2 == 0 && Q1 == 0) lcd.print("8");
  if (Q4 == 1 && Q3 == 0 && Q2 == 0 && Q1 == 1) lcd.print("9");
  if (Q4 == 1 && Q3 == 0 && Q2 == 1 && Q1 == 0) lcd.print("0");
  if (Q4 == 1 && Q3 == 0 && Q2 == 1 && Q1 == 1) lcd.print("*");
  if (Q4 == 1 && Q3 == 1 && Q2 == 0 && Q1 == 0) lcd.print("#");
  if (Q4 == 1 && Q3 == 1 && Q2 == 0 && Q1 == 1) lcd.print("A");
  if (Q4 == 1 && Q3 == 1 && Q2 == 1 && Q1 == 0) lcd.print("B");
  if (Q4 == 1 && Q3 == 1 && Q2 == 1 && Q1 == 1) lcd.print("C");

  ROW = ++ROW;                              // Next 4 lines moves the Cursor, LCD has  
  if (ROW == 16) { ROW = 0; COL = ++COL; }  // more columns in memory then being displayed.
  if (COL == 2) COL = 0;                    // Causing most recent numbers not to be displayed. 
  lcd.setCursor(ROW, COL);                  // This 'manually' moves the cursor

  while(digitalRead(18) != 0) { } // waits for tone to end
}
