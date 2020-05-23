#include <stdio.h>

#include "logfile.h"
#include "hashtable.h"

static HashTable positions;

HashTable* positioncache_init() {
  hashtable_init(&positions);
  logfile_load_positions(&positions);

  return &positions;
}

void positioncache_put(char* key, long position) {
  hashtable_put(&positions, key, position);
  hashtable_print(&positions, "%s -> %ld\n");
}

long positioncache_get(char* key) {
  return hashtable_get(&positions, key);
}

void positioncache_delete(char* key) {
  hashtable_delete(&positions, key);
}