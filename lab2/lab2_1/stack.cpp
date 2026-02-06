/******************************************************************************
FILENAME:   stack.cpp 
COURSE:     MREN 178
LAB:        2 

STUDENT:   Ali         STUDENT ID: 20542690  
PARTNER:   Andrés      STUDENT ID: 20529215

OTHER CONTRIBUTORS: [AS PART OF ACADEMIC INTEGRITY POLICY, ANY CONTRIBUTIONS
OBTAINED FROM ADDITIONAL SOURCES MUST BE LISTED. THESE INCLUDE WEBSITES, OTHER
STUDENTS, ETC.]
******************************************************************************/

#include <stdio.h>
#include "Stack.h"

void CreateStackArray(Stack stack_array[], int num_stacks){
  for (int i=0; i<num_stacks; i++) {
    Stack stack; 
    stack.top = -1; 
    stack_array[i] = stack;
  }
}

int Push(pStack stack, long timestamp, char data) {
  
  /*-------------------------insert your code here--------------------------*/
  stack->top++;
  stack->stack_of_items[stack->top] = StampedItem {.timestamp = timestamp, .data = data};
  // this return value will need to be changed once your code is written
  return EXIT_OK;
}

int Pop(pStack stack, long &timestamp, char &data) {
  
  /*-------------------------insert your code here--------------------------*/
  stack->top--;
  // this return value will need to be changed once your code is written
  return EXIT_OK;
}

bool IsStackFull (pStack stack) {
  /*-------------------------insert your code here--------------------------*/
  // this return value will need to be changed once your code is written
  return stack->top==MAX_STACK_LEN-1;
}

bool IsStackEmpty (pStack stack) {
  /*-------------------------insert your code here--------------------------*/
  return stack->top==-1;
}

void PopAll(pStack stack) {
  /*-------------------------insert your code here--------------------------*/
  stack->top = -1;
}

long GetTime(DS3231 rtc_clock) {
  /*-------------------------insert your code here--------------------------*/
  // this return value will need to be changed once your code is written
  bool am = false;
  bool h12 = false;
  long curr_time = rtc_clock.getHour(h12, am)+rtc_clock.getMinute()+rtc_clock.getSecond();
  return curr_time;
}
