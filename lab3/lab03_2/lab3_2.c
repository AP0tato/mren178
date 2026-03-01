#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define LIMIT 10 // 0 for no limit

typedef struct Node {
    char* password;
    unsigned int id;
    struct Node* left;
    struct Node* right;
} Node;

int is_bst_range(Node *node, unsigned int min, unsigned int max)
{
    if(node == NULL) return TRUE;

    if(node->id <= min || node->id >= max) return FALSE;

    return is_bst_range(node->left, min, node->id) && is_bst_range(node->right, node->id, max);
}

int is_bst(Node *root)
{
    return is_bst_range(root, 0u, UINT32_MAX);
}

int nodeManagement(Node **head, unsigned int userID, char *userPassword)
{
    // CREATE NODE
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL)
    {
        printf("Memory allocation failed\n");
        return 1;
    }

    // SET NODE VALUES
    newNode->id = userID;
    newNode->password = userPassword;
    newNode->left = NULL;
    newNode->right = NULL;

    // SORT NODE
    if (*head == NULL) { *head = newNode; return 0; }

    Node *traverser = *head;

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
    else if (userID == traverser->id) 
    {
        free(traverser->password); 
        traverser->password = userPassword; 
        free(newNode); 
    }
    return 0;
}

void delete(Node **head, unsigned int id)
{
    if(*head == NULL)
        return;

    Node *parent = NULL;
    Node *curr = *head;

    while(curr != NULL && curr->id != id)
    {
        parent = curr;
        if(id < curr->id)
            curr = curr->left;
        else
            curr = curr->right;
    }

    if(curr == NULL)
        return;

    if(parent == NULL)
        parent = curr;

    if(curr->right == NULL && curr->left == NULL)
    {
        if(parent == curr)
        {
            free(curr->password);
            free(curr);
            *head = NULL;
            return;
        }
        else if(parent->right == curr)
            parent->right = NULL;
        else
            parent->left = NULL;
        free(curr->password);
        free(curr);
        return;
    }
    else if(curr->left != NULL && curr->right == NULL)
    {
        if(curr == parent) *head = curr->left;
        else if(parent->left == curr) parent->left = curr->left;
        else parent->right = curr->left;
        free(curr->password);
        free(curr);
        return;
    }
    else if(curr->right != NULL && curr->left == NULL)
    {
        if(curr == parent) *head = curr->right;
        else if(parent->left == curr) parent->left = curr->right;
        else parent->right = curr->right;
        free(curr->password);
        free(curr);
        return;
    }
    else if(curr->right != NULL && curr->left != NULL)
    {
        Node *swap_node = curr->left;
        Node *s_parent = curr;

        while(swap_node->right != NULL)
        {
            s_parent = swap_node;
            swap_node = swap_node->right;
        }

        if(s_parent->right == swap_node)
            s_parent->right = swap_node->left;
        else
            s_parent->left = swap_node->left;

        curr->id = swap_node->id;

        free(curr->password);
        curr->password = swap_node->password;

        free(swap_node);
    }
}

char* search(Node *head, unsigned int id)
{
    Node *curr = head;
    while(curr != NULL && id != curr->id)
    {
        if(id > curr->id)
            curr = curr->right;
        else
            curr = curr->left;
    }
    if(curr != NULL && curr->id == id)
        return curr->password;
    else
        return "Not Found";
}

int main(int argc, char **argv) 
{
    Node *head = NULL;
    // IDENTS.TXT
    char *filename = "IDENTS.TXT";
    FILE *fptr = fopen(filename, "r");
    if (!fptr)
        return -1;

    char buffer[128];

    while(fgets(buffer, 128, fptr))
    {
        char *u = malloc(11);
        char *p = malloc(13);
        
        if(u == NULL || p == NULL)
        {
            printf("Could not allocate memory.");
            return 1;
        }

        for(int i = 0; i < 10; i++)
            u[i] = buffer[i];

        for(int i = 0; i < 12; i++)
            p[i] = buffer[i+14];

        u[10] = '\0';
        p[12] = '\0';

        char *end;
        unsigned long user = strtoul(u, &end, 10);

        nodeManagement(&head, (unsigned int) user, p);

        free(u);
    }
    fclose(fptr);

    printf("%s\n", is_bst(head)?"Valid BST":"Not Valid BST");

    // DELETES.TXT
    filename = "DELETES.TXT";
    fptr = fopen(filename, "r");
    if (!fptr)
        return -1;

    while(fgets(buffer, 128, fptr))
    {
        char *u = malloc(11);
        
        if(u == NULL)
        {
            printf("Could not allocate memory.");
            return 1;
        }

        for(int i = 0; i < 10; i++)
            u[i] = buffer[i];

        u[10] = '\0';

        char *end;
        unsigned long user = strtoul(u, &end, 10);

        delete(&head, (unsigned int) user);

        free(u);
    }
    fclose(fptr);

    // LOOKUPS.TXT
    filename = "LOOKUPS.TXT";
    fptr = fopen(filename, "r");
    if (!fptr)
        return -1;

    while(fgets(buffer, 128, fptr))
    {
        char *u = malloc(11);
        
        if(u == NULL)
        {
            printf("Could not allocate memory.");
            return 1;
        }

        for(int i = 0; i < 10; i++)
            u[i] = buffer[i];

        u[10] = '\0';

        char *end;
        unsigned long user = strtoul(u, &end, 10);

        printf("%010u %s\n", (unsigned int) user, search(head, (unsigned int) user));

        free(u);
    }
    fclose(fptr);
    
    return 0;
}
