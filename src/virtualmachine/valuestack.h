#ifndef VALUESTACK_H
#define VALUESTACK_H

#include "vmstructs.h"

// initializes the value stack
struct ValueStack* valuestack_make();

// push onto stack
void valuestack_push(struct ValueStack* vs, struct Value* value);

// pop from stack
struct Value* valuestack_pop(struct ValueStack* vs);

// peek value at the top of the stack
struct Value* valuestack_peek(struct ValueStack* vs);

// free stack
void valuestack_free(struct ValueStack* vs);

// print value stack
void valuestack_print(struct ValueStack* vs);

#endif /* VALUESTACK_H */