#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "envstack.h"
#include "env.h"
#include "vmerror.h"

void envstack_init(struct EnvStack* es) {
	es->envs = malloc(sizeof(struct Env));
	es->size = 0;
	es->max = 1;
}

struct EnvStack* envstack_make() {
	struct EnvStack* es = malloc(sizeof(struct EnvStack));
	envstack_init(es);
	return es;
}

void envstack_free(struct EnvStack* es) {
	for (uint64_t i = 0; i < es->size; i ++){
		*es->envs[i].inUse -= 1;
		env_freeContent(&es->envs[i]);
	}
	free(es->envs);
	free(es);
}

struct Env* envstack_peek(struct EnvStack* es) {
	return &es->envs[es->size - 1];
}

void envstack_push(struct EnvStack* es, uint64_t pos) {
	if (es->size == es->max) {
		struct Env* newenvs = malloc(sizeof(struct Env) * es->max * 2);
		for (uint64_t i = 0; i < es->size; i ++)
			newenvs[i] = es->envs[i];
		free(es->envs);
		es->envs = newenvs;
		es->max *= 2;
	}
	env_init(&es->envs[es->size++], pos);
	*es->envs[es->size - 1].inUse += 1;
}

void envstack_pushEnv(struct EnvStack* es, struct Env* env) {
	if (es->size == es->max) {
		struct Env* newenvs = malloc(sizeof(struct Env) * es->max * 2);
		for (uint64_t i = 0; i < es->size; i ++)
			newenvs[i] = es->envs[i];
		free(es->envs);
		es->envs = newenvs;
		es->max *= 2;
	}

	es->envs[es->size++] = *env;
	*env->inUse += 1;
}

uint64_t envstack_pop(struct EnvStack* es) {
	if (es->size == 0)
		vmerror_raise(RUNTIME_ERROR, "Environment stack empty");
	uint64_t pos = es->envs[es->size - 1].stackPos;
	*es->envs[es->size - 1].inUse -= 1;
	env_freeContent(&es->envs[--es->size]);
	return pos;
}

struct Value* envstack_loadName(struct EnvStack* es, uint8_t* name) {
	for (uint64_t i = 0; i < es->size; i ++) {
		struct Value* value = env_loadName(&es->envs[es->size - i - 1], name);
		if (value != NULL)
			return value;
	}

	char* msg_prefix = "Undefined name '";
	char* msg = malloc(sizeof(char) * (strlen(msg_prefix) + strlen((char*) name) + 1));
	strcpy(msg, msg_prefix);
	strcpy(&msg[strlen(msg_prefix)], (char*) name);
	msg[strlen(msg_prefix) + strlen((char*) name)] = '\'';
	msg[strlen(msg_prefix) + strlen((char*) name) + 1] = 0;
	vmerror_raise(NAME_ERROR, msg);
	return NULL;
}

void envstack_storeName(struct EnvStack* es, uint8_t* name) {
	env_storeName(&es->envs[es->size - 1], name);
}

void envstack_assignName(struct EnvStack* es, uint8_t* name, struct Value* value) {
	for (int64_t i = es->size - 1; i >= 0; i --) {
		struct Value* valueout = env_assignName(&es->envs[i], name, value);
		if (valueout != NULL)
			return;
	}

	char* msg_prefix = "Undefined name '";
	char* msg = malloc(sizeof(char) * (strlen(msg_prefix) + strlen((char*) name) + 1));
	strcpy(msg, msg_prefix);
	strcpy(&msg[strlen(msg_prefix)], (char*) name);
	msg[strlen(msg_prefix) + strlen((char*) name)] = '\'';
	msg[strlen(msg_prefix) + strlen((char*) name) + 1] = 0;
	vmerror_raise(NAME_ERROR, msg);
}

void envstack_print(struct EnvStack* es) {
	printf("----envstack [%x]\n", (unsigned int) es);
	for (uint64_t i = 0; i < es->size; i ++) {
		printf("[%lli] ", i);
		env_print(&es->envs[i]);
		printf("\n");
	}
	printf("----envstack end\n");
}

