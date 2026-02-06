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
  return queue->count==0;
}

int Enqueue (pQueue queue, pStampedItem item) {
  /*-------------------------insert your code here--------------------------*/
  if(queue->count == 0)
    queue->back = queue->front = item;
  else
    queue->back = queue->back->next = item;
  queue->count++;
  // this return value may need to be changed once your code is written
  return EXIT_OK;
}

int Dequeue (pQueue queue, long &timestamp) {
  /*-------------------------insert your code here--------------------------*/
  if(IsQEmpty(queue))
    return EXIT_ERR;
  pStampedItem ptr = queue->front;
  timestamp = ptr->timestamp;
  queue->front = queue->front->next;
  if(!queue->front)
    queue->back = NULL;
  free(ptr);
  queue->count--;
  // this return value may need to be changed once your code is written
  return EXIT_OK;
}

int DequeueAll (pQueue queue) {
  /*-------------------------insert your code here--------------------------*/
  if(IsQEmpty(queue))
    return EXIT_ERR;
  // this return value may need to be changed once your code is written
  while(queue->front)
  {
    pStampedItem ptr = queue->front;
    queue->front = queue->front->next;
    free(ptr);
  }
  queue->count = 0;
  queue->back = NULL;
  return EXIT_OK; 
}

long GetTime(DS3231 rtc_clock) {
  /*-------------------------insert your code here--------------------------*/
  bool h12 = false, am = false;
  // this return value may need to be changed once your code is written
  return ((long)rtc_clock.getHour(h12, am)*3600)+rtc_clock.getMinute()*60+rtc_clock.getSecond();
}
