#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtualmachine.h"
#include "chunk.h"
#include "opcode.h"
#include "vmerror.h"

void exec_and(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);

	if (!value_typeCheck(value1, value2, BOOL, BOOL))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for 'and' operator");

	struct Value* res = value_make(BOOL);
	res->boolValue = value1->boolValue && value2->boolValue;
	valuestack_push(vm->valueStack, res);
}

void exec_or(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);

	if (!value_typeCheck(value1, value2, BOOL, BOOL))
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for 'or' operator");

	struct Value* res = value_make(BOOL);
	res->boolValue = value1->boolValue || value2->boolValue;
	valuestack_push(vm->valueStack, res);
}

void exec_not(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);

	if (value->type != BOOL)
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for 'not' operator");

	struct Value* res = value_make(BOOL);
	res->boolValue = !value->boolValue;
	valuestack_push(vm->valueStack, res);
}

void exec_lt(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(BOOL);

	if (value_typeCheck(value1, value2, INT, INT))
		res->boolValue = value1->intValue < value2->intValue;
	else if (value_typeCheck(value1, value2, INT, FLOAT))
		res->boolValue = value1->intValue < value2->floatValue;
	else if (value_typeCheck(value1, value2, FLOAT, INT))
		res->boolValue = value1->floatValue < value2->intValue;
	else if (value_typeCheck(value1, value2, FLOAT, FLOAT))
		res->boolValue = value1->floatValue < value2->floatValue;
	else
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '<' operator");

	valuestack_push(vm->valueStack, res);
}

void exec_lte(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(BOOL);

	if (value_typeCheck(value1, value2, INT, INT))
		res->boolValue = value1->intValue <= value2->intValue;
	else if (value_typeCheck(value1, value2, INT, FLOAT))
		res->boolValue = value1->intValue <= value2->floatValue;
	else if (value_typeCheck(value1, value2, FLOAT, INT))
		res->boolValue = value1->floatValue <= value2->intValue;
	else if (value_typeCheck(value1, value2, FLOAT, FLOAT))
		res->boolValue = value1->floatValue <= value2->floatValue;
	else
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '<=' operator");

	valuestack_push(vm->valueStack, res);
}

void exec_gt(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(BOOL);

	if (value_typeCheck(value1, value2, INT, INT))
		res->boolValue = value1->intValue > value2->intValue;
	else if (value_typeCheck(value1, value2, INT, FLOAT))
		res->boolValue = value1->intValue > value2->floatValue;
	else if (value_typeCheck(value1, value2, FLOAT, INT))
		res->boolValue = value1->floatValue > value2->intValue;
	else if (value_typeCheck(value1, value2, FLOAT, FLOAT))
		res->boolValue = value1->floatValue > value2->floatValue;
	else
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '>' operator");

	valuestack_push(vm->valueStack, res);
}

void exec_gte(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(BOOL);

	if (value_typeCheck(value1, value2, INT, INT))
		res->boolValue = value1->intValue >= value2->intValue;
	else if (value_typeCheck(value1, value2, INT, FLOAT))
		res->boolValue = value1->intValue >= value2->floatValue;
	else if (value_typeCheck(value1, value2, FLOAT, INT))
		res->boolValue = value1->floatValue >= value2->intValue;
	else if (value_typeCheck(value1, value2, FLOAT, FLOAT))
		res->boolValue = value1->floatValue >= value2->floatValue;
	else
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '>=' operator");

	valuestack_push(vm->valueStack, res);
}

