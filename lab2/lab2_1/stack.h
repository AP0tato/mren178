  /**
  * @brief The Stack structure and its functions for Stack Problem in Lab 2
  * A stack can be implmeneted using both Array and Linked-list, for this 
  * code, we are using Linked-List implementation, but do try Array one as 
  * well if you are interested!
  * 
  */

  #ifndef STACK_H_
  #define STACK_H_

  #include <Arduino.h>
  #include <Wire.h>
  #include <DS3231.h>

  #define NO_OF_STACKS  4 // number of queues to create
  #define MAX_STACK_LEN 10 // maximum legnth of a stack

  #define EXIT_OK       0
  #define EXIT_ERR     -1

  /////////////////////////////////////STRUCTS////////////////////////////////////

  // Item structure
  typedef struct item	{
    long timestamp;
    char data;
  } StampedItem;

  typedef struct stack {
    StampedItem stack_of_items[MAX_STACK_LEN];
    int top;
  } Stack, *pStack;

  ///////////////////////////////FUNCTION PROTOTYPES//////////////////////////////

  // fills an array (of size num_stacks) with Stacks 
  void CreateStackArray(Stack stack_array[], int num_stacks);

  // creates a new StampedItem from timestam and data input parameters, and pushes
  // it on the top of a stack. Returns EXIT_OK if it succeeds, and EXIT_ERR if not.
  int Push(pStack stack, long timestamp, char data);

  // pops top item from a stack, returns EXIT_OK if it succeeds, and EXIT_ERR if 
  // it does not. The popped item's timestamp and char data are stored in passed
  // by reference input parameters
  int Pop(pStack stack, long& timestamp, char& data);

  // pops all items from a stack
  void PopAll(pStack stack);

  // if stack is empty return true, otherwise, return false 
  bool IsStackEmpty (pStack stack);

  // if stack is full return true, otherwise, return false 
  bool IsStackFull (pStack stack);

  // returns the current time from DS3231 module
  long GetTime(DS3231 rtc_clock);
  #endif
