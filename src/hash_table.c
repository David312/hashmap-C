#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "hash_table.h"
#include "primes.h"

#define DEFAULT_SIZE 5 // must be a prime number
#define HT_PRIME_A 227
#define HT_PRIME_B 773
#define MAX_LOAD 75
#define MIN_LOAD 10

static ht_item HT_DELETED_ITEM = {NULL,NULL}; // for more details
// see https://stackoverflow.com/questions/9127207/hash-table-why-deletion-is-difficult-in-open-addressing-scheme

static ht_item* new_item(const char* k, const char* v);
static void delete_item(ht_item* i);
static ht_hash_table* new_ht_hash_table();
static void delete_ht_hash_table(ht_hash_table* ht);
static void resize(ht_hash_table* ht, const int size);
static void resize_up(ht_hash_table* ht);
static void resize_down(ht_hash_table* ht);
static int ht_load(ht_hash_table* ht);

/*
The variable `a` should be a prime number larger than the size of the alphabet.
We're hashing ASCII strings, which has an alphabet size of 128, so we should
choose a prime larger than that. 
*/
static int ht_get_hash(const char*s, int a, int ht_size);
static int hash(const char*s, int ht_size, int attempts);

static void ht_insert(ht_hash_table* ht, const char* key, const char* value);
static char* ht_search(ht_hash_table* ht, const char* key);
static void ht_delete(ht_hash_table* h, const char* key);

static ht_hash_table* new_ht_sized_hash_table(const int base_size);


hash_table* new_hash_table(){
  return new_ht_hash_table();
}

void delete_hash_table(hash_table* ht){
  delete_ht_hash_table(ht);
}


void insert(hash_table* ht, const char* key, const char* value){
  ht_insert(ht,key,value);
}

char* search(hash_table* ht, const char* key){
  return ht_search(ht,key);
}

void delete(hash_table* ht, const char* key){
  ht_delete(ht,key);
}


/*
 * Internal functions definitions
 */
static ht_item* new_item(const char* k, const char* v){
  ht_item* new = malloc(sizeof(ht_item));

  new->key = strdup(k);
  new->value = strdup(v);

  return new;
}


static void delete_item(ht_item* i){
  free(i->key);
  free(i->value);
  free(i);
}

static ht_hash_table* new_ht_sized_hash_table(const int base_size){
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  ht->base_size = base_size;
  ht->size = next_prime(base_size);
  ht->count = 0;

  ht->items = calloc(ht->size,sizeof(ht_item*));
  
  return ht;

}


static ht_hash_table* new_ht_hash_table(){
  return new_ht_sized_hash_table(DEFAULT_SIZE);
}

static void resize(ht_hash_table* ht, const int size){
  int i, tmp_size;
  ht_item* item, **tmp_items;
  ht_hash_table* new_ht;

  if(size < DEFAULT_SIZE)
    return;
  
  new_ht = new_ht_sized_hash_table(size);
  for(i = 0; i < ht->size; i++){
    item = ht->items[i];
    if(item != NULL && item != &HT_DELETED_ITEM){
      ht_insert(new_ht,item->key,item->value);
    }
  }

  // transfer new attributes
  ht->base_size = new_ht->base_size;
  ht->count = new_ht->count;

  // swap size attributes to delete new_ht
  tmp_size = ht->size;
  ht->size = new_ht->size;
  new_ht->size = tmp_size;

  // idem with items array
  tmp_items = ht->items;
  ht->items = new_ht->items;
  new_ht->items = tmp_items;

  delete_ht_hash_table(new_ht);
}

static void resize_up(ht_hash_table* ht){
  resize(ht, ht->base_size * 2);
}

static void resize_down(ht_hash_table* ht){
  resize(ht, ht->base_size / 2);
}

static int ht_load(ht_hash_table* ht){
  return ht->count * 100 / ht->size;
}


static void delete_ht_hash_table(ht_hash_table* ht){
  int i;

  for(i = 0; i < ht->size; i++){
    if(ht->items[i] != NULL && ht->items[i] != &HT_DELETED_ITEM)
      delete_item(ht->items[i]);
  }

  free(ht->items);
  free(ht);
}

static int ht_get_hash(const char* s, int a, int ht_size){
  long hash = 0;
  int i;
  const int len_s = strlen(s);

  for(i = 0; i < len_s; i++){
    hash += (long)pow(a, len_s - (i+1)) * s[i]; // 'a' is a prime number
    hash = hash % ht_size;
  }

  return (int)hash;
}

static int hash(const char* s, int ht_size, int attempts){
  int hash_a, hash_b;

  hash_a = ht_get_hash(s,HT_PRIME_A,ht_size);
  hash_b = ht_get_hash(s,HT_PRIME_B,ht_size);
  return (hash_a + (attempts * (hash_b + 1))) % ht_size;
}

static void ht_insert(ht_hash_table* ht, const char* key, const char* value){
  ht_item* item;
  int i, index;

  if(ht_load(ht) > MAX_LOAD)
    resize_up(ht);
  
  index = hash(key,ht->size,0);
  item = ht->items[index];
  i = 1;
  while(item != NULL && item != &HT_DELETED_ITEM){
    if(strcmp(item->key,key) == 0){ // item already exists
      delete_item(item); // delete previous item
      ht->count--; // because we are updating, not inserting
      break;
    }
    index = hash(key,ht->size,i);
    item = ht->items[index];
    i++;
  }
  
  ht->items[index] = new_item(key, value);
  ht->count++;
}

static char* ht_search(ht_hash_table* ht, const char* key){
  int i, index;
  ht_item* item;
  
  index = hash(key,ht->size,0);
  item = ht->items[index];
  i = 1;
  while(item != NULL){
    if(item != &HT_DELETED_ITEM && strcmp(item->key,key) == 0)
      return item->value;
    index = hash(key,ht->size,i);
    item = ht->items[index];
    i++;    
  }

  return NULL;
}

static void ht_delete(ht_hash_table* ht, const char* key){
  ht_item* item;
  int i, index;

  if(ht_load(ht) < MIN_LOAD)
    resize_down(ht);
  
  index = hash(key, ht->size, 0);
  item = ht->items[index];
  i = 1;

  while(item != NULL){
    if(item != &HT_DELETED_ITEM){ 
      if(strcmp(item->key,key) == 0){
	delete_item(item);
	ht->items[index] = &HT_DELETED_ITEM; // mark as deleted
	break;
      }
    }
    index = hash(key,ht->size,i);
    item = ht->items[index];
    i++;
  }
  ht->count--;
}
