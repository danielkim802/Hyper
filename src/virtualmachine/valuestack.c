#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "valuestack.h"
#include "value.h"

struct ValueStack* valuestack_make() {
	struct ValueStack* vs = malloc(sizeof(struct ValueStack));
	vs->values = malloc(sizeof(struct Value));
	vs->size = 0;
	vs->max = 1;
	return vs;
}

void valuestack_push(struct ValueStack* vs, struct Value* value) {
	if (vs->size == vs->max) {
		struct Value** newvalues = malloc(sizeof(struct Value*) * vs->max * 2);
		for (uint64_t i = 0; i < vs->size; i ++)
			newvalues[i] = vs->values[i];
		free(vs->values);
		vs->values = newvalues;
		vs->max *= 2;
	}

	vs->values[vs->size++] = value;
}

struct Value* valuestack_pop(struct ValueStack* vs) {
	return vs->values[--vs->size];
}

struct Value* valuestack_peek(struct ValueStack* vs) {
	return vs->values[vs->size - 1];
}

void valuestack_free(struct ValueStack* vs) {
	free(vs->values);
	free(vs);
}

void valuestack_print(struct ValueStack* vs) {
	printf("----valuestack\n");
	for (uint64_t i = 0; i < vs->size; i ++) {
		printf("[%lli] ", i);
		value_print(vs->values[i]);
		printf("\n");
	}
	printf("----valuestack end\n");
}