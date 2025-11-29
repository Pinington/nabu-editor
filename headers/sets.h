#ifndef SETS_H
#define SETS_H

#define MAX_SIZE 2048 // Will make it dynamic in the future

struct set_entry {
    char* key;
    // Chained collisions
    struct sets_entry * Next; 
};
typedef struct set_entry * Entry;

struct set {
    Entry table[MAX_SIZE];
};
typedef struct set * Set;

unsigned int hash_function(const char * str);
Set initialize_set();
void free_set(Set s);
void add_value(Set s, const char* key);
void remove_value(Set s, const char* key);
int is_present(Set s, const char* key);

#endif