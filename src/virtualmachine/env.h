#ifndef ENV_H
#define ENV_H

#include <stdint.h>
#include "value.h"

struct Node {
	uint8_t c;
	struct Value* value;
	struct Node* children;
	uint8_t size;
	uint8_t max;
};

struct Env {
	struct Node* head;
};

// initializes environment
void env_init(struct Env* env);

// stores a name in the environment
void env_storeName(struct Env* env, uint8_t* name);

// assigns a name to a value
void env_assignName(struct Env* env, uint8_t* name, struct Value* value);

// loads value of a name 
void env_loadName(struct Env* env, uint8_t* name, struct Value* value);

// frees environment
void env_free(struct Env* env);

#endif /* ENV_H */