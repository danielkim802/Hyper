#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtualmachine.h"
#include "opcode.h"
#include "chunk.h"
#include "value.h"
#include "valuestack.h"
#include "envstack.h"

struct VM {
	uint8_t* mainMem;
	uint64_t mainMemSize;
	uint64_t pc;
	struct ValueStack* valueStack;
	struct EnvStack* envStack;
};

void exec_and(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	if (value1.type != BOOL || value2.type != BOOL) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	res->boolValue = value1.boolValue && value2.boolValue;
	valuestack_push(vm->valueStack, res);
}

void exec_or(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	if (value1.type != BOOL || value2.type != BOOL) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	res->boolValue = value1.boolValue || value2.boolValue;
	valuestack_push(vm->valueStack, res);
}

void exec_not(struct VM* vm, struct Chunk* chunk) {
	struct Value value;
	valuestack_pop(vm->valueStack, &value);

	if (value.type != BOOL) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	res->boolValue = !value.boolValue;
	valuestack_push(vm->valueStack, res);
}

void exec_lt(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue < value1.intValue; break;
				case FLOAT: res->boolValue = value2.floatValue < value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue < value1.floatValue; break;
				case FLOAT: res->boolValue = value2.floatValue < value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_lte(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue <= value1.intValue; break;
				case FLOAT: res->boolValue = value2.floatValue <= value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue <= value1.floatValue; break;
				case FLOAT: res->boolValue = value2.floatValue <= value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_gt(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue > value1.intValue; break;
				case FLOAT: res->boolValue = value2.floatValue > value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue > value1.floatValue; break;
				case FLOAT: res->boolValue = value2.floatValue > value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_gte(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue >= value1.intValue; break;
				case FLOAT: res->boolValue = value2.floatValue >= value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res->boolValue = value2.intValue >= value1.floatValue; break;
				case FLOAT: res->boolValue = value2.floatValue >= value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_ne(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	if (value1.type != value2.type) {
		res->boolValue = 1;
	} else {
		switch (value1.type) {
			case INT:
				res->boolValue = value1.intValue != value2.intValue;
				break;
			case FLOAT:
				res->boolValue = value1.floatValue != value2.floatValue;
				break;
			case STRING:
				res->boolValue = strcmp((char*) value1.stringValue, (char*) value2.stringValue);
				break;
			case BOOL:
				res->boolValue = (value1.boolValue && value2.boolValue) == 0;
				break;
			case FUN:
				res->boolValue = value1.funValue != value2.funValue;
				break;
			case STRUCT:
				res->boolValue = value1.structValue != value2.structValue;
				break;
			case NIL:
				res->boolValue = 0;
			default:
				break;
		}
	}
	valuestack_push(vm->valueStack, res);
}

void exec_eq(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value1);
	valuestack_pop(vm->valueStack, &value2);

	struct Value* res = malloc(sizeof(struct Value));
	res->type = BOOL;
	if (value1.type != value2.type) {
		res->boolValue = 0;
	} else {
		switch (value1.type) {
			case INT:
				res->boolValue = value1.intValue == value2.intValue;
				break;
			case FLOAT:
				res->boolValue = value1.floatValue == value2.floatValue;
				break;
			case STRING:
				res->boolValue = !strcmp((char*) value1.stringValue, (char*) value2.stringValue);
				break;
			case BOOL:
				res->boolValue = (value1.boolValue && value2.boolValue) != 0;
				break;
			case FUN:
				res->boolValue = value1.funValue == value2.funValue;
				break;
			case STRUCT:
				res->boolValue = value1.structValue == value2.structValue;
				break;
			case NIL:
				res->boolValue = 1;
			default:
				break;
		}
	}
	valuestack_push(vm->valueStack, res);
}

void exec_add(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value2);
	valuestack_pop(vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res->type = INT;
					res->intValue = value1.intValue + value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.intValue + value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res->type = FLOAT;
					res->floatValue = value1.floatValue + value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.floatValue + value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_sub(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value2);
	valuestack_pop(vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res->type = INT;
					res->intValue = value1.intValue - value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.intValue - value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res->type = FLOAT;
					res->floatValue = value1.floatValue - value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.floatValue - value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_mul(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value2);
	valuestack_pop(vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res->type = INT;
					res->intValue = value1.intValue * value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.intValue * value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res->type = FLOAT;
					res->floatValue = value1.floatValue * value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.floatValue * value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_div(struct VM* vm, struct Chunk* chunk) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(vm->valueStack, &value2);
	valuestack_pop(vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT)) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res->type = INT;
					res->intValue = value1.intValue / value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.intValue / value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res->type = FLOAT;
					res->floatValue = value1.floatValue / value2.intValue;
					break;
				case FLOAT:
					res->type = FLOAT;
					res->floatValue = value1.floatValue / value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_plus(struct VM* vm, struct Chunk* chunk) {
	struct Value value;
	valuestack_pop(vm->valueStack, &value);

	if (value.type != INT && value.type != FLOAT) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	switch (value.type) {
		case INT:
			res->type = INT;
			res->intValue = +value.intValue;
			break;
		case FLOAT:
			res->type = FLOAT;
			res->floatValue = +value.floatValue;
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_minus(struct VM* vm, struct Chunk* chunk) {
	struct Value value;
	valuestack_pop(vm->valueStack, &value);

	if (value.type != INT && value.type != FLOAT) {
		// raise type error
	}

	struct Value* res = malloc(sizeof(struct Value));
	switch (value.type) {
		case INT:
			res->type = INT;
			res->intValue = -value.intValue;
			break;
		case FLOAT:
			res->type = FLOAT;
			res->floatValue = -value.floatValue;
			break;
		default:
			break;
	}
	valuestack_push(vm->valueStack, res);
}

void exec_fun_call(struct VM* vm, struct Chunk* chunk) {

}

void exec_get_attr(struct VM* vm, struct Chunk* chunk) {

}

void exec_arr_idx(struct VM* vm, struct Chunk* chunk) {

}

void exec_load_int(struct VM* vm, struct Chunk* chunk) {
	struct Value* value = malloc(sizeof(struct Value));
	value->type = INT;
	value->intValue = chunk->intArg;
	valuestack_push(vm->valueStack, value);
}

void exec_load_float(struct VM* vm, struct Chunk* chunk) {
	struct Value* value = malloc(sizeof(struct Value));
	value->type = FLOAT;
	value->floatValue = chunk->floatArg;
	valuestack_push(vm->valueStack, value);
}

void exec_load_name(struct VM* vm, struct Chunk* chunk) {
}

void exec_load_bool(struct VM* vm, struct Chunk* chunk) {
	struct Value* value = malloc(sizeof(struct Value));
	value->type = BOOL;
	value->boolValue = chunk->boolArg;
	valuestack_push(vm->valueStack, value);
}

void exec_load_null(struct VM* vm, struct Chunk* chunk) {

}

void exec_load_string(struct VM* vm, struct Chunk* chunk) {
	struct Value* value = malloc(sizeof(struct Value));
	value->type = STRING;
	value->stringValue = chunk->stringArg;
	valuestack_push(vm->valueStack, value);
}

void exec_make_fun(struct VM* vm, struct Chunk* chunk) {

}

void exec_make_struct(struct VM* vm, struct Chunk* chunk) {

}

void exec_make_arr(struct VM* vm, struct Chunk* chunk) {

}

void exec_push_env(struct VM* vm, struct Chunk* chunk) {

}

void exec_pop_env(struct VM* vm, struct Chunk* chunk) {

}

void exec_assign_name(struct VM* vm, struct Chunk* chunk) {

}

void exec_store_arr(struct VM* vm, struct Chunk* chunk) {

}

void exec_store_attr(struct VM* vm, struct Chunk* chunk) {

}

void exec_store_name(struct VM* vm, struct Chunk* chunk) {

}

void exec_return(struct VM* vm, struct Chunk* chunk) {

}

void exec_print(struct VM* vm, struct Chunk* chunk) {
	struct Value value;
	valuestack_peek(vm->valueStack, &value);

	switch (value.type) {
		case INT:
			printf("%lli\n", value.intValue);
			break;
		case FLOAT:
			printf("%f\n", value.floatValue);
			break;
		case STRING:
			printf("%s\n", value.stringValue);
			break;
		case BOOL:
			printf(value.boolValue ? "true\n" : "false\n");
			break;
		case FUN:
			printf("fun at %llx\n", value.funValue);
			break;
		case STRUCT:
			printf("struct at %llx\n", value.structValue);
			break;
		default:
			printf("\n");
			break;
	}
}

void exec_btrue(struct VM* vm, struct Chunk* chunk) {

}

void exec_bfalse(struct VM* vm, struct Chunk* chunk) {

}

void exec_jmp(struct VM* vm, struct Chunk* chunk) {

}

void exec_halt(struct VM* vm, struct Chunk* chunk) {
	while (1);
}

void exec_len_arr(struct VM* vm, struct Chunk* chunk) {

}

void (*exec_func[0x28]) (struct VM* vm, struct Chunk* chunk);

void vm_init(struct VM* vm, char* filename) {
	// setup stacks
	vm->valueStack = malloc(sizeof(struct ValueStack));
	vm->envStack = malloc(sizeof(struct EnvStack));
	valuestack_init(vm->valueStack);
	envstack_init(vm->envStack);

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

	// setup jump table
	// expressions
	exec_func[AND]         = exec_and;
	exec_func[OR]          = exec_or;
	exec_func[NOT]         = exec_not;
	exec_func[LT]          = exec_lt;
	exec_func[LTE]         = exec_lte;
	exec_func[GT]          = exec_gt;
	exec_func[GTE]         = exec_gte;
	exec_func[NE]          = exec_ne;
	exec_func[EQ]          = exec_eq;
	exec_func[ADD]         = exec_add;
	exec_func[SUB]         = exec_sub;
	exec_func[MUL]         = exec_mul;
	exec_func[DIV]         = exec_div;
	exec_func[PLUS]        = exec_plus;
	exec_func[MINUS]       = exec_minus;
	exec_func[FUN_CALL]    = exec_fun_call;
	exec_func[GET_ATTR]    = exec_get_attr;
	exec_func[ARR_IDX]     = exec_arr_idx;

	// literals
	exec_func[LOAD_INT]    = exec_load_int;
	exec_func[LOAD_FLOAT]  = exec_load_float;
	exec_func[LOAD_NAME]   = exec_load_name;
	exec_func[LOAD_BOOL]   = exec_load_bool;
	exec_func[LOAD_NULL]   = exec_load_null;
	exec_func[LOAD_STRING] = exec_load_string;
	exec_func[MAKE_FUN]    = exec_make_fun;
	exec_func[MAKE_STRUCT] = exec_make_struct;
	exec_func[MAKE_ARR]    = exec_make_arr;

	// vm functions
	exec_func[PUSH_ENV]    = exec_push_env;
	exec_func[POP_ENV]     = exec_pop_env;

	// statements
	exec_func[ASSIGN_NAME] = exec_assign_name;
	exec_func[STORE_ARR]   = exec_store_arr;
	exec_func[STORE_ATTR]  = exec_store_attr;
	exec_func[STORE_NAME]  = exec_store_name;
	exec_func[RETURN]      = exec_return;
	exec_func[PRINT]       = exec_print;

	// control
	exec_func[BTRUE]       = exec_btrue;
	exec_func[BFALSE]      = exec_bfalse;
	exec_func[JMP]         = exec_jmp;
	exec_func[HALT]        = exec_halt;

	// library functions
	exec_func[LEN_ARR]     = exec_len_arr;
}

void vm_disassemble(struct VM* vm) {
	printf("[%llu bytes read]\n", vm->mainMemSize);
	while (vm->pc < vm->mainMemSize) {
		printf("[%5llu] ", vm->pc);
		struct Chunk chunk;
		vm->pc += chunk_get(&chunk, &vm->mainMem[vm->pc]);
		chunk_free(&chunk);
	}
}

void vm_free(struct VM* vm) {
	free(vm->mainMem);
	valuestack_free(vm->valueStack);
	envstack_free(vm->envStack);
	free(vm->valueStack);
	free(vm->envStack);
}

void vm_exec(struct VM* vm) {
	struct Chunk chunk;
	vm->pc += chunk_get(&chunk, &vm->mainMem[vm->pc]);
	chunk_print(&chunk);
	(*exec_func[chunk.opcode])(vm, &chunk);
	chunk_free(&chunk);
}

int main(void) {
	struct VM vm;
	vm_init(&vm, "../../data/test.hyp.o");
	while (1) {
		vm_exec(&vm);
	}
	return 0;
}