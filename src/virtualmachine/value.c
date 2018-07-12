#include <stdlib.h>
#include "value.h"

void value_free(struct Value* value) {
	if (value->type == FUN || value->type == STRUCT)
		env_free(value->env);
	if (value->type == STRING)
		free(value->stringValue);
}