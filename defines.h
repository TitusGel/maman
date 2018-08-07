#ifndef DEFINES_H
#define DEFINES_H

/* This file contains enumerations and defines of constants */


/* boolean enum */
enum boolean {FALSE, TRUE};


/* Magic number */

#define DECIMAL_BASE       10
#define LABEL_LENGTH       32
#define LINE_LENGTH        81
#define WORD_LENGTH        50
#define FILE_NAME_LEN      21
#define NUM_OF_REGISTERS   8
#define OPCODES_AMOUNT     16
#define MAX_CODE_LINES     256
#define MAX_COMMAND_LENGTH 10
#define NUM_OF_OPCODES 15

#define PUSH_SOURCE_OPERAN <<4
#define PUSH_TARGET_OPERAN <<2
#define PUSH_FIRST_PARAMER <<12
#define PUSH_SECOND_PARAME <<10
#define PUSH_OPCODE        <<6
#define PUSH_IMMEDIATE     <<2
#define PUSH_SRC_REG       <<8
#define PUSH_TARG_REG      <<2
#define MAX_LINE 80

/* Error massages */
#define FILE_ERROR "printf('File opening error, please check it and try again');"
#define FILE_NOT_FOUND "printf('File not found, please check it and try again');"

/* Directive enum - used during the first pass to determine which directive has been recieved.
NONE means no directive has been recieved and ERROR means that the string was recognized as a directive but the syntax was wrong*/
enum directive{DIRECTIVE_NONE, DIRECTIVE_DATA, DIRECTIVE_STRING, DIRECTIVE_ENTRY, DIRECTIVE_EXTERN, DIRECTIVE_ERROR};

/* Addressing method enum - NONE means first word */
enum addresing_method {NONE = -1, IMMEDIATE = 0, DIRECT = 1, INDEX = 2, REGISTER = 3};

static char * registers[NUM_OF_REGISTERS] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/* Directive string constants, to be compared with input */
#define STR_DIRECTIVE_DATA   ".data"
#define STR_DIRECTIVE_STRING ".string"
#define STR_DIRECTIVE_ENTRY  ".entry"
#define STR_DIRECTIVE_EXTERN ".extern"




/* Maximum and minimum values that can be stored in 13 bits */
#define DATA_MAX 16383
#define DATA_MIN -16384
#define IMMEDIATE_MAX 4095
#define IMMEDIATE_MIN -4096

/* File extensions */
#define AS_EXTENSION ".as"
#define TXT_EXTENSION ".txt"
#define OB_EXTENSION ".ob"
#define ENT_EXTENSION ".ent"
#define EXT_EXTENSION ".ext"



#define CMP_OPCODE 1
#define ADD_OPCODE 2
#define SUB_OPCODE 3
#define NOT_OPCODE 4
#define CLR_OPCODE 5
#define LEA_OPCODE 6
#define INC_OPCODE 7
#define DEC_OPCODE 8
#define JMP_OPCODE 9
#define BNE_OPCODE 10
#define RED_OPCODE 11
#define PRN 12
#define JSR 13
#define RTS 14
#define STOP 15

/*
 * Adam's category, if you don't like it then we can change it
 * Functional MACROS OR better yet: Multi - lined MACROS ?
 */

#define DIE(MESSAGE)\
addError(#MESSAGE, line, NULL);\
free(first);\
free(second);\
free(third);\
return;

#define OPCODE_POSITION 6

#define SOURCE_OPERAND(SOURCE_TO_GEN) SOURCE_TO_GEN<<12

#define TARGET_OPERAND(TARGET_TO_GEN) TARGET_TO_GEN<<10

#define SOURCE_OPERAND(SOURCE_TO_GEN) SOURCE_TO_GEN<<4

#define TARGET_OPERAND(TARGET_TO_GEN) TARGET_TO_GEN<<2





#endif