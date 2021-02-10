#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

int main(int argc, char *argv[]) {
    // Try out your code as you're building it here.
    // If you just run "make" in this directory, it
    // will compile this file and make an executable
    // called "main".
    
    // When you've got most of the functions in place,
    // try compiling the tests with "make test" and
    // running them via the executable called "test".

    node *head = node_make(1);
    printf("I made a node @ %p, with value %d!\n", head, head->value);

	
	node *new = list_insert(head, 2);
	printf("It worked?\n");
	
	printf("%p is the previous node, and the new pointer is to %p with a value of %d\n", head, new, head->next->value);
	
	node *new2 = list_insert(new, 7);
	node *new3 = list_insert(new2, 5);
	node *new4 = list_insert(new3, 0);
	node *new5 = list_insert(new4, 4);
	
	list_print(head);
	
	int valueToLookFor = 71;
	
	printf("%d is the value we are looking for, and we found it at %p\n", valueToLookFor, list_find(head, valueToLookFor));
	
	list_remove(new3);
	
	printf("New output \n");
	list_print(head);
	
	list_free(head);
	
    return 0;
}