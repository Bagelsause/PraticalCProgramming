#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "llist.h"

// Precondition: len > 0
node *list_from_array(const int *values, size_t len) {
    node *head = node_make(values[0]);

    node *cur_node = head;
    for (size_t i = 1; i < len; i++) {
        cur_node = list_insert(cur_node, values[i]);
    }

    return head;
}

#define list_from_imm(...) ({ \
    int vals[] = {__VA_ARGS__}; \
    size_t len = sizeof(vals) / sizeof(int); \
    list_from_array(vals, len); })

bool node_sanity_check(const node *n) {
    bool sane = true;

    if(n->prev && n->prev->next != n) {
        printf("[node @ %p (value: %d)] is not its prev node's next!\n", n, n->value);
        sane = false;
    }

    if(n->next && n->next->prev != n) {
        printf("[node @ %p (value %d)] is not its next node's prev!\n", n, n->value);
        sane = false;
    }

    return sane;
}

bool list_sanity_check(const node *head) {
    bool sane = true;

    if(head->prev != NULL) {
        printf("[node @ %p (value %d)] purports to be the head, but has a prev!\n", head, head->value);
        sane = false;
    }

    const node *n = head->next;
    while(n != NULL) {
        sane = sane && node_sanity_check(n);
        n = n->next;
    }

    return sane;
}


#define assert_msg(expr, msg) do { \
        typeof(expr) res = (expr); \
        if(!res) { \
            puts("💥 Test failed: " msg "\n"); \
            puts("The expression that failed is:\n" #expr); \
            printf("(line %d in %s in %s)\n", __LINE__, __FUNCTION__, __FILE__); \
            exit(1); \
        } \
    } while(0)

#define assert_sane(head, ctx) assert_msg(list_sanity_check(head), "list is not linked correctly after " ctx " - see details above")

#define pass(msg) puts("👍 " msg " looks good!")


void test_node_make() {
    node *head = node_make(5);
    assert_msg(
        head != NULL && head->value == 5 && head->next == NULL && head->prev == NULL,
        "node_make should make a standalone node with the given value");
    
    free(head);

    pass("node_make");
}

void test_insert_at_end() {
    node *head = node_make(1);
    node *new_node = list_insert(head, 2);
    
    assert_msg(new_node != NULL && new_node->value == 2,
        "list_insert should return the newly created node");
    
    assert_msg(head->next == new_node && new_node->prev == head && new_node->next == NULL,
        "list_insert should correctly insert a node at the end of the list");
    
    assert_sane(head, "list_insert at end");

    free(head);
    free(new_node);

    pass("list_insert at the end of the list");
}

void test_insert_in_middle() {
    node *head = node_make(1);

    // We've already tested this use of list_insert, so we know it works
    node *three = list_insert(head, 3);

    // This the new use case we want to test, since head is now followed by something
    node *two = list_insert(head, 2);

    assert_msg(head->next == two && two->next == three,
        "list_insert should correctly insert a node in the middle of the list");
    
    assert_sane(head, "list_insert in middle");

    free(head);
    free(two);
    free(three);

    pass("list_insert in the middle of the list");
}

void test_find() {
    // Since we've tested list_insert now, we can use things that
    // rely on list_from_array, like this macro:
    node *head = list_from_imm(1, 2, 3, 3, 4);
    node *two = head->next;
    node *three = two->next;
    node *second_three = three->next;
    node *four = second_three->next;

    assert_msg(list_find(head, 1) == head,
        "list_find should return the head node if it matches");
    
    assert_msg(list_find(head, 2) == two,
        "list_find should return a matching node in the list");
    
    assert_msg(list_find(head, 3) == three,
        "list_find should return the *first* node with a matching value");

    assert_msg(list_find(head, 4) == four,
        "list_find should return the tail node if it matches");
    
    assert_msg(list_find(head, 5) == NULL,
        "list_find should return NULL if no matching node is found");
    
    assert_sane(head, "list_find");

    free(head);
    free(two);
    free(three);
    free(second_three);
    free(four);

    pass("list_find");
}

void test_remove_head() {
    node *head = list_from_imm(1, 2);
    node *two = head->next;

    list_remove(head);
    assert_msg(two->prev == NULL,
        "calling list_remove on the head should set the next node's prev to NULL");
    
    assert_sane(two, "list_remove of head node");

    // list_remove should also have freed head,
    // but we have no portable way to check that

    free(two);

    pass("list_remove on the head of a list");
}

void test_remove_tail() {
    node *head = list_from_imm(1, 2);
    node *two = head->next;

    list_remove(two);
    assert_msg(head->next == NULL,
        "calling list_remove on the final node should behave appropriately");
    
    assert_sane(head, "list_remove of final node");

    free(head);

    pass("list_remove on the final node of a list");
}

void test_remove_in_middle() {
    node *head = list_from_imm(1, 2, 0, 3);
    node *two = head->next;
    node *outlier = two->next;
    node *three = outlier->next;

    list_remove(outlier);
    assert_msg(two->next == three && three->prev == two,
        "list_remove should correctly remove a node in the middle of a list");
    
    assert_sane(head, "list_remove in middle");

    free(head);
    free(two);
    free(three);

    pass("list_remove in the middle of a list");
}


int main(int argc, char *argv[]) {
    test_node_make();
    test_insert_at_end();
    test_insert_in_middle();
    test_find();
    test_remove_head();
    test_remove_tail();
    test_remove_in_middle();

    puts("\n🎉 YOU DID IT 🎉\n");

    puts("... Well, probably!");
    puts("It's hard to test that you freed things appropriately.");
    puts("Make sure your list_remove and list_free do the right");
    puts("thing before you submit.");

    return 0;
}