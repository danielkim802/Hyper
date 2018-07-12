#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <stdint.h>

enum Type {
	INT, FLOAT, STRING, BOOL, FUN, STRUCT
};

// struct Env {
// 	PrefixTree* tree;
// };

// struct Value {
// 	enum Type type;
// 	int64_t intValue;
// 	double floatValue;
// 	uint8_t boolValue;
// 	uint64_t funValue;
// 	uint64_t structValue;
// 	Env* env;
// };

struct VM {
	uint8_t* mainMem;
	uint64_t mainMemSize;
	uint64_t pc;
	// ValueStack* valueStack;
	// EnvStack* envStack;
};

// sets up the virtual machine
int vm_init(struct VM* vm, char* filename);

// disassembles and prints bytecode
void vm_disassemble(struct VM* vm);

#endif /* VIRTUALMACHINE_H */