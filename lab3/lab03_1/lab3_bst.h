#include <stdlib.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)
	#include <tchar.h>
#endif

#include "SerialClass.h"	
#include <string.h>

typedef int Key;

#define	BST_FAIL	0		// return value when BST function fails
#define	BST_SUCCESS	1		// return value when BST function succeeds

// Node in tree has key and pointer to value associated with key.
// Also contains structural components - two pointers to left and
// right subtrees.
typedef struct Node	{
	Key				key;
	void			*value;
	struct Node		*leftChild, *rightChild;
} Node, pNode;

// Tree is basically pointer to top node in a tree.
typedef struct Tree {
	Node *root;
} Tree;


// Prototypes for functions found in BST.C

// Create new node with key = k and value = v 
Node *initNode (Key k, void *v);

// Create new tree by creating new node with key = k and value = v
// and making it root 
Tree *initTree (Key k, void *v);

// Find node with key k in tree. Returns pointer to Node if found;
// Returns NULL if not found
Node *find (Key k, Node *root);

// Create new node with key=k, value=v and insert it into tree 
// Returns 1 upon success, 0 failure 
int insert (Key k, void *v, Node *root);

// Print text representation of tree (starting at any Node)
void printTree (Node* root);

// Returns Maximum of two integer numbers 
int intmax(int a, int b);

// Find parent of node n where n->key = k
// Returns pointer to parent node if found; Returns NULL if not found
Node* findParent(Key k, Node* root);

// 1. Make copy of node with key=k and returns it
// 2. Delete node with key=k from tree
// Return pointer of node created in 1; Returns NULL if no node
// with specified key value is found
int withdraw(Key k, Node *root, Node **n);

//******************************************************************
//************ FUNCTIONS TO BE IMPLEMENTED BY THE STUDENT **********
//******************************************************************

// Return height of tree (height of specified root)
int _height(Node *root);

// Helper function for findParent - see specification in lab
// instructions
Node* _findParentHelper(Key k, Node* root);

// Delete node from tree while ensuring tree remains valid
void _delete (Node* p, Node *n);
