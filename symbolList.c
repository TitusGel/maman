#include "header.h"


static Label * head = NULL;

/*This function adds a symbol to the symbols list*/
void addSymbol(char * label, int address, int external, int operation, int line)
{
    /*Temp sybol*/
        Label * tmp = NULL;
    /*This function will check if such symbol already exists*/
        checkSymbol(label, line);
    /*Allocate memory for new symbol*/
                             tmp = malloc(sizeof(Label));
    /*Check if allocation succeeded*/
                         checkAllocation(tmp);
    /*Allocate memory for symbol's label*/
    tmp->label = malloc(sizeof(char)*MAX_LINE);
    /*Check if allocation succeeded*/
                         checkAllocation(tmp->label);
    /*Copy the label from the input to this symbol*/
    strcpy(tmp->label, label);
    /*Copy value of DC*/
        tmp->address = address;
    /*Copy external flag*/
        tmp->external = external;
    /*Copy operation tab*/
        tmp->operation = operation;
    /*set next to NULL*/
        tmp->next = NULL;
    /* if list is empty*/
    if (!head)
    {
        /* make head point to temp */
            head = tmp;
    }
    /*if list isn't empty*/
    else
    {
        /* create pointer to head */
            Label * p = head;
        /*get to the last node */
        while (p->next)
            p = p->next;
        /* set last node's next to tmp */
        p->next = tmp;
    }
}

/*This function will update the symbol's addresses to match with the IC counter,
after the first pass*/
void updateSymbols(int IC)
{
    Label * ptr = head;
    while (ptr)
    {
        if ((ptr->external == FALSE) && (ptr->operation == FALSE))
        {
            ptr->address += IC;
        }
        ptr = ptr->next;
    }
}


void printSymbol()
{
    Label * ptr = head;
    puts("symbol-list");
    while (ptr)
    {
        printf("label = %s add= %d ex = %d op = %d \n",ptr->label,ptr->address,ptr->external,ptr->operation);
        ptr = ptr->next;
    }
}


/*This function cleans up the symbol list*/
void freeSymbols()
{
    Label * ptr = head;
    if (!ptr)
        return;

    while (ptr)
    {
        Label * tmp = ptr;
        ptr = ptr->next;
        free(tmp->label);
        free(tmp);
    }
    head = NULL;
}

/* This function checks if a symbol with the same name exists*/
void checkSymbol(char * label, int line)
{
    Label * ptr = head;
    while (ptr)
    {
        if (!strcmp(ptr->label, label))
        {
            addError("Multiple symbols with label", line, label);
            return;
        }
        ptr = ptr->next;
    }
}

/*This function will return a symbol with the same label as the string
entered as a parameter. if no such symbol was found, it will return NULL*/
Label * searchSymbol(char * label)
{
    Label *ptr = head;
    while (ptr)
    {
        if (!strcmp(ptr->label, label))
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

Label * getSymbolHead()
{
    return head;
}