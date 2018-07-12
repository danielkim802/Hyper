#include <stdio.h>
#include <stdlib.h>
#include "virtualmachine.h"
#include "chunk.h"

int vm_init(struct VM* vm, char* filename) {
	// open file and get filesize
	FILE* f = fopen(filename, "r");
	fseek(f, 0L, SEEK_END);
	uint64_t filesize = ftell(f);
	rewind(f);

	// load main program memory and set pc
	vm->mainMemSize = filesize;
	vm->mainMem = (uint8_t*) malloc(sizeof(uint8_t) * filesize);
	for (uint64_t i = 0; i < filesize; i ++)
		vm->mainMem[i] = fgetc(f);
	vm->pc = 0;

	return 0;
}

void vm_disassemble(struct VM* vm) {
	while (vm->pc < vm->mainMemSize) {
		printf("[%5llu] ", vm->pc);
		struct Chunk chunk;
		vm->pc += chunk_get(&chunk, &vm->mainMem[vm->pc]);
		chunk_print(&chunk);
		chunk_free(&chunk);
	}
}

int main(void) {
	struct VM vm;
	vm_init(&vm, "script.txt.o");
	vm_disassemble(&vm);
	return 0;
}