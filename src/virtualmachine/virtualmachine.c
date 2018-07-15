#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtualmachine.h"
#include "opcode.h"
#include "value.h"
#include "valuestack.h"
#include "envstack.h"
#include "vmerror.h"

void exec_and(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	if (value1.type != BOOL || value2.type != BOOL)
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for 'and' operator");

	struct Value res;
	res.type = BOOL;
	res.boolValue = value1.boolValue && value2.boolValue;
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_or(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	if (value1.type != BOOL || value2.type != BOOL)
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for 'or' operator");

	struct Value res;
	res.type = BOOL;
	res.boolValue = value1.boolValue || value2.boolValue;
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_not(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);

	if (value.type != BOOL)
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for 'not' operator");

	struct Value res;
	res.type = BOOL;
	res.boolValue = !value.boolValue;
	valuestack_push(&vm->valueStack, &res);
	value_free(&value);
}

void exec_lt(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '<' operator");

	struct Value res;
	res.type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue < value1.intValue; break;
				case FLOAT: res.boolValue = value2.floatValue < value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue < value1.floatValue; break;
				case FLOAT: res.boolValue = value2.floatValue < value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_lte(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '<=' operator");

	struct Value res;
	res.type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue <= value1.intValue; break;
				case FLOAT: res.boolValue = value2.floatValue <= value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue <= value1.floatValue; break;
				case FLOAT: res.boolValue = value2.floatValue <= value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_gt(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '>' operator");

	struct Value res;
	res.type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue > value1.intValue; break;
				case FLOAT: res.boolValue = value2.floatValue > value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue > value1.floatValue; break;
				case FLOAT: res.boolValue = value2.floatValue > value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_gte(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '>=' operator");

	struct Value res;
	res.type = BOOL;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue >= value1.intValue; break;
				case FLOAT: res.boolValue = value2.floatValue >= value1.intValue; break;
				default: break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: res.boolValue = value2.intValue >= value1.floatValue; break;
				case FLOAT: res.boolValue = value2.floatValue >= value1.floatValue; break;
				default: break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_ne(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	struct Value res;
	res.type = BOOL;
	if (value1.type != value2.type) {
		res.boolValue = 1;
	} else {
		switch (value1.type) {
			case INT:
				res.boolValue = value1.intValue != value2.intValue;
				break;
			case FLOAT:
				res.boolValue = value1.floatValue != value2.floatValue;
				break;
			case STRING:
				res.boolValue = strcmp((char*) value1.stringValue, (char*) value2.stringValue);
				break;
			case BOOL:
				res.boolValue = (value1.boolValue && value2.boolValue) == 0;
				break;
			case FUN:
				res.boolValue = value1.funValue != value2.funValue;
				break;
			case STRUCT:
				res.boolValue = value1.structValue != value2.structValue;
				break;
			case NIL:
				res.boolValue = 0;
				break;
			default:
				break;
		}
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_eq(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value1);
	valuestack_pop(&vm->valueStack, &value2);

	struct Value res;
	res.type = BOOL;
	if (value1.type != value2.type) {
		res.boolValue = 0;
	} else {
		switch (value1.type) {
			case INT:
				res.boolValue = value1.intValue == value2.intValue;
				break;
			case FLOAT:
				res.boolValue = value1.floatValue == value2.floatValue;
				break;
			case STRING:
				res.boolValue = !strcmp((char*) value1.stringValue, (char*) value2.stringValue);
				break;
			case BOOL:
				res.boolValue = (value1.boolValue && value2.boolValue) != 0;
				break;
			case FUN:
				res.boolValue = value1.funValue == value2.funValue;
				break;
			case STRUCT:
				res.boolValue = value1.structValue == value2.structValue;
				break;
			case NIL:
				res.boolValue = 1;
				break;
			default:
				break;
		}
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_add(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value2);
	valuestack_pop(&vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		if (!(value1.type == STRING && value2.type == STRING))
			vmerror_raise(TYPE_ERROR, "Unsupported operand types for '+' operator");

	struct Value res;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res.type = INT;
					res.intValue = value1.intValue + value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.intValue + value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res.type = FLOAT;
					res.floatValue = value1.floatValue + value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.floatValue + value2.floatValue;
					break;
				default:
					break;
			}
			break;
		uint64_t len1;
		uint64_t len2;
		case STRING:
			res.type = STRING;
			len1 = strlen((char*) value1.stringValue);
			len2 = strlen((char*) value2.stringValue);
			res.stringValue = malloc(sizeof(uint8_t) * (len1 + len2 + 1));
			strcpy((char*) res.stringValue, (char*) value1.stringValue);
			strcpy((char*) &res.stringValue[len1], (char*) value2.stringValue);
			res.stringValue[len1 + len2] = 0;
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_sub(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value2);
	valuestack_pop(&vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '-' operator");

	struct Value res;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res.type = INT;
					res.intValue = value1.intValue - value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.intValue - value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res.type = FLOAT;
					res.floatValue = value1.floatValue - value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.floatValue - value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_mul(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value2);
	valuestack_pop(&vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '*' operator");

	struct Value res;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res.type = INT;
					res.intValue = value1.intValue * value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.intValue * value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res.type = FLOAT;
					res.floatValue = value1.floatValue * value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.floatValue * value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_div(struct VM* vm) {
	struct Value value1;
	struct Value value2;
	valuestack_pop(&vm->valueStack, &value2);
	valuestack_pop(&vm->valueStack, &value1);

	if ((value1.type != INT && value1.type != FLOAT) || 
		(value2.type != INT && value2.type != FLOAT))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '/' operator");

	struct Value res;
	switch (value1.type) {
		case INT:
			switch(value2.type) {
				case INT:
					res.type = INT;
					res.intValue = value1.intValue / value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.intValue / value2.floatValue;
					break;
				default:
					break;
			}
			break;
		case FLOAT:
			switch(value2.type) {
				case INT: 
					res.type = FLOAT;
					res.floatValue = value1.floatValue / value2.intValue;
					break;
				case FLOAT:
					res.type = FLOAT;
					res.floatValue = value1.floatValue / value2.floatValue;
					break;
				default:
					break;
			}
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value1);
	value_free(&value2);
}

void exec_plus(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);

	if (value.type != INT && value.type != FLOAT)
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '+' operator");

	struct Value res;
	switch (value.type) {
		case INT:
			res.type = INT;
			res.intValue = +value.intValue;
			break;
		case FLOAT:
			res.type = FLOAT;
			res.floatValue = +value.floatValue;
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value);
}

void exec_minus(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);

	if (value.type != INT && value.type != FLOAT)
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '-' operator");

	struct Value res;
	switch (value.type) {
		case INT:
			res.type = INT;
			res.intValue = -value.intValue;
			break;
		case FLOAT:
			res.type = FLOAT;
			res.floatValue = -value.floatValue;
			break;
		default:
			break;
	}
	valuestack_push(&vm->valueStack, &res);
	value_free(&value);
}

void exec_fun_call(struct VM* vm) {
	// get arg count and return address
	uint64_t argc = vm->chunk.uintArgs[0];
	uint64_t returnAddr = vm->chunk.uintArgs[1];

	// push new environment to store 
}

void exec_get_attr(struct VM* vm) {

}

void exec_arr_idx(struct VM* vm) {
	struct Value arr;
	struct Value idx;
	valuestack_pop(&vm->valueStack, &idx);
	valuestack_pop(&vm->valueStack, &arr);

	if (arr.type != ARR && arr.type != STRING)
		vmerror_raise(TYPE_ERROR, "Unsupported operand type for '[ ]' operator");
	if (idx.type != INT) 
		vmerror_raise(INDEX_ERROR, "Index must be an integer");
	if (arr.type == ARR && (idx.intValue >= arr.arrLen || idx.intValue < 0))
		vmerror_raise(INDEX_ERROR, "Index out of range");
	if (arr.type == STRING && (idx.intValue >= arr.stringLen || idx.intValue < 0))
		vmerror_raise(INDEX_ERROR, "Index out of range");

	if (arr.type == ARR) {
		valuestack_push(&vm->valueStack, &arr.arrValue[idx.intValue]);
		return;
	}
	struct Value str;
	str.type = STRING;
	str.stringValue = malloc(sizeof(uint8_t) * 1);
	str.stringLen = 1;
	str.stringValue[0] = arr.stringValue[idx.intValue];
	valuestack_push(&vm->valueStack, &str);
}

void exec_load_int(struct VM* vm) {
	struct Value value;
	value.type = INT;
	value.intValue = vm->chunk.intArg;
	valuestack_push(&vm->valueStack, &value);
}

void exec_load_float(struct VM* vm) {
	struct Value value;
	value.type = FLOAT;
	value.floatValue = vm->chunk.floatArg;
	valuestack_push(&vm->valueStack, &value);
}

void exec_load_name(struct VM* vm) {
	struct Value value;
	envstack_loadName(&vm->envStack, vm->chunk.stringArg, &value);
	valuestack_push(&vm->valueStack, &value);
}

void exec_load_bool(struct VM* vm) {
	struct Value value;
	value.type = BOOL;
	value.boolValue = vm->chunk.boolArg;
	valuestack_push(&vm->valueStack, &value);
}

void exec_load_null(struct VM* vm) {
	struct Value value;
	value.type = NIL;
	valuestack_push(&vm->valueStack, &value);
}

void exec_load_string(struct VM* vm) {
	struct Value value;
	value.type = STRING;
	value.stringValue = vm->chunk.stringArg;
	value.stringLen = strlen((char*) value.stringValue);
	valuestack_push(&vm->valueStack, &value);
}

void exec_make_fun(struct VM* vm) {
	struct Value value;
	value.type = FUN;
	value.funValue = vm->chunk.uintArgs[0];
	value.funArgc = vm->chunk.uintArgs[1];
	value.funArgs = vm->chunk.stringArgs;
	free(vm->chunk.uintArgs);
	valuestack_push(&vm->valueStack, &value);
}

void exec_make_struct(struct VM* vm) {

}

void exec_make_arr(struct VM* vm) {
	struct Value value;
	value.type = ARR;
	value.arrLen = vm->chunk.uintArg;

	uint64_t max = 1;
	while (max < value.arrLen)
		max *= 2;

	value.arrValue = malloc(sizeof(struct Value) * max);

	for (uint64_t i = 0; i < value.arrLen; i ++)
		valuestack_pop(&vm->valueStack, &value.arrValue[i]);
	valuestack_push(&vm->valueStack, &value);
}

void exec_push_env(struct VM* vm) {
	struct Env env;
	env_init(&env, vm->valueStack.size);
	envstack_push(&vm->envStack, &env);
}

void exec_pop_env(struct VM* vm) {
	struct Env env;
	envstack_pop(&vm->envStack, &env);
	while (vm->valueStack.size > env.stackPos) {
		struct Value value;
		valuestack_pop(&vm->valueStack, &value);
		value_free(&value);
	}
	env_free(&env);
}

void exec_assign_name(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);
	envstack_assignName(&vm->envStack, vm->chunk.stringArg, &value);
}

void exec_store_arr(struct VM* vm) {
	struct Value value;
	struct Value idx;
	struct Value arr;
	valuestack_pop(&vm->valueStack, &arr);
	valuestack_pop(&vm->valueStack, &idx);
	valuestack_pop(&vm->valueStack, &value);

	if (arr.type != ARR && arr.type != STRING)
		vmerror_raise(TYPE_ERROR, "Unsupported operand type for '[ ]' operator");
	if (idx.type != INT)
		vmerror_raise(INDEX_ERROR, "Index must be an integer");
	if (idx.intValue >= arr.arrLen || idx.intValue < 0)
		vmerror_raise(INDEX_ERROR, "Index out of range");

	arr.arrValue[idx.intValue] = value;
}

void exec_store_attr(struct VM* vm) {

}

void exec_store_name(struct VM* vm) {
	envstack_storeName(&vm->envStack, vm->chunk.stringArg);
}

void exec_return(struct VM* vm) {

}

void exec_print_helper(struct Value* value) {
	switch (value->type) {
		case INT:
			printf("%lli", value->intValue);
			break;
		case FLOAT:
			printf("%f", value->floatValue);
			break;
		case STRING:
			printf("%s", value->stringValue);
			break;
		case BOOL:
			printf(value->boolValue ? "true" : "false");
			break;
		case FUN:
			printf("-fun at 0x%llx-", value->funValue);
			break;
		case STRUCT:
			printf("-struct-");
			break;
		case ARR:
			printf("[");
			for (uint64_t i = 0; i < value->arrLen; i ++) {
				if (value->arrValue[i].type == ARR)
					printf("-arr-");
				else
					exec_print_helper(&value->arrValue[i]);
				if (i != value->arrLen - 1)
					printf(", ");
			}
			printf("]");
			break;
		case NIL:
			printf("null");
			break;
		default: 
			break;
	}
}

void exec_print(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);
	exec_print_helper(&value);
	printf("\n");
}

void exec_btrue(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);

	if (value.type != BOOL)
		vmerror_raise(TYPE_ERROR, "!!Unsupported operand type for [btrue]");

	if (!value.boolValue)
		return;
	vm->pc = vm->chunk.uintArg;
}

void exec_bfalse(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);

	if (value.type != BOOL)
		vmerror_raise(TYPE_ERROR, "!!Unsupported operand type for [bfalse]");

	if (value.boolValue)
		return;
	vm->pc = vm->chunk.uintArg;
}

void exec_jmp(struct VM* vm) {
	vm->pc = vm->chunk.uintArg;
}

void exec_halt(struct VM* vm) {
	vm->halt = 1;
}

void exec_len_arr(struct VM* vm) {
	struct Value value;
	valuestack_pop(&vm->valueStack, &value);

	if (value.type != ARR | value.type != STRING)
		vmerror_raise(TYPE_ERROR, "Unsupported length operand");

	struct Value res;
	res.type = INT;
	if (value.type == ARR)
		res.intValue = value.arrLen;
	if (value.type == STRING)
		res.intValue = value.stringLen;
	valuestack_push(&vm->valueStack, &res);
}

void (*exec_func[0x28]) (struct VM* vm);

void vm_init(struct VM* vm, char* filename) {
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
	vm->halt = 0;
	vm->debug = 0;

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

	// setup error
	vmerror_vm = vm;
}

void vm_printValueStack(struct VM* vm) {
	printf("\n----valuestack\n");
	for (uint64_t i = 0; i < vm->valueStack.size; i ++) {
		printf("[%lli] ", i);
		exec_print_helper(&vm->valueStack.values[i]);
		printf("\n");
	}
	printf("----valuestack end\n\n");
}

void vm_disassemble(struct VM* vm) {
	printf("[%llu bytes read]\n", vm->mainMemSize);
	printf(" byte   line\n");
	while (vm->pc < vm->mainMemSize) {
		printf("[%4llu] ", vm->pc);
		vm->pc += chunk_get(&vm->chunk, &vm->mainMem[vm->pc]);
		chunk_print(&vm->chunk);
		chunk_free(&vm->chunk);
	}
}

void vm_free(struct VM* vm) {
	free(vm->mainMem);
	valuestack_free(&vm->valueStack);
	envstack_free(&vm->envStack);
}

void vm_exec(struct VM* vm) {
	if (vm->debug)
		printf("[%5llu] ", vm->pc);
	vm->pc += chunk_get(&vm->chunk, &vm->mainMem[vm->pc]);
	if (vm->debug)
		chunk_print(&vm->chunk);
	(*exec_func[vm->chunk.opcode])(vm);
	// vm_printValueStack(vm);
}

void vm_run(struct VM* vm) {
	while (!vm->halt)
		vm_exec(vm);
}

int main(int argc, char* argv[]) {
	struct VM vm;
	vm_init(&vm, "../../data/test.hypc");
	vm.debug = 0;
	vm_disassemble(&vm);
	vm_free(&vm);
	return 0;
}