#include <stdlib.h>
#include <stdio.h>
#include "chunk.h"
#include "opcode.h"

uint64_t read_opcode(struct Chunk* chunk, uint8_t* mem) {
	chunk->opcode = *mem;
	return 1;
}

uint64_t read_line(struct Chunk* chunk, uint8_t* mem) {
	chunk->line = *((uint64_t*) mem);
	return 8;
}

uint64_t read_bool(struct Chunk* chunk, uint8_t* mem) {
	chunk->boolArg = *mem;
	return 1;
}

uint64_t read_string(struct Chunk* chunk, uint8_t* mem) {
	uint64_t ptr = 0;
	while (mem[ptr++] != 0);
	chunk->stringArg = malloc(sizeof(uint8_t) * ptr);
	for (int i = 0; i < ptr; i ++) {
		chunk->stringArg[i] = mem[i];
	}
	return ptr;
}

uint64_t read_strings(struct Chunk* chunk, uint8_t* mem, uint64_t len) {
	chunk->stringArgs = malloc(sizeof(uint8_t*) * len);
	uint64_t ptr = 0;
	for (uint64_t i = 0; i < len; i ++) {
		uint64_t count = 0;
		while (mem[ptr + count++] != 0);
		chunk->stringArgs[i] = malloc(sizeof(uint8_t) * count);
		for (int j = 0; j < count; j ++) {
			chunk->stringArgs[i][j] = mem[ptr + j];
		}
		ptr += count;
	}
	return ptr;
}

uint64_t read_int(struct Chunk* chunk, uint8_t* mem) {
	chunk->intArg = *((int64_t*) mem);
	return 8;
}

uint64_t read_uint(struct Chunk* chunk, uint8_t* mem) {
	chunk->uintArg = *((uint64_t*) mem);
	return 8;
}

uint64_t read_uints(struct Chunk* chunk, uint8_t* mem, uint64_t len) {
	chunk->uintArgs = (uint64_t*) malloc(sizeof(uint64_t) * len);
	for (uint64_t i = 0; i < len; i ++) {
		chunk->uintArgs[i] = *((uint64_t*) &mem[i * 8]);
	}
	return 8 * len;
}

uint64_t read_float(struct Chunk* chunk, uint8_t* mem) {
	chunk->floatArg = *((double*) mem);
	return 8;
}

uint64_t chunk_get(struct Chunk* chunk, uint8_t* mem) {
	uint64_t ptr = 0;
	ptr += read_opcode(chunk, &mem[ptr]);
	ptr += read_line(chunk, &mem[ptr]);
	switch(chunk->opcode) {
		// expressions
		case AND: break;
		case OR: break;
		case NOT: break;
		case LT: break;
		case LTE: break;
		case GT: break;
		case GTE: break;
		case NE: break;
		case EQ: break;
		case ADD: break;
		case SUB: break;
		case MUL: break;
		case DIV: break;
		case PLUS: break;
		case MINUS: break;
		case FUN_CALL:
			ptr += read_uints(chunk, &mem[ptr], 2);
			break;
		case GET_ATTR:
			ptr += read_string(chunk, &mem[ptr]);
			break;
		case ARR_IDX: break;

		// literals
		case LOAD_INT:
			ptr += read_int(chunk, &mem[ptr]);
			break;
		case LOAD_FLOAT:
			ptr += read_float(chunk, &mem[ptr]);
			break;
		case LOAD_NAME:
			ptr += read_string(chunk, &mem[ptr]);
			break;
		case LOAD_BOOL:
			ptr += read_bool(chunk, &mem[ptr]);
			break;
		case LOAD_NULL: break;
		case LOAD_STRING:
			ptr += read_string(chunk, &mem[ptr]);
			break;
		case MAKE_FUN:
			ptr += read_uints(chunk, &mem[ptr], 2);
			ptr += read_strings(chunk, &mem[ptr], chunk->uintArgs[1]);
			break;
		case MAKE_STRUCT: break;
		case MAKE_ARR:
			ptr += read_uint(chunk, &mem[ptr]);
			break;

		// vm functions
		case PUSH_ENV: break;
		case POP_ENV: break;

		// statements
		case ASSIGN_NAME:
			ptr += read_string(chunk, &mem[ptr]);
			break;
		case STORE_ARR: break;
		case STORE_ATTR:
			ptr += read_string(chunk, &mem[ptr]);
			break;
		case STORE_NAME:
			ptr += read_string(chunk, &mem[ptr]);
			break;
		case RETURN: break;
		case PRINT: break;
		case USE_FILE:
			ptr += read_string(chunk, &mem[ptr]);
			break;

		// control
		case BTRUE:
			ptr += read_uint(chunk, &mem[ptr]);
			break;
		case BFALSE:
			ptr += read_uint(chunk, &mem[ptr]);
			break;
		case JMP:
			ptr += read_uint(chunk, &mem[ptr]);
			break;
		case HALT: break;

		// library functions
		case LEN_ARR: break;
	}

	return ptr;
}

