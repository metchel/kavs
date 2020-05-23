#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logfile.h"
#include "hashtable.h"

#define MAX_BUFFER_SIZE 1024
#define EXTRA_CHARS 2

static char* TOMBSTONE = "__TOMBSTONE__";

static LogFile log;

void logfile_init(char* filename) {
  log.filename = filename;
  log.decompaction = 0;

  FILE *f = fopen(filename, "a");
  fclose(f);
}

long logfile_append(char* line) {
  FILE* f = fopen(log.filename, "a");

  long position = ftell(f);

  if (fprintf(f, "%s", line) < 0) {
    printf("Failed.");
    return -1;
  }

  fclose(f);

  return position;
}

char* logfile_seek(int position) {
  FILE* f = fopen(log.filename, "r");
  int seek = fseek(f, position, SEEK_SET);

  if (seek != 0) {
    printf("FAILURE in seeking.");
    return NULL;
  }

  char buffer[MAX_BUFFER_SIZE];
  char* line = fgets(buffer, MAX_BUFFER_SIZE, f);

  fclose(f);
  return line;
}

void logfile_load_positions(HashTable* positions) {
  FILE* f = fopen(log.filename, "r");

  long position = 0;

  char buffer[MAX_BUFFER_SIZE];

  while (!feof(f)) {

    position = ftell(f);
    char* line = fgets(buffer, MAX_BUFFER_SIZE, f);

    if (line == NULL)
      continue;

    char* k = strtok(line, ",");
    char* v = strtok(NULL, "\n");

    if (strcmp(v, TOMBSTONE) == 0) {
      hashtable_delete(positions, k);
    } else {
      hashtable_put(positions, k, position);
    }
  }

  fclose(f);
}

// This must be run through or else there will be huge problems.
// TODO: make it so that this can partially execute, comeback later, and everything remain correct.
void logfile_compact(HashTable* positions) {

  FILE* f = fopen(log.filename, "r");

  if (f == NULL)
    return;
  
  FILE* shadow = fopen("kavs.log.shadow", "a");

  long position = 0;
  long position_shadow = 0;
  char buffer[MAX_BUFFER_SIZE];

  while (!feof(f)) {
    position = ftell(f);
    position_shadow = ftell(shadow);

    char* line = fgets(buffer, MAX_BUFFER_SIZE, f);
    if (line == NULL)
      continue;
    
    char* k = strtok(line, ",");
    long actualPosition = hashtable_get(positions, k);

    if (actualPosition != position) {
      // this is an old value.
      continue;
    } else {
      char* v = strtok(NULL, "\n");
      if (strcmp(v, TOMBSTONE) != 0) {
        fprintf(shadow, "%s,%s\n", k, v);
        hashtable_put(positions, k, position_shadow);
      }
    }
  }

  fclose(f);
  fclose(shadow);

  system("rm kavs.log");
  system("mv kavs.log.shadow kavs.log");
}