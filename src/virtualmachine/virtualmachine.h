#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <stdint.h>

struct VM;

// sets up the virtual machine
int vm_init(struct VM* vm, char* filename);

// disassembles and prints bytecode
void vm_disassemble(struct VM* vm);

// frees the vm
void vm_free(struct VM* vm);

#endif /* VIRTUALMACHINE_H */