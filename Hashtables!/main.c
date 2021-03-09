#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

int main(int argc, char *argv[]) {
    hashtable *ht = ht_make(100);
    printf("made a new hashtable! length: %zu\n", ht_len(ht));

    // later...
    
    ht_set(ht, "Bob", 10);
    ht_set(ht, "Jane", 35);

    ht_dump(ht);

    int val = 0;
    ht_get(ht, "Bob", &val);
    printf("Bob -> %d\n", val);

    if(!ht_get(ht, "Joe", &val)) {
        printf("Joe's not in the table.\n");
    }
    else {
        printf("Joe -> %d\n", val);
    }
	
    ht_remove(ht, "Bob");

    ht_free(ht);
    

    return 0;
}