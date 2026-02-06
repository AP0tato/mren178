/******************************************************************************
FILENAME:   queue.cpp 
COURSE:     MREN 178
LAB:        2 

STUDENT:   [YOUR NAME HERE]         STUDENT ID: [YOUR STUDENT ID]  
PARTNER:   [PARTNER's NAME HERE]    

OTHER CONTRIBUTORS: [AS PART OF ACADEMIC INTEGRITY POLICY, ANY CONTRIBUTIONS
OBTAINED FROM ADDITIONAL SOURCES MUST BE LISTED. THESE INCLUDE WEBSITES, OTHER
STUDENTS, ETC.]
******************************************************************************/

#include "queue.h"

pQueue CreateQueue (void) {
  pQueue pnew = (pQueue)malloc(sizeof (struct queue));
  if (pnew != NULL) {
    pnew->front = NULL;
    pnew->back =  NULL;
    pnew->count = 0;
  }
  return pnew;
}

int CreateQueueArray(pQueue queue_array[], int num_queues){
  for (int i=0; i<num_queues; i++) {
    queue_array[i] = CreateQueue();
    if (queue_array[i] == NULL) {
      //fprintf (stderr, "ERROR: Failed to create queue %d\n", i);
      Serial.print("ERROR: Failed to create queue ");
      Serial.print(i);
      Serial.println(".");
      return EXIT_ERR;
    }
  }
  return EXIT_OK;
}

// Code to create new item and initialize value field
pStampedItem CreateItem (long timestamp) {
	pStampedItem pnew = (pStampedItem) malloc(sizeof(StampedItem));
	if (pnew != NULL)	{
		pnew->timestamp = timestamp;
    pnew->next = NULL;
	} else {
    //fprintf (stderr, "ERROR: Failed to create queue %d\n", i);
    Serial.print("ERROR: Failed to create new item ");
    Serial.print(timestamp);
    Serial.println(".");
  }
	return pnew;
}

bool IsQEmpty (pQueue queue) {
  /*-------------------------insert your code here--------------------------*/

  // this return value may need to be changed once your code is written
  return true;
}

int Enqueue (pQueue queue, pStampedItem item) {
  /*-------------------------insert your code here--------------------------*/

  // this return value may need to be changed once your code is written
  return EXIT_ERR;
}

int Dequeue (pQueue queue, long &timestamp) {
  /*-------------------------insert your code here--------------------------*/

  // this return value may need to be changed once your code is written
  return EXIT_ERR;
}

int DequeueAll (pQueue queue) {
  /*-------------------------insert your code here--------------------------*/

  // this return value may need to be changed once your code is written
  return EXIT_ERR; 
}

long GetTime(DS3231 rtc_clock) {
  /*-------------------------insert your code here--------------------------*/

  // this return value may need to be changed once your code is written
  return 0;
}
