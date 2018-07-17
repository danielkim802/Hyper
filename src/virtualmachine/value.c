#include <stdlib.h>
#include <stdio.h>
#include "value.h"
#include "env.h"
#include "envstack.h"

struct Value* value_make(enum Type type) {
	struct Value* value = malloc(sizeof(struct Value));
	value->type = type;
	value->marked = 0;
	// TODO: add to garbage collector
	return value;
}

void value_free(struct Value* value) {
	switch (value->type) {
		case FUN:
			envstack_free(value->funEnvStack);
			for (uint64_t i = 0; i < value->funArgc; i ++)
				free(value->funArgs[i]);
			free(value->funArgs);
			break;
		case STRUCT:
			env_free(value->structValue);
			break;
		case ARR:
			free(value->arrValue);
			break;
		case STRING:
			free(value->stringValue);
			break;
		default:
			break;
	}
	free(value);
}

void value_arrAppend(struct Value* value, struct Value* app) {
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

uint8_t value_typeCheck(struct Value* value1, struct Value* value2, enum Type type1, enum Type type2) {
	return value1->type == type1 && value2->type == type2;
}

void value_print(struct Value* value) {
	switch (value->type) {
		case INT:
			printf("%lli", value->intValue);
			break;
		case FLOAT:
			printf("%f", value->floatValue);
			break;
		case STRING:
			printf("%s", value->stringValue);
			break;
		case BOOL:
			printf(value->boolValue ? "true" : "false");
			break;
		case FUN:
			printf("fun [0x%llx]", value->funValue);
			break;
		case STRUCT:
			printf("struct");
			break;
		case ARR:
			printf("[");
			for (uint64_t i = 0; i < value->arrLen; i ++) {
				value_print(value->arrValue[i]);
				if (i != value->arrLen - 1)
					printf(", ");
			}
			printf("]");
			break;
		case NIL:
			printf("null");
			break;
		default: 
			break;
	}
}