#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <stdint.h>
#include "vmutils.h"

// sets up the virtual machine
void vm_init(struct VM* vm, char* filename);

// creates a virtual machine; collects garbage every 
// 'cleanPeriod' cycles
struct VM* vm_make(char* filename);

// disassembles and prints bytecode
void vm_disassemble(struct VM* vm);

// frees the vm
void vm_free(struct VM* vm);

// executes a single command
void vm_exec(struct VM* vm);

// runs the vm; stops when it hits halt command
void vm_run(struct VM* vm);

#endif /* VIRTUALMACHINE_H */