#ifndef VALUE_H
#define VALUE_H

#include "vmstructs.h"

void value_free(struct Value* value);

void value_arrAppend(struct Value* value, struct Value* app);

#endif /* VALUE_H */