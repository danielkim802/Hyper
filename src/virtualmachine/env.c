#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "env.h"
#include "value.h"
#include "vmerror.h"

void node_init(struct Node* node, uint8_t c, struct Value* value) {
	node->children = malloc(sizeof(struct Node));
	node->size = 0;
	node->max = 1;
	node->c = c;
	node->value = value;
}

struct Node* node_make(uint8_t c, struct Value* value) {
	struct Node* node = malloc(sizeof(struct Node));
	node_init(node, c, value);
	return node;
}

void node_freeContent(struct Node* node) {
	for (uint8_t i = 0; i < node->size; i ++)
		node_freeContent(&node->children[i]);
	free(node->children);
}

void node_free(struct Node* node) {
	node_freeContent(node);
	free(node);
}

void node_addChild(struct Node* node, uint8_t c, struct Value* value) {
	if (node->size == node->max) {
		struct Node* newchildren = malloc(sizeof(struct Node) * node->max * 2);
		for (uint8_t i = 0; i < node->size; i ++)
			newchildren[i] = node->children[i];
		free(node->children);
		node->children = newchildren;
		node->max *= 2;
	}

	node_init(&node->children[node->size++], c, value);
}

void node_storeName(struct Node* node, uint8_t* name, uint64_t ptr) {
	if (name[ptr] == 0) {
		if (node->value != NULL) {
			char* msg_prefix = "Name '";
			char* msg_postfix = "' already declared";
			char* msg = malloc(sizeof(char) * (strlen(msg_prefix) + strlen((char*) name) + strlen(msg_postfix) + 1));
			strcpy(msg, msg_prefix);
			strcpy(&msg[strlen(msg_prefix)], (char*) name);
			strcpy(&msg[strlen(msg_prefix) + strlen((char*) name)], msg_postfix);
			msg[strlen(msg)] = 0;
			vmerror_raise(NAME_ERROR, msg);
		}
		node->value = value_make(NIL);
		return;
	}

	for (uint8_t i = 0; i < node->size; i ++)
		if (node->children[i].c == name[ptr])
			return node_storeName(&node->children[i], name, ptr + 1);

	node_addChild(node, name[ptr], NULL);
	node_storeName(&node->children[node->size - 1], name, ptr + 1);
}

struct Value* node_loadName(struct Node* node, uint8_t* name, uint64_t ptr) {
	if (name[ptr] == 0)
		return node->value;

	for (uint8_t i = 0; i < node->size; i ++)
		if (node->children[i].c == name[ptr])
			return node_loadName(&node->children[i], name, ptr + 1);

	return NULL;
}

struct Value* node_assignName(struct Node* node, uint8_t* name, uint64_t ptr, struct Value* value) {
	if (name[ptr] == 0) {
		if (node->value == NULL)
			return NULL;
		struct Value* prev = node->value;
		node->value = value;
		return prev;
	}

	for (uint8_t i = 0; i < node->size; i ++)
		if (node->children[i].c == name[ptr])
			return node_assignName(&node->children[i], name, ptr + 1, value);

	return NULL;
}

void env_init(struct Env* env, uint64_t stackPos) {
	env->head = node_make(0, NULL);
	env->stackPos = stackPos;
}

struct Env* env_make(uint64_t pos) {
	struct Env* env = malloc(sizeof(struct Env));
	env_init(env, pos);
	return env;
}

void env_free(struct Env* env) {
	node_free(env->head);
	free(env);
}

void env_freeContent(struct Env* env) {
	node_free(env->head);
}

void env_storeName(struct Env* env, uint8_t* name) {
	node_storeName(env->head, name, 0);
}

struct Value* env_assignName(struct Env* env, uint8_t* name, struct Value* value) {
	return node_assignName(env->head, name, 0, value);
}

struct Value* env_loadName(struct Env* env, uint8_t* name) {
	return node_loadName(env->head, name, 0);
}

void env_print(struct Env* env) {
	if (env->head->size == 0) {
		printf("empty");
		return;
	}
	printf("%c", env->head->children[0].c);
}