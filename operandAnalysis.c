#include "header.h"

#define DIE(MESSAGE)\
addError(MESSAGE, line, NULL);\
freeWord(first);\
return;


Opcode opCodes[NUM_OF_OPCODES] = {
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



Pattern legalPatterns[] = {
        {"command does not support source addressing method 0", 1<<10},
        {"command does not support source addressing method 1", 1<<9},
        {"command does not support source addressing method 2", 1<<8},
        {"command does not support source addressing method 3", 1<<7},
        {"command does not support target addressing method 0", 1<<6},
        {"command does not support target addressing method 1", 1<<5},
        {"command does not support target addressing method 2", 1<<4},
        {"command does not support target addressing method 3", 1<<3},
        {"command does not support 0 operand", 1<<2},
        {"command does not support 1 operand", 1<<1},
        {"command does not support 2 operand", 1}
};

void analyzeOperation(char * currWord, int line, int *IC)
{


    /*Index for loops*/
    int i;
    /*How much do we want to increment IC?*/
    int commandFound = NONE;
    unsigned int patternToCheck = 0;
    int secondAddressingMethod  =NONE;
    int thirdAddressingMethod   =NONE;
    int secondParameteAddressingMethod = NONE;
    int firstParameterAddressingMethod = NONE;
    char secondWord[MAX_LINE]   = "\0";
    char thirdWord[MAX_LINE]    = "\0";

    /* Is in use in order to check for the second method*/
    char allParameters[MAX_LINE]    = "\0";

    /* parameters */
    char *firstParameter = NULL;
    char *secondParamete = NULL;
    char *temp;
    int firstOpen;
    char command[MAX_LINE]  = "\0";

    WordLine *first  = (WordLine *)(calloc(1,sizeof(WordLine)));
    checkAllocation(first);



    strcpy(command,currWord);

    strcpy(allParameters,currWord+((int)strlen(currWord)+1));

    currWord = strtok(NULL, " ,\t\n");
    if(currWord) {
        strcpy(secondWord, currWord);
    }
    currWord = strtok(NULL, " ,\t\n");
    if(currWord) {
        strcpy(thirdWord, currWord);
    }

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

            char * ptr;
            char * errorptr;
            ptr = secondWord + 1;
            strtol(ptr, &errorptr, DECIMAL_BASE);

            /*if number conversion failes*/
            if (*errorptr)
            {
                DIE("Invalid Number")
            }
            secondAddressingMethod = IMMEDIATE;
            /* Check if the word is an register operand */
        } else if (isRegister(secondWord)){
            secondAddressingMethod = REGISTER;

            /* After nagative result for the first two It is methods 1/2 */
        } else {
            firstOpen = secondMethodFormValidation(allParameters);
            if(firstOpen){
                secondAddressingMethod = JUMP;
                secondWord[firstOpen]='\0';
                firstParameter = &secondWord[firstOpen+1];


                secondParamete = thirdWord;
                temp = secondParamete;
                while(*temp != ')'){temp++;}
                *temp = '\0';

                if (*firstParameter == '#')
                {

                    char * ptr2;
                    char * errorptr2;
                    ptr2 = firstParameter + 1;
                    strtol(ptr2, &errorptr2, DECIMAL_BASE);

                    /*if number conversion failes*/
                    if (*errorptr2)
                    {
                        DIE("Invalid Number")
                    }
                    firstParameterAddressingMethod = IMMEDIATE;
                    /*  Check if the operand is a register */
                } else if (isRegister(firstParameter)){
                    firstParameterAddressingMethod = REGISTER;

                    /* must be a label */
                } else {
                    if(labelNamingValidation(firstParameter)){
                        if(!labelReservedNameValidation(firstParameter)){
                            firstParameterAddressingMethod = DIRECT;
                        }else{
                            DIE("Label is not declared")
                        }

                    } else {
                        DIE("Invalid label naming")
                    }
                }
                if (*secondParamete == '#')
                {
                    char * ptr2;
                    char * errorptr2;
                    ptr2 = secondParamete + 1;
                    (int)strtol(ptr2, &errorptr2, DECIMAL_BASE);

                    /*if number conversion failes*/
                    if (*errorptr2)
                    {
                        DIE("Invalid Number")
                    }
                    secondParameteAddressingMethod = IMMEDIATE;
                    /* Check if the operand is a register */
                } else if (isRegister(secondParamete)){
                    secondParameteAddressingMethod = REGISTER;

                    /* must be a label */
                } else {
                    if(labelNamingValidation(secondParamete)){




                        if(!labelReservedNameValidation(secondParamete)){
                            secondParameteAddressingMethod = JUMP;
                        }else{
                            DIE("Label is not declared")
                        }



                    } else {
                        DIE("Invalid label naming")
                    }
                }

                /* The second param's method must be 1 */
            } else {
                if(labelNamingValidation(secondWord)){




                    if(!labelReservedNameValidation(secondWord)){
                        secondAddressingMethod = DIRECT;
                    }else{
                        DIE("Label is not declared")
                    }



                } else {
                    DIE("Invalid label naming")
                }
            }
        }
        if((secondAddressingMethod != JUMP) && *thirdWord){

            if (*thirdWord == '#')
            {
                char * ptr2;
                char * errorptr2;
                ptr2 = thirdWord + 1;
                strtol(ptr2, &errorptr2, DECIMAL_BASE);

                /*if number conversion failes*/
                if (*errorptr2)
                {
                    DIE("Invalid Number")
                }
                thirdAddressingMethod = IMMEDIATE;
                /* Check if the operand is a register */
            } else if (isRegister(thirdWord)){
                thirdAddressingMethod = REGISTER;

                /* must be a label */
            } else {
                if(labelNamingValidation(thirdWord)){



                    if(!labelReservedNameValidation(thirdWord)){
                        thirdAddressingMethod = DIRECT;
                    }else{
                        DIE("Label is not declared")
                    }





                } else {
                    DIE("Invalid label naming")
                }
            }
        }
    }


    /* If we have recieved no operands at all */
    if(secondAddressingMethod == NONE) {

        /*  Update the oparand count*/
        patternToCheck |= legalPatterns[8].p;

        /* If we have recieved only one operand */
    } else if(thirdAddressingMethod == NONE){
        patternToCheck |= legalPatterns[9].p;
        patternToCheck |= legalPatterns[secondAddressingMethod+4].p;
        first->word    |= (secondAddressingMethod PUSH_TARGET_OPERAN);
        /* If we have recieved two operands */
    } else {
        patternToCheck  |= legalPatterns[10].p;
        patternToCheck  |= legalPatterns[secondAddressingMethod].p;
         patternToCheck |= legalPatterns[thirdAddressingMethod+4].p;
        first->word     |= (secondAddressingMethod PUSH_SOURCE_OPERAN);
        first->word     |= (thirdAddressingMethod PUSH_TARGET_OPERAN);
    }

    /* Now check if the pattern is valid to the command
    
     ( It sould not have flags that the command w'ont support)
     So - we will check if the & of the pattern to check and the
     supported patterns is equals to the pattern to check
     */

    i = NUM_OF_OPCODES;
    while(i){
        if(!strcmp(command, opCodes[i-1].str)){commandFound = i-1;}
        i--;
    }
    if(!(commandFound+1)){
        DIE("Command not found")
    }
    if((patternToCheck & opCodes[commandFound].p) == patternToCheck){
        (*IC)++;
        first->line  = line;
        first->word |= opCodes[commandFound].code PUSH_OPCODE;
        if(secondAddressingMethod == 2){
            first->word |= firstParameterAddressingMethod PUSH_FIRST_PARAMER;
            first->word |= secondParameteAddressingMethod PUSH_SECOND_PARAME;
        }
        addWordLine(first);
        if(secondAddressingMethod != NONE ){
            /* If we have only one operand */
            if(thirdAddressingMethod == NONE ){
                if(secondAddressingMethod == 2){

                    addAdditional(secondWord,1,0,IC,line);

                    if(firstParameterAddressingMethod == REGISTER && secondParameteAddressingMethod == 3){
                        int num1;
                        int num2;
                        WordLine *wordToAdd = (WordLine *)(calloc(1,sizeof(WordLine)));
                        checkAllocation(wordToAdd);
                        (*IC)++;
                        wordToAdd->line = line;
                        num1 = atoi(firstParameter+1);
                        num2 = atoi(secondParamete+1);
                        wordToAdd->word |= num1 PUSH_SRC_REG;
                        wordToAdd->word |= num2 PUSH_TARG_REG;
                        addWordLine(wordToAdd);
                    } else {
                        addAdditional(firstParameter,firstParameterAddressingMethod,1,IC,line);
                        addAdditional(secondParamete,secondParameteAddressingMethod,0,IC,line);
                    }
                }else{
                    addAdditional(secondWord,secondAddressingMethod,0,IC,line);
                }
                /* If we have two operands */
            } else {
                if(secondAddressingMethod == REGISTER && thirdAddressingMethod == REGISTER){
                    WordLine *wordToAdd = (WordLine *)(calloc(1,sizeof(WordLine)));
                    int num1;
                    int num2;
                    checkAllocation(wordToAdd);
                    (*IC)++;
                    wordToAdd->line = line;
                    num1 = atoi(secondWord + 1);
                    num2 = atoi(thirdWord + 1);
                    wordToAdd->word |= num1 PUSH_SRC_REG;
                    wordToAdd->word |= num2 PUSH_TARG_REG;
                    addWordLine(wordToAdd);
                    /* Send two operands to creation*/
                } else {
                    addAdditional(secondWord,secondAddressingMethod,1,IC,line);
                    addAdditional(thirdWord,thirdAddressingMethod,0,IC,line);
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











