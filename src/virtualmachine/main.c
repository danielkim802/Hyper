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
	char* filepath = malloc(len + 2);

	// find  first '/' from the right
	int pos = len - 1;
	while (pos >= 0 && argv[1][pos] != '/')
		pos--;

	// make filename and filepath
	unsigned int namelen = (len - pos) + 1;
	unsigned int pathlen = pos >= 0 ? pos + 2 : 3;
	strcpy(filename, &argv[1][pos + 1]);
	if (pos >= 0)
		strcpy(filepath, argv[1]);
	else
		strcpy(filepath, "./");
	filename[namelen - 2] = 'c';
	filename[namelen - 1] = 0;
	filepath[pathlen - 1] = 0;

	// make vm and garbage collector and run
	struct GarbageCollector* gc = garbagecollector_make();
	struct VM* vm = vm_make(filepath, filename, gc);
	vm_run(vm);
	vm_free(vm);
	garbagecollector_free(gc);
	free(filepath);
	free(filename);
	return 0;
}