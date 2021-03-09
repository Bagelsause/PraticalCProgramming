#ifndef _KVP_LIST_H
#define _KVP_LIST_H

struct kvp_node {
    char *key;
    int value;

    struct kvp_node *prev;
    struct kvp_node *next;
};

typedef struct kvp_node kvp_node;

// Creates a new node on the heap with the given key
// and value and NULL next and prev. The key is copied.
kvp_node *kvp_node_make(const char *key, int value);

// Frees a node and the storage for its key.
// Precondition: node != NULL
void kvp_node_free(kvp_node *node);

// Inserts a new node with the given key and value
// after prev_node. Returns the new node.
// Precondition: prev_node != NULL
kvp_node *kvp_list_insert(kvp_node *prev_node, const char *key, int value);

// Returns the node with a matching key, or NULL
// if no node in the list has the given key.
// Compares keys with strcmp.
kvp_node *kvp_list_find(kvp_node *head, const char *key);

// Removes the give node from list to which it belongs.
// The node is freed.
// Precondition: node != NULL
void kvp_list_remove(kvp_node *node);

// Prints the keys and values of all nodes in the list
// starting at head.
// Precondition: head != NULL
void kvp_list_print(const kvp_node *head);

// Frees all nodes in the list starting at head.
// Precondition: head != NULL
void kvp_list_free(kvp_node *head);

#endif