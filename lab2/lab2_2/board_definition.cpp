#include "board_definition.h"

LiquidCrystal* InitLCD( int pin_rs, int pin_en, int pin_d4, int pin_d5, 
                        int pin_d6, int pin_d7, int lcd_width, int lcd_height) {                         
  LiquidCrystal *lcd = new LiquidCrystal(pin_rs, pin_en, pin_d4, pin_d5, pin_d6, pin_d7);
  lcd->begin(lcd_width, lcd_height);
  return lcd; 
}

Button ButtonCheck() {
  Button button; 
  int x = analogRead(A0);
  if (x < 60) button = right;
  else if (x > 60 && x < 200) button = up;
  else if (x > 200 && x < 400) button = down;
  else if (x > 400 && x < 600) button = left;
  else if (x < 800 && x >= 600) button = select;
  else button = none;
  return button;
}

// Returns selected menu option - 1: enqueue 2: display queue 3: dequeue  
int Menu(LiquidCrystal *lcd) {
  int menu_option = 1; 

  //Displays the initial menu
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Select Action");
  lcd->setCursor(12, 1);
  lcd->print("MENU");
  
  lcd->setCursor(0, 1);
  lcd->print("Enqueue    "); // 11 chars

  // look for button actions, if any are found go with the 
  // proceeding operations
  Button button;
  while (1) {
    button = ButtonCheck();
    lcd->setCursor(0, 1);

    if ( button != none ) {
      if ( button == right || button == up) {
        menu_option++;
        if (menu_option > 3) menu_option = 1;
      }
      else if ( button == left || button == down) {
        menu_option--;
        if (menu_option < 1) menu_option = 3;
      }
      else if ( button == select ) {
        delay(250);
        return menu_option;
      }

      // print menu choices on row 2 of lcd
      lcd->setCursor(0, 1);
      switch (menu_option) {
        case 1:
            lcd->print("Enqueue    ");
            break;
        case 2:
            lcd->print("Display    ");
            break;
        case 3:
            lcd->print("Dequeue     ");
            break;
      } 
      delay(250);
    }
  }
}

void ReturnToMenu(LiquidCrystal *lcd) {
  lcd->setCursor(0,1);
  lcd->print("Return to MENU        ");
  while(1) {
    //select button press detected?
    if(ButtonCheck() == select) {
        delay(250);
        break;
    }
  }
}

int QueueSelect(LiquidCrystal *lcd) {
  int queue_number = 0;

  lcd->setCursor(0, 1);
  lcd->print("Which Queue?    ");
  lcd->setCursor(14, 1);
  lcd->print(queue_number);
  Button button;
  while (1) {
    lcd->setCursor(0, 1);
    button = ButtonCheck();
    if ( button != none ) {
      if ( button == right || button == up) 
        queue_number = ++queue_number % NO_OF_QUEUES;
      else if ( button == left || button == down) {
        if(--queue_number < 0) queue_number = NO_OF_QUEUES - 1;
      } else if ( button == select ) {
        delay(250);
        return queue_number;
      }
  
      // print menu choices on Row 2
      lcd->setCursor(14, 1);
      lcd->print(queue_number);
      
      delay(250);
    }
  }
}

void EnqueueInterface (LiquidCrystal *lcd, DS3231 rtc_clock, 
                        pQueue queue_array []) {
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Enqueue Item");

  int queue_number = QueueSelect(lcd);
  pStampedItem item = CreateItem (GetTime(rtc_clock)); 
  lcd->clear();
  lcd->setCursor(0,0);
  if(Enqueue(queue_array[queue_number], item) == EXIT_OK) {
    // A quick message on the board showing that the item is enqueued
    lcd->print("Enqueued in ");
    lcd->print(queue_number);
    lcd->setCursor(0,1);
    lcd->print(item->timestamp);
  } else {
    lcd->print("Enqueue failed! ");
    lcd->setCursor(0,1);
    lcd->print(item->timestamp); 
  }
  delay(1000);
}

int DequeueInterface(LiquidCrystal *lcd, pQueue queue_array[]) {
  long timestamp;
  int queue_number = QueueSelect(lcd);
  lcd->clear();
  DequeueOption option = DequeueMenu(lcd);
  lcd->clear();
  
  switch (option) {
    case dequeue_one:
      lcd->setCursor(0, 0);
      lcd->print("Dequeue One Node");

      // performs dequeue
      if (Dequeue (queue_array[queue_number], timestamp) == EXIT_OK) {
        lcd->setCursor(0, 1);
        lcd->print(timestamp);
        delay(1000);
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("Dequeued from ");
        lcd->print(queue_number);
        lcd->setCursor(0, 1);
        lcd->print(timestamp);
      } else {
        lcd->clear();
        lcd->setCursor(0, 1);
        lcd->print("Dequeue failed! ");
      }

      delay(1000);
      break;
      
    case dequeue_all:
      lcd->setCursor(0, 0);
      lcd->print("Dequeuing All...");
      delay(500);
      lcd->clear();
      lcd->setCursor(0, 0);
      if(DequeueAll(queue_array[queue_number]) == EXIT_OK) {  
        lcd->print("All nodes in ");
        lcd->print(queue_number);
        lcd->setCursor(0, 1);
        lcd->print("dequeued. ");
      } else {
        lcd->print("Dequeue failed!"); 
      }
      
      delay(1000);
      break;
  }
  return EXIT_OK;
}

DequeueOption DequeueMenu(LiquidCrystal *lcd) {
  DequeueOption menu_option = dequeue_one;

  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Dequeue One/All?");
  lcd->setCursor(0, 1);
  lcd->print("Dequeue One");

  while (1) {
    Button button = ButtonCheck();
    if (button != none) {
      if (button != select) {
        if (menu_option == dequeue_all) menu_option = dequeue_one;
        else if (menu_option == dequeue_one) menu_option = dequeue_all;
      } else {
        // select pressed
        delay(250);
        return menu_option;
      }

      // printing
      lcd->setCursor(0, 1);
      switch (menu_option) {
        case dequeue_one:
          lcd->print("Dequeue One");
          break;
        case dequeue_all:
          lcd->print("Dequeue All");
          break;
      }
      delay(250);
    }
  }
}

void PrintItem(LiquidCrystal *lcd, pStampedItem item) {          
  lcd->print(item->timestamp);
  lcd->print(" ->");
  if (item->next == NULL) lcd->print(" END");
}

void Print(LiquidCrystal *lcd, pQueue queue_array[]) {
  for(int i=0; i<NO_OF_QUEUES; i++) {  
    //write out the queue #
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Showing Queue ");
    lcd->print(i);
    lcd->setCursor(0, 1);
    lcd->print("Press Select");

    //waiting to press select
    while(true){
      if(ButtonCheck() == select){
        lcd->clear();
        lcd->setCursor(0, 0);
        delay(250);
        break;
      }
    }

    pStampedItem temp_node = queue_array[i]->front;
    
    if(temp_node == NULL) {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("<EMPTY LIST>");
      lcd->setCursor(0, 1);
      lcd->print("Press Select");
      while (ButtonCheck() != select){}
      delay(250);
      continue;
    }
    
    //proceed through the printing process
    while(temp_node) {
      lcd->clear();
      lcd->setCursor(0, 0);   
      PrintItem(lcd, temp_node);
      temp_node = temp_node->next;
      lcd->setCursor(0, 1);
      lcd->print("Press Select");
      while (ButtonCheck() != select){}
      delay(250);
    }
    
    lcd->setCursor(0, 1);
    lcd->print("Press Select");
    while (ButtonCheck() != select){}
    delay(250);    
  }
}
