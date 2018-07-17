#ifndef VALUE_H
#define VALUE_H

#include "vmstructs.h"

struct Value* value_make(enum Type type);

void value_free(struct Value* value);

void value_arrAppend(struct Value* value, struct Value* app);

uint8_t value_typeCheck(struct Value* value1, struct Value* value2, enum Type type1, enum Type type2);

void value_print(struct Value* value);

#endif /* VALUE_H */