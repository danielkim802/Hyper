#ifndef ENV_H
#define ENV_H

#include "vmstructs.h"

// initializes environment
void env_init(struct Env* env, uint64_t pos);

// makes an environment
struct Env* env_make();

// frees environment
void env_free(struct Env* env);

// frees contents of environment
void env_freeContent(struct Env* env);

// stores a name in the environment
void env_storeName(struct Env* env, uint8_t* name);

// assigns a name to a value; returns previously assigned value. 
// If name was not stored, return value will be null and value
// was not assigned.
struct Value* env_assignName(struct Env* env, uint8_t* name, struct Value* value);

// loads value of a name 
struct Value* env_loadName(struct Env* env, uint8_t* name);

// prints first character in prefix tree 
void env_print(struct Env* env);

#endif /* ENV_H */