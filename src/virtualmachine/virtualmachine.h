#ifndef VIRTUALMACHINE_H
#define VIRTUALMACHINE_H

#include <stdint.h>
#include "vmutils.h"

// sets up the virtual machine
void vm_init(struct VM* vm, char* filepath, char* filename, struct GarbageCollector* gc);

// creates a virtual machine; collects garbage every
// 'cleanPeriod' cycles
struct VM* vm_make(char* filepath, char* filename, struct GarbageCollector* gc);

// disassembles and prints bytecode
void vm_disassemble(struct VM* vm);

// loads a context into the vm
void vm_loadContext(struct VM* vm, struct Context* context);

// returns current pc offset
uint64_t vm_pcOffset(struct VM* vm);

// frees the vm
void vm_free(struct VM* vm);

// executes a single command
void vm_exec(struct VM* vm);

// runs the vm; stops when it hits halt command
void vm_run(struct VM* vm);

#endif /* VIRTUALMACHINE_H */