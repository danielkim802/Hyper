#include <stdlib.h>
#include "value.h"

void value_free(struct Value* value) {
	// if (value->type == FUN || value->type == STRUCT)
	// 	env_free(value->env);
	if (value->type == ARR)
		free(value->arrValue);
	if (value->type == STRING)
		free(value->stringValue);
}

void value_arrAppend(struct Value* value, struct Value* app) {
	if (value->arrLen == value->arrMax) {
		struct Value* newvalues = malloc(sizeof(struct Value) * value->arrLen * 2);
		for (uint64_t i = 0; i < value->arrLen; i ++)
			newvalues[i] = value->arrValue[i];
		free(value->arrValue);
		value->arrValue = newvalues;
		value->arrMax *= 2;
	}

	value->arrValue[value->arrLen++] = *app;
}