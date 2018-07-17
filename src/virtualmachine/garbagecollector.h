#ifndef GARBAGECOLLECTOR_H
#define GARBAGECOLLECTOR_H

#include "vmstructs.h"

// prints values
void garbagecollector_print(struct GarbageCollector* gc);

// makes garbage collector
struct GarbageCollector* garbagecollector_make();

// frees garbage collector
void garbagecollector_free(struct GarbageCollector* gc);

// adds a value to the record
void garbagecollector_addValue(struct GarbageCollector* gc, struct Value* value);

// cleans out unused values
void garbagecollector_clean(struct GarbageCollector* gc, struct VM* vm);

#endif /* GARBAGECOLLECTOR_H */