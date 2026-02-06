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
pQueue queue_array[NO_OF_QUEUES]; 

void setup(){
  // Start the I2C interface
  Wire.begin();
  // Start the serial interface
  Serial.begin(9600);
  // Initialize LCD panel
  lcd = InitLCD(PINRS, PINEN, PIND4, PIND5, PIND6, PIND7,
                  LCD_WIDTH, LCD_HEIGHT);
  if(CreateQueueArray(queue_array, NO_OF_QUEUES) == EXIT_ERR)
    Serial.println("Creating queue array failed.");
}

void loop(){
  // Shows the menu on the LCD
  switch (Menu(lcd)) {
  case 1:
    EnqueueInterface(lcd, rtc_clock, queue_array); 
    ReturnToMenu(lcd);
    break;
  case 2:
    Print(lcd, queue_array);
    ReturnToMenu(lcd);
    break;
  case 3:
    DequeueInterface(lcd, queue_array);
    ReturnToMenu(lcd);
    break;
  }
} 
