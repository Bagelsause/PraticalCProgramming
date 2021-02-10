#include <stdio.h>
#include <stdlib.h>
#include "llist.h"

node *node_make(int value) {
    node *n = malloc(sizeof(node));
    n->value = value;
    n->next = n->prev = NULL;
    
    return n;
}

node *list_insert(node *prev_node, int value) {
	node *currentNode = node_make(value);
	currentNode->prev = prev_node;
	node *nextNode;
	
	if (prev_node->next){
		nextNode = prev_node->next;
		prev_node->next = currentNode;
		nextNode->prev = currentNode;
		currentNode->next = nextNode;
	} else {
		//printf("End of the list! Adding onto end of the list"); <- debug purposes lmao
		prev_node->next = currentNode;
		currentNode->next = NULL;
	}
	
	
	return currentNode;
}

void list_print(const node *head) {
	node *currentNode = head;
	
	while (currentNode) {
		printf("|%d|→", currentNode->value);
		currentNode = currentNode->next;
	}
	
	printf("END \n");
}

node *list_find(node *head, int value){
	node *currentNode = head;
	
	while (currentNode) {
		if (currentNode->value == value) {
			return currentNode;
		}
		currentNode = currentNode->next;
	}
	
	return NULL;
	
}

void list_remove(node *n) {
	node *prevNode = n->prev;
	node *nextNode = n->next;
	printf("setting");
	
	if (prevNode){ //I completely forgot abt these, took me like 2 hours to debug it out
		if (nextNode){ //My two braincells might not be the best workers, but smart enough to figure it out eventually
			prevNode->next = nextNode;
			nextNode->prev = prevNode;
			printf("changing");
		} else {
			prevNode->next = NULL;
		}
	} else {
		nextNode->prev = NULL;
	}
	free(n);
	printf("freeing");
}

void list_free(node *head){
	node *currentNode = head;
	while (currentNode->next != NULL) {
		currentNode = currentNode->next;
	}
	
	while(currentNode->prev != NULL) {
		free(currentNode->next);
		//printf("Free'd one value");
		currentNode = currentNode->prev; //turns out this line is pretty important, got in a few inf loops with this one
	}
	
	free(currentNode);
}