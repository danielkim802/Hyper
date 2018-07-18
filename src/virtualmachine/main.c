#include "virtualmachine.h"

int main(int argc, char* argv[]) {
	struct VM* vm = vm_make("../../data/test.hypc", 50, 0);
	vm_run(vm);
	vm_free(vm);
	return 0;
}