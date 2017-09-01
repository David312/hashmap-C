#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "hash_table.h"

#define DEFAULT_SIZE 53 // must be a prime number
#define HT_PRIME_A 227
#define HT_PRIME_B 773

static ht_item* new_item(const char* k, const char* v);
static void delete_item(ht_item* i);
static ht_hash_table* new_ht_hash_table();
static void delete_ht_hash_table(ht_hash_table* ht);

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


hash_table* new_hash_table(){
  return new_ht_hash_table();
}

void delete_hash_table(hash_table* ht){
  delete_ht_hash_table(ht);
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

static ht_hash_table* new_ht_hash_table(){
  ht_hash_table* ht = malloc(sizeof(ht_hash_table));

  ht->size = DEFAULT_SIZE;
  ht->count = 0;

  ht->items = calloc(ht->size,sizeof(ht_item*));
  
  return ht;
}


static void delete_ht_hash_table(ht_hash_table* ht){
  int i;

  for(i = 0; i < ht->size; i++){
    if(ht->items[i] != NULL)
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
  int i, index;

  i = 0;
  do{
    index = hash(key,ht->size,i);
    i++;
  }while(ht->items[index] != NULL);
  
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
    if(strcmp(item->key,key) == 0)
      return item->value;
    index = hash(key,ht->size,i);
    item = ht->items[index];
    i++;    
  }
  return NULL;
}
