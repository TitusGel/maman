#ifndef HEADER_H
#define HEADER_H

/*
How it works:
  Read a line from input.
    -get first token.
    -Determine if first token is a label. if it is, store it in the symbols list.
    -get next token.
    -Is it a directive?
      -If it's .string or .data, store it in the data list and increment DC.
      -If it's .entry, store a new entry with the next token as label.
      -If it's .extern, store it in the symbols list, with next token as label.
    -If it's not a directive, check for command.
    -If it's a command, analize the command and count # of words. increment IC,
      and add the command to the words list.
  Go back to read another line. If there are no more lines:

    Now we have all the symbols and data. Now we'll update every data\string symbols's address.
    If there were errors, we will create an error file and write the errors to that file.
    if there aren't any errors, we will create an object file, and extern/entry files if needed.
*/

/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "defines.h"


/* function implemented in firstStage.c functions */
void firstPass(FILE * f, int * counter, int * DC);
char *  labelNamingValidation(char * label);
char *  labelFormValidation(char * candidate);

/* function implemented in secondStage.c functions */
void secondPass();

/* function implenented in thirdStage.c*/
void thirdStage(char * name);

/* function implemented in errorHandler.c functions */
void addError(char * err, int line, char * str);
void printErrors(char * filename);
void freeErrors();
int isError();
int getMaxLine();

/* function implemented in symbolList.c functions */
void addSymbol(char * label, int address, int external, int operation, int line);
void freeSymbols();
void checkSymbol(char * label, int line);
Label * searchSymbol(char * label);
void updateSymbols(int IC);

/* function implemented in dataList.c functions */
void addNumber(char * number, int line, int * DC);
void splitNumbers(char * token, int line, int * DC);
void freeData();
void addString(char * string, int line, int *DC);
void addChar(char c);
Data * getHeadData();
int countData();
void updateDataAddresses(int IC);

/* function implemented in utils.c functions */
void checkAllocation(void * ptr);
int getDirective(char * token);
int checkComment(char * candidate);
void convertToStrange2(unsigned int word, char* str );
int labelReservedNameValidation(char * str);
FILE * openFile(char* filename, char * mode, char * extension);


/* function implemented in entryList.c functions */
void addEntry(char * label, int line);
void freeEntries();
int checkEntry(char * label);
Entry * getEntry();
Entry * getHeadEntry();

/* function implemented in operandAnalysis.c functions */
void analyzeOperation(char * token, int line, int *IC);
int isRegister(char * token);
int isOperation(char * token);
int secondMethodFormValidation(char * candidate);
void addAdditional(char *param,int method,int isSource,int *IC,int line);
/* function implemented in wordsList.c functions */
void addWordLine(WordLine * node);
void freeWords();
void freeWord(WordLine * ptr);
WordLine * getMissingLabel();
WordLine * getHeadWord();
int countWords();
void updateAddresses();



#endif