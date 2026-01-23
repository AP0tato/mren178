/******************************************************************************
FILENAME:     lab_1_1.ino 
COURSE:       MREN 178
LAB:          1
STUDENT(S):   Ali Abid: 20542690, Andres Escolar 20529215
DATE:         

ATTRIBUTIONS: [LIST ANY EXTERNAL CONTIBUTORS/CONTRIBUTIONS HERE] 

******************************************************************************/
// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to      
const int pin_RS = 8;
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd(pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);

int curr_num = 100;
const int BTN_DELAY = 200;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Number:");
  lcd.setCursor(1,1);
  lcd.print(curr_num);
}

void loop() {
  int x;
  x = analogRead (0);
  // Serial.println(x);
  lcd.setCursor(10,1);
  if (x < 60) { // RIGHT
    ++curr_num;
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 200) { // UP
    curr_num += 10;
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 400){ // DOWN
    curr_num -= 10;
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 600){ // LEFT
    --curr_num;
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 800){
    lcd.print ("Select");
  }
}
