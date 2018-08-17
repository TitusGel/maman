#include "header.h"

/*Pointer to head*/
static WordLine * head = NULL;

/*Add a word node to the list*/
void addWordLine(WordLine * node)
{
    /*If head is null*/
    if (!head)
    {
        /* point head to node */
        head = node;
    }
        /* If head is not null */
    else
    {
        /* Go through the list until the last node */
        WordLine *p = head;
        while (p->next)
            p = p->next;
        /* Assign the last node's next pointer to the new node */
        p->next = node;
    }
}

/*This function will update all of the words with their corresponding addresses*/
void updateAddresses()
{
    int address = STARTING_ADDRESS;
    WordLine * ptr = getHeadWord();
    if (!ptr)
        return;

    while (ptr)
    {
        ptr->address = address;
        address++;
        ptr = ptr->next;
    }
}


/* This function returns the number of nodes in the list */
int countWords()
{
    int counter = 0;
    WordLine * ptr = head;
    if (!ptr)
        return counter;

    while (ptr)
    {
        counter++;
        ptr = ptr->next;
    }
    return counter;
}

/* This function cleans up the list */
void freeWords()
{
    WordLine * ptr = head;
    if (!ptr)
        return;

    while (ptr)
    {
        WordLine * tmp = ptr;
        ptr = ptr->next;

        free(tmp->missingLabel);
        free(tmp);
    }
    head = NULL;
}
void freeWord(WordLine * ptr)
{

    if (!ptr)
        return;


        free(ptr->missingLabel);
        free(ptr);
}

/*This function will search for a word with a missing label and return it.
if no such word was found, it will return NULL*/
WordLine * getMissingLabel()
{
    WordLine * p = head;

    while (p)
    {
        if (p->missingLabel)
        {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

/* This function return a pointer to the head of the list */
WordLine * getHeadWord()
{
    WordLine * tmp;
    tmp = head;
    return tmp;
}