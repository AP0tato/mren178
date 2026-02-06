#ifndef BOARD_DEFINITION_H_
#define BOARD_DEFINITION_H_

// include the library code:

#include <LiquidCrystal.h>
#include "queue.h"

#define EXIT_OK      0
#define EXIT_ERROR  -1

typedef enum {none, right, left, up, down, select} Button;
typedef enum {dequeue_one, dequeue_all} DequeueOption;

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

// Prompt asking user which queue 
// Returns Queue number
int QueueSelect(LiquidCrystal *lcd);

// Enqueue LCD Interface
void EnqueueInterface(LiquidCrystal *lcd, DS3231 rtc_clock, pQueue queue_array[]);

// Dequeue LCD Interface
int DequeueInterface(LiquidCrystal *lcd, pQueue queue_array[]);

// Handles options for dequeuing - dequeue one or all nodes
DequeueOption DequeueMenu(LiquidCrystal *lcd);

// Logic to print one item
// Pass reference of which node to be printed
void PrintItem(LiquidCrystal *lcd, pStampedItem item);
void Print(LiquidCrystal *lcd, pQueue queue_array[]);

#endif
