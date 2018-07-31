#ifndef OPCODE_H
#define OPCODE_H

#include <stdint.h>

// expressions
static const uint8_t AND         = 0x00;
static const uint8_t OR          = 0x01;
static const uint8_t NOT         = 0x02;
static const uint8_t LT          = 0x03;
static const uint8_t LTE         = 0x04;
static const uint8_t GT          = 0x05;
static const uint8_t GTE         = 0x06;
static const uint8_t NE          = 0x07;
static const uint8_t EQ          = 0x08;
static const uint8_t ADD         = 0x09;
static const uint8_t SUB         = 0x0A;
static const uint8_t MUL         = 0x0B;
static const uint8_t DIV         = 0x0C;
static const uint8_t PLUS        = 0x0D;
static const uint8_t MINUS       = 0x0E;
static const uint8_t FUN_CALL    = 0x0F;
static const uint8_t GET_ATTR    = 0x10;
static const uint8_t ARR_IDX     = 0x11;

// literals
static const uint8_t LOAD_INT    = 0x12;
static const uint8_t LOAD_FLOAT  = 0x13;
static const uint8_t LOAD_NAME   = 0x14;
static const uint8_t LOAD_BOOL   = 0x15;
static const uint8_t LOAD_NULL   = 0x16;
static const uint8_t LOAD_STRING = 0x17;
static const uint8_t MAKE_FUN    = 0x18;
static const uint8_t MAKE_STRUCT = 0x19;
static const uint8_t MAKE_ARR    = 0x1A;

// vm functions
static const uint8_t PUSH_ENV    = 0x1B;
static const uint8_t POP_ENV     = 0x1C;

// statements
static const uint8_t ASSIGN_NAME = 0x1D;
static const uint8_t STORE_ARR   = 0x1E;
static const uint8_t STORE_ATTR  = 0x1F;
static const uint8_t STORE_NAME  = 0x20;
static const uint8_t RETURN      = 0x21;
static const uint8_t PRINT       = 0x22;
static const uint8_t USE_FILE    = 0x23;

// control
static const uint8_t BTRUE       = 0x24;
static const uint8_t BFALSE      = 0x25;
static const uint8_t JMP         = 0x26;
static const uint8_t HALT        = 0x27;

// library functions
static const uint8_t LEN_ARR     = 0x28;
static const uint8_t INPUT       = 0x29;
static const uint8_t EXIT        = 0x2A;

static const uint8_t NUM_CMDS    = 0x2B;

#endif /* OPCODE_H */