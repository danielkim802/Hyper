#ifndef CONTEXTSTACK_H
#define CONTEXTSTACK_H

#include "vmstructs.h"

// initializes the context stack
struct ContextStack* contextstack_make();

// push onto stack
void contextstack_push(struct ContextStack* vs, struct VM* vm);

// pop from stack
struct Context* contextstack_pop(struct ContextStack* cs);

// peek value at the top of the stack
struct Context* contextstack_peek(struct ContextStack* cs);

// free stack
void contextstack_free(struct ContextStack* cs);

#endif /* CONTEXTSTACK_H */