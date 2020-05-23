#ifndef DATUM_H
#define DATUM_H

typedef struct {
    unsigned int size;
    char* contents;
    int timestamp;
} Datum;

void touch(Datum* datum, int timestamp);

#endif