#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <stdint.h>

struct VM {
	uint8_t* mainMem;
	uint64_t mainMemSize;
	struct ValueStack* valueStack;
	struct EnvStack* envStack;
	struct Chunk* chunk;
	uint64_t pc;
	uint8_t halt;
	uint8_t debug;
};

// sets up the virtual machine
void vm_init(struct VM* vm, char* filename);

// disassembles and prints bytecode
void vm_disassemble(struct VM* vm);

// frees the vm
void vm_free(struct VM* vm);

// executes a single command
void vm_exec(struct VM* vm);

// runs the vm; stops when it hits halt command
void vm_run(struct VM* vm);

#endif /* VIRTUALMACHINE_H */