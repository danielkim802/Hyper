#include "virtualmachine.h"
#include "hypercompiler.h"

int main(int argc, char* argv[]) {
	char* usage = "usage: hyper [filename] [-d]";

	// check for argument counts
	if (argc == 1) {
		printf("%s\n", usage);
		return 0;
	}

	// get arguments
	char* file;
	char disassemble = 0;
	int i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "-d") == 0)
			disassemble = 1;
		else
			file = argv[i];
		i ++;
	}

	// compile to bytecode
	if (!compile(file))
		return 0;

	// run bytecode
	unsigned int len = strlen(file);
	char* filename = malloc(len + 2);
	char* filepath = malloc(len + 2);

	// find  first '/' from the right
	int pos = len - 1;
	while (pos >= 0 && file[pos] != '/')
		pos--;

	// make filename and filepath
	unsigned int namelen = (len - pos) + 1;
	unsigned int pathlen = pos >= 0 ? pos + 2 : 3;
	strcpy(filename, &file[pos + 1]);
	if (pos >= 0)
		strcpy(filepath, file);
	else
		strcpy(filepath, "./");
	filename[namelen - 2] = 'c';
	filename[namelen - 1] = 0;
	filepath[pathlen - 1] = 0;

	// make vm and garbage collector and run
	struct GarbageCollector* gc = garbagecollector_make();
	struct VM* vm = vm_make(filepath, filename, gc);
	if (disassemble)
		vm_disassemble(vm);
	else
		vm_run(vm);
	vm_free(vm);
	garbagecollector_free(gc);
	free(filepath);
	free(filename);
	return 0;
}