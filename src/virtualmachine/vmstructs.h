#ifndef VMSTRUCTS_H
#define VMSTRUCTS_H

enum Type {
	INT, FLOAT, STRING, BOOL, FUN, STRUCT, NIL, ARR
};

struct Env;
struct EnvStack;

struct Value {
	enum Type type;
	int64_t intValue;
	double floatValue;
	uint8_t* stringValue;
	uint64_t stringLen;
	uint8_t boolValue;
	uint64_t funValue;
	struct EnvStack* funEnvStack;
	uint64_t funArgc;
	uint8_t** funArgs;
	uint64_t funReturn;
	struct Env* structValue;
	struct Value* arrValue;
	uint64_t arrLen;
	uint64_t arrMax;
	uint8_t inUse;
};

// dynamic array
struct ValueStack {
	struct Value* values;
	uint64_t size;
	uint64_t max;
};

struct Node {
	uint8_t c;
	struct Value* value;
	struct Node* children;
	uint8_t size;
	uint8_t max;
};

// prefix tree
struct Env {
	struct Node* head;
	uint64_t stackPos;
};

// dynamic array
struct EnvStack {
	struct Env* envs;
	uint64_t size;
	uint64_t max;
};

#endif /* VMSTRUCTS_H */