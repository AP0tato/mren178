#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include <Arduino.h>
/**
 * @brief The node structure used in Lab 1 for linked-list
 * 
 */

#define EXIT_OK         0
#define EXIT_ERROR     -1

struct node {
    struct node *p_next_node; // a pointer pointing to the next node in the list
    int data_val;             // node data - an int ranged 0000 - 9999
};

typedef struct node Node; 

extern Node *p_head;  // pointer to the first node in Linked list
extern Node *p_tail;  // pointer to the last node in Linked list

// Function Prototypes
Node* create_node (int);
int insert_data_at_head (int);
int insert_data_at_middle (int, int);
int insert_data_at_tail (int);
int find_and_delete_data (int);
int delete_all_data ();

#endif 
