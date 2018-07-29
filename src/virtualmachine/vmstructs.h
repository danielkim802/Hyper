#ifndef VMSTRUCTS_H
#define VMSTRUCTS_H

#include <stdint.h>
#include "chunk.h"

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
	struct ValueStack* funClosureStack;
	struct Env* structValue;
	struct Value** arrValue;
	uint64_t arrLen;
	uint64_t arrMax;
	uint8_t marked;
};

// dynamic array
struct ValueStack {
	struct Value** values;
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
	uint64_t* inUse;
};

// dynamic array
struct EnvStack {
	struct Env* envs;
	uint64_t size;
	uint64_t max;
};

struct GarbageCollector {
	struct Value** values;
	uint64_t size;
	uint64_t max;
};

struct Context {
	uint8_t* mainMem;
	uint64_t mainMemSize;
	struct ValueStack* valueStack;
	struct ValueStack* callStack;
	struct EnvStack* envStack;
	struct Env* globalEnv;

	uint64_t pc;
	uint8_t halt;
	struct Chunk chunk;
};

struct ContextStack {
	struct Context* contexts;
	uint64_t size;
	uint64_t max;
};

// virtual machine
struct VM {
	// main datastructures
	uint8_t* mainMem;
	uint64_t mainMemSize;
	struct ValueStack* valueStack;
	struct ValueStack* callStack;
	struct EnvStack* envStack;
	struct Env* globalEnv;
	struct ContextStack* contextStack;
	struct GarbageCollector* gc;

	// control fields
	uint64_t pc;
	uint8_t halt;
	struct Chunk chunk;
	uint8_t* dir;

	// flags
	uint8_t debug;
	uint8_t debugValueStack;
	uint8_t debugCallStack;
	uint8_t debugEnvStacks;
	uint8_t debugGarbage;
	uint64_t maxCallStack;
	uint64_t cleanPeriod;
	uint64_t lastCleaned;
};

#endif /* VMSTRUCTS_H */