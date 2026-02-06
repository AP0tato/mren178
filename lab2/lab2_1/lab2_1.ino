/*
 * This lab is designed for MREN 178 LAB 2
 * 
 * MENU DESCRIPTION:
 * Push: 
 * 1.select push on the LCD BOARD menu; you would see "insert char" on the menu page
 * 2.Open serial monitors using Alt+Shift+M
 * 3.on the row at the time, enter a string with the max size of 10 characters (10 is the buffer size)
 * 4. click send, on the LCD board it should say insert completed and then sent back to the menu  page
 * 
 * Pop:
 * 1. select pop on the LCD menu, you should see "pop all" and "pop one" if there is a single element in
 * the stack (if not, you should see <empty list>)
 * 2. if selecting pop one, you should be seeing nodes popping out, starting with the most recent one;
 * 3. Once finished, you will be taken back to the menu page
 * 
 * 
 * 
 */
#include "board_definition.h"

#define LCD_WIDTH   16
#define LCD_HEIGHT  2
#define LCD_SIZE    (LCD_WIDTH * LCD_HEIGHT)

#define PINRS       8
#define PINEN       9
#define PIND4       4 
#define PIND5       5 
#define PIND6       6 
#define PIND7       7  
#define BUTTONPIN   A0

LiquidCrystal *lcd;
DS3231 rtc_clock;
Stack stack_array[NO_OF_STACKS]; 

void setup(){
  // Start the I2C interface
  Wire.begin();
  // Start the serial interface
  Serial.begin(9600);
  
  // Initialize LCD panel
  lcd = InitLCD(PINRS, PINEN, PIND4, PIND5, PIND6, PIND7,
                  LCD_WIDTH, LCD_HEIGHT);
  CreateStackArray(stack_array, NO_OF_STACKS);
}

void loop(){
  // Shows the menu on the LCD
  switch (Menu(lcd)) {
  case 1:
    PushInterface(lcd, rtc_clock, stack_array); 
    ReturnToMenu(lcd);
    break;
  case 2:
    Print(lcd, stack_array);
    ReturnToMenu(lcd);
    break;
  case 3:
    PopInterface(lcd, stack_array);
    ReturnToMenu(lcd);
    break;
  }
} 
