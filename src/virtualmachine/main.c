#include "virtualmachine.h"
#include "hypercompiler.h"

int main(int argc, char* argv[]) {
	// check for argument counts
	if (argc == 1) {
		printf("usage: hyper [filename]\n");
		return 0;
	}
	
	// compile to bytecode
	if (!compile(argv[1]))
		return 0;

	// run bytecode
	unsigned int len = strlen(argv[1]);
	char* filename = malloc(len + 2);
	strcpy(filename, argv[1]);
	filename[len] = 'c';
	filename[len + 1] = 0;
	struct VM* vm = vm_make(filename);
	vm_run(vm);
	vm_free(vm);
	return 0;
}