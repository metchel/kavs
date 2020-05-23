#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cache.h"
#include "logfile.h"
#include "command.h"

static char* NOT_FOUND = "NOT FOUND";

void command_init(Command* command, CommandType type, char* args[]) {
  command->type = type;
  command->args[0] = args[0];
  command->args[1] = args[1];
}

int command_exec(Command* command, Response* res) {
  switch(command->type) {
    case COMMAND_GET:
      return command_get(res, command->args[0]);
    case COMMAND_PUT:
      return command_put(res, command->args[0], command->args[1]);
    case COMMAND_DELETE:
      return command_delete(res, command->args[0]);
    default:
      printf("Undefined command. Usage: get <key>, put <key> <value>, delete <key>\n");
  }

  return 0;
}

int command_get(Response* res, char* key) {
  long position = positioncache_get(key);

  res->errorCode = -1;
  res->type = COMMAND_GET;
  res->data = NOT_FOUND;

  if (position < 0)
    return -1;

  char* line = logfile_seek(position);

  if (line == NULL) 
    return -1;

  char* k = strtok(line, ",");
  char* v = strtok(NULL, "\n");

  if (v == NULL)
    printf("Cache miss...");

  res->errorCode = 1;
  res->data = strdup(v);

  return res->errorCode;
}

int command_put(Response* res, char* key, char* value) {
  
  long position, offset;
  position = logfile_append(key);
  offset = logfile_append(",");
  offset = logfile_append(value);
  offset = logfile_append("\n");

  if (position >= 0) {
    positioncache_put(key, position);
  }

  res->errorCode = 1;
  res->type = COMMAND_PUT;
  res->data = NULL;

  return res->errorCode;
}

int command_delete(Response* res, char* key) {
  
  long position;
  position = logfile_append(key);
  position = logfile_append(",__TOMBSTONE__\n");

  if (position >= 0) {
    positioncache_delete(key);
  }
  
  res->errorCode = 1;
  res->type = COMMAND_DELETE;
  res->data = NULL;

  return res->errorCode;
}
