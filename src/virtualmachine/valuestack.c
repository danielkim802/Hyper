#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "valuestack.h"
#include "value.h"

void valuestack_init(struct ValueStack* vs) {
	vs->values = malloc(sizeof(struct Value));
	vs->size = 0;
	vs->max = 1;
}

void valuestack_peek(struct ValueStack* vs, struct Value* value) {
	if (vs->size != 0)
		*value = vs->values[vs->size - 1];
}

void valuestack_push(struct ValueStack* vs, struct Value* value) {
	if (vs->size == vs->max) {
		struct Value* newvalues = malloc(sizeof(struct Value) * vs->max * 2);
		for (uint64_t i = 0; i < vs->size; i ++)
			newvalues[i] = vs->values[i];
		free(vs->values);
		vs->values = newvalues;
		vs->max *= 2;
	}

	vs->values[vs->size++] = *value;
}

void valuestack_pop(struct ValueStack* vs, struct Value* value) {
	*value = vs->values[--vs->size];
}

void valuestack_free(struct ValueStack* vs) {
	for (uint64_t i = 0; i < vs->size; i ++)
		value_free(&vs->values[i]);
	free(vs->values);
}