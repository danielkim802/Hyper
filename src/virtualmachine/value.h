#ifndef VALUE_H
#define VALUE_H

#include "env.h"

enum Type {
	INT, FLOAT, STRING, BOOL, FUN, STRUCT, NIL, ARR
};

struct Value {
	enum Type type;
	int64_t intValue;
	double floatValue;
	uint8_t* stringValue;
	uint64_t stringLen;
	uint8_t boolValue;
	uint64_t funValue;
	struct Env* funEnv;
	uint64_t funArgc;
	uint8_t** funArgs;
	struct Env* structValue;
	struct Value* arrValue;
	uint64_t arrLen;
	uint64_t arrMax;
	uint8_t valid;
};

void value_free(struct Value* value);

#endif /* VALUE_H */