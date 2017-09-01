#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct{
  char *key;
  char *value;
}ht_item;

typedef struct{
  int size;
  int count;
  int base_size;
  ht_item **items;
}ht_hash_table;

typedef ht_hash_table hash_table;

hash_table* new_hash_table();
void delete_hash_table(hash_table* ht);

void insert(hash_table* ht, const char* key, const char* value);
char* search(hash_table* ht, const char* key);
void delete(hash_table* ht, const char* key);

#endif
