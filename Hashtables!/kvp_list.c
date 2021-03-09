#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "kvp_list.h"

kvp_node *kvp_node_make(const char *key, int value) {
    kvp_node *n = malloc(sizeof(kvp_node));
    // copying the key in case its memory is in use elsewhere:
    n->key = strdup(key);
    n->value = value;
    n->next = n->prev = NULL;
    
    return n;
}

void kvp_node_free(kvp_node *node) {
    free(node->key);
    free(node);
}

// Precondition: prev_node != NULL
kvp_node *kvp_list_insert(kvp_node *prev_node, const char *key, int value) {
    kvp_node *n = kvp_node_make(key, value);
    
    n->prev = prev_node;
    n->next = prev_node->next;
    
    if(n->next) {
        n->next->prev = n;
    }
    
    prev_node->next = n;
    
    return n;
}

// Precondition: node != NULL
void kvp_list_remove(kvp_node *node) {
    if(node->prev) {
        node->prev->next = node->next;
    }
    
    if(node->next) {
        node->next->prev = node->prev;
    }

    kvp_node_free(node);
}

kvp_node *kvp_list_find(kvp_node *head, const char *key) {
    kvp_node *n = head;
    while(n != NULL) {
        if(strcmp(n->key, key) == 0) {
            return n;
        }
        
        n = n->next;
    }
    
    return NULL;
}

// Precondition: head != NULL
void kvp_list_print(const kvp_node *head) {
    const kvp_node *n = head;
    while(n != NULL) {
        printf("('%s': %d)->", n->key, n->value);
        n = n->next;
    }
    printf("END\n");
}

// Precondition: head != NULL
void kvp_list_free(kvp_node *head) {
    kvp_node *n = head;
    while(n->next != NULL) {
        kvp_node *temp = n;
        n = n->next;
        kvp_node_free(temp);
    }

    kvp_node_free(n);
}
