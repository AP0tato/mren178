#include "board_definition.h"

#define BUFFER_SIZE 10
char buf[BUFFER_SIZE];

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

// Returns selected menu option - 1: push 2: display stack 3: pop  
int Menu(LiquidCrystal *lcd) {
  int menu_option = 1; 

  //Displays the initial menu
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Select Action");
  lcd->setCursor(12, 1);
  lcd->print("MENU");
  
  lcd->setCursor(0, 1);
  lcd->print("Push       "); // 11 chars

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
            lcd->print("Push       ");
            break;
        case 2:
            lcd->print("Display    ");
            break;
        case 3:
            lcd->print("Pop        ");
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

int StackSelect(LiquidCrystal *lcd) {
  int stack_number = 0;

  lcd->setCursor(0, 1);
  lcd->print("Which Stack?    ");
  lcd->setCursor(14, 1);
  lcd->print(stack_number);
  Button button;
  while (1) {
    lcd->setCursor(0, 1);
    button = ButtonCheck();
    if ( button != none ) {
      if ( button == right || button == up) 
        stack_number = ++stack_number % NO_OF_STACKS;
      else if ( button == left || button == down){
        if(--stack_number < 0) stack_number = NO_OF_STACKS - 1;
      } else if ( button == select ) {
        delay(250);
        return stack_number;
      }
  
      // print menu choices on Row 2
      lcd->setCursor(14, 1);
      lcd->print(stack_number);
      
      delay(250);
    }
  }
}

void PushInterface (LiquidCrystal *lcd, DS3231 rtc_clock, 
                        Stack stack_array []) {
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Push Item");

  int stack_number = StackSelect(lcd);

  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("   Waiting on   ");
  lcd->setCursor(0, 1);
  lcd->print(" serial data... ");

  int read_length = 0, output;
  long timestamp;

  while(1) {
    // read in data from serial
    if (Serial.available() > 0) {      
      // read the incoming byte
      read_length = Serial.readBytes(buf, BUFFER_SIZE);
      //if (read_length == 10) break; //to skip the cases for "\n"
      for (int i = 0; i < read_length; i++) {
        // if decimal value of byte is less than 33, disregard since 
        // these are ascii special chars. we only want visible chars
        if (buf[i] < 33) continue; 

        timestamp = GetTime(rtc_clock);
        output = Push(&stack_array[stack_number], timestamp, buf[i]);
        Serial.println(buf[i]);
        lcd->clear();
        lcd->setCursor(0, 0);
        lcd->print("D:");
        lcd->print(buf[i]);
        lcd->print(" T:");
        lcd->print(timestamp);
        lcd->setCursor(0, 1);
       
        if (output == EXIT_OK){
          lcd->print("Pushed in Stk# ");
          lcd->print(stack_number);
          delay(1000);
        }
        else {
          lcd->print("  Push Failed!  ");
          delay(1000);
          break;
        }
        
      } //for
      break;
    } //if(Serial.available()>0)
  } //while(1)
}

int PopInterface(LiquidCrystal *lcd, Stack stack_array[]) {
  long timestamp;
  char data;
  int stack_number = StackSelect(lcd);
  lcd->clear();
  PopOption option = PopMenu(lcd);
  lcd->clear();
  
  switch (option) {
    case pop_one:
      lcd->setCursor(0, 0);
      lcd->print("Pop One Node");

      // performs pop
      if (Pop(&stack_array[stack_number], timestamp, data) == EXIT_OK) {
        lcd->clear();
        lcd->setCursor(0, 1);
        lcd->print("Popped from ");
        lcd->print(stack_number);
        lcd->setCursor(0, 0);
        lcd->print("D:");
        lcd->print(data);
        lcd->print(" T:");
        lcd->print(timestamp);
      } else {
        lcd->clear();
        lcd->setCursor(0, 1);
        lcd->print("Pop failed! ");
      }
      delay(1000);
      break;
      
    case pop_all:
      lcd->setCursor(0, 0);
      lcd->print("Popping All...");
      delay(500);
      lcd->clear();
      lcd->setCursor(0, 0);
      PopAll(&stack_array[stack_number]); 
      lcd->print("All nodes in ");
      lcd->print(stack_number);
      lcd->setCursor(0, 1);
      lcd->print("popped. ");
      delay(1000);
      break;
  }
  return EXIT_OK;
}

PopOption PopMenu(LiquidCrystal *lcd) {
  PopOption menu_option = pop_one;

  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print("Pop One or All? ");
  lcd->setCursor(0, 1);
  lcd->print("Pop One");

  while (1) {
    Button button = ButtonCheck();
    if (button != none) {
      if (button != select) {
        if (menu_option == pop_all) menu_option = pop_one;
        else if (menu_option == pop_one) menu_option = pop_all;
      } else {
        // select pressed
        delay(250);
        return menu_option;
      }

      // printing
      lcd->setCursor(0, 1);
      switch (menu_option) {
        case pop_one:
          lcd->print("Pop One");
          break;
        case pop_all:
          lcd->print("Pop All");
          break;
      }
      delay(250);
    }
  }
}

void Print(LiquidCrystal *lcd, Stack stack_array[]) {
  for(int i=0; i<NO_OF_STACKS; i++) {  
    //write out the stack #
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Showing Stack ");
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

    if(stack_array[i].top == -1) {
      lcd->clear();
      lcd->setCursor(0, 0);
      lcd->print("<EMPTY STACK>");
      lcd->setCursor(0, 1);
      lcd->print("Press Select");
      while (ButtonCheck() != select){}
      delay(250);
      continue;
    }
    
    //proceed through the printing process
    for(int j=0; j <= stack_array[i].top; j++) {
      lcd->clear();
      lcd->setCursor(0, 0); 
      lcd->print("D:");
      lcd->print(stack_array[i].stack_of_items[j].data);
      lcd->print(" T:");
      lcd->print(stack_array[i].stack_of_items[j].timestamp);        
      lcd->setCursor(0, 1);
      if(j == stack_array[i].top) {
        lcd->print("  -------> END  ");
      } else {
        lcd->print("  ----------->  ");
      }
      while (ButtonCheck() != select){}
      delay(250);
    }
    
    lcd->setCursor(0, 1);
    lcd->print("Press Select    ");
    while (ButtonCheck() != select){}
    delay(250);    
  }
}
