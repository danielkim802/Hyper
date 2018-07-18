#include "virtualmachine.h"

int main(int argc, char* argv[]) {
	struct VM* vm = vm_make(argv[1]);
	vm_run(vm);
	vm_free(vm);
	return 0;
}