#include "header.h"

/*This function exports the output files*/
void thirdStage(int IC, int DC, char * name)
{
    /*FILE pointers*/
    FILE * object = NULL;
    FILE * entries = NULL;
    FILE * externs = NULL;
    /*Word holder*/
    /*Extern flag*/
    int nExtern = FALSE;
    char toPrint[BITS_IN_WORD];
    /*Entry flag*/
    int nEntry = 0;
    /*Pointer to WordLine*/
    WordLine * p = NULL;
    /*Pointer to entry*/
    Entry * pe = NULL;
    /*Pointer to data*/
    Data * pd = NULL;
    /*open an object file in write mode*/
    object = openFile(name, "w", OB_EXTENSION);
    /*assign wordNode pointer with wordNode head*/
    p = getHeadWord();
    /*Print the number of words and number of data */
    fprintf(object, "%d %d\n", countWords(), countData());
    /*go through the entire words list*/
    while (p)
    {
        convertToStrange2(p->word,toPrint);
        fprintf(object, "%d\t%s\n", p->address, toPrint);

        /*If we found an external word, set flag to 0*/
        if (p->isExternal == TRUE)
        {
            nExtern=TRUE;
        }
        p = p->next;
    }
    /*Assign pointer with data list head*/
    pd = getHeadData();
    /*print all the data & addresses in hexadecimal*/
    while (pd)
    {
        convertToStrange2(pd->word,toPrint);
        fprintf(object, "%d\t%s\n", pd->address, toPrint);

        pd = pd->next;
    }

    /*Assign pointer with entry list head*/
    pe = getHeadEntry();
    /*If the list is not empty*/
    if (pe)
    {
        /*open a file in write mode for entries*/
        entries = openFile(name, "w", ENT_EXTENSION);
        /*go through the entry list and write the labels and addresses in hexadecimal*/
        while (pe)
        {
            fprintf(entries, "%s\t%d\n", pe->label, pe->address);
            pe = pe->next;
            nEntry++;
        }
    }
    /*If we found an extern word earlier*/
    if (nExtern)
    {
        /*open an extern file in write mode*/
        externs = openFile(name, "w", EXT_EXTENSION);
        /*assign pointer with word list head*/
        p = getHeadWord();
        /*Go through the list*/
        while (p)
        {
            /*if the word is external*/
            if (p->isExternal == TRUE)
            {
                convertToStrange2(p->word,toPrint);
                fprintf(object, "%d\t%s\n", p->externalLabel, p->address);
            }
            p = p->next;
        }
    }
    printf("Success! files exported.\n");
    /*Clean up*/
    if (object)
        fclose(object);
    if (entries)
        fclose(entries);
    if (externs)
        fclose(externs);
}