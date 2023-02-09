#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

// hash_table.c

typedef struct linked_list
{
    void *data;
    struct linked_list *next;
} linked_list;

typedef void *hash_element;

typedef struct hash_elements
{
    hash_element *values;
    size_t values_size;
    size_t capacity;
} hash_elements;

typedef struct node
{
    hash_element key;
    hash_element val;
    hash_elements fat_val;
    size_t depth;
    struct node *next;
} node;

typedef struct hashtable
{
    size_t size;
    size_t count;
    size_t (*hash_function)(hash_element, size_t);
    size_t (*compare_function)(hash_element, hash_element);
    node **array;
    void (*key_free_function)(hash_element);
    void (*val_free_function)(hash_element);
    pthread_mutex_t *locks;
    bool is_fat;
} hashtable;

typedef struct node_array
{
    size_t node_size;
    node **array;

} node_array;

size_t hash_string(hash_element str, size_t size);

int create_ht(hashtable **ht, size_t size, size_t (*hash_function)(hash_element, size_t),
              size_t (*compare_function)(hash_element, hash_element), bool fat, void (*key_free_function)(hash_element), void (*val_free_function)(hash_element));
int put_ht(hashtable *ht, hash_element key, hash_element value);
int fat_put_ht(hashtable *ht, hash_element key, hash_element value);
hash_elements fat_get_ht(hashtable *ht, hash_element key);
void free_array(void *tofree);
int fat_deallocate_ht(hashtable *ht, bool free_key, bool free_value);
hash_elements get_ht(hashtable *ht, hash_element key);
node *get_raw_ht(hashtable *ht, hash_element key);
int erase_ht(hashtable *ht, hash_element key);
node_array get_keys(hashtable *ht);
int deallocate_ht(hashtable *ht, bool free_key, bool free_value); // the boolean is to determine if the key should be freed or not
void fat_print_ht(hashtable *ht);

#endif