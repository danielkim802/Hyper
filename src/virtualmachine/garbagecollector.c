#include <stdlib.h>
#include <stdio.h>
#include "garbagecollector.h"
#include "value.h"
#include "valuestack.h"
#include "context.h"
#include "contextstack.h"

void garbagecollector_print(struct GarbageCollector* gc) {
	printf("----gc\n");
	for (uint64_t i = 0; i < gc->size; i ++) {
		if (gc->values[i]->marked)
			printf("{");
		value_print(gc->values[i]);
		if (gc->values[i]->marked)
			printf("}");
		printf("  ");
	}
	printf("\n----gc end\n");
}

struct GarbageCollector* garbagecollector_make() {
	struct GarbageCollector* gc = malloc(sizeof(struct GarbageCollector));
	gc->values = malloc(sizeof(struct Value*));
	gc->size = 0;
	gc->max = 1;
	value_gc = gc;
	return gc;
}

void garbagecollector_free(struct GarbageCollector* gc) {
	free(gc->values);
	free(gc);
}

void garbagecollector_addValue(struct GarbageCollector* gc, struct Value* value) {
	if (gc->size == gc->max) {
		struct Value** newvalues = malloc(sizeof(struct Value*) * gc->max * 2);
		for (uint64_t i = 0; i < gc->size; i ++)
			newvalues[i] = gc->values[i];
		free(gc->values);
		gc->values = newvalues;
		gc->max *= 2;
	}

	gc->values[gc->size++] = value;
}

void mark_value(struct Value* value);

void mark_node(struct Node* node) {
	if (node->value != NULL)
		mark_value(node->value);
	for (uint8_t i = 0; i < node->size; i ++)
		mark_node(&node->children[i]);
}

void mark_env(struct Env* env) {
	mark_node(env->head);
}

void mark_envStack(struct EnvStack* es) {
	for (uint64_t i = 0; i < es->size; i ++)
		mark_env(&es->envs[i]);
}

void mark_value(struct Value* value) {
	if (value->marked)
		return;

	value->marked = 1;
	switch (value->type) {
		case INT: return;
		case FLOAT: return;
		case STRING: return;
		case BOOL: return;
		case FUN:
			mark_envStack(value->funEnvStack);
			return;
		case STRUCT:
			mark_env(value->structValue);
			return;
		case NIL: return;
		case ARR:
			for (uint64_t i = 0; i < value->arrLen; i ++)
				mark_value(value->arrValue[i]);
			return;
		default:
			return;
	}
}

void mark_valueStack(struct ValueStack* vs) {
	for (uint64_t i = 0; i < vs->size; i ++)
		mark_value(vs->values[i]);
}

void mark_context(struct Context* context) {
	mark_valueStack(context->valueStack);
	mark_valueStack(context->callStack);
}

void mark_contextStack(struct ContextStack* cs) {
	for (uint64_t i = 0; i < cs->size; i ++)
		mark_context(&cs->contexts[i]);
}

void garbagecollector_markValues(struct VM* vm) {
	mark_valueStack(vm->valueStack);
	mark_valueStack(vm->callStack);
	mark_contextStack(vm->contextStack);
}

void garbagecollector_freeValues(struct GarbageCollector* gc) {
	// free all unmarked values and keep marked ones
	struct ValueStack* temp = valuestack_make();
	for (uint64_t i = 0; i < gc->size; i ++) {
		if (!gc->values[i]->marked) {
			value_free(gc->values[i]);
		} else {
			gc->values[i]->marked = 0;
			valuestack_push(temp, gc->values[i]);
		}
	}

	// set new list of active values
	free(gc->values);	
	gc->values = temp->values;
	gc->size = temp->size;
	gc->max = temp->max;
	free(temp);	
}

void garbagecollector_clean(struct GarbageCollector* gc, struct VM* vm) {
	garbagecollector_markValues(vm);
	garbagecollector_freeValues(gc);
}