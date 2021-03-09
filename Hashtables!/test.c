#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hashtable.h"

static bool show_individual_tests = false;


#define assert_msg(expr, msg) do { \
        typeof(expr) res = (expr); \
        if(!res) { \
            puts("💥 Test failed: " msg "\n"); \
            puts("The expression that failed is:\n" #expr); \
            printf("(line %d in %s in %s)\n", __LINE__, __FUNCTION__, __FILE__); \
            exit(1); \
        } \
        else if(show_individual_tests) { \
            puts("✅ " msg); \
        } \
    } while(0)

#define pass(msg) puts("👍 " msg " looks good!")


void test_ht_make() {
    hashtable *ht = ht_make(100);
    assert_msg(ht != NULL,
        "ht_make should return a non-NULL value");

    assert_msg(ht_len(ht) == 0,
        "ht_make should return a table with a length of 0");
    
    ht_free(ht);
    pass("ht_make");
}

void test_simple_sets_and_gets() {
    hashtable *ht = ht_make(100);
    
    ht_set(ht, "Bob", 5);

    assert_msg(ht_len(ht) == 1,
        "adding a new key should increment the length of the table");


    int bob_value = 0;
    bool get_res = ht_get(ht, "Bob", &bob_value);

    assert_msg(get_res == true,
        "ht_get should return true for a key that is in the table");
    
    assert_msg(bob_value == 5,
        "ht_get should return the correct value for a key");
    

    ht_set(ht, "Bob", 6);
    get_res = ht_get(ht, "Bob", &bob_value);

    assert_msg(get_res == true && bob_value == 6,
        "setting a value for a pre-existing key should overwrite its value");
    
    assert_msg(ht_len(ht) == 1,
        "changing the value for a pre-existing key should not change the table's length");
    
    ht_free(ht);
    pass("simple getting and setting");
}

void test_multiple_keys() {
    hashtable *ht = ht_make(100);

    ht_set(ht, "Bob", 5);
    ht_set(ht, "Jane", 10);
    ht_set(ht, "Lane", 20);

    assert_msg(ht_len(ht) == 3,
        "adding multiple keys should update the length correctly");
    
    int b_val = 0, j_val = 0, l_val = 0;
    bool b_get = ht_get(ht, "Bob", &b_val);
    bool j_get = ht_get(ht, "Jane", &j_val);
    bool l_get = ht_get(ht, "Lane", &l_val);

    assert_msg(
        b_get && j_get && l_get &&
        b_val == 5 && j_val == 10 && l_val == 20,
        "getting should work when there are multiple keys");
    
    ht_free(ht);
    pass("getting/setting multiple keys");
}

void test_missing_key() {
    hashtable *ht = ht_make(100);

    ht_set(ht, "Bob", 5);
    ht_set(ht, "Jane", 10);
    ht_set(ht, "Lane", 20);

    int val = 0;
    bool get_res = ht_get(ht, "Joan", &val);

    assert_msg(get_res == false,
        "ht_get should return false for a key not in the table");
    
    ht_free(ht);
    pass("getting missing keys");
}

void test_simple_remove() {
    hashtable *ht = ht_make(100);

    ht_set(ht, "Bob", 5);
    ht_set(ht, "Jane", 10);
    ht_set(ht, "Lane", 20);

    bool rem_res = ht_remove(ht, "Bob");
    assert_msg(rem_res == true,
        "ht_remove should return true when removing a known key");
    
    assert_msg(ht_len(ht) == 2,
        "ht_remove should update the table length if it removes a key");
    
    rem_res = ht_remove(ht, "Ian");
    assert_msg(rem_res == false,
        "ht_remove should return false when asked to remove a key not in the table");
    
    assert_msg(ht_len(ht) == 2,
        "ht_remove should not change the length unless it removes a key");
    
    int val = 0;
    bool get_res = ht_get(ht, "Bob", &val);

    assert_msg(get_res == false,
        "ht_remove should actually remove the entry for a key");
    
    ht_free(ht);
    pass("simple removal");
}

void test_remove_all() {
    hashtable *ht = ht_make(100);

    ht_set(ht, "Bob", 5);
    ht_set(ht, "Jane", 10);
    ht_set(ht, "Lane", 20);

    ht_remove(ht, "Bob");
    ht_remove(ht, "Jane");
    ht_remove(ht, "Lane");

    assert_msg(ht_len(ht) == 0,
        "table length should be zero after all elements removed");

    int b_val = 0, j_val = 0, l_val = 0;
    bool b_get = ht_get(ht, "Bob", &b_val);
    bool j_get = ht_get(ht, "Jane", &j_val);
    bool l_get = ht_get(ht, "Lane", &l_val);

    assert_msg(!b_get && !j_get && !l_get,
        "no keys should remain in the table after all are removed");
    
    ht_free(ht);
    pass("removing all elements");
}

