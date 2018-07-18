#ifndef ENVSTACK_H
#define ENVSTACK_H

#include "vmstructs.h"

// initializes environment stack
void envstack_init(struct EnvStack* es);

// makes an env stack
struct EnvStack* envstack_make();

// free stack
void envstack_free(struct EnvStack* es);

// peek env at the top of the stack
struct Env* envstack_peek(struct EnvStack* es);

// push new env onto stack
void envstack_push(struct EnvStack* es, uint64_t pos);

// push existing env onto stack
void envstack_pushEnv(struct EnvStack* es, struct Env* env);

// pop from stack
uint64_t envstack_pop(struct EnvStack* es);

// traverses down the stack to dereference a name
struct Value* envstack_loadName(struct EnvStack* es, uint8_t* name);

// stores name in top most environment
void envstack_storeName(struct EnvStack* es, uint8_t* name);

// assigns name in top most environment
void envstack_assignName(struct EnvStack* es, uint8_t* name, struct Value* value);

// prints environment stack
void envstack_print(struct EnvStack* es);

#endif /* ENVSTACK_H */