void exec_ne(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(BOOL);

	if (value1->type != value2->type)
		res->boolValue = 1;
	else if (value1->type == INT)
		res->boolValue = value1->intValue != value2->intValue;
	else if (value1->type == FLOAT)
		res->boolValue = value1->floatValue != value2->floatValue;
	else if (value1->type == STRING)
		res->boolValue = strcmp((char*) value1->stringValue, (char*) value2->stringValue);
	else if (value1->type == BOOL)
		res->boolValue = !value1->boolValue != !value2->boolValue;
	else if (value1->type == FUN)
		res->boolValue = value1 != value2;
	else if (value1->type == STRUCT)
		res->boolValue = value1 != value2;
	else if (value1->type == ARR)
		res->boolValue = value1 != value2;
	else if (value1->type == NIL)
		res->boolValue = 0;

	valuestack_push(vm->valueStack, res);
}

void exec_eq(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(BOOL);

	if (value1->type != value2->type)
		res->boolValue = 0;
	else if (value1->type == INT)
		res->boolValue = value1->intValue == value2->intValue;
	else if (value1->type == FLOAT)
		res->boolValue = value1->floatValue == value2->floatValue;
	else if (value1->type == STRING)
		res->boolValue = !strcmp((char*) value1->stringValue, (char*) value2->stringValue);
	else if (value1->type == BOOL)
		res->boolValue = !value1->boolValue == !value2->boolValue;
	else if (value1->type == FUN)
		res->boolValue = value1 == value2;
	else if (value1->type == STRUCT)
		res->boolValue = value1 == value2;
	else if (value1->type == ARR)
		res->boolValue = value1 == value2;
	else if (value1->type == NIL)
		res->boolValue = 1;

	valuestack_push(vm->valueStack, res);
}

