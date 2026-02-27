#include "lab3_bst.h"

#if !defined(_WIN32) && !defined(_WIN64)
	#include <unistd.h>
	#ifndef Sleep
		#define Sleep(ms) usleep((ms) * 1000)
	#endif
#endif

#if defined(_WIN32) || defined(_WIN64)
	#define COMPORT "COM3" // adjust as needed
#elif defined(__APPLE__) && defined(__MACH__)
	#define COMPORT "/dev/cu.usbmodem1101" // adjust as needed
#endif

#define NODEADDED 10
#define DATAREADGAP 1000

// The lab requires that you implement 3 functions:
//	int height (Node *);
//	Node* findParentHelper (Key, Node*);
//	delete (Node*, Node*);

#define	HEIGHT_WRITTEN	0
#define	FINDPARENTHELPER_WRITTEN	0
#define	DELETE_WRITTEN	0
//
// *** YOU SHOULD NOT MAKE ANY OTHER CHANGES TO THIS CODE ***

int main(void)
{
	Node	*n = NULL;		// eliminates compiler warning
	int v1 = 1;
	Tree *t = initTree (50,&v1);//the tree will be starting with a initial node of 50

	
	printf("Connecting to serial port ");
	printf(COMPORT);
	printf("\n");

	Serial(COMPORT);    
	if (IsConnected())
	printf("We're connected\n\n");

	//keeping an array to keep track of the node values that have been stored
	int numArray[NODEADDED];

	for(int i=0;i<NODEADDED;i++){
		numArray[i]=DataRead();
		insert(numArray[i],&v1,t->root);
		Sleep(DATAREADGAP);
	}

	printf("\n\n");
	
	printf("Original Tree: \n");
	printTree(t->root);
	printf("\n\n");

	if (HEIGHT_WRITTEN == 1)	{
		printf("Height of tree: %d\n\n", _height(t->root));
		}

	if (FINDPARENTHELPER_WRITTEN == 1)	{
		printf("Parent of %d: %d\n",numArray[3], findParent(numArray[3],t->root)->key);
		printf("Parent of %d: %d\n",numArray[6], findParent(numArray[6],t->root)->key);
		printf("Parent of %d: %d\n\n",numArray[8], findParent(numArray[8],t->root)->key);
		}

	if (DELETE_WRITTEN == 1)	{
		printf ("Tree after deleting %d:\n",numArray[3]);
		withdraw (numArray[3],t->root,&n);
		printTree (t->root);
		printf ("\n\n");
		//free (n);
		printf("Tree after deleting %d:\n",numArray[6]);
		withdraw(numArray[6],t->root,&n);
		printTree(t->root);
		printf("\n\n");
		//free(n);

	    printf("Tree after deleting %d:\n",numArray[8]);
		withdraw(numArray[8],t->root,&n);
		printTree(t->root);
		printf("\n");
		//free(n);
		}
	
	return 0;
}//main()
