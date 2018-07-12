#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

// expressions
const uint8_t AND         = 0x00;
const uint8_t OR          = 0x01;
const uint8_t NOT         = 0x02;
const uint8_t LT          = 0x03;
const uint8_t LTE         = 0x04;
const uint8_t GT          = 0x05;
const uint8_t GTE         = 0x06;
const uint8_t NE          = 0x07;
const uint8_t EQ          = 0x08;
const uint8_t ADD         = 0x09;
const uint8_t SUB         = 0x0A;
const uint8_t MUL         = 0x0B;
const uint8_t DIV         = 0x0C;
const uint8_t PLUS        = 0x0D;
const uint8_t MINUS       = 0x0E;
const uint8_t FUN_CALL    = 0x0F;
const uint8_t GET_ATTR    = 0x10;
const uint8_t ARR_IDX     = 0x11;

// literals
const uint8_t LOAD_INT    = 0x12;
const uint8_t LOAD_FLOAT  = 0x13;
const uint8_t LOAD_NAME   = 0x14;
const uint8_t LOAD_BOOL   = 0x15;
const uint8_t LOAD_NULL   = 0x16;
const uint8_t LOAD_STRING = 0x17;
const uint8_t MAKE_FUN    = 0x18;
const uint8_t MAKE_STRUCT = 0x19;
const uint8_t MAKE_ARR    = 0x1A;

// vm functions
const uint8_t PUSH_ENV    = 0x1B;
const uint8_t POP_ENV     = 0x1C;

// statements
const uint8_t ASSIGN_NAME = 0x1D;
const uint8_t STORE_ARR   = 0x1E;
const uint8_t STORE_ATTR  = 0x1F;
const uint8_t STORE_NAME  = 0x20;
const uint8_t RETURN      = 0x21;
const uint8_t PRINT       = 0x22;

// control
const uint8_t BTRUE       = 0x23;
const uint8_t BFALSE      = 0x24;
const uint8_t JMP         = 0x25;
const uint8_t HALT        = 0x26;

// library functions
const uint8_t LEN_ARR     = 0x27;

#endif /* OPCODE_H */