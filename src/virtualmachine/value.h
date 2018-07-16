#ifndef VALUE_H
#define VALUE_H

#include "vmstructs.h"

struct Value* value_make();

// void value_free(struct Value** value);

void value_arrAppend(struct Value** value, struct Value* app);

void value_destroy(struct Value** value);
#endif /* VALUE_H */