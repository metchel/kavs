#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include "cache.h"
#include "logfile.h"
#include "command.h"
#include "hashtable.h"

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
  if (key == NULL)
    printf("Usage: get <key>");

  HashTableNode* node = positioncache_get(key);

  res->type = COMMAND_GET;

  if (node == NULL) {
      res->errorCode = -1;
      res->data = NOT_FOUND;
      return -1;
  }

  char* result;

  if (node->datum != NULL && node->datum->contents != NULL) {
    result = node->datum->contents;
  } else {
    long position = node->position;
    char* line = logfile_seek(position);

    if (line == NULL)
      return -1;

    char* k = strtok(line, ",");
    char* v = strtok(NULL, "\n");

    result = v;

    valuecache_put(k, v);
  }

  res->errorCode = 1;
  res->data = result;

  return res->errorCode;
}

int command_put(Response* res, char* key, char* value) {
  if (key == NULL || value == NULL)
    printf("Usage: put <key> <value>");
  
  long position, offset;
  position = logfile_append(key);
  offset = logfile_append(",");
  offset = logfile_append(value);
  offset = logfile_append("\n");

  if (position >= 0) {
    positioncache_put(key, position);
    //valuecache_put(key, value);
  }

  res->errorCode = 1;
  res->type = COMMAND_PUT;
  res->data = NULL;

  return res->errorCode;
}

int command_delete(Response* res, char* key) {
  if (key == NULL)
    printf("Usage: delete <key>");
  
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
