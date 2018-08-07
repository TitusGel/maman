


//
//  operandsAnalysis.c
//  maman14
//
//  Created by roeyki on 29.7.2018.
//  Copyright © 2018 roeyki. All rights reserved.
//
#include "header.h"
/* This file contains functions which handle input analyzing for operations. */

/* Array holding information about each opcode, for internal use */
static Opcode opCodes[NUM_OF_OPCODES] = {
        {"mov", 0, 1705},
        {"cmp", 1, 1769},
        {"add", 2, 1705},
        {"sub", 3, 1705},
        {"not", 4, 42},
        {"clr", 5, 42},
        {"lea", 6, 553},
        {"inc", 7, 42},
        {"dec", 8, 42},
        {"jmp", 9, 58},
        {"bne", 10, 58},
        {"red", 11, 42},
        {"prn", 12, 106},
        {"jsr", 13, 58},
        {"rts", 14, 4},
        {"stop", 15, 4}};

static Pattern legalPatterns[] = {
        {"0 operand", 1<<2},
        {"1 operand", 1<<1},
        {"2 operand", 1},
        {"source addressing method 0", 1<<10},
        {"source addressing method 1", 1<<9},
        {"source addressing method 2", 1<<8},
        {"source addressing method 3", 1<<7},
        {"source addressing method 0", 1<<6},
        {"target addressing method 1", 1<<5},
        {"target addressing method 2", 1<<4},
        {"target addressing method 3", 1<<3}
};

void analyzeOperation(char * currWord, int line, int *IC, char * label)
{

    WordLine *first  = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(first);

    WordLine *second = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(second);

    WordLine *third  = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(third);

    /* Index for loops */
    int i;
    /* How much do we want to increment IC? */
    int increment_value_IC;
    int operandsFound = 0;
    unsigned int patternToCheck = 0;
    int secondAddressingMethod  =-1;
    int thirdAddressingMethod   =-1;
    char secondWord[MAX_LINE]   = "\0";
    char thirdWord[MAX_LINE]    = "\0";
    char command[MAX_LINE]      = "\0";


    strcpy(command,currWord);
    currWord = strtok(NULL, " ,\t\n");
    strcpy(secondWord,currWord);
    currWord = strtok(NULL, " ,\t\n");
    strcpy(thirdWord,currWord);

    if(!command){
        return;
    } else if(secondWord) {

    }



    if ((currWord = strtok(NULL, " ,\t\n"))){


        if (*currWord == '#')
        {
            int num;
            char * ptr;
            char * errorptr;



            ptr = currWord + 1;
            num = strtol(ptr, &errorptr, DECIMAL_BASE);

            /* if number conversion failes */
            if (*errorptr != 0)
            {
                DIE("Invalid Number")
            }

            second->isExternal = FALSE;
            second->line = line;
            second->missingLabel = NULL;
            second->externalLabel = NULL;
            second->word = num << 2;
            second->next = NULL;
            operandsFound++;
            increment_value_IC++;
            secondAddressingMethod =0;

            // Check if the operand is a register
        } else if (isRegister(currWord)){
            /* Update first word and create a second word */


            second->isExternal = FALSE;
            second->line = line;
            second->missingLabel = NULL;
            second->externalLabel = NULL;
            second->word = num << 2;
            second->next = NULL;
            operandsFound++;
            increment_value_IC++;
            secondAddressingMethod = 3;
            second->isExternal = FALSE;
            second->line = line;
            second->missingLabel = NULL;
            second->externalLabel = NULL;


            second->holder.reg.source = (currWord[1] - '0');
            second->holder.reg.destination = 0;
            second->holder.reg.era = 0;
            second->next = NULL;
            increment_value_IC++;

            // must be a label
        } else {

        }




        if(operandsFound > 2){
            DIE("Too many operands")
        } else if(operandsFound == 1) {
            if(secondAddressingMethod == 0)}
    } else if(operandsFound == 2){

    }



    /* Insert number of operands to the first word and to the pattern*/