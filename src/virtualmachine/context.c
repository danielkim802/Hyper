#include "context.h"

void context_init(struct Context* context, struct VM* vm) {
	context->valueStack = vm->valueStack;
	context->callStack = vm->callStack;
	context->envStack = vm->envStack;
	context->globalEnv = vm->globalEnv;
	context->pc = vm->pc;
	context->halt = vm->halt;
	context->chunk = vm->chunk;
	context->dir = vm->dir;
}