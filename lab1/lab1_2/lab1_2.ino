/******************************************************************************
FILENAME:     lab_1_1.ino 
COURSE:       MREN 178
LAB:          1
STUDENT(S):   Ali Abid: 20542690,
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
int arr[5];
int curr_index = 0;

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Number:");
  lcd.setCursor(1,1);
  lcd.print(curr_num);
}

void clear_row(byte row)
{
  lcd.setCursor(0, row);
  lcd.print("                ");
}

void loop() {
  int x;
  x = analogRead (0);
  // Serial.println(x);
  lcd.setCursor(10,1);
  if (x < 60) { // RIGHT
    ++curr_num;
    clear_row(1);
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 200) { // UP
    curr_num += 10;
    clear_row(1);
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 400){ // DOWN
    curr_num -= 10;
    clear_row(1);
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 600){ // LEFT
    --curr_num;
    clear_row(1);
    lcd.setCursor(1,1);
    lcd.print(curr_num);
    delay(BTN_DELAY);
  }
  else if (x < 800){ // SELECT
    if(curr_index > 4)
    {
      clear_row(1);
      lcd.setCursor(1,1);
      lcd.print("Array is full");
      Serial.print("Array is full\n");
    }
    else
    {
      arr[curr_index] = curr_num;
      for(int i = 0; i <= curr_index; i++)
      {
        Serial.print(arr[i]);
        Serial.print(" ");
      }
      Serial.print("\n");
      ++curr_index;
    }
    delay(BTN_DELAY);
  }
}
