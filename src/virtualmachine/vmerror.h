#ifndef VMERROR_H
#define VMERROR_H

#include "virtualmachine.h"

struct VM* vmerror_vm;

enum VMError {
	TYPE_ERROR, NAME_ERROR, INDEX_ERROR, SYNTAX_ERROR
};

void vmerror_raise(enum VMError err, char* msg);

#endif /* VMERROR_H */