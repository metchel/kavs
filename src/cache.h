#ifndef CACHE_H
#define CACHE_H

#include "hashtable.h"

HashTable* positioncache_init();
void positioncache_put(char* key, long value);
HashTableNode* positioncache_get(char* key);
void positioncache_delete(char* key);

void valuecache_init();
void valuecache_put(char* key, char* value);
#endif