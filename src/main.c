#include <stdio.h>
#include <string.h>

#include "hashtable.h"
#include "command.h"
#include "logfile.h"
#include "cache.h"

#define PROMPT "> "
#define MAX_BUFFER_SIZE 1024
#define SKIP_WHITESPACE(c) \
  while (*c == ' ' || *c == '\t' || *c == '\r') { c++; }

int repl(char* buffer) {
  printf("%s", PROMPT);
  char* input = fgets(buffer, MAX_BUFFER_SIZE, stdin);
  return !ferror(stdin);
}

int parse(Command *cmd, char* buffer) {
  // skip over any whitespace on the left
  SKIP_WHITESPACE(buffer);

  CommandType type;
  char* commandString = strtok(buffer, " ");

  switch(*commandString) {
    // There is a bug here for something like "get "
    case 'g':
      if (strcmp(commandString, "get") == 0) {
        type = COMMAND_GET;
        break;
      }
    case 'p':
      if (strcmp(commandString, "put") == 0) {
        type = COMMAND_PUT;
        break;
      }
    case 'd':
      if (strcmp(commandString, "delete") == 0) {
        type = COMMAND_DELETE;
        break;
      }
    default:
      printf("Undefined command \"%s\"\n", commandString);
      return 0;
  }

  char* args[2];

  args[0] = strtok(NULL, " \t\n");

  if (type == COMMAND_PUT) {
    args[1] = strtok(NULL, " \t\n");
  } else {
    args[1] = NULL;
  }

  command_init(cmd, type, args);

  return 1;
}

int main(int argc, const char* argv[]) {
  char buffer[MAX_BUFFER_SIZE];

  logfile_init("kavs.log");

  HashTable* positions = positioncache_init();
  valuecache_init();

  logfile_compact(positions);

  Command command;

  while (repl(buffer)) {
    if (!parse(&command, buffer)) continue;

    Response res;

    if (command_exec(&command, &res) && res.data) {
      printf("%s\n", res.data);
    }
  }
}