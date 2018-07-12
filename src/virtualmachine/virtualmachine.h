#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <stdint.h>

struct VM;

// sets up the virtual machine
void vm_init(struct VM* vm, char* filename);

// disassembles and prints bytecode
void vm_disassemble(struct VM* vm);

// frees the vm
void vm_free(struct VM* vm);

// executes a single command
void vm_exec(struct VM* vm);

#endif /* VIRTUALMACHINE_H */