#include "header.h"

/*This function creates the output files */
void createOutputFiles(int IC, int DC, char* name)
{
    FILE* object = NULL;
    FILE* entires = NULL;
    FILE* externs = NULL;
    char str[14];

    object = openFile(name, "w", OB_EXTENSION);
    fprintf(object,"%d %d\n", IC - 100, DC  );/* change magic numbers 100 is the presumed begining memory address*/
    while(wordList)/*while the word list data base has words in it */
    {
        convertToStrange2(curr->word, &str);
        fprintf(object, "%d \t %s\n", curr->address, str);
        curr = curr->next;
    }

    while(dataList)
    {
        convertToStrange2(curr->word, &str);
        fprintf(object, "%d \t %s\n", curr->address, str);
    }




    printf("Files created successfuly".\n");
    /*Clean up*/
    if (object)
        fclose(object);
    if (entries)
        fclose(entries);
    if (externs)
        fclose(externs);

}