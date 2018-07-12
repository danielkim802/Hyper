#ifndef ENVSTACK_H
#define ENVSTACK_H

#include <stdint.h>
#include "env.h"
#include "value.h"

// dynamic array
struct EnvStack {
	struct Env* envs;
	uint64_t size;
	uint64_t max;
};

// initializes the env stack
void envstack_init(struct EnvStack* es);

// peek env at the top of the stack
void envstack_peek(struct EnvStack* es, struct Env* env);

// push onto stack
void envstack_push(struct EnvStack* es, struct Env* env);

// pop from stack
void envstack_pop(struct EnvStack* es, struct Env* env);

// free stack
void envstack_free(struct EnvStack* es);

// traverses down the stack to find a name
void envstack_loadName(struct EnvStack* es, uint8_t* name, struct Value* value);

#endif /* ENVSTACK_H */