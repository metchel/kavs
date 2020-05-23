#ifndef CACHE_H
#define CACHE_H

#include "hashtable.h"

HashTable* positioncache_init();
void positioncache_put(char* key, long value);
long positioncache_get(char* key);
void positioncache_delete(char* key);

#endif