#ifndef VALUE_H
#define VALUE_H

#include "env.h"

enum Type {
	INT, FLOAT, STRING, BOOL, FUN, STRUCT, NIL
};

struct Value {
	enum Type type;
	int64_t intValue;
	double floatValue;
	uint8_t* stringValue;
	uint8_t boolValue;
	uint64_t funValue;
	uint64_t structValue;
	struct Env* env;
};

void value_free(struct Value* value);

#endif /* VALUE_H */