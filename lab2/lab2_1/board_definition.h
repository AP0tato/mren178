#ifndef BOARD_DEFINITION_H_
#define BOARD_DEFINITION_H_

// include the library code:

#include <LiquidCrystal.h>
#include "Stack.h"

#define EXIT_OK         0
#define EXIT_ERROR     -1

typedef enum {none, right, left, up, down, select} Button;
typedef enum {pop_one, pop_all} PopOption;

///////////////////////////////FUNCTION PROTOTYPES//////////////////////////////
LiquidCrystal* InitLCD( int pin_rs, int pin_en, int pin_d4, int pin_d5, 
                        int pin_d6, int pin_d7, int lcd_width, int lcd_height);

// checks button input 
Button ButtonCheck();

// Creates a menu for the lcd
// Returns selected menu option - 1: enqueue 2: display queue 3: dequeue  
int Menu(LiquidCrystal *lcd);

// Return to menu prompt
void ReturnToMenu(LiquidCrystal *lcd);

// Prompt asking user which stack
// Returns Stack number
int StackSelect(LiquidCrystal *lcd);

// Push LCD Interface
void PushInterface(LiquidCrystal *lcd, DS3231 rtc_clock, Stack stack_array[]);

// Pop LCD Interface
int PopInterface(LiquidCrystal *lcd, Stack stack_array[]);

// Handles options for popping - pop one or all nodes
PopOption PopMenu(LiquidCrystal *lcd);

// Prints the complete stack 
void Print(LiquidCrystal *lcd, Stack stack_array[]);

#endif