void exec_add(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res;

	if (value_typeCheck(value1, value2, INT, INT)) {
		res = value_make(INT);
		res->intValue = value1->intValue + value2->intValue;
	} else if (value_typeCheck(value1, value2, INT, FLOAT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->intValue + value2->floatValue;
	} else if (value_typeCheck(value1, value2, FLOAT, INT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->floatValue + value2->intValue;
	} else if (value_typeCheck(value1, value2, STRING, STRING)) {
		res = value_make(STRING);
		uint64_t len1 = strlen((char*) value1->stringValue);
		uint64_t len2 = strlen((char*) value2->stringValue);
		res->stringValue = malloc(sizeof(uint8_t) * (len1 + len2 + 1));
		strcpy((char*) res->stringValue, (char*) value1->stringValue);
		strcpy((char*) &res->stringValue[len1], (char*) value2->stringValue);
		res->stringValue[len1 + len2] = 0;
		res->stringLen = len1 + len2;
	} else if (value_typeCheck(value1, value2, ARR, ARR)) {
		res = value_make(ARR);
		res->arrValue = malloc(sizeof(struct Value*));
		res->arrLen = 0;
		res->arrMax = 1;
		for (uint64_t i = 0; i < value1->arrLen; i ++)
			value_arrAppend(res, value1->arrValue[i]);
		for (uint64_t i = 0; i < value2->arrLen; i ++)
			value_arrAppend(res, value2->arrValue[i]);
	} else {
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '+' operator");
	}

	valuestack_push(vm->valueStack, res);
}

void exec_sub(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res;

	if (value_typeCheck(value1, value2, INT, INT)) {
		res = value_make(INT);
		res->intValue = value1->intValue - value2->intValue;
	} else if (value_typeCheck(value1, value2, INT, FLOAT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->intValue - value2->floatValue;
	} else if (value_typeCheck(value1, value2, FLOAT, INT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->floatValue - value2->intValue;
	} else {
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '-' operator");
	}

	valuestack_push(vm->valueStack, res);
}

void exec_mul(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res;

	if (value_typeCheck(value1, value2, INT, INT)) {
		res = value_make(INT);
		res->intValue = value1->intValue * value2->intValue;
	} else if (value_typeCheck(value1, value2, INT, FLOAT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->intValue * value2->floatValue;
	} else if (value_typeCheck(value1, value2, FLOAT, INT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->floatValue * value2->intValue;
	} else {
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '*' operator");
	}

	valuestack_push(vm->valueStack, res);
}

void exec_div(struct VM* vm) {
	struct Value* value2 = valuestack_pop(vm->valueStack);
	struct Value* value1 = valuestack_pop(vm->valueStack);
	struct Value* res;

	if (value_typeCheck(value1, value2, INT, INT)) {
		res = value_make(INT);
		res->intValue = value1->intValue / value2->intValue;
	} else if (value_typeCheck(value1, value2, INT, FLOAT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->intValue / value2->floatValue;
	} else if (value_typeCheck(value1, value2, FLOAT, INT)) {
		res = value_make(FLOAT);
		res->floatValue = value1->floatValue / value2->intValue;
	} else {
		vmerror_raise(TYPE_ERROR, "Unsupported operand types for '/' operator");
	}

	valuestack_push(vm->valueStack, res);
}

void exec_plus(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);
	struct Value* res;

	if (value->type == INT) {
		res = value_make(INT);
		res->intValue = +value->intValue;
	} else if (value->type == FLOAT) {
		res = value_make(FLOAT);
		res->floatValue = +value->floatValue;
	} else {
		vmerror_raise(TYPE_ERROR, "Unsupported operand type for '+' operator");
	}

	valuestack_push(vm->valueStack, res);
}

void exec_minus(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);
	struct Value* res;

	if (value->type == INT) {
		res = value_make(INT);
		res->intValue = -value->intValue;
	} else if (value->type == FLOAT) {
		res = value_make(FLOAT);
		res->floatValue = -value->floatValue;
	} else {
		vmerror_raise(TYPE_ERROR, "Unsupported operand type for '-' operator");
	}

	valuestack_push(vm->valueStack, res);
}

void exec_fun_call(struct VM* vm) {
	// get arg count, return address, and setup vm
	uint64_t argc = vm->chunk.uintArgs[0];
	uint64_t returnAddr = vm->chunk.uintArgs[1];
	struct Value* fun = valuestack_pop(vm->valueStack);

	// free chunk
	free(vm->chunk.uintArgs);

	// make a copy of the function (needed for recursive calls)
	struct Value* funcopy = value_make(FUN);
	funcopy->funValue = fun->funValue;
	funcopy->funArgc = fun->funArgc;
	funcopy->funArgs = malloc(sizeof(uint8_t*) * fun->funArgc);
	for (uint64_t i = 0; i < fun->funArgc; i ++) {
		uint64_t len = strlen((char*) fun->funArgs[i]);
		funcopy->funArgs[i] = malloc(sizeof(uint8_t) * (len + 1));
		strcpy((char*) funcopy->funArgs[i], (char*) fun->funArgs[i]);
		funcopy->funArgs[i][len] = 0;
	}
	funcopy->funClosures = fun->funClosures;
	funcopy->funEnvStack = envstack_make();

	// copy environment stack
	envstack_pushEnv(funcopy->funEnvStack, vm->globalEnv);
	for (uint64_t i = 1; i < fun->funEnvStack->size; i ++) {
		envstack_pushEnv(funcopy->funEnvStack, &fun->funEnvStack->envs[i]);
		*envstack_peek(funcopy->funEnvStack)->inUse += 1;
	}
	fun = funcopy;

	// check function argument count
	if (fun->funArgc == 0 && argc != 1)
		vmerror_raise(TYPE_ERROR, "Too many arguments to function call");
	else if (fun->funArgc != 0 && argc > fun->funArgc)
		vmerror_raise(TYPE_ERROR, "Too many arguments to function call");

	// push argument/function environment
	envstack_push(fun->funEnvStack, vm->valueStack->size);

	// assign argument names and values one by one
	if (fun->funArgc == 0) {
		struct Value* nullVal = valuestack_pop(vm->valueStack);

		if (nullVal->type != NIL)
			vmerror_raise(TYPE_ERROR, "Function with no arguments must be called with 'null'");
	} else {
		for (uint64_t i = 0; i < argc; i ++) {
			struct Value* arg = valuestack_pop(vm->valueStack);
			envstack_storeName(fun->funEnvStack, fun->funArgs[i]);
			envstack_assignName(fun->funEnvStack, fun->funArgs[i], arg);
		}
	}

	// make closure if not enough arguments
	if (argc < fun->funArgc) {
		fun->funClosures += 1;
		*envstack_peek(fun->funEnvStack)->inUse += 1;
		uint8_t** newFunArgs = malloc(sizeof(uint8_t*) * (fun->funArgc - argc));
		for (uint64_t i = argc; i < fun->funArgc; i ++)
			newFunArgs[i - argc] = fun->funArgs[i];
		for (uint64_t i = 0; i < argc; i ++)
			free(fun->funArgs[i]);
		free(fun->funArgs);
		fun->funArgs = newFunArgs;
		fun->funArgc -= argc;
		valuestack_push(vm->valueStack, fun);
		return;
	}

	// push to call stack and assign return address
	fun->funReturn = returnAddr;
	valuestack_push(vm->callStack, fun);

	// set pc to function location and set current environment stack
	vm->envStack = fun->funEnvStack;
	vm->pc = fun->funValue;
}

void exec_get_attr(struct VM* vm) {

}

void exec_arr_idx(struct VM* vm) {
	struct Value* idx = valuestack_pop(vm->valueStack);
	struct Value* arr = valuestack_pop(vm->valueStack);

	if (arr->type != ARR && arr->type != STRING)
		vmerror_raise(TYPE_ERROR, "Unsupported operand type for '[ ]' operator");
	if (idx->type != INT) 
		vmerror_raise(INDEX_ERROR, "Index must be an integer");
	if (arr->type == ARR && (idx->intValue >= arr->arrLen || idx->intValue < 0))
		vmerror_raise(INDEX_ERROR, "Index out of range");
	if (arr->type == STRING && (idx->intValue >= arr->stringLen || idx->intValue < 0))
		vmerror_raise(INDEX_ERROR, "Index out of range");

	if (arr->type == ARR) {
		valuestack_push(vm->valueStack, arr->arrValue[idx->intValue]);
		return;
	}

	struct Value* str = value_make(STRING);
	str->stringValue = malloc(sizeof(uint8_t) * 2);
	str->stringLen = 1;
	str->stringValue[0] = arr->stringValue[idx->intValue];
	str->stringValue[1] = 0;
	valuestack_push(vm->valueStack, str);
}

void exec_load_int(struct VM* vm) {
	struct Value* value = value_make(INT);
	value->intValue = vm->chunk.intArg;
	valuestack_push(vm->valueStack, value);
}

void exec_load_float(struct VM* vm) {
	struct Value* value = value_make(FLOAT);
	value->floatValue = vm->chunk.floatArg;
	valuestack_push(vm->valueStack, value);
}

void exec_load_name(struct VM* vm) {
	struct Value* value = envstack_loadName(vm->envStack, vm->chunk.stringArg);
	valuestack_push(vm->valueStack, value);
	free(vm->chunk.stringArg);
}

void exec_load_bool(struct VM* vm) {
	struct Value* value = value_make(BOOL);
	value->boolValue = vm->chunk.boolArg;
	valuestack_push(vm->valueStack, value);
}

void exec_load_null(struct VM* vm) {
	struct Value* value = value_make(NIL);
	valuestack_push(vm->valueStack, value);
}

void exec_load_string(struct VM* vm) {
	struct Value* value = value_make(STRING);
	value->stringValue = vm->chunk.stringArg;
	value->stringLen = strlen((char*) value->stringValue);
	valuestack_push(vm->valueStack, value);
}

void exec_make_fun(struct VM* vm) {
	// setup function
	struct Value* value = value_make(FUN);
	value->funValue = vm->chunk.uintArgs[0];
	value->funArgc = vm->chunk.uintArgs[1];
	value->funArgs = vm->chunk.stringArgs;
	value->funEnvStack = envstack_make();

	// push global environment
	envstack_pushEnv(value->funEnvStack, vm->globalEnv);

	// push surrounding environment (for closure)
	if (envstack_peek(vm->envStack)->head != vm->globalEnv->head) {
		*envstack_peek(vm->envStack)->inUse += 1;
		envstack_pushEnv(value->funEnvStack, envstack_peek(vm->envStack));
	}

	value->funClosures = value->funEnvStack->size;

	free(vm->chunk.uintArgs);
	valuestack_push(vm->valueStack, value);
}

void exec_make_struct(struct VM* vm) {

}

void exec_make_arr(struct VM* vm) {
	struct Value* value = value_make(ARR);
	value->arrLen = vm->chunk.uintArg;

	uint64_t max = 1;
	while (max < value->arrLen)
		max *= 2;

	value->arrValue = malloc(sizeof(struct Value*) * max);

	for (uint64_t i = 0; i < value->arrLen; i ++)
		value->arrValue[i] = valuestack_pop(vm->valueStack);
	valuestack_push(vm->valueStack, value);
}

void exec_push_env(struct VM* vm) {
	envstack_push(vm->envStack, vm->valueStack->size);
}

void exec_pop_env(struct VM* vm) {
	uint64_t pos = envstack_pop(vm->envStack);
	while (vm->valueStack->size > pos)
		valuestack_pop(vm->valueStack);
}

void exec_assign_name(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);
	envstack_assignName(vm->envStack, vm->chunk.stringArg, value);
	free(vm->chunk.stringArg);
}

void exec_store_arr(struct VM* vm) {
	struct Value* arr = valuestack_pop(vm->valueStack);
	struct Value* idx = valuestack_pop(vm->valueStack);
	struct Value* value = valuestack_pop(vm->valueStack);

	if (arr->type != ARR && arr->type != STRING)
		vmerror_raise(TYPE_ERROR, "Unsupported operand type for '[ ]' operator");
	if (idx->type != INT)
		vmerror_raise(INDEX_ERROR, "Index must be an integer");
	if (idx->intValue >= arr->arrLen || idx->intValue < 0)
		vmerror_raise(INDEX_ERROR, "Index out of range");

	arr->arrValue[idx->intValue] = value;
}

void exec_store_attr(struct VM* vm) {

}

void exec_store_name(struct VM* vm) {
	envstack_storeName(vm->envStack, vm->chunk.stringArg);
	free(vm->chunk.stringArg);
}

void exec_return(struct VM* vm) {
	// pop function off call stack
	struct Value* returnFromFun = valuestack_pop(vm->callStack);

	// get return value from value stack and pop function call environments
	struct Value* returnVal = valuestack_pop(vm->valueStack);
	while (vm->envStack->size > returnFromFun->funClosures) {
		uint64_t pos = envstack_pop(vm->envStack);
		while (vm->valueStack->size > pos)
			valuestack_pop(vm->valueStack);
	}

	// push return value to stack
	valuestack_push(vm->valueStack, returnVal);

	// set pc value and set environment stack
	struct Value* fun = valuestack_peek(vm->callStack);
	vm->envStack = fun->funEnvStack;
	vm->pc = returnFromFun->funReturn;
}

void exec_print(struct VM* vm) {
	value_print(valuestack_pop(vm->valueStack));
	printf("\n");
}

void exec_btrue(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);

	if (value->type != BOOL)
		vmerror_raise(TYPE_ERROR, "!!Unsupported operand type for [btrue]");

	if (value->boolValue)
		vm->pc = vm->chunk.uintArg;
}

void exec_bfalse(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);

	if (value->type != BOOL)
		vmerror_raise(TYPE_ERROR, "!!Unsupported operand type for [bfalse]");

	if (!value->boolValue)
		vm->pc = vm->chunk.uintArg;
}

void exec_jmp(struct VM* vm) {
	vm->pc = vm->chunk.uintArg;
}

void exec_halt(struct VM* vm) {
	vm->halt = 1;
}

void exec_len_arr(struct VM* vm) {
	struct Value* value = valuestack_pop(vm->valueStack);
	struct Value* res = value_make(INT);

	if (value->type == ARR)
		res->intValue = value->arrLen;
	else if (value->type == STRING)
		res->intValue = value->stringLen;
	else
		vmerror_raise(TYPE_ERROR, "Unsupported length operand");

	valuestack_push(vm->valueStack, res);
}

void (*exec_func[0x28]) (struct VM* vm);

void vm_init(struct VM* vm, char* filename) {
	// setup stacks
	vm->valueStack = valuestack_make();
	vm->callStack = valuestack_make();
	vm->envStack = envstack_make();
	envstack_push(vm->envStack, 0);
	vm->globalEnv = malloc(sizeof(struct Env));
	*vm->globalEnv = *envstack_peek(vm->envStack);
	*vm->globalEnv->inUse = 1;
	vm->gc = garbagecollector_make();
	value_gc = vm->gc;

	// setup dummy function in call stack for global environment
	struct Value* dummy = value_make(FUN);
	dummy->funEnvStack = vm->envStack;
	valuestack_push(vm->callStack, dummy);

	// open file and get filesize
	FILE* f = fopen(filename, "r");
	fseek(f, 0, SEEK_END);
	uint64_t filesize = ftell(f);
	rewind(f);

	// load main program memory and set pc
	vm->mainMemSize = filesize;
	vm->mainMem = malloc(sizeof(uint8_t) * filesize);
	for (uint64_t i = 0; i < filesize; i ++)
		vm->mainMem[i] = fgetc(f);
	fclose(f);
	vm->pc = 0;
	vm->halt = 0;
	vm->debug = 0;
	vm->cleanPeriod = 10;
	vm->lastCleaned = 0;

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

void vm_free(struct VM* vm) {
	free(vm->mainMem);
	free(vm->globalEnv);
	valuestack_free(vm->valueStack);	
	valuestack_free(vm->callStack);
	envstack_free(vm->envStack);
}

void vm_printValueStack(struct VM* vm) {
	valuestack_print(vm->valueStack);
}

void vm_printEnvStack(struct VM* vm) {
	envstack_print(vm->envStack);
}

void vm_printEnvStacks(struct VM* vm) {
	for (uint64_t i = 0; i < vm->callStack->size; i ++)
		envstack_print(vm->callStack->values[i]->funEnvStack);
}

void vm_printCallStack(struct VM* vm) {
	valuestack_print(vm->callStack);
}

void vm_printGarbage(struct VM* vm) {
	garbagecollector_print(vm->gc);
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

void vm_exec(struct VM* vm) {
	if (vm->debug)
		printf("[%5llu] ", vm->pc);
	vm->pc += chunk_get(&vm->chunk, &vm->mainMem[vm->pc]);
	if (vm->debug)
		chunk_print(&vm->chunk);
	(*exec_func[vm->chunk.opcode])(vm);
	if (vm->debug) {
		printf("\n");
		// vm_printValueStack(vm);
		// vm_printCallStack(vm);
		// vm_printEnvStacks(vm);
		vm_printGarbage(vm);
		printf("\n");
	}
}

void vm_run(struct VM* vm) {
	while (!vm->halt) {
		// execute command
		vm_exec(vm);

		// handle garbage
		if (vm->lastCleaned++ == vm->cleanPeriod) {
			vm->lastCleaned = 0;
			garbagecollector_clean(vm->gc, vm);
		}
	}
}

int main(int argc, char* argv[]) {
	struct VM vm;
	vm_init(&vm,  "../../data/test.hypc");
	vm.debug = 0;
	vm_run(&vm);
	vm_free(&vm);
	return 0;
}