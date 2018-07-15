#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "vmerror.h"

void node_init(struct Node* node, uint8_t c, struct Value* value) {
	node->children = malloc(sizeof(struct Node));
	node->size = 0;
	node->max = 1;
	node->c = c;
	node->value = value;
}

void node_free(struct Node* node) {
	for (uint8_t i = 0; i < node->size; i ++)
		node_free(&node->children[i]);
	if (node->value != NULL)
		value_free(node->value);
}

void node_addChild(struct Node* node, struct Node* child) {
	if (node->size == node->max) {
		struct Node* newchildren = malloc(sizeof(struct Node) * node->max * 2);
		for (uint8_t i = 0; i < node->size; i ++)
			newchildren[i] = node->children[i];
		free(node->children);
		node->children = newchildren;
		node->max *= 2;
	}

	node->children[node->size++] = *child;
}

void node_storeName(struct Node* node, uint8_t* name, uint64_t ptr) {
	if (name[ptr] == 0) {
		if (node->value != NULL) {
			char* msg_prefix = "Name '";
			char* msg_postfix = "' already declared";
			char* msg = malloc(sizeof(char) * (strlen(msg_prefix) + strlen((char*) name) + strlen(msg_postfix)));
			strcpy(msg, msg_prefix);
			strcpy(&msg[strlen(msg_prefix)], (char*) name);
			strcpy(&msg[strlen(msg_prefix) + strlen((char*) name)], msg_postfix);
			vmerror_raise(NAME_ERROR, msg);
		}
		node->value = malloc(sizeof(struct Value));
		node->value->type = NIL;
		return;
	}

	for (uint8_t i = 0; i < node->size; i ++) {
		if (node->children[i].c == name[ptr]) {
			node_storeName(&node->children[i], name, ptr + 1);
			return;
		}
	}

	struct Node child;
	node_init(&child, name[ptr], NULL);
	node_addChild(node, &child);
	node_storeName(&node->children[node->size - 1], name, ptr + 1);
}

void node_loadName(struct Node* node, uint8_t* name, uint64_t ptr, struct Value* value) {
	if (name[ptr] == 0) {
		if (node->value == NULL) {
			value->valid = 0;
			return;
		}
		*value = *node->value;
		value->valid = 1;
		return;
	}

	for (uint8_t i = 0; i < node->size; i ++) {
		if (node->children[i].c == name[ptr]) {
			node_loadName(&node->children[i], name, ptr + 1, value);
			return;
		}
	}

	value->valid = 0;
}

void node_assignName(struct Node* node, uint8_t* name, uint64_t ptr, struct Value* value) {
	if (name[ptr] == 0) {
		if (node->value == NULL) {
			value->valid = 0;
			return;
		}
		*node->value = *value;
		value->valid = 1;
		return;
	}

	for (uint8_t i = 0; i < node->size; i ++) {
		if (node->children[i].c == name[ptr]) {
			node_assignName(&node->children[i], name, ptr + 1, value);
			return;
		}
	}

	value->valid = 0;
}

void env_init(struct Env* env, uint64_t pos) {
	env->head = malloc(sizeof(struct Node));
	node_init(env->head, 0, NULL);
	env->stackPos = pos;
}

void env_storeName(struct Env* env, uint8_t* name) {
	node_storeName(env->head, name, 0);
}

void env_assignName(struct Env* env, uint8_t* name, struct Value* value) {
	node_assignName(env->head, name, 0, value);
}

void env_loadName(struct Env* env, uint8_t* name, struct Value* value) {
	node_loadName(env->head, name, 0, value);
}

void env_free(struct Env* env) {
	node_free(env->head);
	free(env->head);
}