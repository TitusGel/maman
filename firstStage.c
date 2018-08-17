#include "header.h"
/* This file contains the first Stage function */

/*The first pass function will go throgh each line.
It will first check for a label, then check for a directive.
if there's no directive, it will check for a command. It will add errors
to the errors list if found, and will go through the entire input file*/
void firstPass(FILE * f, int * IC, int * DC)
{
    /* Line buffer */
    char line[MAX_LINE];
    /* Line counter */
    int lineCounter = 0;
    /* Line fetching loop */
    while (fgets(line, MAX_LINE, f))
    {
        /* pointer to currWord */
        char * currWord = NULL;
        /* pointer to label */
        char * label  = NULL;
        /* directive flag */
        int directive = 0;
        /* get first currWord */
                   currWord = strtok(line, " \t\n");

        /* increment line counter */
        lineCounter++;

        /*if line is empty, go to next line */
        if ((!currWord) || (checkComment(currWord)))
        {
            continue;
        }
        /* If label is not NULL after calling getLabel() */
        if ((label = labelFormValidation(currWord)))
        {
            /* If getLabel returned error string */
            if (!labelNamingValidation(label))
            {
                /* Add error */
                     addError("Illegal label", lineCounter, currWord);
                /* set label to null */
                              label = NULL;
            }
            else if (labelReservedNameValidation(label))
            {
                addError("Label name is reserved", lineCounter, NULL);
            }
            /* Get next currWord. */
                      currWord = strtok(NULL, " \t\n");
            /* if next currWord is null, add error and go to next line */
            if (!currWord)
            {
                addError("Must add something next to label", lineCounter, NULL);
                continue;
            }
        }


        /* Check for directive */
        if ((directive = getDirective(currWord)))
        {
            /* if getDirective returned an error */
            if (directive == DIRECTIVE_ERROR)
            {
                /* add error */
                     addError("Invalid directive", lineCounter, currWord);
                continue;
            }
            /*if directive is data directive*/
            else if (directive == DIRECTIVE_DATA)
            {
                /* if there's a label */
                if (label)
                {
                    addSymbol(label, *DC, FALSE, FALSE, lineCounter);
                }
                splitNumbers(currWord, lineCounter, DC);

            }
            /* if directive is string directive*/
            else if (directive == DIRECTIVE_STRING)
            {
                /*get next token*/
                    currWord = strtok(NULL, "\n");
                /*if token is null*/
                if (!currWord)
                {
                    /*add error and go to next line*/
                                              addError("Must specify a string", lineCounter, NULL);
                    continue;
                }
                /*if there's a label*/
                if (label)
                {
                    /* add to symbols list*/
                      addSymbol(label, *DC, FALSE, FALSE, lineCounter);
                }
                /*add to data*/
                addString(currWord, lineCounter, DC);
                /* go to next line */
                continue;

            }
            /* if directive is entry*/
            else if (directive == DIRECTIVE_ENTRY)
            {
                /* if we have a label, print a warning*/
                if (label)
                    printf("Warning, line %d: label \"%s\" before entry directive is meaningless\n", lineCounter, label);
                /* get next token*/
                if (!(currWord = strtok(NULL, " \t\n")))
                {
                    addError("Entry directive must have a label parameter", lineCounter, NULL);
                    continue;
                }
                /* add an entry node*/
                 addEntry(currWord, lineCounter);
                /*if there's another word*/
                (currWord = strtok(NULL, " \t\n"));
                if (currWord)
                {
                    /*add error and go to next line */
                     addError("Entry directive can only take one parameter", lineCounter, NULL );
                    continue;
                }
                /*if next token is null*/
                else
                {
                    /*go to next line*/
                    continue;
                }
            }
            /*if directive is extern*/
            else if (directive == DIRECTIVE_EXTERN)
            {
                /*if there's a label, print a warning*/
                if (label)
                    printf("Warning, line %d: label \"%s\" before extern directive is meaningless\n", lineCounter, label);
                /*get next token*/
                if (!(currWord = strtok(NULL, " \t\n")))
                {
                    addError("Extern directive must have a label parameter", lineCounter, NULL);
                    continue;
                }
                /*add extern symbol*/
                addSymbol(currWord, 0, TRUE, FALSE, lineCounter);
                /*if next token is not null*/
                (currWord = strtok(NULL, "\n"));
                if (currWord)
                {
                    /*add error and go to next line*/
                                              addError("Extern directive can only take one parameter", lineCounter, NULL );
                    continue;
                }
                /*if next token is null*/
                else
                {
                    /*go to next line*/
                    continue;
                }
            }
        }
        /*if the token is not a directive*/
        else
        {
            if(label){
                addSymbol(label,*IC,FALSE,TRUE,lineCounter);
            }
            /*Analyze the operation*/
                         analyzeOperation(currWord, lineCounter, IC);

        }
    }

    /*If the file is empty*/
    if (!lineCounter)
    {
        addError("ERROR: File is empty.", -1, NULL);
    }

}