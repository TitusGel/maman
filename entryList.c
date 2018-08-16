#include "header.h"

/*This file contains function which handle the entries list*/

/*Pointer to head of list*/
static Entry * head = NULL;

/*This functions adds an entry to the list. it takes a label and a line, for error reporting*/
void addEntry(char * label, int line)
{
    /*temp pointer to entry*/
    Entry * tmp = NULL;
    /*Check if a similar entry exists*/
    if (!(checkEntry(label)))
    {
        addError("Multiple entries with same label", line, label);
        return;
    }

    /*allocate memory for new node*/
    tmp = malloc(sizeof(Entry));
    checkAllocation(tmp);
    /* copy the label */
                tmp->label = malloc(sizeof(char)*MAX_LINE);
    strcpy(tmp->label, label);
    /* assign parameters */
            tmp->address = 0;
    tmp->line = line;
    tmp->hasAddress = FALSE;
    tmp->next = NULL;
    /* If head is null, assign it with first node */
    if (!head)
    {
        head = tmp;
    }
        /*if head is not null, go through the list up to the last node
        and assign its next pointer to the new node*/
    else
    {
        Entry * p = head;
        while (p->next)
            p = p->next;
        p->next = tmp;
    }
}

/*This function checks if an entry with the same label already exists
If there is one, it will return 0. else, it will return 1;*/
int checkEntry(char * label)
{
    Entry * ptr = head;
    /* Go through the list */
    while (ptr)
    {
        /* If we find a node with the same label */
        if (!strcmp(ptr->label, label))
        {
            return 0;
        }
        ptr = ptr->next;
    }
    /* If such node doesn't exist, return 1 */
    return 1;
}

void printEntry()
{
    Entry * ptr = head;
    /* Go through the list */
    puts("entry-list");
    while (ptr)
    {
       printf("label = %s line = %d, add= %d has= %d \n",ptr->label,ptr->line,ptr->address,ptr->hasAddress);
        ptr = ptr->next;
    }
}

/* This function frees all the nodes in this list */
void freeEntries()
{
    Entry * ptr = head;
    if (!ptr)
        return;
    while (ptr)
    {
        Entry * tmp = ptr;
        ptr = ptr->next;
        free(tmp);
    }
    head = NULL;
}

/*This function loops through the entries, and returns
an entry without an address assigned to it. for use during the
second pass*/
Entry * getEntry()
{
    Entry *ptr = head;
    while (ptr)
    {
        if (!ptr->hasAddress)
        {
            return ptr;
        }
        ptr = ptr->next;
    }
    return NULL;
}

/* This function returns a pointer to the head */
Entry * getHeadEntry()
{
    Entry * tmp = head;
    return tmp;
}