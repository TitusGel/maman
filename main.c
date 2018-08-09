#include "header.h"

int main(int argc,char* argv[]) {

    /*Index used to loop through the input files*/
    FILE *fp = NULL;
    int IC = 100;
    int DC = 0;

    if(argc < 2){
        FILE_NOT_FOUND;
        exit(1);
    } else if(argc > 2){
        main(argc - 1 , argv);
    }

    fp = fopen(strcat(argv[argc-1],AS_EXTENSION),"r");
    if(!fp){
        FILE_ERROR;
    } else {
        /*first pass*/
        firstPass(fp, &IC, &DC);
        /*update symbols*/
        updateSymbols(IC);
        /*Update words list addresses*/
        updateAddresses();
        /*update data list addresses*/
        updateDataAddresses(IC);
        /*Second pass*/
        secondPass();
        /*If there were no errors, export the files*/
        if (!isError())
        {
            thirdStage(IC, DC, argv[argc-1]);
        }
        /*If there were errors, export the error file*/
        else
        {
            printErrors(argv[argc-1]);
            /*Free the errors list*/
            freeErrors();
        }
        /*Cleanup*/
         freeData();
        freeSymbols();
        freeEntries();
        freeWords();
    }




    return 0;
}