void chunk_free(struct Chunk* chunk) {
	switch(chunk->opcode) {
		// expressions
		case AND: break;
		case OR: break;
		case NOT: break;
		case LT: break;
		case LTE: break;
		case GT: break;
		case GTE: break;
		case NE: break;
		case EQ: break;
		case ADD: break;
		case SUB: break;
		case MUL: break;
		case DIV: break;
		case PLUS: break;
		case MINUS: break;
		case FUN_CALL:
			free(chunk->uintArgs);
			break;
		case GET_ATTR:
			free(chunk->stringArg);
			break;
		case ARR_IDX: break;

		// literals
		case LOAD_INT: break;
		case LOAD_FLOAT: break;
		case LOAD_NAME:
			free(chunk->stringArg);
			break;
		case LOAD_BOOL: break;
		case LOAD_NULL: break;
		case LOAD_STRING:
			free(chunk->stringArg);
			break;
		case MAKE_FUN:
			for (uint64_t i = 0; i < chunk->uintArgs[1]; i ++)
				free(chunk->stringArgs[i]);
			free(chunk->stringArgs);
			free(chunk->uintArgs);
			break;
		case MAKE_STRUCT: break;
		case MAKE_ARR: break;

		// vm functions
		case PUSH_ENV: break;
		case POP_ENV: break;

		// statements
		case ASSIGN_NAME:
			free(chunk->stringArg);
			break;
		case STORE_ARR: break;
		case STORE_ATTR:
			free(chunk->stringArg);
			break;
		case STORE_NAME:
			free(chunk->stringArg);
			break;
		case RETURN: break;
		case PRINT: break;
		case USE_FILE:
			free(chunk->stringArg);
			break;

		// control
		case BTRUE: break;
		case BFALSE: break;
		case JMP: break;
		case HALT: break;

		// library functions
		case LEN_ARR: break;
	}
}

void chunk_print(struct Chunk* chunk) {
	// print line number
	printf("[%4llu] ", chunk->line);

	switch(chunk->opcode) {
		// expressions
		case AND: printf("AND"); break;
		case OR: printf("OR"); break;
		case NOT: printf("NOT"); break;
		case LT: printf("LT"); break;
		case LTE: printf("LTE"); break;
		case GT: printf("GT"); break;
		case GTE: printf("GTE"); break;
		case NE: printf("NE"); break;
		case EQ: printf("EQ"); break;
		case ADD: printf("ADD"); break;
		case SUB: printf("SUB"); break;
		case MUL: printf("MUL"); break;
		case DIV: printf("DIV"); break;
		case PLUS: printf("PLUS"); break;
		case MINUS: printf("MINUS"); break;
		case FUN_CALL: printf("FUN_CALL %llu %llu", chunk->uintArgs[0], chunk->uintArgs[1]); break;
		case GET_ATTR: printf("GET_ATTR %s", chunk->stringArg); break;
		case ARR_IDX: printf("ARR_IDX"); break;

		// literals
		case LOAD_INT: printf("LOAD_INT %lli", chunk->intArg); break;
		case LOAD_FLOAT: printf("LOAD_FLOAT %f", chunk->floatArg); break;
		case LOAD_NAME: printf("LOAD_NAME %s", chunk->stringArg); break;
		case LOAD_BOOL: printf("LOAD_BOOL %i", (int) chunk->boolArg); break;
		case LOAD_NULL: printf("LOAD_NULL"); break;
		case LOAD_STRING: printf("LOAD_STRING \'%s\'", chunk->stringArg); break;
		case MAKE_FUN:
			printf("MAKE_FUN %llu %llu ", chunk->uintArgs[0], chunk->uintArgs[1]);
			for (uint64_t i = 0; i < chunk->uintArgs[1]; i ++)
				printf("%s ", chunk->stringArgs[i]);
			break;
		case MAKE_STRUCT: printf("MAKE_STRUCT"); break;
		case MAKE_ARR: printf("MAKE_ARR %llu", chunk->uintArg); break;

		// vm functions
		case PUSH_ENV: printf("PUSH_ENV"); break;
		case POP_ENV: printf("POP_ENV"); break;

		// statements
		case ASSIGN_NAME: printf("ASSIGN_NAME %s", chunk->stringArg); break;
		case STORE_ARR: printf("STORE_ARR"); break;
		case STORE_ATTR: printf("STORE_ATTR %s", chunk->stringArg); break;
		case STORE_NAME: printf("STORE_NAME %s", chunk->stringArg); break;
		case RETURN: printf("RETURN"); break;
		case PRINT: printf("PRINT"); break;
		case USE_FILE: printf("USE_FILE %s", chunk->stringArg); break;

		// control
		case BTRUE: printf("BTRUE %llu", chunk->uintArg); break;
		case BFALSE: printf("BFALSE %llu", chunk->uintArg); break;
		case JMP: printf("JMP %llu", chunk->uintArg); break;
		case HALT: printf("HALT"); break;

		// library functions
		case LEN_ARR: printf("LEN_ARR"); break;

		default: printf("!!UNKNOWN_FUNC %u", chunk->opcode); break;
	}

	printf("\n");
}