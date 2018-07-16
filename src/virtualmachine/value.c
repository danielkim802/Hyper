#include <stdlib.h>
#include "value.h"

struct Value* value_make() {
	return malloc(sizeof(struct Value));
}

void value_arrAppend(struct Value** value, struct Value* app) {
	struct Value* value = *value;
	if (value->arrLen == value->arrMax) {
		struct Value** newvalues = malloc(sizeof(struct Value*) * value->arrLen * 2);
		for (uint64_t i = 0; i < value->arrLen; i ++)
			newvalues[i] = value->arrValue[i];
		free(value->arrValue);
		value->arrValue = newvalues;
		value->arrMax *= 2;
	}

	value->arrValue[value->arrLen++] = app;
}

void value_free(struct Value** value) {
	struct Value* value = *value;
	// if (value->type == FUN || value->type == STRUCT)
	// 	env_free(value->env);
	if (value->type == ARR)
		free(value->arrValue);
	if (value->type == STRING)
		free(value->stringValue);
}

void value_destroy(struct Value** value) {
	value_free(value);
	free(*value);
}

void value_destroy2(struct Value** value1, struct Value** value2) {
	if ((*value)->inUse)
		return;
	if (*value1 == *value2)
		value_destroy(value1);
	else {
		value_destroy(value1);
		value_destroy(value2);
	}
}