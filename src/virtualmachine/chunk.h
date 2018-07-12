#ifndef CHUNK_H
#define CHUNK_H

#include <stdint.h>

struct Chunk {
	uint8_t opcode;
	uint64_t line;
	uint64_t uintArg;
	uint64_t* uintArgs;
	uint8_t boolArg;
	int64_t intArg;
	int64_t* intArgs;
	uint8_t* stringArg;
	uint8_t** stringArgs;
	double floatArg;
	double* floatArgs;
};

// returns number of bytes read
uint64_t chunk_get(struct Chunk* chunk, uint8_t* mem);

// frees chunk
void chunk_free(struct Chunk* chunk);

// prints chunk
void chunk_print(struct Chunk* chunk);

#endif /* CHUNK_H */