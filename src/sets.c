#include <stdlib.h>
#include "sets.h"

unsigned int hash_function(const char * str) {
    unsigned int hash = 2166136261;
    const unsigned int fnv_prime = 16777619;
    for (const char* c = str; *c; c++) {
        hash ^= *c;
        hash *= fnv_prime;
    }
    return hash % MAX_SIZE;
}


Set initialize_set() {
    Set new_set = malloc(sizeof(struct set));
    for (int i = 0; i < MAX_SIZE; i++) 
        new_set->table[i] = NULL;
    
    return new_set;
}


void free_set(Set s) {
    if (!s) return;

    for (int i = 0; i < MAX_SIZE; i++) {
        Entry curr = s->table[i];
        while (curr) {
            Entry next = curr->Next;
            free(curr->key); // strdup'd the key so must free
            free(curr);       
            curr = next;
        }
        s->table[i] = NULL;
    }

    free(s);
}


void add_value(Set s, const char* key) {
    int slot = hash_function(key);
    Entry curr = s->table[slot];
    while (curr) {
        if (strcmp(curr->key, key) == 0) return;
        curr = curr->Next;
    }

    Entry new_entry = malloc(sizeof(struct set_entry));
    new_entry->key = strdup(key);
    new_entry->Next = s->table[slot];

    s->table[slot] = new_entry;
}


void remove_value(Set s, const char* key) {
    int slot = hash_function(key);
    Entry curr = s->table[slot];
    Entry prev = NULL;
    while (curr) {
        if (strcmp(curr->key, key) == 0) {
            if (prev == NULL) {
                s->table[slot] = curr->Next;
            } else {
                prev->Next = curr->Next;
            }   
            free(curr->key);
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->Next;
    }

    printf("Entry does not exist in specified set\n");
}


int is_present(Set s, const char* key) {
    int slot = hash_function(key);
    Entry curr = s->table[slot];
    if (!curr) {
        printf("Entry does not exist in specified dictionnary\n");
    }
    while (curr) {
        if (strcmp(curr->key, key) == 0) return 1;
        curr = curr->Next;
    }

    return 0;
}
