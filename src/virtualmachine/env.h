#ifndef ENV_H
#define ENV_H

#include "vmstructs.h"

// initializes environment
void env_init(struct Env* env, uint64_t pos);

// stores a name in the environment
void env_storeName(struct Env* env, uint8_t* name);

// assigns a name to a value
void env_assignName(struct Env* env, uint8_t* name, struct Value* value);

// loads value of a name 
void env_loadName(struct Env* env, uint8_t* name, struct Value* value);

// frees environment
void env_free(struct Env* env);

#endif /* ENV_H */