#ifndef LOGFILE_H
#define LOGFILE_H

#include "hashtable.h"

typedef struct {
  char* filename;
  int decompaction;
} LogFile;

void logfile_init(char* filename);
long logfile_append(char* buffer);
char* logfile_seek(int position);
void logfile_load_positions(HashTable* table);
void logfile_compact(HashTable* postitions);
#endif
