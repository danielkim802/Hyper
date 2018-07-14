#include <stdlib.h>
#include "envstack.h"

void envstack_init(struct EnvStack* es) {
	es->envs = malloc(sizeof(struct Env));
	es->size = 0;
	es->max = 1;
}

void envstack_peek(struct EnvStack* es, struct Env* env) {
	if (es->size != 0)
		*env = es->envs[es->size - 1];
}

void envstack_push(struct EnvStack* es, struct Env* env) {
	if (es->size == es->max) {
		struct Env* newenvs = malloc(sizeof(struct Env) * es->max * 2);
		for (uint64_t i = 0; i < es->size; i ++)
			newenvs[i] = es->envs[i];
		free(es->envs);
		es->envs = newenvs;
		es->max *= 2;
	}

	es->envs[es->size++] = *env;
}

void envstack_pop(struct EnvStack* es, struct Env* env) {
	*env = es->envs[--es->size];
}

void envstack_free(struct EnvStack* es) {
	for (uint64_t i = 0; i < es->size; i ++)
		env_free(&es->envs[i]);
	free(es->envs);
}

void envstack_loadName(struct EnvStack* es, uint8_t* name, struct Value* value) {
	for (int64_t i = es->size - 1; i >= 0; i --) {
		value->valid = 1;
		env_loadName(&es->envs[i], name, value);
		if (value->valid)
			return;
	}

	// raise name error
}

void envstack_storeName(struct EnvStack* es, uint8_t* name) {
	env_storeName(&es->envs[es->size - 1], name);
}

void envstack_assignName(struct EnvStack* es, uint8_t* name, struct Value* value) {
	for (int64_t i = es->size - 1; i >= 0; i --) {
		value->valid = 1;
		env_assignName(&es->envs[i], name, value);
		if (value->valid)
			return;
	}

	// raise name error
}



