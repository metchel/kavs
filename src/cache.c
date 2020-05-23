#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logfile.h"
#include "hashtable.h"
#include "lru.h"

static HashTable positions;
static LRU values;

static int timestamp = 0;

HashTable* positioncache_init() {
  hashtable_init(&positions);
  logfile_load_positions(&positions);
  return &positions;
}

void positioncache_put(char* key, long position) {
  hashtable_put(&positions, key, position);
}

HashTableNode* positioncache_get(char* key) {
  HashTableNode* node = hashtable_get(&positions, key);
  timestamp++;
  if (node->datum != NULL) {
    node->datum->timestamp = timestamp;
  }

  return node;
}

void positioncache_delete(char* key) {
  hashtable_delete(&positions, key);
}

void valuecache_init() {
  LRU_init(&values);
}

void valuecache_put(char* key, char* value) {
  Datum* datum = (Datum*)malloc(sizeof(Datum));
  datum->timestamp = timestamp;
  datum->contents = strdup(value);
  LRU_insert(&values, datum);

  hashtable_store(&positions, key, datum);
}