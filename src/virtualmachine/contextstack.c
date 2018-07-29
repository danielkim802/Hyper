#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "contextstack.h"
#include "context.h"

struct ContextStack* contextstack_make() {
	struct ContextStack* cs = malloc(sizeof(struct ContextStack));
	cs->contexts = malloc(sizeof(struct Context));
	cs->size = 0;
	cs->max = 1;
	return cs;
}

void contextstack_push(struct ContextStack* cs, struct VM* vm) {
	if (cs->size == cs->max) {
		struct Context* newcontexts = malloc(sizeof(struct Context) * cs->max * 2);
		for (uint64_t i = 0; i < cs->size; i ++)
			newcontexts[i] = cs->contexts[i];
		free(cs->contexts);
		cs->contexts = newcontexts;
		cs->max *= 2;
	}

	context_init(&cs->contexts[cs->size++], vm);
}

struct Context* contextstack_pop(struct ContextStack* cs) {
	return &cs->contexts[--cs->size];
}

struct Context* contextstack_peek(struct ContextStack* cs) {
	return &cs->contexts[cs->size - 1];
}

void contextstack_free(struct ContextStack* cs) {
	free(cs->contexts);
	free(cs);
}