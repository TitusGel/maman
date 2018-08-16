#ifndef STRUCTS_H
#define STRUCTS_H


typedef struct wordLine
{
    unsigned int word:14;
    /*The address assigned to it*/
    int address;
    /*Which line in input?*/
    int line;
    /* Missing label */
    char * missingLabel;
    /* Label of external */
    char * externalLabel;
    /* Is it external? */
    int isExternal;
    /* Pointer to next */
    struct wordLine * next;

} WordLine;


/* definition of error type. used in the errors list */
typedef struct error
{
    /* The error itself */
    char * str_error;
    /* The line where the error was found */
    int line;
    /* The string where the error was found */
    char * str;
    /* Pointer to next */
    struct error * next;
} Error;

/*definition of symbol type
contains a string (label),
address, and external/operation flags*/
typedef struct label   /* yo royy how did I let you change this to "label" I objecT!*/
{
    /* Label of the symbol */
    char * label;
    /* Address of the symbol */
    int address;
    /* Is it external? */
    int external;
    /* is it an operation? */
    int operation;
    /* pointer to next */
    struct label* next;
} Label;

/*definition of data type
contains an address and a value (which stores the number/char)*/
typedef struct data
{
    /* address of data word */
    int address;
    /* Value */
    int word:14;
    /* pointer to next */
    struct data * next;
} Data;

/*definition of entry type
Contains a label, an address, an address flag, a line value,
and a pointer to the next node*/
typedef struct entry
{
    /*Label of entry */
    char * label;
    /*Address */
    int address;
    /* Does it have an address? (for internal use) */
    int hasAddress;
    /* Line of entry */
    int line;
    /* pointer to next*/
    struct entry * next;
} Entry;



typedef struct pattern
{
    /* The operation string */
    char * str;
    /* The group */
    unsigned int p;
} Pattern;

typedef struct opcode
{
    /* The operation string */
    char * str;
    /* The opcode */
    int code;
    /* The group */
    unsigned int p;
} Opcode;

#endif