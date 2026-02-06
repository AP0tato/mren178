#ifndef QUEUE_H_
#define QUEUE_H_

#include <Arduino.h>
#include <Wire.h>
#include <DS3231.h>

#define NO_OF_QUEUES 4 // number of queues to create

#define EXIT_OK      0
#define EXIT_ERR    -1

/////////////////////////////////////STRUCTS////////////////////////////////////

// Node structure
typedef struct item	{
	struct item	*next;
	long timestamp;
} StampedItem, *pStampedItem;

// Queue structure
typedef struct queue{
	pStampedItem front;	// Pointer to front queue element
	pStampedItem back;	// Pointer to back queue element
	int		count;		    // Number of items in queue
} Queue, *pQueue;

///////////////////////////////FUNCTION PROTOTYPES//////////////////////////////

// creates and returns pointer to a queue 
pQueue CreateQueue (void);

// creates an array[NO_OF_QUEUES] of pointers to queues
// returns EXIT_OK if successful, EXIT_ERR otherwise
int CreateQueueArray(pQueue queue_array[], int num_queues);

// creates and returns a pointer to a StampedItem, taking in a (timestamp) value.
pStampedItem CreateItem (long timestamp);

//checks to see if queue is empty
bool IsQEmpty (pQueue queue);

// enqueues an item into queue, returning EXIT_OK if succeeding, and EXIT_ERR if not.
int Enqueue (pQueue queue, pStampedItem item);

// dequeues an item from the queue, returning EXIT_OK if succeeding, and 
// EXIT_ERR if not.
int Dequeue (pQueue queue, long &timestamp);

// dequques all items from a queue, returning EXIT_OK if succeeding, and 
// EXIT_ERR if not.
int DequeueAll (pQueue queue);

// returns the current time from DS3231 module
long GetTime(DS3231 rtc_clock);

#endif
