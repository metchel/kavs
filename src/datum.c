#include "datum.h"

void touch(Datum* datum, int timestamp){
    datum->timestamp = timestamp;
}