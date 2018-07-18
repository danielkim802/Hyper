#ifndef VMCONFIG_H
#define VMCONFIG_H

#include <stdint.h>

// turns debugging on which prints out commands and their byte numbers
static const uint8_t  VM_DEBUG           = 0;

// prints value stack after each command
static const uint8_t  VM_DEBUGVALUESTACK = 0;

// prints call stack after each command
static const uint8_t  VM_DEBUGCALLSTACK  = 0;

// prints all environment stacks after each command
static const uint8_t  VM_DEBUGENVSTACKS  = 0;

// prints all active values in the program after each command
static const uint8_t  VM_DEBUGGARBAGE    = 0;

// determines number of cycles between garbage collection
static const uint64_t VM_CLEANPERIOD     = 1000;

// determines max size of call stack
static const uint64_t VM_MAXCALLSTACK    = 999;

#endif /* VMCONFIG_H */