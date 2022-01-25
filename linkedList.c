#include "gestor.h"

//Referência: https://cboard.cprogramming.com/c-programming/119467-adding-several-nodes-linked-list-using-loop.html

//Usado para implementar a gestão de memória 

NODO* NewNode(int pid)
{
    NODO* new = malloc(sizeof(NODO));
	
    new->pid = pid;
    new->next = NULL;	
	
    return(new);	
} 

/* LAST NODE */
NODO* LastNode(NODO *head)
{
    NODO *temp = head;
    while (temp != NULL)
	  temp = temp->next;
		
    return(temp);
}

//----------------------------------------------------------//
//Referência: https://www.javatpoint.com/program-to-create-a-singly-linked-list-of-n-nodes-and-count-the-number-of-nodes
//countNodes(): will count the nodes present in the list  
int countNodes(NODO* memMan) {  
    int count = 0;  
    //Node current will point to head   
      
    while(memMan != NULL) {  
        //Increment the count by 1 for each node  
        count++;  
        memMan = memMan->next;  
    }  
    return count;  
}  

//display(): will display all the nodes present in the list  
void display(NODO* memMan) {  
      
    if(memMan== NULL) {  
        printf("List is empty\n");  
        return;  
    }  
    printf("|");
    while(memMan->next != NULL) {  
        //Prints each node by incrementing pointer  
        printf("%d |",memMan->pid);  
        memMan = memMan->next;  
    }  
    printf("\n");  
}
//----------------------------------------------------------// 
//Referência: https://www.geeksforgeeks.org/delete-n-nodes-after-m-nodes-of-a-linked-list/

//Function to skip M nodes and then delete N nodes of the linked list. 
void skipMdeleteN(NODO* memMan, int M, int N) 
{ 
    NODO *curr = memMan, *t; 
    int count; 
  
    // The main loop that traverses through the whole list 
    while (curr) 
    { 
        // Start from next node and delete N nodes 
        t = curr; 
        for (count = 1; count<=N && t!= NULL; count++) 
        { 
            NODO* temp = t; 
            t = t->next; 
            free(temp); 
        } 
        curr->next = t; // Link the previous list with remaining nodes 
  
        // Set current pointer for next iteration 
        curr = t; 
    } 
}