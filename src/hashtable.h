#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "datum.h"

typedef struct HashTableNode {
  char* key;
  long position;
  Datum* datum;
  struct HashTableNode* next;
} HashTableNode;

typedef struct {
  unsigned int capacity;
  unsigned int size;
  HashTableNode* data[256];
} HashTable;

void hashtable_init(HashTable* table);
unsigned int hashtable_hash(HashTable* table, char* key);

void hashtable_put(HashTable* table, char* key, long value);
HashTableNode* hashtable_get(HashTable* table, char* key);

void hashtable_store(HashTable* table, char* key, Datum* datum);

void hashtable_delete(HashTable* table, char* key);
void hashtable_print(HashTable* table, char* format);
#endif
