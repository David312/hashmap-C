#include <stdio.h>

#include "hash_table.h"


char bf1[50];
char bf2[50];
hash_table* ht;

static void ins();
static void srch();
static void del();

int main(int argc, char*argv[]){
  int opt = -1;
  ht = new_hash_table();
  while(opt != 0){
    printf("0) end\n1) insert\n2) search\n3) delete\n>> ");
    scanf("%d",&opt);
    switch(opt){
    case 0: delete_hash_table(ht); return 0;
    case 1: ins(); break;
    case 2: srch(); break;
    case 3: del(); break;
    default:
      continue;
    }
  }
  delete_hash_table(ht);
  return 1;
}

static void ins(){
  printf("key value \n>> ");
  scanf("%s %s",&bf1,&bf2);
  insert(ht,bf1,bf2);
}

static void srch(){
  char* res = NULL;
  printf("key\n>> ");
  scanf("%s",&bf1);
  res = search(ht,bf1);
  if(res != NULL)
    printf("Found: %s\n",res);
  else
    printf("Not found\n");
}

static void del(){
  printf("key\n>> ");
  scanf("%s",&bf1);
  delete(ht,bf1);
}
