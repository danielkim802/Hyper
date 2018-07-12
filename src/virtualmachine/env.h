#ifndef ENV_H
#define ENV_H

#include <stdint.h>
#include "value.h"

struct Node {
	uint8_t c;
	char terminal;
	struct Node* children;
	struct Value* value;
};

struct Env {
	struct Node* head;
};

// stores a name in the environment
int env_storeName(struct Env* env, uint8_t* name);

// assigns a name to a value
int env_assignName(struct Env* env, uint8_t* name);

// loads value of a name 
int env_loadName(struct Env* env, uint8_t* name, struct Value* value);

// frees environment
void env_free(struct Env* env);

#endif /* ENV_H */