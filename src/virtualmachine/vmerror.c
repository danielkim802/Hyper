#include <stdlib.h>
#include <stdio.h>
#include "vmerror.h"
#include "chunk.h"

void vmerror_raise(enum VMError err, char* msg) {
	printf("line %llu ", vmerror_vm->chunk.line);

	switch (err) {
		case TYPE_ERROR:
			printf("[TypeError]");
			break;
		case NAME_ERROR:
			printf("[NameError]");
			break;
		case INDEX_ERROR:
			printf("[IndexError]");
			break;
		case SYNTAX_ERROR:
			printf("[SyntaxError]");
			break;
		default:
			break;
	}
	printf(": %s\n", msg);
	exit(0);
}