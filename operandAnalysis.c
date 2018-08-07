#define DIE(MESSAGE)\
addError(#MESSAGE, line, NULL);\
freeWord(first);\
freeWord(second);\
freeWord(third);\
return;
#include "header.h"

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
        {"source addressing method 0", 1<<10},
        {"source addressing method 1", 1<<9},
        {"source addressing method 2", 1<<8},
        {"source addressing method 3", 1<<7},
        {"target addressing method 0", 1<<6},
        {"target addressing method 1", 1<<5},
        {"target addressing method 2", 1<<4},
        {"target addressing method 3", 1<<3},
        {"0 operand", 1<<2},
        {"1 operand", 1<<1},
        {"2 operand", 1}
};

void analyzeOperation(char * currWord, int line, int *IC, char * label)
{
    WordLine *first  = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(first);
    WordLine *second = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(second);
    WordLine *third  = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(third);
    /*Index for loops*/
    int i;
    /*How much do we want to increment IC?*/
    int increment_value_IC;
    int commandFound = -1;
    unsigned int patternToCheck = 0;
    int secondAddressingMethod  =-1;
    int thirdAddressingMethod   =-1;
    int secondParameteAddressingMethod = -1;
    int firstParameterAddressingMethod = -1;
    char secondWord[MAX_LINE]   = "\0";
    char thirdWord[MAX_LINE]    = "\0";

    // parameters
    char *firstParameter;
    char *secondParamete;
    char *temp;
    int firstOpen;
    char command[MAX_LINE]  = "\0";
    char *parameterHolder   = 0;
    strcpy(command,currWord);
    currWord = strtok(NULL, " ,\t\n");
    strcpy(secondWord,currWord);
    currWord = strtok(NULL, " ,\t\n");
    strcpy(thirdWord,currWord);

    if(strtok(NULL, " ,\t\n")){
        DIE("Too many operands..")
    }
    if(!*command){
        return;
    }



    if(*secondWord){

        /* Check if the word is an imidiete operand */
        if (*secondWord == '#')
        {
            int num;
            char * ptr;
            char * errorptr;
            ptr = secondWord + 1;
            num = strtol(ptr, &errorptr, DECIMAL_BASE);

            /*if number conversion failes*/
            if (*errorptr)
            {
                DIE("Invalid Number")
            }
            secondAddressingMethod = 0;
            /* Check if the word is an register operand */
        } else if (isRegister(secondWord)){
            secondAddressingMethod = 3;

            /* After nagative result for the first two It is methods 1/2 */
        } else {
            firstOpen = secondMethodFormValidation(secondWord);
            if(firstOpen){
                secondAddressingMethod = 2;
                firstParameter = &secondWord[firstOpen+1];
                temp = firstParameter;
                while(*temp != ','){temp++;}
                *temp = '\0';
                temp ++;
                secondParamete = temp;
                while(*temp != ')'){temp++;}
                *temp = '\0';

                if (*firstParameter == '#')
                {
                    int num2;
                    char * ptr2;
                    char * errorptr2;
                    ptr2 = firstParameter + 1;
                    num2 = strtol(ptr2, &errorptr2, DECIMAL_BASE);

                    /*if number conversion failes*/
                    if (*errorptr2)
                    {
                        DIE("Invalid Number")
                    }
                    firstParameterAddressingMethod = 0;
                    // Check if the operand is a register
                } else if (isRegister(firstParameter)){
                    firstParameterAddressingMethod = 3;

                    // must be a label
                } else {
                    if(labelNamingValidation(secondWord)){
                        firstParameterAddressingMethod = 1;
                    } else {
                        DIE("Invalid label naming")
                    }
                }
                if (*secondParamete == '#')
                {
                    int num2;
                    char * ptr2;
                    char * errorptr2;
                    ptr2 = secondParamete + 1;
                    num2 = strtol(ptr2, &errorptr2, DECIMAL_BASE);

                    /*if number conversion failes*/
                    if (*errorptr2)
                    {
                        DIE("Invalid Number")
                    }
                    secondParameteAddressingMethod = 0;
                    // Check if the operand is a register
                } else if (isRegister(secondParamete)){
                    secondParameteAddressingMethod = 3;

                    // must be a label
                } else {
                    if(labelNamingValidation(secondParamete)){
                        secondParameteAddressingMethod = 1;
                    } else {
                        DIE("Invalid label naming")
                    }
                }

                /* The second word's method must be 1 */
            } else {
                if(labelNamingValidation(secondWord)){
                    secondAddressingMethod = 1;
                } else {
                    DIE("Invalid label naming")
                }
            }
        }
        if(*thirdWord){
            if (*thirdWord == '#')
            {
                int num2;
                char * ptr2;
                char * errorptr2;
                ptr2 = thirdWord + 1;
                num2 = strtol(ptr2, &errorptr2, DECIMAL_BASE);

                /*if number conversion failes*/
                if (*errorptr2)
                {
                    DIE("Invalid Number")
                }
                thirdAddressingMethod = 0;
                // Check if the operand is a register
            } else if (isRegister(thirdWord)){
                thirdAddressingMethod = 3;

                // must be a label
            } else {
                if(labelNamingValidation(secondWord)){
                    secondAddressingMethod = 1;
                } else {
                    DIE("Invalid label naming")
                }
            }
        }
    }


    /* If we have recieved no operands at all */
    if(secondAddressingMethod == -1) {

        /*  Update the oparand count*/
        patternToCheck |= legalPatterns[8].p;

        /* If we have recieved only one operand */
    } else if(thirdAddressingMethod == -1){
        patternToCheck |= legalPatterns[9].p;
        patternToCheck |= legalPatterns[secondAddressingMethod].p;
        first->Word    |= (secondAddressingMethod PUSH_TARGET_OPERAN);
        /* If we have recieved two operands */
    } else {
        patternToCheck |= legalPatterns[10].p;
        patternToCheck |= legalPatterns[secondAddressingMethod].p;
        patternToCheck |= legalPatterns[thirdAddressingMethod+4].p;
        first->Word    |= (thirdAddressingMethod PUSH_TARGET_OPERAN);
        first->Word    |= (thirdAddressingMethod PUSH_TARGET_OPERAN);
    }

    /* Now check if the pattern is valid to the command
    
     ( It sould not have flags that the command w'ont support)
     So - we will check if the & of the pattern to check and the
     supported patterns is equals to the pattern to check
     */
    i = NUM_OF_OPCODES;
    while(i--){
        if(!strcmp(command, opCodes[i-1])){commandFound = i-1;}
    }
    if(!(commandFound+1)){
        DIE("Command not found")
    }
    if((patternToCheck & opCodes[commandFound]->p) == patternToCheck){
        first->line  = line;
        second->line = line;
        third->line  = line;
        first->word |= opCodes[commandFound]->code PUSH_OPCODE;

        if(secondAddressingMethod != -1 ){
            /* If we have only one operand */
            if(thirdAddressingMethod == -1 ){
                if(secondAddressingMethod == 2){
                    if(firstParameterAddressingMethod == 3 && secondParameteAddressingMethod == 3){
                        /* TODO : create word menualy*/

                    } else {
                        addAdditional(firstParameter,firstParameterAddressingMethod,0,IC);
                        addAdditional(secondParamete,secondParameteAddressingMethod,1,IC);
                    }
                }else{
                    addAdditional(secondWord,secondAddressingMethod,0,IC);
                }
                /* If we have two operands */
            } else {
                if(secondAddressingMethod == 3 && thirdAddressingMethod == 3){
                    /* TODO : create word menualy*/

                    /* Send two operands to creation*/
                } else {
                    addAdditional(secondWord,secondAddressingMethod,0,IC);
                    addAdditional(thirdWord,secondAddressingMethod,1,IC);
                }
            }

        }

    } else {
        patternToCheck ^= patternToCheck & opCodes->p;
        /*Check for the unsupported flag and print the error*/
        for(i=0;i<11; i++){
            if(patternToCheck & legalPatterns[i].p) {
                DIE(legalPatterns[i].str)
            }
        }
    }
}











