#include "header.h"

/*
 --------------- general --------------
 
 general utils functions for every stage
 ---------------------------------------
 */


/* This function checks if pointer is null */
void checkAllocation(void * ptr)
{
    if (!ptr)
    {
        printf("The program has quit due to a memory allocation error.\n");
        exit(0);
    }
}


/*this function checks if token is a comment */
int checkComment(char * candidate)
{
    return(*candidate == ';')? TRUE : FALSE;
}

/*This function takes a string as a parameter, and splits it into tokens of whole numbers.
 if the string recieved is null, it will add an error. otherwise, it will loop through the string's
 tokens and send each one to the addNumber function*/
void splitNumbers(char * currNum, int line, int * DC)
{
    /* if the token is null */
    if (!(currNum = strtok(NULL, " ,\t\n")))
    {
        /* add error and return */
        addError("You must specify numbers", line, NULL);
        return;
    }
    /* if token is not null */
    else
    {
        /* send first token to addNumber() */
            addNumber(currNum, line, DC);
        /* send the rest tokens to addNumber() */
        while ((currNum = strtok(NULL, " ,\t\n")))
        {
            addNumber(currNum, line, DC);
        }
    }
}

/*
 --------------- firstStage --------------
 
 firstStage utils functions the first stage
 
 -----------------------------------------
 */


/*This function checks for a label.
if there is no label, it will return NULL.
if there's an illegal label, it will return $.
if there's a legal label, it will return the label.*/

/* Check if candidate is meant to be a label   */
char *  labelFormValidation(char * candidate){

    if(candidate[strlen(candidate) - 1] == ':'){
        candidate[strlen(candidate) - 1] = '\0';
        return candidate;
    }
    return 0;
}
/* Check if label is valid (by syntax)   */
char *  labelNamingValidation(char * label){

    char * ptr = label;

    if (!isalpha(*ptr)){
        return 0;
    }
    /*  */
    while ((*ptr) != '\0')
    {
        if (!isalpha(*ptr))
        {
            if (!isdigit(*ptr))
            {
                return 0;
            }
        }
        ptr++;
    }
    return label;
}
int labelReservedWordValidation(char * label){

    return (isOperation(label)||isRegister(label))? FALSE: TRUE;
}

/*This function recieves a string and returns:
 1 if the string is an operation
 0 if the string is not an operation*/
int isOperation(char * currWord)
{
    int i;

    for (i = 0; i < OPCODES_AMOUNT; i++)
    {
        if (!strcmp(currWord, opCodes[i].str))
        {
            return 1;
        }
    }
    return 0;
}

/*This function recieves a string and returns:
 1 if the string is a registers
 0 if the string is not a register*/
int isRegister(char * currWord)
{
    int i;
    if (strlen(currWord) == 2)
    {
        if (currWord[0] == 'r')
        {
            if (isdigit(currWord[1]))
            {
                for (i=0; i<NUM_OF_REGISTERS; i++)
                {
                    if (!strcmp(currWord, registers[i]))
                    {
                        return 1;
                    }
                }

            }
        }
    }
    return 0;
}


/*This function determines if we received a directive.
 it will return an enum int of the proper directive, or an error flag
 if there's no such directive. if the string doesn't start with ".",
 it will return an enum defined as NONE, meaning it is not a directive*/
int getDirective(char * token)
{
    /* if first char is '.' */
    if (*token == '.')
    {
        /* return enum corresponding to the directive */
        if (!strcmp(token, STR_DIRECTIVE_DATA))
        {
            return DIRECTIVE_DATA;
        }
        else if (!strcmp(token, STR_DIRECTIVE_STRING))
        {
            return DIRECTIVE_STRING;
        }
        else if (!strcmp(token, STR_DIRECTIVE_ENTRY))
        {
            return DIRECTIVE_ENTRY;
        }
        else if (!strcmp(token, STR_DIRECTIVE_EXTERN))
        {
            return DIRECTIVE_EXTERN;
        }
        /* if string is unknown */
        else
        {
            /* return error */
            return DIRECTIVE_ERROR;
        }
    }
    /* if this isn't a directive */
    else
    {
        return DIRECTIVE_NONE;
    }

}

/*
 --------------- thrirdStage --------------

 firstStage utils functions the first stage

 -----------------------------------------
 */

void convertToStrange2(unsigned int word, char* str )
{
    int i = 14;
    for (; 0 <= i; i-- )
    {
        *(str + i) = ((1<<i) & word)? '/':'.';
    }

}