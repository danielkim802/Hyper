#include "context.h"

void context_init(struct Context* context, struct VM* vm) {
	context->mainMem = vm->mainMem;
	context->mainMemSize = vm->mainMemSize;
	context->valueStack = vm->valueStack;
	context->callStack = vm->callStack;
	context->envStack = vm->envStack;
	context->globalEnv = vm->globalEnv;
	context->pc = vm->pc;
	context->halt = vm->halt;
	context->chunk = vm->chunk;
	context->lastCleaned = vm->lastCleaned;
}