#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0


typedef struct Node {
    int id;
    int password;
    struct Node* left;
    struct Node* right;
} Node;

Node *head = NULL;


int nodeManagement(int userID, int userPassword)
{

    // CREATE NODE

    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return FALSE;
    }

    // SET NODE VALUES
    
    newNode->id = userID;
    newNode->password = userPassword;
    newNode->left = NULL;
    newNode->right = NULL;

    // SORT NODE

    if (head == NULL) head = newNode; return TRUE;

    Node *traverser = head;

    while (TRUE)
    {
        // Finds where the new node should be placed in the tree 
        if (userID < traverser->id && traverser->left != NULL) traverser = traverser->left;
        else if (userID > traverser->id && traverser->right != NULL) traverser = traverser->right;
        else break; 
    }

    // Puts the new node in the tree in the correct place, or updates the password if the userID already exists
    if (userID < traverser->id) traverser->left = newNode;
    else if (userID > traverser->id) traverser->right = newNode;
    else if (userID == traverser->id) traverser->password = userPassword;
    return TRUE;

}

int main() 
{

    

    return TRUE;
}