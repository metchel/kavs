#ifndef COMMAND_H
#define COMMAND_H

typedef enum {
  COMMAND_NULL,
  COMMAND_GET,
  COMMAND_PUT,
  COMMAND_DELETE,
} CommandType;

typedef struct {
  CommandType type;
  char* args[2];
} Command;

typedef struct {
  int errorCode;
  CommandType type;
  char* data;
} Response;

void command_init(Command* command, CommandType type, char* args[]);
int command_exec(Command* command, Response* res);

int command_get(Response* res, char* key);
int command_put(Response* res, char* key, char* value);
int command_delete(Response* res, char*key);

#endif
