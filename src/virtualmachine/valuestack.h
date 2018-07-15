#ifndef VALUESTACK_H
#define VALUESTACK_H

#include "vmstructs.h"

// initializes the value stack
void valuestack_init(struct ValueStack* vs);

// peek value at the top of the stack
void valuestack_peek(struct ValueStack* vs, struct Value* value);

// push onto stack
void valuestack_push(struct ValueStack* vs, struct Value* value);

// pop from stack
void valuestack_pop(struct ValueStack* vs, struct Value* value);

// free stack
void valuestack_free(struct ValueStack* vs);

#endif /* VALUESTACK_H */