void test_collision_gets_and_sets() {
    hashtable *ht = ht_make(2);

    // Only using 2 buckets so we get collisions galore.
    // bucket 0: Bob (hash 275502), Bo (hash 8884)
    // bucket 1: Joe (hash 283193), Anne (hash 8510283)

    ht_set(ht, "Bob", 1);
    ht_set(ht, "Bo", 2);

    assert_msg(ht_len(ht) == 2,
        "length should be updated correctly with collisions");
    
    int bob_val = 0, bo_val = 0;
    bool bob_get = ht_get(ht, "Bob", &bob_val);
    bool bo_get = ht_get(ht, "Bo", &bo_val);

    assert_msg(bob_get && bo_get && bob_val == 1 && bo_val == 2,
        "keys with colliding hashes should be independent of one another");
    
    // just to be safe...
    ht_set(ht, "Bob", 3);
    bob_get = ht_get(ht, "Bob", &bob_val);
    bo_get = ht_get(ht, "Bo", &bo_val);
    assert_msg(bob_get && bo_get && bob_val == 3 && bo_val == 2,
        "updating values for keys with collisions should not effect other keys");

    ht_free(ht);
    pass("getting and setting with collisions");
}

void test_collision_removals() {
    hashtable *ht = ht_make(2);

    ht_set(ht, "Bob", 1);
    ht_set(ht, "Bo", 2);

    bool rem_res = ht_remove(ht, "Bob");
    assert_msg(rem_res == true,
        "ht_remove should return true when removing keys with collisions");
    
    assert_msg(ht_len(ht) == 1,
        "length should be updated correctly when removing keys with collisions");
    
    int bo_val = 0, bob_val = 0;
    bool bo_get = ht_get(ht, "Bo", &bo_val);
    bool bob_get = ht_get(ht, "Bob", &bob_val);

    assert_msg(bo_get == true && bo_val == 2,
        "removing a key should not effect other keys with the same hash");
    
    assert_msg(bob_get == false,
        "removing a key that collides with others should actually remove the given key");
    
    ht_free(ht);
    pass("removal with collisions");
}

void test_crazy_collisions() {
    hashtable *ht = ht_make(1);

    // Throw the letters a - z in the table, with
    // values equal to their ASCII value.
    char c[2] = "a\0";
    while(c[0] <= 'z') {
        ht_set(ht, c, (int)c[0]);
        c[0]++;
    }

    assert_msg(ht_len(ht) == 26,
        "length is correctly updated with many collisions");
    
    // Read them all back and check that they're correct
    c[0] = 'a';
    while(c[0] <= 'z') {
        int val = 0;
        bool get_res = ht_get(ht, c, &val);

        assert_msg(get_res == true && val == c[0],
            "keys and values are mapped correctly with many collisions");

        c[0]++;
    }

    // Update the values to be the ASCII code + 1,
    // and check that they read back properly.
    c[0] = 'a';
    while(c[0] <= 'z') {
        ht_set(ht, c, c[0] + 1);
        
        int val = 0;
        bool get_res = ht_get(ht, c, &val);

        assert_msg(get_res == true && val == c[0] + 1,
            "updating the value for pre-existing keys works with many collisions");
        
        c[0]++;
    }

    // Remove everything and check that gets fail.
    c[0] = 'a';
    while(c[0] <= 'z') {
        int val = 0;
        bool rem_res = ht_remove(ht, c);
        bool get_res = ht_get(ht, c, &val);
        assert_msg(rem_res == true && get_res == false,
            "removal works correctly with many collisions");
        
        c[0]++;
    }

    assert_msg(ht_len(ht) == 0,
        "removing all elements sets length to zero with many collisions");

    ht_free(ht);
    pass("behavior under tons of collision");
}


int main(int argc, char *argv[]) {
    if(argc == 2 && strcmp(argv[1], "-v") == 0) {
        show_individual_tests = true;
    }

    test_ht_make();
    test_simple_sets_and_gets();
    test_multiple_keys();
    test_missing_key();
    test_simple_remove();
    test_remove_all();
    test_collision_gets_and_sets();
    test_collision_removals();
    test_crazy_collisions();

    puts("\n🎉 YOU DID IT 🎉\n");

    puts("... Well, probably!");
    puts("It's hard to test that you freed things appropriately.");
    puts("Make sure your ht_free does the right thing before you submit.");

    return 0;
}