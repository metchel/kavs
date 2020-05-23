#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

void hashtable_init(HashTable* table) {
  table->capacity = 256;
  table->size = 0;

  for (int i = 0; i < 256; i++) {
    table->data[i] =  NULL;
  }
}

unsigned int hashtable_hash(HashTable* table, char* key) {
  unsigned int hash = 5381;
  int c;
  while ((c = *key++) != 0)
    hash = ((hash << 5) + hash) + c;

  return hash;
}

void hashtable_put(HashTable* table, char* key, long value) {
  unsigned int i = hashtable_hash(table, key) % table->capacity;

  if (table->data[i] != NULL) {
    HashTableNode* tmp = table->data[i];
    int match = 0;
    while (tmp->next != NULL && (match = strcmp(tmp->key, key)) != 0) {
      tmp = tmp->next;
    }

    if (match == 0) {
      tmp->position = value;
    } else {
      HashTableNode* node = (HashTableNode*)malloc(sizeof(HashTableNode));
      node->key = strdup(key);
      node->position = value;
      node->next = NULL;
      tmp->next = node;
    }
  } else {
    HashTableNode* node = (HashTableNode*)malloc(sizeof(HashTableNode));
    node->key = strdup(key);
    node->position = value;
    node->next = NULL;
    table->data[i] = node;
  }
}

void hashtable_store(HashTable* table, char* key, Datum* datum) {
  unsigned int i = hashtable_hash(table, key) % table->capacity;

  if (table->data[i] != NULL) {
    HashTableNode* tmp = table->data[i];
    int match = 0;
    while (tmp->next != NULL && (match = strcmp(tmp->key, key)) != 0) {
      tmp = tmp->next;
    }

    if (match == 0) {
      tmp->datum = datum;
    }
  }
}

HashTableNode* hashtable_get(HashTable* table, char* key) {
  unsigned int i = hashtable_hash(table, key) % table->capacity;

  if (table->data[i] == NULL) {
    return NULL;
  } 

  HashTableNode* tmp = table->data[i];
  int match = 0;
  while ((match = strcmp(tmp->key, key)) != 0 && tmp->next != NULL)  {
    tmp = tmp->next;
  }

  return match == 0 ? tmp : NULL;
}

void hashtable_delete(HashTable* table, char* key) {
  unsigned int i = hashtable_hash(table, key) % table->capacity;

  HashTableNode* tmp = table->data[i];
  HashTableNode* last = NULL;
  int match = 0;
  while ((match = strcmp(tmp->key, key)) != 0 && tmp->next != NULL)  {
    last = tmp;
    tmp = tmp->next;
  }

  if (match == 0) {
    // this is the only node
    if (tmp->next == NULL && last == NULL) {
      table->data[i] = NULL;
      free(tmp);
    // this node is at the end (right)
    } else if (tmp->next == NULL && last != NULL) {
      last->next = NULL;
      free(tmp);
    // this node is sandwiched
    } else if (tmp->next != NULL && last != NULL) {
      last->next = tmp->next;
      free(tmp);
    // this node is on the left
    } else if (tmp->next != NULL && last == NULL) {
      table->data[i] = tmp->next;
      free(tmp);
    } else {
      // This is terribly unlikely...
    }
  }
}

void hashtable_print(HashTable* table, char* format) {
  for (int i = 0; i < table->capacity; i++) {
    HashTableNode* tmp = table->data[i];

    if (tmp != NULL) 
      printf(format, tmp->key, tmp->position);
  }
}