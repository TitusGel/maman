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


/* Array of register string, for comparing with input */
static char * registers[NUM_OF_REGISTERS] = {"r1", "r2", "r3", "r4", "r5", "r6", "r7"};

/*This function will split the input string to currWords and will analyze each currWord.
It will create the corresponding word nodes, add them to the word list,
and increment the IC as needed.*/
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
    /* Found a command flag */
    int found = FALSE;
    /* How much do we want to increment IC? */
    int increment_value_IC;
    /* Check if command is known */
    for (i = 0; i < OPCODES_AMOUNT; i++)
    {
        /* if it is, create a new word object, initiate it and assign it with everything */
        if (!strcmp(currWord, opCodes[i].str))
        {
            /* Set "found command" flag to true */
                                         found = TRUE;
            /* Allocate memory for the first word */
                                           checkAllocation(first);
            /* Set all of first word's parameters */
            first->line = line;

            first->word |= opCodes[i].code << OPCODE_POSITION;

            // increment the IC
            increment_value_IC++;
        }
    }
    /* if the command wasnt found, error */
    if (!found)
    {
        addError("Unknown command", line, currWord);
        return;
    }

    /*
    A command word exists.
    If we have parameters after the command*/
    if ((currWord = strtok(NULL, " ,\t\n")))
    {
        /* If the command is group 2 */
        if (first.group == 2)
        {
            /* If first currWord is an immediate number */
            if (*currWord == '#')
            {
                int num;
                char * ptr;
                char * errorptr;
                ptr = currWord;
                ptr++;
                num = strtol(ptr, &errorptr, 10);
                /* if command is LEA, error&quit */
                if (first->holder.first.opcode == OPCODE_LEA)
                {
                    addError("This command can't take an immediate number as a first operand", line, NULL);
                    free(first);
                    return;
                }
                /* if number conversion failes */
                if (*errorptr != 0)
                {
                    addError("Invalid number", line, NULL);
                    free(first);
                    return;
                }

                /* If syntax is correct */
                else
                {
                    /* Update first word and create a second word */
                                                           second->isExternal = FALSE;
                    second->line = line;
                    second->missingLabel = NULL;
                    second->externalLabel = NULL;
                    second->word = num << 2;
                    second->next = NULL;
                    increment_value_IC++;
                }
            }
            /* if first currWord is register */
            else if (isRegister(currWord))
            {
                /* If the command is lea, add error and return */
                if (first->holder.first.opcode == OPCODE_LEA)
                {
                    addError("This command can't take an immediate number as a first operand", line, NULL);
                    free(first);
                    return;
                }
                /* Update first word and create a second word */
                                                       second->isExternal = FALSE;
                second->line = line;
                second->missingLabel = NULL;
                second->externalLabel = NULL;


                second->word = currWord[1]<<;
                second->holder.reg.destination = 0;
                second->holder.reg.era = 0;
                second->next = NULL;
                increment_value_IC++;
            }
            /* if first currWord is index */
            else if (isIndex(currWord))
            {
                int left = (currWord[1] - '0');
                int right = (currWord[4] - '0');

                /* if numbers are not legit registers */
                if ((left < 0 || left > 7 ) || (right < 0 || right > 7))
                {
                    addError("Illegal register", line, NULL);
                    free(first);
                    return;
                }
                /* if the numbers are legit registers */
                else
                {
                    /* if first is odd and second is even */
                    if (((left % 2 ) != 0) && ((currWord[4] - '0') % 2 == 0))
                    {
                        first->holder.first.source = INDEX;
                        second = malloc(sizeof(WordLine));
                        second->method = INDEX;
                        second->isExternal = FALSE;
                        second->line = line;
                        second->missingLabel = NULL;
                        second->externalLabel = NULL;
                        second->holder.index.last = 0;
                        second->holder.index.unused = 0;
                        second->holder.index.rightOperand = right;
                        second->holder.index.leftOperand = left;
                        second->holder.index.era = 0;
                        second->next = NULL;
                        increment_value_IC++;
                    }
                    else
                    {
                        addError("Left register must be odd and right register must be even", line, currWord);
                        free(first);
                        return;
                    }
                }
            }
            /* if we didnt recieve immediate number/register/index, it must be a label */
            else
            {
                char * ptr;
                ptr = currWord;
                /* check if first char is letter (valid label) יש לנו פעולה לזה */
                if (isalpha(*ptr))
                {
                    /* checks for invalid chars */
                    while (*ptr != '\0')
                    {
                        if ((!isdigit(*ptr)) && (!isalpha(*ptr)))
                        {
                            addError("Invalid character", line, currWord);
                            free(first);
                            return;
                        }
                        ptr++;
                    }
                    /* create memory address opcode */
                                           first->holder.first.source = DIRECT;
                    second = malloc(sizeof(WordLine));
                    second->method = DIRECT;
                    second->isExternal = FALSE;
                    second->line = line;
                    second->missingLabel = malloc(sizeof(char)*MAX_LINE);
                    second->externalLabel = NULL;
                    strcpy(second->missingLabel, currWord);
                    second->holder.immediate.last = 0;
                    second->holder.immediate.number = 0;
                    second->holder.immediate.era = 1;
                    second->next = NULL;
                    increment_value_IC++;
                }
                else
                {
                    addError("Labels must start with a letter", line, currWord);
                    free(first);
                    return;
                }
            }
            /* Move to second currWord */
            if ((currWord = strtok(NULL, " ,\t\n")))
            {
                if (*currWord == '#')
                {
                    int num;
                    char * ptr;
                    char * errorptr;
                    ptr = currWord;
                    ptr++;
                    num = strtol(ptr, &errorptr, 10);
                    /* if command is LEA, error&quit */
                    if (first->holder.first.opcode != OPCODE_CMP)
                    {
                        addError("This command can't take an immediate number as a second operand", line, NULL);
                        free(first);
                        free(second);
                        return;
                    }
                    /* if number conversion failes */
                    if (*errorptr != 0)
                    {
                        addError("Invalid number", line, NULL);
                        free(first);
                        free(second);
                        return;
                    }
                    /* if the number isn't within range */
                    if (num > MAX_NUM_IMMEDIATE || num < MIN_NUMBER_IMMEDIATE)
                    {
                        addError("Immediate number is out of range", line, NULL);
                        free(first);
                        return;
                    }
                    /If syntax is correct/
                    else
                    {
                        first->holder.first.destination = IMMEDIATE;
                        third = malloc(sizeof(WordLine));
                        third->method = IMMEDIATE;
                        third->isExternal = FALSE;
                        third->line = line;
                        third->missingLabel = NULL;
                        third->externalLabel = NULL;
                        third->holder.immediate.last = 0;
                        third->holder.immediate.number = num;
                        third->holder.immediate.era = 0;
                        third->next = NULL;
                        increment_value_IC++;
                    }
                }
                else if (isRegister(currWord))
                {
                    if (second->method == REGISTER)
                    {
                        first->holder.first.destination = REGISTER;
                        second->holder.reg.destination = (currWord[1] - '0');
                    }
                    else
                    {
                        first->holder.first.destination = REGISTER;
                        third = malloc(sizeof(WordLine));
                        third->method = REGISTER;
                        third->isExternal = FALSE;
                        third->line = line;
                        third->missingLabel = NULL;
                        third->externalLabel = NULL;
                        third->holder.reg.last = 0;
                        third->holder.reg.unused = 0;
                        third->holder.reg.source = 0;
                        third->holder.reg.destination = (currWord[1] - '0');
                        third->holder.reg.era = 0;
                        third->next = NULL;
                        increment_value_IC++;
                    }
                }
                else if (isIndex(currWord))
                {
                    int left = (currWord[1] - '0');
                    int right = (currWord[4] - '0');
                    /* if numbers are not legit registers */
                    if ((left < 0 || left > 7 ) || (right < 0 || right > 7))
                    {
                        addError("Illegal register", line, NULL);
                        free(first);
                        return;
                    }
                    /* if the numbers are legit registers */
                    else
                    {
                        /* if first is odd and second is even */
                        if (((left % 2 ) != 0) && ((currWord[4] - '0') % 2 == 0))
                        {
                            first->holder.first.destination = INDEX;
                            third = malloc(sizeof(WordLine));
                            third->method = INDEX;
                            third->isExternal = FALSE;
                            third->line = line;
                            third->missingLabel = NULL;
                            third->externalLabel = NULL;
                            third->holder.index.last = 0;
                            third->holder.index.rightOperand = right;
                            third->holder.index.leftOperand = left;
                            third->holder.index.era = 0;
                            third->next = NULL;
                            increment_value_IC++;
                        }
                        else
                        {
                            addError("Left register must be odd and right register must be even", line, currWord);
                            free(first);
                            free(second);
                            return;
                        }
                    }
                }
                /* if currWord is not immediate/index/register */
                else
                {
                    char * ptr;
                    ptr = currWord;
                    /* check if first char is letter (valid label) */
                    if (isalpha(*ptr))
                    {
                        /* checks for invalid chars */
                        while (*ptr != '\0')
                        {
                            if ((!isdigit(*ptr)) && (!isalpha(*ptr)))
                            {
                                addError("Invalid character", line, currWord);
                                free(first);
                                return;
                            }
                            ptr++;
                        }
                        /* create memory address opcode */
                        first->holder.first.destination = DIRECT;
                        third = malloc(sizeof(WordLine));
                        third->method = DIRECT;
                        third->isExternal = FALSE;
                        third->line = line;
                        third->missingLabel = malloc(sizeof(char)*MAX_LINE);
                        third->externalLabel = NULL;
                        strcpy(third->missingLabel, currWord);
                        third->holder.immediate.last = 0;
                        third->holder.immediate.number = 0;
                        third->holder.immediate.era = 1;
                        third->next = NULL;
                        increment_value_IC++;
                    }
                    /* if the label didn't start with a letter */
                    else
                    {
                        addError("Labels must start with a letter", line, currWord);
                        free(first);
                        free(second);
                        return;
                    }
                }
                /* check if the string has more arguments */
                if ((currWord = strtok(NULL, " ,\t\n")))
                {
                    addError("Too many arguments", line, NULL);
                    free(first);
                    free(second);
                    free(third);
                    return;
                }
            }
            /* Check if there's less than 2 arguments */
            else
            {
                addError("This command must take 2 parameters", line, NULL);
                free(first);
                free(second);
                return;
            }
        }
        /* if the command is group 1 */

        else if (first->holder.first.group == 1)
        {
            /* If first currWord is an immediate number */
            if (*currWord == '#')
            {
                int num;
                char * ptr;
                char * errorptr;
                ptr = currWord;
                ptr++;
                num = strtol(ptr, &errorptr, 10);
                /* if command is LEA, error&quit */
                if (first->holder.first.opcode != OPCODE_PRN)
                {
                    addError("This command can't take an immediate number as a first operand", line, NULL);
                    free(first);
                    return;
                }
                /* if number conversion failes */
                if (*errorptr != 0)
                {
                    addError("Invalid number", line, NULL);
                    free(first);
                    return;
                }
                /* if the number isn't within range */
                if (num > MAX_NUM_IMMEDIATE || num < MIN_NUMBER_IMMEDIATE)
                {
                    addError("Immediate number is out of range", line, NULL);
                    free(first);
                    return;
                }
                /* If syntax is correct */
                else
                {
                    first->holder.first.destination = IMMEDIATE;
                    second = malloc(sizeof(WordLine));
                    second->method = IMMEDIATE;
                    second->isExternal = FALSE;
                    second->line = line;
                    second->missingLabel = NULL;
                    second->externalLabel = NULL;
                    second->holder.immediate.last = 0;
                    second->holder.immediate.number = num;
                    second->holder.immediate.era = 0;
                    second->next = NULL;
                    increment_value_IC++;
                }
            }
            /* Check if currWord is a register */
            else if (isRegister(currWord))
            {
                if (first->holder.first.opcode == OPCODE_LEA)
                {
                    first->holder.first.destination = REGISTER;
                    second = malloc(sizeof(WordLine));
                    second->method = REGISTER;
                    second->isExternal = FALSE;
                    second->line = line;
                    second->missingLabel = NULL;
                    second->externalLabel = NULL;
                    second->holder.reg.last = 0;
                    second->holder.reg.unused = 0;
                    second->holder.reg.source = (currWord[1] - '0');
                    second->holder.reg.destination = 0;
                    second->holder.reg.era = 0;
                    second->next = NULL;
                    increment_value_IC++;
                }
            }
            /* Check if currWord is index */
            else if (isIndex(currWord))
            {
                int left = (currWord[1] - '0');
                int right = (currWord[4] - '0');
                /* if numbers are not legit registers */
                if ((left < 0 || left > 7 ) || (right < 0 || right > 7))
                {
                    addError("Illegal register", line, NULL);
                    free(first);
                    return;
                }
                /* if the numbers are legit registers */
                else
                {
                    /* if first is odd and second is even */
                    if (((left % 2 ) != 0) && ((currWord[4] - '0') % 2 == 0))
                    {
                        first->holder.first.destination = INDEX;
                        second = malloc(sizeof(WordLine));
                        second->method = INDEX;
                        second->isExternal = FALSE;
                        second->line = line;
                        second->missingLabel = NULL;
                        second->externalLabel = NULL;
                        second->holder.index.last = 0;
                        second->holder.index.rightOperand = right;
                        second->holder.index.leftOperand = left;
                        second->holder.index.era = 0;
                        second->next = NULL;
                        increment_value_IC++;
                    }
                    /* If left register isn't odd or right register isn't even */
                    else
                    {
                        addError("Left register must be odd and right register must be even", line, currWord);
                        free(first);
                        return;
                    }
                }
            }
            /* If currWord not immediate/register/currWord */
            else
            {
                char * ptr;
                ptr = currWord;
                /* check if first char is letter (valid label) */
                if (isalpha(*ptr))
                {
                    /* checks for invalid chars */
                    while (*ptr != '\0')
                    {
                        if ((!isdigit(*ptr)) && (!isalpha(*ptr)))
                        {
                            addError("Invalid character", line, currWord);
                            free(first);
                            return;
                        }
                        ptr++;
                    }

                    /* create memory address opcode */
                                           first->holder.first.destination = DIRECT;
                    second = malloc(sizeof(WordLine));
                    second->line = line;
                    second->method = DIRECT;
                    second->isExternal = FALSE;
                    second->missingLabel = malloc(sizeof(char)*MAX_LINE);
                    second->externalLabel = NULL;
                    strcpy(second->missingLabel, currWord);
                    second->holder.immediate.last = 0;
                    second->holder.immediate.number = 0;
                    second->holder.immediate.era = 1;
                    second->next = NULL;
                    increment_value_IC++;
                }
                /* if the label didn't start with a letter */
                else
                {
                    addError("Labels must start with a letter", line, currWord);
                    free(first);
                    free(second);
                    return;
                }
            }
            /* if string has more arguments */
            if ((currWord = strtok(NULL, " ,\t\n")))
            {
                addError("Too many arguments", line, NULL);
                free(first);
                free(second);
                free(third);
                return;
            }
        }
        /* if the command is group 0 */
        else
        {
            addError("This command cannot take parameters", line, NULL);
            free(first);
            return;
        }
    }
    /* If we didn't recieve any parameters after the command */
    else
    {
        if (first->holder.first.opcode != OPCODE_RTS && first->holder.first.opcode != OPCODE_STOP)
        {
            addError("This command must take parameters", line, NULL);
            free(first);
            return;
        }
    }
    /* Check if there's a label with the command. if there is, add it to the symbols table */
    if (label)
    {
        addSymbol(label, *IC, FALSE, TRUE, line);
    }
    /* Add each word (as needed) to the words list */
    if (first)
        addWordLine(first);
    if (second)
        addWordLine(second);
    if (third)
        addWordLine(third);
    /* Increment the IC as needed */
                         *IC += increment_value_IC;
}



/*This function recieves a string and returns:
  1 if the string is an index
  0 if the string is not an index*/
int isIndex(char * currWord)
{
    if (strlen(currWord) == 6)
    {
        if (*currWord == 'r')
        {
            if (isdigit(currWord[1]))
            {
                if (currWord[2] == '[')
                {
                    if (currWord[3] == 'r')
                    {
                        if (isdigit(currWord[4]))
                        {
                            if (currWord[5] == ']')
                            {
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}