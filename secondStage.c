#include "header.h"

void secondPass()
{
    /*Temporary word node */
    WordLine * tmpWordLine = NULL;
    /* Temporary entry node */
    Entry * tmpEntry = NULL;
    /* Temporary symbol node */
    Label * tmpSymbol = NULL;
    /* If the overall count of words and data exceeds the memory limit, add error and quit */
    if ((countWords() + countData()) > MAX_CODE_LINES )
    {
        addError("Memory limit overflow", MAX_CODE_LINES , NULL);
        return;
    }
    /* Get every label which is missing an address*/
    while ((tmpWordLine = getMissingLabel()))
    {

        /* Search for a symbol in the symbols list with the same address */
        tmpSymbol = searchSymbol(tmpWordLine->missingLabel);
        /* If no such symbol was found, add an error */
        if(!tmpSymbol)
        {
            char *tmpString;
            tmpString = malloc(sizeof(char)*MAX_LINE);
            strcpy(tmpString, "You must declare the label \"");
            strcat(tmpString, tmpWordLine->missingLabel);
            strcat(tmpString, "\" in order to use it !");
            addError(tmpString, tmpWordLine->line, NULL);
        }
        /* If the symbol was found */
        else
        {
            /* If it's external, change the word accordingly */
            if (tmpSymbol->external)
            {
                tmpWordLine->word = 1;
                tmpWordLine->isExternal = TRUE;
                tmpWordLine->externalLabel = tmpSymbol->label;
            }
            /* If it's not external, change the word accordingly and assign the address */
            else
            {
                tmpWordLine->word = tmpSymbol->address PUSH_IMMEDIATE;
                tmpWordLine->word |= 2;
            }
        }
        tmpWordLine->missingLabel = NULL;
    }
    /* Go through the entry list */
    while ((tmpEntry = getEntry()))
    {
        /* Search for symbols with the same label */
        tmpSymbol = searchSymbol(tmpEntry->label);
        /* If there is no such symbol, add an error */
        if(!tmpSymbol)
        {
            char *tmpString;
            tmpString = malloc(sizeof(char)*MAX_LINE);
            strcpy(tmpString, "You must declare the label \"");
            strcat(tmpString, tmpEntry->label);
            strcat(tmpString, "\" in order to use it !");
            addError(tmpString, tmpEntry->line, NULL);
            tmpEntry->hasAddress = TRUE;
        }
        /* If a symbol was found, assign its address to the entry node */
        else
        {
            tmpEntry->address = tmpSymbol->address;
            tmpEntry->hasAddress = TRUE;
        }
    }
}