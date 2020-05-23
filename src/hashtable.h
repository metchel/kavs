#ifndef HASHTABLE_H
#define HASHTABLE_H

typedef struct HashTableNode {
  char* key;
  long value;
  char* data;
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
void hashtable_store(HashTable* table, char* key, char* data);
long hashtable_get(HashTable* table, char* key);

void hashtable_delete(HashTable* table, char* key);
void hashtable_print(HashTable* table, char* format);
#endif
