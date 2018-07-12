#include <stdio.h>
#include <stdlib.h>
#include "virtualmachine.h"
#include "chunk.h"
#include "value.h"
#include "valuestack.h"
#include "envstack.h"

struct VM {
	uint8_t* mainMem;
	uint64_t mainMemSize;
	uint64_t pc;
	struct ValueStack valueStack;
	struct EnvStack envStack;
};

int vm_init(struct VM* vm, char* filename) {
	// setup stacks
	valuestack_init(&vm->valueStack);
	envstack_init(&vm->envStack);

	// open file and get filesize
	FILE* f = fopen(filename, "r");
	fseek(f, 0L, SEEK_END);
	uint64_t filesize = ftell(f);
	rewind(f);

	// load main program memory and set pc
	vm->mainMemSize = filesize;
	vm->mainMem = malloc(sizeof(uint8_t) * filesize);
	for (uint64_t i = 0; i < filesize; i ++)
		vm->mainMem[i] = fgetc(f);
	vm->pc = 0;

	return 0;
}

void vm_disassemble(struct VM* vm) {
	printf("[%llu bytes read]\n", vm->mainMemSize);
	while (vm->pc < vm->mainMemSize) {
		printf("[%5llu] ", vm->pc);
		struct Chunk chunk;
		vm->pc += chunk_get(&chunk, &vm->mainMem[vm->pc]);
		chunk_print(&chunk);
		chunk_free(&chunk);
	}
}

void vm_free(struct VM* vm) {
	free(vm->mainMem);
	valuestack_free(&vm->valueStack);
	envstack_free(&vm->envStack);
}

int main(void) {
	struct VM vm;
	vm_init(&vm, "script.txt.o");
	vm_disassemble(&vm);
	vm_free(&vm);
	return 0;
}